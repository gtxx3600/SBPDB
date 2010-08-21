/*
 * RM_Manager.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */
#include "rm.h"

RC RM_CreateFile(const char* fileName, int recordSize)
{
	return NORMAL;
}
RC RM_DestroyFile(const char *fileName)
{
	return NORMAL;
}
RC RM_OpenFile(const char *fileName, struct RM_FileHandle *fileHandle)
{
	return NORMAL;
}
RC RM_CloseFile(struct RM_FileHandle *fileHandle)
{
	return NORMAL;
}
RC initRM_Manager(struct RM_Manager* rmm, struct PF_Manager * pfm)
{
	rmm->CreateFile = RM_CreateFile;
	rmm->DestroyFile = RM_DestroyFile;
	rmm->OpenFile = RM_OpenFile;
	rmm->CloseFile = RM_CloseFile;
	return NORMAL;
}
