/*
 * ix.h
 *
 *  Created on: 2010-8-22
 *      Author: hhf
 */

#ifndef IX_H_
#define IX_H_

#include "sbpdb.h"
#include "pf.h"
#include "rid.h"
typedef struct IX_Manager IX_Manager;
typedef struct IX_IndexHandle IX_IndexHandle;
typedef struct IX_IndexScan IX_IndexScan;
struct IX_Manager{
	RC (*CreateIndex)(IX_Manager *this,
						const char *fileName,
						int        indexNo,
						AttrType   attrType,
						int        attrLength);
	RC (*DestroyIndex)(IX_Manager *this,
						const char *fileName,
						int        indexNo);
	RC (*OpenIndex)  (IX_Manager *this,
						const char *fileName,
						int        indexNo,
						IX_IndexHandle *indexHandle);
	RC (*CloseIndex) (IX_Manager *this,
						IX_IndexHandle *indexHandle);
};

RC initIX_Manager(IX_Manager* this, PF_Manager* pfm);

struct IX_IndexHandle{
    RC (*InsertEntry)     (IX_IndexHandle* this, void *pData, const RID *rid);  // Insert new index entry
    RC (*DeleteEntry)     (IX_IndexHandle* this, void *pData, const RID *rid);  // Delete index entry
    RC (*ForcePages)      (IX_IndexHandle* this);                             // Copy index to disk
};

RC initIX_IndexHandle(IX_IndexHandle* this);

struct IX_IndexScan{
	RC (*OpenScan)		(IX_IndexScan* this,
								const IX_IndexHandle *idxh,
								CompOp op,
								void* value,
								ClientHint ch);
	RC (*GetNextEntry)	(IX_IndexScan* this, RID *rid);
	RC (*CloseScan)		(IX_IndexScan* this);
};

RC initIX_IndexScan(IX_IndexScan *this);
#endif /* IX_H_ */
