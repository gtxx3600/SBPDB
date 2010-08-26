/*
 * BTNode.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */

#include "BTNode.h"
#include <assert.h>
extern int (**typeOP[TYPE_NUM])(void* left,void*right,int len);
//int findPageNum(void* node, PageNum pageNum, AttrType attrType, int attrLength)
//{
//#include "NODEL.h"
//	NODE *n = (NODE*)node;
//	int i;
//	for(i=0; i < n->totalEntry; i++)
//	{
//		if(n->entrys[i].page == pageNum)
//		{
//			return i;
//		}
//	}
//	return -1;
//}
//int findPosToInsert( void* node, void* value, AttrType attrType, int attrLength, int level)
//{
//	int (**funcOP)(void* left,void*right,int len) = typeOP[attrType];
//#include "NODEL.h"
//		NODE *n = (NODE*)node;
//		int left = 0,right = n->totalEntry - 1;
//		int p;
//
//		while(left != right)
//		{
//			p = (left + right) / 2 ;
//			if(funcOP[LE_OP](n->entrys[p].value, value, attrLength))
//			{
//				left = p+1;
//				continue;
//			}else
//			{
//				right = p;
//			}
//		}
//		if(funcOP[GT_OP](n->entrys[left].value, value, attrLength))
//		{
//			p = left;
//		}else
//		{
//			p = left + 1;
//		}
//		return p;
//
//}
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

//RC insertChild(const IX_HeadPage *head,PF_FileHandle* pffh, void* node, PageNum child, void* value)
//{
//	if (node == NULL)return DB_PARAM;
//	int attrLength = head->attrLength;
//	#include "NODEL.h"
//	NODE *n = (NODE*)node;
//	int total = n->totalEntry;
//	int maxEntryInNode = head->maxEntryInNode;
//	int p = findPosToInsert(node, value, head->attrType, head->attrLength);
//	if(total >= maxEntryInNode)
//	{
//		if(n->level == 0)
//		{
//			return IX_FULL;
//		}
//		PF_PageHandle rootPage;
//		char * pData;
//		NODE * root;
//		initPF_PageHandle(&rootPage);
//		pffh->GetThisPage(pffh, head->root, &rootPage);
//		rootPage.GetData(&rootPage, &pData);
//		root = (NODE*)pData;
//		if(root->totalEntry >= maxEntryInNode)
//		{
//			pffh->UnpinPage(pffh, rootPage.pagenum);
//			return IX_FULL;
//		}
//		PF_PageHandle newBro;
//		initPF_PageHandle(&newBro);
//		pffh->AllocatePage(pffh,&newBro);
//		NODE * bro;
//		newBro.GetData(&newBro,((char**)&bro));
//		bro->level = n->level;
//		bro->totalEntry = n->totalEntry/2;
//		bro->lastP = n->brother;
//		n->totalEntry -= bro->totalEntry;
//		n->brother = newBro.pagenum;
//		memcpy(bro->entrys, &n->entrys[n->totalEntry], bro->totalEntry * sizeof(NODEENTRY));
//		if(p < n->totalEntry)
//		{
//			INSERT_DATA_INTO_NODE(n)
//		}else
//		{
//			p -= n->totalEntry;
//			INSERT_DATA_INTO_NODE(bro)
//		}
//		insertChild(head,pffh,root, newBro.pagenum, &bro->entrys[0].value);
//		pffh->MarkDirty(pffh, newBro.pagenum);
//		pffh->MarkDirty(pffh, rootPage.pagenum);
//		pffh->UnpinPage(pffh, newBro.pagenum);
//		pffh->UnpinPage(pffh, rootPage.pagenum);
//	}else
//	{
//		INSERT_DATA_INTO_NODE(n)
//	}
//	return NORMAL;
//}

//RC splitNode(const IX_HeadPage *head,PF_FileHandle* pffh, PageNum node_pageNum,PageNum parent_node_pageNum)
//{
//	int attrLength = head->attrLength;
//#include "NODEL.h"
//	NODE * n;
//	NODE * p;
//	PF_PageHandle nodeph;
//	PF_PageHandle parentph;
//	PF_PageHandle broph;
//	initPF_PageHandle(&broph);
//	initPF_PageHandle(&parentph);
//	initPF_PageHandle(&nodeph);
//	pffh->GetThisPage(pffh, node_pageNum,&nodeph);
//	pffh->GetThisPage(pffh, parent_node_pageNum,&parentph);
//	n = (NODE *)nodeph.page;
//	p = (NODE *)nodeph.page;
//	int maxEntryInNode = head->maxEntryInNode;
//	if(p->totalEntry >= maxEntryInNode)
//	{
//		pffh->UnpinPage(pffh,node_pageNum);
//		pffh->UnpinPage(pffh,parent_node_pageNum);
//		return IX_FULL;
//	}else
//	{
//		pffh->AllocatePage(pffh,&broph);
//		char* broData;
//		broph.GetData(&broph,&broData);
//		NODE* b = (NODE*) broData;
//		b->level = n->level;
//		b->totalEntry = n->totalEntry/2;
//		b->brother = n->brother;
//		n->totalEntry -= b->totalEntry;
//		n->brother = broph.pagenum;
//		memcpy(b->entrys,&n->entrys[n->totalEntry], b->totalEntry*sizeof(NODEENTRY));
//		RID rid;
//		initRID(&rid, broph.pagenum,0);
//		int pos = findPageNum(p, node_pageNum, head->attrType, attrLength);
//		memcpy(&p->entrys[pos].value, n->entrys, attrLength);
//		writeNode(head, p, &rid, &b->entrys[0],pos);
//		pffh->MarkDirty(pffh, node_pageNum);
//		pffh->UnpinPage(pffh, node_pageNum);
//		pffh->MarkDirty(pffh, parent_node_pageNum);
//		pffh->UnpinPage(pffh, parent_node_pageNum);
//		pffh->MarkDirty(pffh, broph.pagenum);
//		pffh->UnpinPage(pffh, broph.pagenum);
//	}
//	return NORMAL;
//}
//RC writeNode(const IX_HeadPage *head, void* node, const RID * rid, void* value, int p)
//{
//	int attrLength = head->attrLength;
//#include "NODEL.h"
//	NODE * n = (NODE*)node;
//	int total = n->totalEntry;
//	int maxEntryInNode = head->maxEntryInNode;
//
//	if(total >= maxEntryInNode)
//	{
//		return IX_FULL;
//	}else
//	{
//		int i;
//		for(i = 0; i < total - p; i++)
//		{
//			memcpy(&n->entrys[total - i],&n->entrys[total - i - 1],sizeof(NODEENTRY));
//		}
//		memcpy(n->entrys[p].value, value, attrLength);
//		n->entrys[p].page = rid->pageNum;
//		n->entrys[p].slot = rid->slotNum;
//		n->totalEntry++;
//	}
//	return NORMAL;
//}
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
RC deleteFromLeaf(const IX_HeadPage *head,PF_FileHandle* pffh, PageNum leaf,void *pData, const RID *rid,PageNum* leafout)
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
			assert(deleteFromLeaf(head,pffh,broph.pagenum,pData, rid,&tmp) == NORMAL);
		}else
		{
			assert(deleteFromLeaf(head,pffh,leaf,pData, rid,&tmp) == NORMAL);
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
RC deleteFromBranch(const IX_HeadPage *head,PF_FileHandle* pffh, PageNum branch,void *pData, const RID *rid,PageNum* branchout)
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
	return NORMAL;
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
