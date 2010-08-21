/*
 * RM_FileHandle.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */

#include "rm.h"

RC RM_GetRec	(const RID *rid, RM_Record *rec)
{
	return NORMAL;
}
RC RM_InsertRec	(const char *pData, RID *rid)
{
	return NORMAL;
}
RC RM_DeleteRec	(const RID *rid)
{
	return NORMAL;
}
RC RM_UpdateRec	(const RM_Record *rec)
{
	return NORMAL;
}
RC RM_ForcePages	(PageNum pageNum)
{
	return NORMAL;
}

RC initRM_FileHandle(RM_FileHandle* rmfh)
{
	rmfh->DeleteRec = RM_DeleteRec;
	rmfh->ForcePages = RM_ForcePages;
	rmfh->GetRec = RM_GetRec;
	rmfh->InsertRec = RM_InsertRec;
	rmfh->UpdateRec = RM_UpdateRec;

}
