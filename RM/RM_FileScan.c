/*
 * RM_FileScan.c
 *
 *  Created on: 2010-8-22
 *      Author: hhf
 */

#include "rm.h"

RC RM_OpenScan(RM_FileScan *this,
RM_FileHandle *fileHandle,
AttrType      attrType,
int           attrLength,
int           attrOffset,
CompOp        compOp,
void          *value,
ClientHint    pinHint)
{
	return NORMAL;
}

RC RM_GetNextRec(RM_FileScan *this, RM_Record *rec)
{
	return NORMAL;
}
RC RM_CloseScan(RM_FileScan *this)
{
	return NORMAL;
}
RC initRM_FileScan(RM_FileScan *this)
{
	this->CloseScan = RM_CloseScan;
	this->GetNextRec = RM_GetNextRec;
	this->OpenScan = RM_OpenScan;
	return NORMAL;
}
