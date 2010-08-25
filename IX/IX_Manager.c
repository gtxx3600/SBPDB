/*
 * IX_Manager.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */

#include "ix.h"
#include <assert.h>
#include "BTNode.h"


RC IX_CreateIndex
(IX_Manager *this,
const char *fileName,
int        indexNo,
AttrType   attrType,
int        attrLength)
{
	char tmp[2*MAX_FILENAME];
	char* pData;
	if(strlen(fileName) >= MAX_FILENAME)
	{
		return DB_PARAM;
	}
	sprintf(tmp,"%s.%d",fileName,indexNo);
	if(strlen(tmp) >= MAX_FILENAME)
	{
		return DB_PARAM;
	}
	int ret;
	if((ret = this->pfm->CreateFile(this->pfm,tmp))!=NORMAL)
	{
		return ret;
	}
	PF_FileHandle pffh;
	initPF_FileHandle(&pffh);
	PF_PageHandle headph;
	initPF_PageHandle(&headph);
	PF_PageHandle rootph;
	initPF_PageHandle(&rootph);
	PF_PageHandle branchph;
	initPF_PageHandle(&branchph);
	/* init head page */
	if((ret = this->pfm->OpenFile(this->pfm, tmp, &pffh))!= NORMAL)
	{
		return ret;
	}
	if((ret = pffh.AllocatePage(&pffh, &headph)) != NORMAL)
	{
		return ret;
	}
	headph.GetData(&headph, &pData);
	IX_HeadPage *head = (IX_HeadPage *)pData;
	head->attrLength = attrLength;
	head->attrType = attrType;
	head->indexNo = indexNo;
	head->root = 1;
	head->maxEntryInLeaf = EntryNumInNode(attrType, attrLength, LEAF);
	head->maxEntryInNLeaf = EntryNumInNode(attrType, attrLength, NOTLEAF);

	/* init root page */
	if((ret = pffh.AllocatePage(&pffh, &rootph)) != NORMAL)
	{
		return ret;
	}
	assert(rootph.pagenum == 1);
	char* rootData;
	headph.GetData(&rootph, &rootData);
	initBTNodeNL(rootData, 0, -1, attrType, attrLength);

	/* init branch page */
	if((ret = pffh.AllocatePage(&pffh, &branchph)) != NORMAL)
	{
		return ret;
	}
	assert(branchph.pagenum == 1);
	char* branchData;
	headph.GetData(&branchph, &branchData);
	initBTNodeNL(branchData, 0, -1, attrType, attrLength);


	char tmpvalue[attrLength];
	bzero(tmpvalue, attrLength);
	if((ret = insertChild(head, &pffh, rootData, branchph.pagenum, tmpvalue)))
	pffh.MarkDirty(&pffh, headph.pagenum);
	pffh.UnpinPage(&pffh, headph.pagenum);
	pffh.MarkDirty(&pffh, rootph.pagenum);
	pffh.UnpinPage(&pffh, rootph.pagenum);
	pffh.MarkDirty(&pffh, branchph.pagenum);
	pffh.UnpinPage(&pffh, branchph.pagenum);
	pffh.ForcePages(&pffh, ALL_PAGES);

	return ret;
}
RC IX_DestroyIndex
(IX_Manager *this,
const char *fileName,
int        indexNo)
{
	return NORMAL;
}
RC IX_OpenIndex
(IX_Manager *this,
const char *fileName,
int        indexNo,
IX_IndexHandle *indexHandle)
{
	char tmp[2*MAX_FILENAME];
	char* pData;
	if(strlen(fileName) >= MAX_FILENAME)
	{
		return DB_PARAM;
	}
	sprintf(tmp,"%s.%d",fileName,indexNo);
	if(strlen(tmp) >= MAX_FILENAME)
	{
		return DB_PARAM;
	}
	int ret;
	PF_FileHandle *pffh = &indexHandle->pffh;
	initPF_FileHandle(pffh);
	PF_PageHandle headph;
	initPF_PageHandle(&headph);
	if((ret = this->pfm->OpenFile(this->pfm,tmp, pffh))!=NORMAL)
	{
		return ret;
	}
	if((ret = pffh->GetFirstPage(pffh, &headph)) != NORMAL)
	{
		return ret;
	}
	headph.GetData(&headph, &pData);
	IX_HeadPage *head = (IX_HeadPage *)pData;
	memcpy(&indexHandle->head, head, sizeof(head));

	pffh->UnpinPage(pffh, headph.pagenum);
	return NORMAL;
}
RC IX_CloseIndex
(IX_Manager *this,
IX_IndexHandle *indexHandle)
{
	return NORMAL;
}

RC initIX_Manager(IX_Manager* this, PF_Manager* pfm)
{
	this->pfm = pfm;
	this->CloseIndex = IX_CloseIndex;
	this->CreateIndex = IX_CreateIndex;
	this->DestroyIndex = IX_DestroyIndex;
	this->OpenIndex = IX_OpenIndex;

	return NORMAL;
}
