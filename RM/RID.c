/*
 * RID.c
 *
 *  Created on: 2010-8-22
 *      Author: hhf
 */

#include "rid.h"


RC RID_GetPageNum(RID* this, PageNum *pageNum)
{
	*pageNum = this->pageNum;
	return NORMAL;
}
RC RID_GetSlotNum(RID* this, SlotNum *slotNum)
{
	*slotNum = this->slotNum;
	return NORMAL;
}
RC RID_SetPageNum(RID* this, PageNum pageNum)
{
	this->pageNum = pageNum;
	return NORMAL;
}
RC RID_SetSlotNum(RID* this, SlotNum slotNum)
{
	this->slotNum = slotNum;
	return NORMAL;
}
RC initRID(RID * this, PageNum pageNum, SlotNum slotNum)
{
	this->pageNum = pageNum;
	this->slotNum = slotNum;
	this->GetPageNum = RID_GetPageNum;
	this->GetSlotNum = RID_GetSlotNum;
	this->SetPageNum = RID_SetPageNum;
	this->SetSlotNum = RID_SetSlotNum;
	return NORMAL;
}
