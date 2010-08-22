/*
 * RM_Manager.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */
#include "rm.h"

RC RM_CreateFile(RM_Manager* this, const char* fileName, int recordSize)
{
	return NORMAL;
}
RC RM_DestroyFile(RM_Manager* this, const char *fileName)
{
	return NORMAL;
}
RC RM_OpenFile(RM_Manager* this, const char *fileName, struct RM_FileHandle *fileHandle)
{
	return NORMAL;
}
RC RM_CloseFile(RM_Manager* this, struct RM_FileHandle *fileHandle)
{
	return NORMAL;
}
RC initRM_Manager(RM_Manager* this, struct PF_Manager * pfm)
{
	this->pf_Manager = pfm;
	this->CreateFile = RM_CreateFile;
	this->DestroyFile = RM_DestroyFile;
	this->OpenFile = RM_OpenFile;
	this->CloseFile = RM_CloseFile;
	return NORMAL;
}
