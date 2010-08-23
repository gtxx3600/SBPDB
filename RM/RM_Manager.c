/*
 * RM_Manager.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */
#include "rm.h"

RC RM_CreateFile(RM_Manager* this, const char* fileName, int recordSize)
{
	if(this == NULL||fileName == NULL||recordSize <= 0)
	{
		return DB_PARAM;
	}
	int ret = 0;
	if ((ret = this->pf_Manager->CreateFile(this->pf_Manager, fileName)) == NORMAL)
	{

	}
	return ret;
}
RC RM_DestroyFile(RM_Manager* this, const char *fileName)
{
	if(this == NULL||fileName == NULL)
	{
		return DB_PARAM;
	}
	return NORMAL;
}
RC RM_OpenFile(RM_Manager* this, const char *fileName, struct RM_FileHandle *fileHandle)
{
	if(this == NULL||fileName == NULL||fileHandle ==NULL)
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
