/*
 * RM.h
 *
 *  Created on: 2010-8-21
 *      Author: hhf
 */

#ifndef RM_H_
#define RM_H_
struct PF_Manager;
#include "pf.h"
#include "sbpdb.h"

struct RM_Manager;
struct RM_FileHandle;

typedef struct RM_Manager{
	RC (*CreateFile)	(const char* fileName, int recordSize);
	RC (*DestroyFile)	(const char *fileName);
	RC (*OpenFile)	(const char *fileName,struct RM_FileHandle *fileHandle);
	RC (*CloseFile)	(struct RM_FileHandle *fileHandle);
}RM_Manager;

RC initRM_Manager(RM_Manager* rmm, struct PF_Manager * pfm);

typedef struct RM_FileHandle{

}RM_FileHandle;

RC initRM_FileHandle(RM_FileHandle* rmfh);
#endif /* RM_H_ */
