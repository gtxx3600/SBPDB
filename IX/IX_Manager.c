/*
 * IX_Manager.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */

#include "ix.h"
#include <assert.h>
#include "BTNode.h"

extern int (**typeOP[TYPE_NUM])(void* left,void*right,int len);
RC IX_CreateIndex
(IX_Manager *this,
const char *fileName,
int        indexNo,
AttrType   attrType,
int        attrLength)
{
	int (**funcOP)(void* left,void*right,int len) = typeOP[attrType];
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
	char tmpvalue[2*attrLength];
	char tmpvalue2[2*attrLength];
	bzero(tmpvalue,2*attrLength);
	bzero(tmpvalue2,2*attrLength);
#include "NODEL.h"
	PF_FileHandle pffh;
	initPF_FileHandle(&pffh);
	PF_PageHandle headph;
	initPF_PageHandle(&headph);
	PF_PageHandle r,b1,b2,l1,l2,l3,l4;
	NODE* nr,*nb1,*nb2,*nl1,*nl2,*nl3,*nl4;
	initPF_PageHandle(&r);
	initPF_PageHandle(&b1);
	initPF_PageHandle(&b2);
	initPF_PageHandle(&l1);
	initPF_PageHandle(&l2);
	initPF_PageHandle(&l3);
	initPF_PageHandle(&l4);

	IX_HeadPage *head = (IX_HeadPage *)pData;

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
	pffh.AllocatePage(&pffh, &r);
	pffh.AllocatePage(&pffh, &b1);
	pffh.AllocatePage(&pffh, &b2);
	pffh.AllocatePage(&pffh, &l1);
	pffh.AllocatePage(&pffh, &l2);
	pffh.AllocatePage(&pffh, &l3);
	pffh.AllocatePage(&pffh, &l4);
	nr = (NODE*) r.page;
	nb1 = (NODE*) b1.page;
	nb2 = (NODE*) b2.page;
	nl1 = (NODE*) l1.page;
	nl2 = (NODE*) l2.page;
	nl3 = (NODE*) l3.page;
	nl4 = (NODE*) l4.page;
	headph.GetData(&headph, &pData);
	head->attrLength = attrLength;
	head->attrType = attrType;
	head->indexNo = indexNo;
	head->root = r.pagenum;
	head->maxEntryInNode = EntryNumInNode(attrType, attrLength);
	nr->level = 0;
	nb1->level = nb2->level = 1;
	nl1->level = nl2->level = nl3->level = nl4->level = 2;
	nr->totalEntry = 1;
	nb1->totalEntry = nb2->totalEntry = 1;
	nl1->totalEntry = nl2->totalEntry = nl3->totalEntry = nl4->totalEntry = 1;

	funcOP[INC_OP](tmpvalue, tmpvalue2, attrLength);
	funcOP[INC_OP](tmpvalue2, tmpvalue, attrLength);
	memcpy(nb1->values, tmpvalue, attrLength);
	funcOP[INC_OP](tmpvalue, tmpvalue2, attrLength);
	memcpy(nr->values, tmpvalue2, attrLength);
	funcOP[INC_OP](tmpvalue2, tmpvalue, attrLength);
	memcpy(nb2->values, tmpvalue, attrLength);
	nr->pointers[0].page = b1.pagenum;
	nr->pointers[1].page = b2.pagenum;

	nb1->pointers[0].page = l1.pagenum;
	nb1->pointers[1].page = l2.pagenum;

	nb2->pointers[0].page = l3.pagenum;
	nb2->pointers[1].page = l4.pagenum;
	nl1->pointers[ENTRYSINBTNODE].page = l2.pagenum;
	nl2->pointers[ENTRYSINBTNODE].page = l3.pagenum;
	nl3->pointers[ENTRYSINBTNODE].page = l4.pagenum;
	nl4->pointers[ENTRYSINBTNODE].page = -1;
	pffh.MarkDirty(&pffh, r.pagenum);
	pffh.MarkDirty(&pffh, b1.pagenum);
	pffh.MarkDirty(&pffh, b2.pagenum);
	pffh.MarkDirty(&pffh, l1.pagenum);
	pffh.MarkDirty(&pffh, l2.pagenum);
	pffh.MarkDirty(&pffh, l3.pagenum);
	pffh.MarkDirty(&pffh, l4.pagenum);

	pffh.UnpinPage(&pffh, r.pagenum);
	pffh.UnpinPage(&pffh, b1.pagenum);
	pffh.UnpinPage(&pffh, b2.pagenum);
	pffh.UnpinPage(&pffh, l1.pagenum);
	pffh.UnpinPage(&pffh, l2.pagenum);
	pffh.UnpinPage(&pffh, l3.pagenum);
	pffh.UnpinPage(&pffh, l4.pagenum);


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
	return this->pfm->CloseFile(this->pfm, &indexHandle->pffh);
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
