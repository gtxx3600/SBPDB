/*
 * RM_Manager.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */
#include "rm.h"a
#include <string.h>
RC initHeadPage(char* pData, RecordSize recordSize)
{
	int slotSize = (ALL_PAGE_SIZE - sizeof(PageNum) - sizeof(RecordSize)) / (recordSize + 1.0/8);
	printf("slotSize:%d,recordSize:%d\n",slotSize,recordSize);
	int bitmapSize = slotSize/8 + slotSize % 8 ? 1 : 0;
	*((PageNum*)pData) = 0;
	*((RecordSize*)(&pData[sizeof(PageNum)])) = recordSize;
	bzero(&pData[sizeof(RecordSize) + sizeof(PageNum)],bitmapSize);

}
RC RM_CreateFile(RM_Manager* this, const char* fileName, int recordSize)
{
	if(this == NULL||filename == NULL||recordSize <= 0||recordSize > ALL_PAGE_SIZE - sizeof(int) - sizeof(char))
	{
		return DB_PARAM;
	}
	int ret = 0;
	if ((ret = this->pf_Manager->CreateFile(this->pf_Manager, fileName)) == NORMAL)
	{
		PF_FileHandle fileHandle;
		PF_PageHandle pageHandle;
		char* pData;
		PageNum pageNum;

		pf_Manager->OpenFile(pf_Manager, fileName, &fileHandle);
		fileHandle.AllocatePage(&pageHandle);
		pageHandle.GetData(&pData);
	}
	return ret;
}
RC RM_DestroyFile(RM_Manager* this, const char *fileName)
{
	if(this == NULL||filename == NULL)
	{
		return DB_PARAM;
	}
	return NORMAL;
}
RC RM_OpenFile(RM_Manager* this, const char *fileName, struct RM_FileHandle *fileHandle)
{
	if(this == NULL||filename == NULL||fileHandle ==NULL)
	{
		return DB_PARAM;
	}
	return NORMAL;
}
RC RM_CloseFile(RM_Manager* this, struct RM_FileHandle *fileHandle)
{
	if(this == NULL||fileHandle ==NULL)
	{
		return DB_PARAM;
	}
	return NORMAL;
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
