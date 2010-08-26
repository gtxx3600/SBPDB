/*
 * RM_Manager.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */
#include "rm.h"
#include <string.h>
void initHeadPage(char* pData, RecordSize recordSize)
{
	SlotNum slotNum = (ALL_PAGE_SIZE - sizeof(PageNum)) / (recordSize + 1.0/8);
	printf("slotNum:%d,recordSize:%d\n",slotNum,recordSize);
	*((PageNum*)pData) = 0;
	*((RecordSize*)(&pData[sizeof(PageNum)])) = recordSize;
	*((SlotNum*)(&pData[sizeof(RecordSize) + sizeof(PageNum)])) = slotNum;

}
RC RM_CreateFile(RM_Manager* this, const char* fileName, int recordSize)
{
	if(this == NULL||fileName == NULL||recordSize <= 0||recordSize > ALL_PAGE_SIZE - sizeof(int) - sizeof(char))
	{
		return DB_PARAM;
	}
	int ret;
	if ((ret = this->pf_Manager->CreateFile(this->pf_Manager, fileName)) == NORMAL)
	{
		PF_FileHandle fileHandle;
		initPF_FileHandle(&fileHandle);
		PF_PageHandle pageHandle;
		initPF_PageHandle(&pageHandle);
		char* pData;
		PageNum pageNum;

		this->pf_Manager->OpenFile(this->pf_Manager, fileName, &fileHandle);
		fileHandle.AllocatePage(&fileHandle, &pageHandle);
		pageHandle.GetData(&pageHandle, &pData);
		initHeadPage(pData, recordSize);
		pageHandle.GetPageNum(&pageHandle, &pageNum);
		fileHandle.MarkDirty(&fileHandle, pageNum);
		fileHandle.ForcePages(&fileHandle, pageNum);

	}
	return ret;
}
RC RM_DestroyFile(RM_Manager* this, const char *fileName)
{
	if(this == NULL||fileName == NULL)
	{
		return DB_PARAM;
	}

	return this->pf_Manager->DestroyFile(this->pf_Manager, fileName);

}
RC RM_OpenFile(RM_Manager* this, const char *fileName, struct RM_FileHandle *fileHandle)
{
	if(this == NULL||fileName == NULL||fileHandle ==NULL)
	{
		return DB_PARAM;
	}
	PF_FileHandle* pffileHandle = (PF_FileHandle*)malloc(sizeof(PF_FileHandle));
	initPF_FileHandle(pffileHandle);
	PF_PageHandle pfpageHandle;
	initPF_PageHandle(&pfpageHandle);
	char* pData;
	int ret;
	if((ret = this->pf_Manager->OpenFile(this->pf_Manager, fileName, pffileHandle)) == NORMAL)
	{
		fileHandle->opened = 1;
		fileHandle->pf_FileHandle = pffileHandle;
		pffileHandle->GetFirstPage(pffileHandle, &pfpageHandle);
		pfpageHandle.GetData(&pfpageHandle, &pData);
		fileHandle->firstFree = *(PageNum*)pData;
		fileHandle->recordSize = *(RecordSize*)(&pData[sizeof(PageNum)]);
		fileHandle->slotInOnePage = *(SlotNum*)(&pData[sizeof(PageNum) + sizeof(RecordSize)]);
		//fileHandle->totalPageNum = pffileHandle->GetNpage(pffileHandle);
		fileHandle->pageHeaderLength = sizeof(PageNum) + fileHandle->slotInOnePage / 8 + 1;
		fileHandle->bitmappos = sizeof(PageNum);

	}else
	{
		free(pffileHandle);
	}
	return ret;
}
RC RM_CloseFile(RM_Manager* this, struct RM_FileHandle *fileHandle)
{
	if(this == NULL||fileHandle ==NULL)
	{
		return DB_PARAM;
	}
	if(fileHandle->opened){
		fileHandle->ForcePages(fileHandle, ALL_PAGES);
		free(fileHandle->pf_FileHandle);
		return NORMAL;
	}
	return PF_CLOSEDFILE;
}
RC initRM_Manager(RM_Manager* this, struct PF_Manager * pfm)
{
	if(this == NULL||pfm ==NULL)
	{
		return DB_PARAM;
	}
	this->pf_Manager = pfm;
	this->CreateFile = RM_CreateFile;
	this->DestroyFile = RM_DestroyFile;
	this->OpenFile = RM_OpenFile;
	this->CloseFile = RM_CloseFile;
	return NORMAL;
}
