/*
 * pf.h
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#ifndef PF_H_
#define PF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sbpdb.h"

typedef int PageNum;

#define PF_BUFFER_SIZE 40
typedef struct PF_Manager PF_Manager;
typedef struct PF_PageHandle PF_PageHandle;
typedef struct PF_FileHandle PF_FileHandle;

struct PF_FileHandle {
	int currentPage;
	char filename[MAX_FILENAME];
	int mapnum[PF_BUFFER_SIZE];
	int if_open;
	PageNum npage;
	RC (*GetFirstPage)(PF_FileHandle *this, PF_PageHandle *pageHandle); // Get the first page
	RC (*GetLastPage)(PF_FileHandle *this, PF_PageHandle *pageHandle); // Get the last page
	RC (*GetNextPage)(PF_FileHandle *this, struct PF_PageHandle *pageHandle); // Get the next page
	RC (*GetPrevPage)(PF_FileHandle *this, struct PF_PageHandle *pageHandle);
	RC (*GetThisPage)(PF_FileHandle *this, PageNum pageNum,
			struct PF_PageHandle *pageHandle); // Get a specific page
	RC (*AllocatePage)(PF_FileHandle *this, struct PF_PageHandle *pageHandle);
	RC (*SetIfOpen)(PF_FileHandle *this, int bln);
	RC (*GetIfOpen)(PF_FileHandle *this);
	RC (*SetNpage)(PF_FileHandle *this, PageNum pn);
	RC (*MarkDirty)(PF_FileHandle *this, PageNum pageNum);
	RC (*UnpinPage)(PF_FileHandle *this, PageNum pageNum);
	RC (*ForcePages)(PF_FileHandle *this, PageNum pageNum);
	PageNum (*GetNpage)(PF_FileHandle *this);
};

RC initPF_FileHandle(PF_FileHandle *this);

//pf manager//
struct PF_Manager {
	RC (*CreateFile)(PF_Manager *this, const char *fileName); // Create a new file
	RC (*DestroyFile)(PF_Manager *this, const char *fileName); // Destroy a file
	RC (*OpenFile)(PF_Manager *this, const char *fileName,
			struct PF_FileHandle *fileHandle);

	RC (*CloseFile)(PF_Manager *this, struct PF_FileHandle *fileHandle); // Close a file
	RC (*AllocateBlock)(PF_Manager *this, char *buffer); // Allocate a new scratch page in buffer
	RC (*DisposeBlock)(PF_Manager *this, char *buffer); // Dispose of a scratch page
};

RC initPF_Manager(PF_Manager * this);

//pf page handle//
struct PF_PageHandle {

	char filename[MAX_FILENAME];
	PageNum pagenum;
	char *page;

	RC (*GetData)(PF_PageHandle *this, char **pData);
	RC (*GetPageNum)(PF_PageHandle *this, PageNum *pageNum);
};
RC initPF_PageHandle(PF_PageHandle * this);

#endif /* PF_H_ */
