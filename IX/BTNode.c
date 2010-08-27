/*
 * BTNode.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */

#include "BTNode.h"
#include <assert.h>
extern int (**typeOP[TYPE_NUM])(void* left,void*right,int len);

int EntryNumInNode(AttrType at,int attrLength)
{
		#include "NODEL.h"
		return ENTRYSINBTNODE;
}

void initBTNode(void* node,int level,PageNum parent,AttrType at,int attrLength)
{
	if (node == NULL)return;
	#include "NODEL.h"
	NODE *n = (NODE*)node;
	n->level = level;
	n->totalEntry = 0;

}


int findPos(void *node,void* value, AttrType attrType,int attrLength)
{
	int (**funcOP)(void* left,void*right,int len) = typeOP[attrType];
	#include "NODEL.h"
	NODE *n = (NODE*)node;
	int total = n->totalEntry;
	int i;
	for(i=0;i<total;i++)
	{
		if(funcOP[GT_OP](value, &n->values[i], attrLength))
		{
			return i;
		}
	}
	return total;
}
int findExactPos(void *node,void* value, const RID* rid,AttrType attrType,int attrLength)
{
	int (**funcOP)(void* left,void*right,int len) = typeOP[attrType];
	#include "NODEL.h"
	NODE *n = (NODE*)node;
	int total = n->totalEntry;
	int i;
	for(i=0;i<total;i++)
	{
		if(funcOP[EQ_OP](value, &n->values[i], attrLength))
		{
			if(n->pointers[i].page == rid->pageNum && n->pointers[i].slot == rid->slotNum)
			{
				return i;
			}
		}
	}
	return total;
}
RC getFirstValue(const IX_HeadPage *head, PF_FileHandle* pffh, PageNum pagenum, void* out)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle leafph;
	initPF_PageHandle(&leafph);
	NODE * n;
	pffh->GetThisPage(pffh, pagenum,&leafph);
	n = (NODE*)leafph.page;

	memcpy(out, n->values, attrLength);
	return NORMAL;
}
RC getFirstPointer(const IX_HeadPage *head, PF_FileHandle* pffh, PageNum pagenum, PageNum* out)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle branchph;
	initPF_PageHandle(&branchph);
	NODE * n;
	pffh->GetThisPage(pffh, pagenum,&branchph);
	n = (NODE*)branchph.page;

	*out = n->pointers[0].page;
	return NORMAL;
}
PageNum findStartPage(IX_IndexHandle *idxh)
{
	PageNum root = idxh->head.root;
	PageNum branch;
	PageNum leaf;
	getFirstPointer(&idxh->head,&idxh->pffh,root,&branch);
	getFirstPointer(&idxh->head,&idxh->pffh,branch,&leaf);
	return leaf;

}
RC deleteFromLeaf(const IX_HeadPage *head,PF_FileHandle* pffh, PageNum leaf,void *pData, const RID *rid,PageNum leftBrother)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle leafph;
	initPF_PageHandle(&leafph);
	NODE * n;
	pffh->GetThisPage(pffh, leaf,&leafph);
	n = (NODE*)leafph.page;
	int pos = findExactPos(n,pData,rid,head->attrType,head-> attrLength);
	if(n->totalEntry == 1)
	{
		if(leftBrother){
			PF_PageHandle broph;
			initPF_PageHandle(&broph);
			pffh->GetThisPage(pffh, leftBrother, &broph);
			NODE* b = (NODE*)broph.page;
			b->pointers[ENTRYSINBTNODE].page = n->pointers[ENTRYSINBTNODE].page;
			pffh->MarkDirty(pffh, leftBrother);
			pffh->UnpinPage(pffh, leftBrother);
			pffh->UnpinPage(pffh, leaf);
	//TODO  delete page
			pffh->DisposePages(pffh,leaf);

			return NEED_UNION;
		}else
		{
			n->totalEntry = 0;
			pffh->MarkDirty(pffh, leaf);
			pffh->UnpinPage(pffh, leaf);
			return  NORMAL;
		}
	}else{
		int i;
		for(i = 0; i < n->totalEntry - pos - 1; i++)
		{
			memcpy(&n->values[pos + i],&n->values[pos + i + 1],sizeof(EntryValue));
			memcpy(&n->pointers[pos + i],&n->pointers[pos + i + 1],sizeof(EntryPointer));
		}

		n->totalEntry-- ;
	}
	pffh->MarkDirty(pffh, leaf);
	pffh->UnpinPage(pffh, leaf);

	return NORMAL;
}

RC insertIntoLeaf(const IX_HeadPage *head,PF_FileHandle* pffh, PageNum leaf,void *pData, const RID *rid,PageNum* leafout)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle leafph;
	initPF_PageHandle(&leafph);
	NODE * n;
	pffh->GetThisPage(pffh, leaf,&leafph);
	n = (NODE*)leafph.page;
	int ret;
	int pos = findPos(n,pData,head->attrType,head-> attrLength);
	if(n->totalEntry >= ENTRYSINBTNODE)
	{
		ret = NEED_SPLIT;
		PF_PageHandle broph;
		initPF_PageHandle(&broph);
		pffh->AllocatePage(pffh, &broph);
		*leafout = broph.pagenum;
		NODE* bro = (NODE*)broph.page;
		bro->level = n->level;
		bro->pointers[ENTRYSINBTNODE].page = n->pointers[ENTRYSINBTNODE].page;
		n->pointers[ENTRYSINBTNODE].page = broph.pagenum;
		bro->totalEntry = n->totalEntry / 2;
		n->totalEntry -= bro->totalEntry;
		memcpy(bro->pointers, &n->pointers[n->totalEntry], bro->totalEntry);
		memcpy(bro->values, &n->values[n->totalEntry], bro->totalEntry);
		PageNum tmp;
		if(pos >= n->totalEntry)
		{
			assert(insertIntoLeaf(head,pffh,broph.pagenum,pData, rid,&tmp) == NORMAL);
		}else
		{
			assert(insertIntoLeaf(head,pffh,leaf,pData, rid,&tmp) == NORMAL);
		}
		pffh->MarkDirty(pffh, broph.pagenum);
		pffh->UnpinPage(pffh, broph.pagenum);
	}else
	{
		int i;
		for(i = 0; i < n->totalEntry - pos; i++)
		{
			memcpy(&n->values[n->totalEntry - i],&n->values[n->totalEntry - i - 1],sizeof(EntryValue));
		}
		for(i = 0; i < n->totalEntry - pos; i++)
		{
			memcpy(&n->pointers[n->totalEntry - i],&n->pointers[n->totalEntry - i - 1],sizeof(EntryPointer));
		}
		memcpy(&n->values[pos], pData, attrLength);
		n->pointers[pos].page = rid->pageNum;
		n->pointers[pos].slot = rid->slotNum;
		n->totalEntry ++ ;
		ret = NORMAL;
	}
	pffh->MarkDirty(pffh, leaf);
	pffh->UnpinPage(pffh, leaf);

	return ret;
}
RC deleteFromBranch(const IX_HeadPage *head,PF_FileHandle* pffh, PageNum branch,void *pData, const RID *rid,PageNum leftBrother, PageNum rightBrother)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle branchph;
	initPF_PageHandle(&branchph);
	NODE * n;
	pffh->GetThisPage(pffh, branch,&branchph);
	n = (NODE*)branchph.page;

	int pos = findPos(n,pData,head->attrType,head-> attrLength);
	PageNum leaf = n->pointers[pos].page;

	int ret;

	if((ret = deleteFromLeaf(head,pffh,leaf,pData, rid,pos > 0 ? n->pointers[pos - 1].page : 0)) == NEED_UNION)
	{
		if(n->totalEntry == 1)
		{
			assert(pos == 0 || pos == 1);
			if(leftBrother){
				PF_PageHandle left;
				initPF_PageHandle(&left);
				pffh->GetThisPage(pffh, leftBrother, &left);
				NODE* b = (NODE*)left.page;
				if(b->totalEntry == 1)
				{
					b->pointers[b->totalEntry + 1].page = n->pointers[1-pos].page;
					getFirstValue(head, pffh,n->pointers[1-pos].page, &b->values[b->totalEntry]);
					b->totalEntry++;
					pffh->MarkDirty(pffh, leftBrother);
					pffh->UnpinPage(pffh, leftBrother);
					//TODO delete n
					pffh->UnpinPage(pffh, branch);
					pffh->DisposePages(pffh, branch);
					return NEED_UNION;
				}else
				{
					n->pointers[1].page = n->pointers[1-pos].page;
					getFirstValue(head, pffh, n->pointers[1-pos].page, n->values);
					n->pointers[0].page = b->pointers[b->totalEntry].page;
					b->totalEntry--;
					pffh->MarkDirty(pffh, leftBrother);
					pffh->UnpinPage(pffh, leftBrother);
					pffh->MarkDirty(pffh, branch);
					pffh->UnpinPage(pffh, branch);
					return NORMAL;
				}

			}else if(rightBrother)
			{
				PF_PageHandle right;
				initPF_PageHandle(&right);
				pffh->GetThisPage(pffh, rightBrother, &right);
				NODE* b = (NODE*)right.page;
				if(b->totalEntry == 1)
				{
					b->pointers[2].page = b->pointers[1].page;
					b->pointers[1].page = b->pointers[0].page;
					getFirstValue(head, pffh,b->pointers[1].page, b->values);
					b->pointers[0].page = n->pointers[1-pos].page;
					b->totalEntry++;
					pffh->MarkDirty(pffh, rightBrother);
					pffh->UnpinPage(pffh, rightBrother);
					//TODO delete n
					pffh->UnpinPage(pffh, branch);
					pffh->DisposePages(pffh, branch);
					return NEED_UNION;
				}else
				{
					n->pointers[0].page = n->pointers[1-pos].page;
					n->pointers[1].page = b->pointers[0].page;
					getFirstValue(head, pffh, n->pointers[1].page, n->values);
					int i;
					for(i = 0; i< b->totalEntry - 1;i++)
					{
						memcpy(&b->values[i], &b->values[i+1], sizeof(EntryValue));
						memcpy(&b->pointers[i], &b->pointers[i+1],sizeof(EntryPointer));
					}
					b->pointers[b->totalEntry - 1].page = b->pointers[b->totalEntry].page;
					b->totalEntry--;
					pffh->MarkDirty(pffh, leftBrother);
					pffh->UnpinPage(pffh, leftBrother);
					pffh->MarkDirty(pffh, branch);
					pffh->UnpinPage(pffh, branch);
					return NORMAL;
				}


			}else
			{
				return DB_FATAL;
			}

		}else
		{
			int i;
			int offset = 0;
			if(pos >= 1)
			{
				offset = 1;
			}
			for(i = 0; i < n->totalEntry - pos - 1; i++)
			{
				memcpy(&n->values[pos + i - offset],&n->values[pos + i + 1 - offset],sizeof(EntryValue));
				memcpy(&n->pointers[pos + i],&n->pointers[pos + i + 1],sizeof(EntryPointer));
			}

			n->totalEntry-- ;
			ret = NORMAL;
		}
	}else if(ret == NORMAL)
	{
		goto err_exit;
	}

	pffh->MarkDirty(pffh,branchph.pagenum);
err_exit:
	pffh->UnpinPage(pffh,branchph.pagenum);
	return ret;
}
RC insertIntoBranch(const IX_HeadPage *head,PF_FileHandle* pffh, PageNum branch,void *pData, const RID *rid,PageNum* branchout)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle branchph;
	initPF_PageHandle(&branchph);
	NODE * n;
	pffh->GetThisPage(pffh, branch,&branchph);
	n = (NODE*)branchph.page;

	int pos = findPos(n,pData,head->attrType,head-> attrLength);
	PageNum leaf = n->pointers[pos].page;
	PageNum leafout;
	int ret;
	int i;
	if((ret = insertIntoLeaf(head,pffh,leaf,pData, rid,&leafout)) == NEED_SPLIT)
	{
		if(n->totalEntry >= ENTRYSINBTNODE)
		{
			ret = NEED_SPLIT;
			PF_PageHandle new;
			initPF_PageHandle(&new);
			pffh->AllocatePage(pffh, &new);
			NODE* b = (NODE*)new.page;
			*branchout = new.pagenum;
			b->level = n->level;
			b->totalEntry = n->totalEntry - pos;
			n->totalEntry -= b->totalEntry;
			memcpy(b->values, &n->values[pos], b->totalEntry * sizeof(EntryValue));
			memcpy(&b->pointers[1], &n->pointers[pos+1], b->totalEntry * sizeof(EntryPointer));
			b->pointers[0].page = leafout;
			pffh->MarkDirty(pffh,new.pagenum);
			pffh->UnpinPage(pffh,new.pagenum);
		}else
		{

			for(i = 0; i < n->totalEntry - pos; i++)
			{
				memcpy(&n->values[n->totalEntry - i], &n->values[n->totalEntry - i - 1], sizeof(EntryValue));
				memcpy(&n->pointers[n->totalEntry - i + 1], &n->pointers[n->totalEntry - i], sizeof(EntryPointer)); // pointers[] length = values[] length + 1
			}
			n->pointers[pos+1].page = leafout;
			getFirstValue(head,pffh,leafout,&n->values[pos+1]);
			n->totalEntry++;
		}
	}else if(ret == NORMAL)
	{
		goto err_exit;
	}

	pffh->MarkDirty(pffh,branchph.pagenum);
err_exit:
	pffh->UnpinPage(pffh,branchph.pagenum);
	return ret;
}
RC deleteFromRoot(const IX_HeadPage *head,PF_FileHandle* pffh, void *pData, const RID *rid)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle rootph;
	initPF_PageHandle(&rootph);
	NODE * n;
	pffh->GetThisPage(pffh, head->root,&rootph);
	n = (NODE*)rootph.page;
	int pos = findPos(n,pData,head->attrType,head-> attrLength);
	PageNum branch = n->pointers[pos].page;
	int ret;

	if((ret = deleteFromBranch(head,pffh,branch,pData, rid,
			pos >= 1? n->pointers[pos - 1].page:0,
					pos <n->totalEntry? n->pointers[pos+1].page:0)) == NEED_UNION)
	{
		if(n->totalEntry == 1)
		{
			PF_PageHandle new;
			initPF_PageHandle(&new);
			pffh->AllocatePage(pffh, &new);
			n->pointers[pos].page = new.pagenum;
			NODE* b = (NODE*) new.page;
			b->totalEntry = 0;
			//TODO

		}else
		{
			int i;
			int offset = 0;
			if(pos >= 1)
			{
				offset = 1;
			}
			for(i = 0; i < n->totalEntry - pos - 1; i++)
			{
				memcpy(&n->values[pos + i - offset],&n->values[pos + i + 1 - offset],sizeof(EntryValue));
				memcpy(&n->pointers[pos + i],&n->pointers[pos + i + 1],sizeof(EntryPointer));
			}

			n->totalEntry-- ;
			pffh->MarkDirty(pffh, head->root);
			pffh->UnpinPage(pffh, head->root);
			ret = NORMAL;
		}
	}else if(ret == NORMAL)
	{

	}else
	{

	}
	return ret;
}
RC insertIntoRoot(const IX_HeadPage *head,PF_FileHandle* pffh, void *pData, const RID *rid)
{
	int attrLength = head->attrLength;
	#include "NODEL.h"
	PF_PageHandle rootph;
	initPF_PageHandle(&rootph);
	NODE * n;
	pffh->GetThisPage(pffh, head->root,&rootph);
	n = (NODE*)rootph.page;
	int pos = findPos(n,pData,head->attrType,head-> attrLength);
	PageNum branch = n->pointers[pos].page;
	PageNum out;
	int ret;
	int i;
	if((ret = insertIntoBranch(head,pffh,branch,pData, rid,&out)) == NEED_SPLIT)
	{
		if (n->totalEntry >= ENTRYSINBTNODE)
		{
			pffh->UnpinPage(pffh,rootph.pagenum);
			return IX_FULL;//FIXME
		}
		for(i = 0; i < n->totalEntry - pos; i++)
		{
			memcpy(&n->values[n->totalEntry - i], &n->values[n->totalEntry - i - 1],sizeof(EntryValue));
			memcpy(&n->pointers[n->totalEntry - i + 1], &n->pointers[n->totalEntry - i],sizeof(EntryValue));
		}
		n->totalEntry ++;
		n->pointers[pos+1].page = out;
		PageNum ptr;
		getFirstPointer(head,pffh,out,&ptr);
		getFirstValue(head,pffh,ptr,&n->values[pos]);
		pffh->MarkDirty(pffh,rootph.pagenum);
		pffh->UnpinPage(pffh,rootph.pagenum);
	}else if(ret == NORMAL)
	{

	}else
	{

	}
	return ret;
}

//RC insertTuple(PF_FileHandle * pffh,void* value, PageNum p, SlotNum s)
//{
//	PF_PageHandle pfph;
//	initPF_PageHandle(&pfph);
//	char* pData;
//	pffh->GetFirstPage(pffh, &pfph);
//	pfph.GetData(&pfph, &pData);
//	root = (BTNode*)pData;
//
//	return NORMAL;
//}
