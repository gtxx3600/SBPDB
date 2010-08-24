/*
 * rid.h
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */

#ifndef RID_H_
#define RID_H_
#include "pf.h"
typedef int SlotNum;
typedef struct RID RID;
struct RID{
	PageNum pageNum;
	SlotNum slotNum;

	RC (*GetPageNum) (const RID* rid, PageNum *pageNum);
	RC (*GetSlotNum) (const RID* rid, SlotNum *slotNum);
	RC (*SetPageNum) (RID* rid, PageNum pageNum);
	RC (*SetSlotNum) (RID* rid, SlotNum slotNum);
};

RC initRID(RID* rid,PageNum pageNum, SlotNum slotNum);
#endif /* RID_H_ */
