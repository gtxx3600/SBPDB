/*
 * RM_FileScan.c
 *
 *  Created on: 2010-8-22
 *      Author: hhf
 */

#include "rm.h"

extern int (*intOP[OP_NUM])(void* left,void*right,int len);
extern int (*floatOP[OP_NUM])(void* left,void*right,int len);
extern	int (*strOP[OP_NUM])(void* left,void*right,int len);

extern int (**typeOP[TYPE_NUM])(void* left,void*right,int len);
void RM_NextRID(RM_FileScan *this)
{
	RID *rid = &this->crid;
	rid->slotNum++;
	if(rid->slotNum >= this->rmfh->slotInOnePage)
	{
		rid->slotNum = 0;
		rid->pageNum ++;
	}
}
RC RM_OpenScan(RM_FileScan *this,
RM_FileHandle *fileHandle,
AttrType      attrType,
int           attrLength,
int           attrOffset,
CompOp        compOp,
void          *value,
ClientHint    pinHint)
{
	this->al = attrLength;
	this->at = attrType;
	this->ao = attrOffset;
	this->op = compOp;
	this->value = value;
	this->ch = pinHint;
	this->rmfh = fileHandle;
	this->start = 1;
	initRID(&this->crid, FIRST_AVAILABLE_PAGE, -1);
	this->opfunc = typeOP[attrType][compOp];
	return NORMAL;
}

RC RM_GetNextRec(RM_FileScan *this, RM_Record *rec)
{
	rec->data = NULL;
	while(1)
	{
		this->NextRID(this);

		if(this->crid.pageNum >=  this->rmfh->pf_FileHandle->GetNpage(this->rmfh->pf_FileHandle))
		{
			return RM_EOF;
		}
		if(rec->data)free(rec->data);
		int ret = this->rmfh->GetRec(this->rmfh, &this->crid, rec);
		if(ret == RM_EOF)
		{
			continue;
		}else if(ret == NORMAL)
		{
			if(this->value == NULL)
			{
				return NORMAL;
			}
			else
			{
				if(this->opfunc(&rec->data[this->ao],this->value, this->al))
				{
					return NORMAL;
				}else
				{
					continue;
				}
			}
		}else
		{
			return DB_INTERNAL;
		}
	}
	return NORMAL;
}
RC RM_CloseScan(RM_FileScan *this)
{
	this->opfunc = NULL;
	this->al = 0;
	this->ao = 0;
	this->at = 0;
	this->crid.pageNum = 0;
	this->crid.slotNum = -1;
	this->op = 0;
	return NORMAL;
}
RC initRM_FileScan(RM_FileScan *this)
{
	this->CloseScan = RM_CloseScan;
	this->GetNextRec = RM_GetNextRec;
	this->OpenScan = RM_OpenScan;
	this->NextRID = RM_NextRID;
	return NORMAL;
}
