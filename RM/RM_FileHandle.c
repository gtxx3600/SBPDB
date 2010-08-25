/*
 * RM_FileHandle.c
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */

#include "rm.h"
void sync(RM_FileHandle* rmfh)
{
	if(!rmfh->modified)
		return;
	PF_PageHandle pageHandle;
	initPF_PageHandle(&pageHandle);
	char* pData;
	rmfh->pf_FileHandle->GetFirstPage(rmfh->pf_FileHandle,&pageHandle);
	pageHandle.GetData(&pageHandle, &pData);
	*(PageNum*)pData = rmfh->firstFree;
	rmfh->pf_FileHandle->MarkDirty(rmfh->pf_FileHandle, pageHandle.pagenum);
	rmfh->pf_FileHandle->UnpinPage(rmfh->pf_FileHandle, pageHandle.pagenum);
	rmfh->pf_FileHandle->ForcePages(rmfh->pf_FileHandle, pageHandle.pagenum);
}
int hasAvailableSlot(RM_FileHandle * rmfh, char* pData)
{
	char* bitmap = &pData[rmfh->bitmappos];
	int tmp;
	int i;
	char cc = '\xff';
	for( i = 0;i < rmfh->slotInOnePage/8;i++)
	{
		if(bitmap[i] != cc)
		{
			return 1;
		}
	}
	if(rmfh->slotInOnePage % 8)
	{
		tmp = 8 - rmfh->slotInOnePage % 8;
		char c = '\xff';
		char b = bitmap[i];
		c = (c << tmp)>>tmp;
		b = (b << tmp)>>tmp;
		if(b != c)
		{
			return 1;
		}
	}
	return 0;
}
RC writeSlot(RM_FileHandle* rmfh, char* pData, const char* src, SlotNum sn)
{
	char * dst = &pData[rmfh->pageHeaderLength + rmfh->recordSize * sn];
	memcpy(dst,src,rmfh->recordSize);
	char* bitmap = &pData[rmfh->bitmappos];
	bitmap[sn / 8] |= 1 << (sn % 8);
	int i =0;
	for(;i<5;i++)
	{
		printf("bitmap %x\n",bitmap[i]);

	}
	if(!hasAvailableSlot(rmfh,pData))
	{
		rmfh->firstFree = *(PageNum*)pData;
		*(PageNum*)pData = 0;
		rmfh->modified = 1;
	}

	return NORMAL;
}
RC updateSlot(RM_FileHandle* rmfh, char* pData, const char* src, SlotNum sn)
{
	char * dst = &pData[rmfh->pageHeaderLength + rmfh->recordSize * sn];
	memcpy(dst,src,rmfh->recordSize);
	return NORMAL;
}
RC delSlot(RM_FileHandle* rmfh, char* pData, PageNum pn, SlotNum sn)
{
	char* bitmap = &pData[rmfh->bitmappos];
	bitmap[sn / 8] &= ~(1 << (sn % 8));
	if(!hasAvailableSlot(rmfh,pData))
	{
		*(PageNum*)pData = rmfh->firstFree;
		rmfh->firstFree = pn;
		rmfh->modified = 1;
	}
	return NORMAL;
}
SlotNum getAvailableSlot(RM_FileHandle * rmfh, char* pData)
{
	SlotNum slt = -1;
	char* bitmap = &pData[rmfh->bitmappos];

	int i,j;
	char c = '\xff';
	for( i = 0;i < rmfh->slotInOnePage/8;i++)
	{
		if((char)bitmap[i] != c)
		{
			int tmp = 1;
			for (j = 0; j< 8; j++)
			{
				if(!(bitmap[i] & tmp))
				{
					slt = 8 * i  + j;
					return slt;
				}
				tmp <<= 1;
			}
		}
	}
	if(bitmap[i] != c)
	{
		int tmp = 1;
		for (j = 0; j<= rmfh->slotInOnePage % 8; j++)
		{
			if(!(bitmap[i] & tmp))
			{
				slt = 8 * i + j;
				return slt;
			}
			tmp <<= 1;
		}
	}
	return slt;
}
RC RM_GetRec	(RM_FileHandle* this, const RID *rid, RM_Record *rec)
{
	PageNum pageNum;
	SlotNum slotNum;
	rid->GetPageNum(rid, &pageNum);
	rid->GetSlotNum(rid, &slotNum);
	PF_PageHandle pfpageHandle;
	initPF_PageHandle(&pfpageHandle);
	char* pData;
	if(pageNum < this->totalPageNum && slotNum < this->slotInOnePage)
	{
		this->pf_FileHandle->GetThisPage(this->pf_FileHandle, pageNum, &pfpageHandle);
		pfpageHandle.GetData(&pfpageHandle, &pData);
		if(pData[this->bitmappos + slotNum/8] & (1 << slotNum % 8)){ //slot not empty
			rec->data = (char*)malloc(this->recordSize);
			memcpy(rec->data, &pData[this->pageHeaderLength + this->recordSize * slotNum], this->recordSize);
			rec->rid.pageNum = pageNum;
			rec->rid.slotNum = slotNum;
			this->pf_FileHandle->UnpinPage(this->pf_FileHandle, pageNum);
		}else
		{
			this->pf_FileHandle->UnpinPage(this->pf_FileHandle, pageNum);
			return RM_EOF;
		}
		return NORMAL;
	}
	else
	{
		return DB_PARAM;
	}

}
RC RM_InsertRec	(RM_FileHandle* this, const char *data, RID *rid)
{
	PF_PageHandle pfpageHandle;
	initPF_PageHandle(&pfpageHandle);
	char* pData;
	int ret;
	SlotNum availableSlot;
	if(this->firstFree)
	{

		if((ret = this->pf_FileHandle->GetThisPage(this->pf_FileHandle, this->firstFree, &pfpageHandle))!= NORMAL)
		{
			return ret;
		}
		pfpageHandle.GetData(&pfpageHandle, &pData);
		availableSlot = getAvailableSlot(this, pData);
		printf("find free slot :%d\n",availableSlot);
	}
	else
	{
		if((ret = this->pf_FileHandle->AllocatePage(this->pf_FileHandle, &pfpageHandle))!= NORMAL)
		{

			return ret;
		}
		this->firstFree = pfpageHandle.pagenum;
		pfpageHandle.GetData(&pfpageHandle, &pData);
		*(int*)pData = 0;
		this->modified = 1;
		availableSlot = 0;
		printf("No slot\n");
	}
#ifdef _DEBUG_

	printf("get Available Slot :page:%d,slot:%d \n",pfpageHandle.pagenum,availableSlot);

#endif
	writeSlot(this, pData, data, availableSlot);
	this->pf_FileHandle->MarkDirty(this->pf_FileHandle, pfpageHandle.pagenum);
	this->pf_FileHandle->UnpinPage(this->pf_FileHandle, pfpageHandle.pagenum);
	rid->pageNum = pfpageHandle.pagenum;
	rid->slotNum = availableSlot;

	sync(this);
	return ret;

}
RC RM_DeleteRec	(RM_FileHandle* this, const RID *rid)
{
	PF_PageHandle pfpageHandle;
	initPF_PageHandle(&pfpageHandle);
	char* pData;
	int ret;
	if((ret = this->pf_FileHandle->GetThisPage(this->pf_FileHandle, rid->pageNum, &pfpageHandle))!= NORMAL)
	{
		return ret;
	}
	pfpageHandle.GetData(&pfpageHandle, &pData);
	delSlot(this, pData, rid->pageNum, rid->slotNum);
	this->pf_FileHandle->MarkDirty(this->pf_FileHandle, pfpageHandle.pagenum);
	this->pf_FileHandle->UnpinPage(this->pf_FileHandle, pfpageHandle.pagenum);

	sync(this);
	return ret;
}
RC RM_UpdateRec	(RM_FileHandle* this, const RM_Record *rec)
{
	PF_PageHandle pfpageHandle;
	initPF_PageHandle(&pfpageHandle);
	char* pData;
	int ret;
	const RID* rid = &rec->rid;
	if((ret = this->pf_FileHandle->GetThisPage(this->pf_FileHandle, rid->pageNum, &pfpageHandle))!= NORMAL)
	{
		return ret;
	}
//	printf(" page num %d ppp %p\n",pfpageHandle.pagenum,pfpageHandle.page);
	pfpageHandle.GetData(&pfpageHandle, &pData);
	updateSlot(this, pData, rec->data, rid->slotNum);
	this->pf_FileHandle->MarkDirty(this->pf_FileHandle, pfpageHandle.pagenum);
	this->pf_FileHandle->UnpinPage(this->pf_FileHandle, pfpageHandle.pagenum);
	return NORMAL;
}
RC RM_ForcePages	(RM_FileHandle* this, PageNum pageNum)
{

	return this->pf_FileHandle->ForcePages(this->pf_FileHandle, pageNum);
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
