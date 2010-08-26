/*
 * IX_IndexHandle.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */
#include "ix.h"
#include "BTNode.h"
RC IX_InsertEntry     (IX_IndexHandle* this, void *pData, const RID *rid){
	return insertIntoRoot(&this->head, &this->pffh, pData, rid);
}
RC IX_DeleteEntry     (IX_IndexHandle* this, void *pData, const RID *rid){
	return NORMAL;
}
RC IX_ForcePages      (IX_IndexHandle* this){
	return NORMAL;
}


RC initIX_IndexHandle(IX_IndexHandle * this)
{
	this->DeleteEntry = IX_DeleteEntry;
	this->ForcePages = IX_ForcePages;
	this->InsertEntry = IX_InsertEntry;
	return NORMAL;
}
