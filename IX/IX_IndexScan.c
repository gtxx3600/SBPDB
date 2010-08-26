/*
 * IX_IndexScan.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */
#include "ix.h"

RC IX_OpenScan(IX_IndexScan* this, const IX_IndexHandle *idxh, CompOp op, void* value, ClientHint ch)
{

	return NORMAL;
}
RC IX_GetNextEntry(IX_IndexScan* this, RID *rid)
{
	return NORMAL;
}
RC IX_CloseScan(IX_IndexScan* this)
{
	return NORMAL;
}

RC initIX_IndexScan(IX_IndexScan *this)
{
	this->OpenScan = IX_OpenScan;
	this->GetNextEntry = IX_GetNextEntry;
	this->CloseScan = IX_CloseScan;
	return NORMAL;
}
