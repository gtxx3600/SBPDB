/*
 * RM_FileHandle.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */

#include "rm.h"

RC RM_GetRec	(RM_FileHandle* this, const RID *rid, RM_Record *rec)
{
	return NORMAL;
}
RC RM_InsertRec	(RM_FileHandle* this, const char *pData, RID *rid)
{
	return NORMAL;
}
RC RM_DeleteRec	(RM_FileHandle* this, const RID *rid)
{
	return NORMAL;
}
RC RM_UpdateRec	(RM_FileHandle* this, const RM_Record *rec)
{
	return NORMAL;
}
RC RM_ForcePages	(RM_FileHandle* this, PageNum pageNum)
{
	return NORMAL;
}

RC initRM_FileHandle(RM_FileHandle* this)
{
	this->DeleteRec = RM_DeleteRec;
	this->ForcePages = RM_ForcePages;
	this->GetRec = RM_GetRec;
	this->InsertRec = RM_InsertRec;
	this->UpdateRec = RM_UpdateRec;
	return NORMAL;
}
