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

typedef struct _IX_Manager IX_Manager;
typedef struct _IX_IndexHandle IX_IndexHandle;
typedef struct _IX_IndexScan IX_IndexScan;
typedef struct IX_HeadPage{
	int indexNo;
	int attrLength;
	int maxEntryInNode;
	AttrType attrType;
	PageNum root;

	char reserved[4096 - 2*sizeof(int) - sizeof(AttrType) - sizeof(PageNum)];
}IX_HeadPage;
struct _IX_Manager{
	PF_Manager* pfm;
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

struct _IX_IndexHandle{
	IX_HeadPage head;
	PF_FileHandle pffh;

    RC (*InsertEntry)     (IX_IndexHandle* this, void *pData, const RID *rid);  // Insert new index entry
    RC (*DeleteEntry)     (IX_IndexHandle* this, void *pData, const RID *rid);  // Delete index entry
    RC (*ForcePages)      (IX_IndexHandle* this);                             // Copy index to disk
};

RC initIX_IndexHandle(IX_IndexHandle* this);

struct _IX_IndexScan{
	IX_IndexHandle *idxh;
	CompOp op;
	void* value;
	ClientHint ch;
	PageNum curr;
	PageNum left;
	PageNum right;
	int curr_offset;
	int left_offset;
	int right_offset;
	int inEQregin;
	int end;
	RC (*OpenScan)		(IX_IndexScan* this,
								IX_IndexHandle *idxh,
								CompOp op,
								void* value,
								ClientHint ch);
	RC (*GetNextEntry)	(IX_IndexScan* this, RID *rid);
	RC (*CloseScan)		(IX_IndexScan* this);
};

RC initIX_IndexScan(IX_IndexScan *this);
#endif /* IX_H_ */
