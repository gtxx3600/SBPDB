/*
 * BTNode.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */

#include "BTNode.h"
extern int (**typeOP[TYPE_NUM])(void* left,void*right,int len);
int findPosToInsert( void* node, void* value, AttrType attrType, int attrLength, int level)
{
	//int attrLength = ixh->attrLength;
	int (**funcOP)(void* left,void*right,int len) = typeOP[attrType];
	if(level <= 1)
	{
#include "NODENL.h"
		NODE *n = (NODE*)node;
		int left = 0,right = n->totalEntry - 1;
		int p;

		while(left != right)
		{
			p = (left + right) / 2 ;
			if(funcOP[LE_OP](n->entrys[p].value, value, attrLength))
			{
				left = p+1;
				continue;
			}else
			{
				right = p;
			}
		}
		if(funcOP[GT_OP](n->entrys[left].value, value, attrLength))
		{
			p = left;
		}else
		{
			p = left + 1;
		}
		return p;

	}else
	{
#include "NODEL.h"
		NODE *n = (NODE*)node;
		int left = 0,right = n->totalEntry - 1;
		int p;

		while(left != right)
		{
			p = (left + right) / 2 ;
			if(funcOP[LE_OP](n->entrys[p].value, value, attrLength))
			{
				left = p+1;
				continue;
			}else
			{
				right = p;
			}
		}
		if(funcOP[GT_OP](n->entrys[left].value, value, attrLength))
		{
			p = left;
		}else
		{
			p = left + 1;
		}
		return p;
	}
}
int EntryNumInNode(AttrType at,int attrLength, int nodeType)
{
	if(nodeType == NOTLEAF)
	{
		#include "NODENL.h"
		return ENTRYSINBTNODE;
	}else if(nodeType == LEAF)
	{
		#include "NODEL.h"
		return  ENTRYSINBTNODE;
	}
	return -1;
}

void initBTNodeNL(void* node,int level,PageNum brother,AttrType at,int attrLength)
{
	if (node == NULL)return;
	#include "NODENL.h"
	NODE *n = (NODE*)node;
	n->level = level;
	n->totalEntry = 0;
	n->brother = brother;
}
void initBTNodeL(void* node,int level,PageNum brother,AttrType at,int attrLength)
{
	if (node == NULL)return;
	#include "NODEL.h"
	NODE *n = (NODE*)node;
	n->level = level;
	n->totalEntry = 0;
	n->brother = brother;
}
RC insertChild(IX_HeadPage *head,PF_FileHandle* pffh, void* node, PageNum child, void* value)
{
	if (node == NULL)return DB_PARAM;
	int attrLength = head->attrLength;
	#include "NODENL.h"
	NODE *n = (NODE*)node;
	int total = n->totalEntry;
	int maxEntryInNode = head->maxEntryInNLeaf;
	int p = findPosToInsert(node, value, head->attrType, head->attrLength,0);
	if(total >= maxEntryInNode)
	{
		if(n->level == 0)
		{
			return IX_FULL;
		}
		PF_PageHandle rootPage;
		char * pData;
		NODE * root;
		initPF_PageHandle(&rootPage);
		pffh->GetThisPage(pffh, head->root, &rootPage);
		rootPage.GetData(&rootPage, &pData);
		root = (NODE*)pData;
		if(root->totalEntry >= maxEntryInNode)
		{
			pffh->UnpinPage(pffh, rootPage.pagenum);
			return IX_FULL;
		}
		PF_PageHandle newBro;
		initPF_PageHandle(&newBro);
		pffh->AllocatePage(pffh,&newBro);
		NODE * bro;
		newBro.GetData(&newBro,((char**)&bro));
		bro->level = n->level;
		bro->totalEntry = n->totalEntry/2;
		bro->brother = n->brother;
		n->totalEntry -= bro->totalEntry;
		n->brother = newBro.pagenum;
		memcpy(bro->entrys, &n->entrys[n->totalEntry], bro->totalEntry * sizeof(NODEENTRY));
		if(p < n->totalEntry)
		{
			INSERT_DATA_INTO_NODE(n)
		}else
		{
			p -= n->totalEntry;
			INSERT_DATA_INTO_NODE(bro)
		}
		insertChild(head,pffh,root, newBro.pagenum, &bro->entrys[0].value);
		pffh->MarkDirty(pffh, newBro.pagenum);
		pffh->MarkDirty(pffh, rootPage.pagenum);
		pffh->UnpinPage(pffh, newBro.pagenum);
		pffh->UnpinPage(pffh, rootPage.pagenum);
	}else
	{
		INSERT_DATA_INTO_NODE(n)
	}
	return NORMAL;
}
//RC insertTuple(PF_FileHandle * pffh,void* value, PageNum p, SlotNum s)
//{
//	struct test{
//		int a[p];
//	};
//
//	PF_PageHandle pfph;
//	initPF_PageHandle(&pfph);
//	BTNode* root;
//	char* pData;
//	pffh->GetFirstPage(pffh, &pfph);
//	pfph.GetData(&pfph, &pData);
//	root = (BTNode*)pData;
//
//	return NORMAL;
//}
