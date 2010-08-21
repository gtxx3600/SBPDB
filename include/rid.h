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
typedef struct RID{
	RC (*GetPageNum) (PageNum *pageNum);  // Return page number
	RC (*GetSlotNum) (SlotNum *slotNum);  // Return slot number
}RID;

RC initRID(PageNum pageNum, SlotNum slotNum);
#endif /* RID_H_ */
