/*
 * RM_FileScan.c
 *
 *  Created on: 2010-8-22
 *      Author: hhf
 */

#include "rm.h"

RC RM_OpenScan(RM_FileHandle *fileHandle,
AttrType      attrType,
int           attrLength,
int           attrOffset,
CompOp        compOp,
void          *value,
ClientHint    pinHint)
{
	return NORMAL;
}

RC RM_GetNextRec(RM_Record *rec)
{
	return NORMAL;
}
RC RM_CloseScan()
{
	return NORMAL;
}
RC initRM_FileScan(RM_FileScan *rmfs)
{
	rmfs->CloseScan = RM_CloseScan;
	rmfs->GetNextRec = RM_GetNextRec;
	rmfs->OpenScan = RM_OpenScan;
	return NORMAL;
}
