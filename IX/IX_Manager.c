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
	PF_FileHandle pffh;
	initPF_FileHandle(&pffh);
	PF_PageHandle headph;
	initPF_PageHandle(&headph);
	PF_PageHandle rootph;
	initPF_PageHandle(&rootph);
	PF_PageHandle branchph;
	initPF_PageHandle(&branchph);
	PF_PageHandle leafph;
	initPF_PageHandle(&leafph);
	IX_HeadPage *head = (IX_HeadPage *)pData;
	char* rootData;
	char* branchData;
	char* leafData;
	char tmpvalue[attrLength];
	if((ret = this->pfm->CreateFile(this->pfm,tmp))!=NORMAL)
	{
		goto err_exit;
	}

	/* init head page */
	if((ret = this->pfm->OpenFile(this->pfm, tmp, &pffh))!= NORMAL)
	{
		goto err_exit;

	}
	if((ret = pffh.AllocatePage(&pffh, &headph)) != NORMAL)
	{

		goto err_exit0;
	}
	headph.GetData(&headph, &pData);

	head->attrLength = attrLength;
	head->attrType = attrType;
	head->indexNo = indexNo;
	head->root = 1;
	head->maxEntryInLeaf = EntryNumInNode(attrType, attrLength, LEAF);
	head->maxEntryInNLeaf = EntryNumInNode(attrType, attrLength, NOTLEAF);

	/* init root page */
	if((ret = pffh.AllocatePage(&pffh, &rootph)) != NORMAL)
	{
		goto err_exit1;
	}
	assert(rootph.pagenum == 1);

	rootph.GetData(&rootph, &rootData);
	initBTNodeNL(rootData, 0, -1, attrType, attrLength);

	/* init branch page */
	if((ret = pffh.AllocatePage(&pffh, &branchph)) != NORMAL)
	{
		goto err_exit2;
	}
	assert(branchph.pagenum == 1);

	branchph.GetData(&branchph, &branchData);
	initBTNodeNL(branchData, 1, -1, attrType, attrLength);

	/* init leaf page */
	if((ret = pffh.AllocatePage(&pffh, &leafph)) != NORMAL)
	{
		goto err_exit3;
	}

	leafph.GetData(&leafph, &leafData);
	initBTNodeL(leafData, 2, -1, attrType, attrLength );

	bzero(tmpvalue, attrLength);
	if((ret = insertChild(head, &pffh, rootData, branchph.pagenum, tmpvalue)) != NORMAL)
	{
		goto err_exit4;
	}
	if((ret = insertChild(head, &pffh, branchData, leafData.pagenum, tmpvalue)) != NORMAL)
	{
		goto err_exit4;
	}

err_exit4:
	pffh.MarkDirty(&pffh, leafph.pagenum);
	pffh.UnpinPage(&pffh, leafph.pagenum);
err_exit3:
	pffh.MarkDirty(&pffh, branchph.pagenum);
	pffh.UnpinPage(&pffh, branchph.pagenum);
err_exit2:
	pffh.MarkDirty(&pffh, rootph.pagenum);
	pffh.UnpinPage(&pffh, rootph.pagenum);
err_exit1:
	pffh.MarkDirty(&pffh, headph.pagenum);
	pffh.UnpinPage(&pffh, headph.pagenum);
	pffh.ForcePages(&pffh, ALL_PAGES);
err_exit0:
	this->pfm->CloseFile(this->pfm, &pffh);
err_exit:
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
