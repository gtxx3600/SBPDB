/*
 * BTNode.h
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */

#ifndef BTNODE_H_
#define BTNODE_H_
#include "ix.h"

#define NOTLEAF	0
#define LEAF		1
//#if 0
//#define ENTRYSINBTNODE	(ALL_PAGE_SIZE-2*sizeof(PageNum)-3*sizeof(int))/sizeof(BTNodeEntry)
//typedef struct BTNodeEntry{
//	PageNum page;
//	SlotNum slot;
//	char value[4];
//}BTNodeEntry;
//typedef struct BTNode{
//	int level;
//	int totalEntry;
//	PageNum bro;
//	BTNodeEntry entrys[ENTRYSINBTNODE];
//	char reserved[4096 - ENTRYSINBTNODE*sizeof(BTNodeEntry) - sizeof(PageNum) - 3*sizeof(int)];
//}BTNode;
//#endif
void initBTNode(void* n,int level,PageNum parent, AttrType at,int attrLength);
int EntryNumInNode(AttrType at,int attrLength);

RC insertIntoRoot(const IX_HeadPage *head,PF_FileHandle* pffh, void *pData, const RID *rid);
RC deleteFromRoot(const IX_HeadPage *head,PF_FileHandle* pffh, void *pData, const RID *rid);
#define INSERT_DATA_INTO_NODE(node_use_in_macro)				int i;\
for(i = 0; i < node_use_in_macro->totalEntry - p; i++)\
{\
	memcpy(&node_use_in_macro->entrys[node_use_in_macro->totalEntry - i],&node_use_in_macro->entrys[node_use_in_macro->totalEntry - i - 1],sizeof(NODEENTRY));\
}\
memcpy(node_use_in_macro->entrys[p].value, value, attrLength);\
node_use_in_macro->entrys[p].page = child;\
node_use_in_macro->totalEntry++;\



#endif /* BTNODE_H_ */
