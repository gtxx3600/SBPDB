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

struct RM_Manager {
	PF_Manager *pf_Manager;
	RC (*CreateFile)(const char* fileName, int recordSize);
	RC (*DestroyFile)(const char *fileName);
	RC (*OpenFile)(const char *fileName, struct RM_FileHandle *fileHandle);
	RC (*CloseFile)(struct RM_FileHandle *fileHandle);
};

RC initRM_Manager(RM_Manager* rmm, struct PF_Manager * pfm);

struct RM_FileHandle {

	int recordSize;
	int opened;
	int modified;

	PageNum pageNum;
	SlotNum slotNum;
	PF_FileHandle *pf_FileHandle;

	RC (*GetRec)		(const RID *rid, RM_Record *rec); 	// Get a record
	RC (*InsertRec)	(const char *pData, RID *rid); 		// Insert a new record
	RC (*DeleteRec)	(const RID *rid); 					// Delete a record
	RC (*UpdateRec)	(const RM_Record *rec); 				// Update a record
	RC (*ForcePages)	(PageNum pageNum); 					// Write dirty page(s)
};

RC initRM_FileHandle(RM_FileHandle* rmfh);

struct RM_Record {
	RID rid;
	RC (*GetData)	(char **pData);   // Set pData to point to
	RC (*GetRid)	(RID *rid);       // Get the record id
};

RC initRM_Record(RM_Record * rmr);
#endif /* RM_H_ */
