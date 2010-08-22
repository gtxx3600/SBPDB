/*
 * RM_Record.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */
#include "rm.h"

RC RM_GetData	(RM_Record * this, char **pData)
{
	*pData = this->data;
	return NORMAL;
}
RC RM_GetRid	(RM_Record * this, RID *rid)
{
	rid->pageNum = this->rid.pageNum;
	rid->slotNum = this->rid.slotNum;
	return NORMAL;
}
RC initRM_Record(RM_Record * this)
{
	initRID(&this->rid, 0, 0);
	this->GetData = RM_GetData;
	this->GetRid = RM_GetRid;
	return NORMAL;
}
