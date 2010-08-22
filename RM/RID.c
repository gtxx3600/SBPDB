/*
 * RID.c
 *
 *  Created on: 2010-8-22
 *      Author: hhf
 */

#include "rid.h"


RC RID_GetPageNum(RID* rid, PageNum *pageNum)
{
	*pageNum = rid->pageNum;
	return NORMAL;
}
RC RID_GetSlotNum(RID* rid, SlotNum *slotNum)
{
	*slotNum = rid->slotNum;
	return NORMAL;
}
RC RID_SetPageNum(RID* rid, PageNum pageNum)
{
	rid->pageNum = pageNum;
	return NORMAL;
}
RC RID_SetSlotNum(RID* rid, SlotNum slotNum)
{
	rid->slotNum = slotNum;
	return NORMAL;
}
RC initRID(RID * rid, PageNum pageNum, SlotNum slotNum)
{
	rid->pageNum = pageNum;
	rid->slotNum = slotNum;
	rid->GetPageNum = RID_GetPageNum;
	rid->GetSlotNum = RID_GetSlotNum;
	rid->SetPageNum = RID_SetPageNum;
	rid->SetSlotNum = RID_SetSlotNum;
	return NORMAL;
}
