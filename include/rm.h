/*
 * RM.h
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */

#ifndef RM_H_
#define RM_H_
#include "pf.h"
#include "rid.h"
#include "sbpdb.h"

typedef struct RM_Manager RM_Manager;
typedef struct RM_FileHandle RM_FileHandle;
typedef struct RM_Record RM_Record;
typedef struct RM_FileScan RM_FileScan;
typedef int RecordSize;

#define FIRST_AVAILABLE_PAGE	1
struct RM_Manager {
	PF_Manager *pf_Manager;
	RC (*CreateFile)(RM_Manager* rmm, const char* fileName, int recordSize);
	RC (*DestroyFile)(RM_Manager* rmm, const char *fileName);
	RC (*OpenFile)(RM_Manager* rmm, const char *fileName, RM_FileHandle *fileHandle);
	RC (*CloseFile)(RM_Manager* rmm, RM_FileHandle *fileHandle);
};

RC initRM_Manager(RM_Manager* rmm, struct PF_Manager * pfm);

struct RM_FileHandle {

	RecordSize recordSize;
	int opened;
	int modified;
	int pageHeaderLength;
	int bitmappos;
	//PageNum totalPageNum;
	SlotNum totalSlotNum;
	SlotNum slotInOnePage;
	PageNum firstFree;
	PF_FileHandle *pf_FileHandle;

	RC (*GetRec)		(RM_FileHandle* rmfh, const RID *rid, RM_Record *rec); 	// Get a record
	RC (*InsertRec)	(RM_FileHandle* rmfh, const char *pData, RID *rid); 		// Insert a new record
	RC (*DeleteRec)	(RM_FileHandle* rmfh, const RID *rid); 					// Delete a record
	RC (*UpdateRec)	(RM_FileHandle* rmfh, const RM_Record *rec); 				// Update a record
	RC (*ForcePages)	(RM_FileHandle* rmfh, PageNum pageNum); 					// Write dirty page(s)
};

RC initRM_FileHandle(RM_FileHandle* rmfh);

struct RM_Record {
	RID rid;
	char* data;
	int recordSize;

	RC (*GetData)	(RM_Record * rmr, char **pData);   // Set pData to point to
	RC (*GetRid)	(RM_Record * rmr, RID *rid);       // Get the record id
};

RC initRM_Record(RM_Record * rmr);

struct RM_FileScan{
	AttrType at;
	int al;
	int ao;
	CompOp op;
	void* value;
	ClientHint ch;

	RM_FileHandle * rmfh;
	int start;
	RID crid;				//current RID
	RC (*OpenScan)     (RM_FileScan *rmfs,
							RM_FileHandle *fileHandle,
							AttrType      attrType,
							int           attrLength,
							int           attrOffset,
							CompOp        compOp,
							void          *value,
							ClientHint    pinHint);

	RC (*GetNextRec)   (RM_FileScan *rmfs, RM_Record *rec);
	RC (*CloseScan)    (RM_FileScan *rmfs);
	void (*NextRID)    (RM_FileScan *rmfs);
	int (*opfunc)      (void* left, void* right, int len);
};

RC initRM_FileScan(RM_FileScan *rmfs);
#endif /* RM_H_ */
