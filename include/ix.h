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
typedef struct IX_Manager IX_Manager;
typedef struct IX_IndexHandle IX_IndexHandle;

struct IX_Manager{
	RC CreateIndex  (IX_Manager *this,
						const char *fileName,
						int        indexNo,
						AttrType   attrType,
						int        attrLength);
	RC DestroyIndex (IX_Manager *this,
						const char *fileName,
						int        indexNo);
	RC OpenIndex    (IX_Manager *this,
						const char *fileName,
						int        indexNo,
						IX_IndexHandle *indexHandle);
	RC CloseIndex   (IX_Manager *this,
						IX_IndexHandle *indexHandle);
};

RC initIX_Manager(IX_Manager* this, PF_Manager* pfm);


#endif /* IX_H_ */
