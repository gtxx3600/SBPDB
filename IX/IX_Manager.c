/*
 * IX_Manager.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */

#include "ix.h"
RC IX_CreateIndex
(IX_Manager *this,
const char *fileName,
int        indexNo,
AttrType   attrType,
int        attrLength)
{
	return NORMAL;
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
	this->CloseIndex = IX_CloseIndex;
	this->CreateIndex = IX_CreateIndex;
	this->DestroyIndex = IX_DestroyIndex;
	this->OpenIndex = IX_OpenIndex;

	return NORMAL;
}
