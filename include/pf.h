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
#define MAX_FILENAME   10
#define PF_BUFFER_SIZE 40

struct PF_PageHandle;

typedef struct PF_FileHandle {

	char filename[MAX_FILENAME];
	int mapnum[PF_BUFFER_SIZE];
	int if_open;
	PageNum npage;
	RC (*GetFirstPage)(struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle); // Get the first page
	RC (*GetLastPage)(struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle); // Get the last page
	RC (*GetNextPage)(PageNum current, struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle); // Get the next page
	RC (*GetPrevPage)(PageNum current, struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle);
	RC (*GetThisPage)(PageNum pageNum, struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle); // Get a specific page
	RC (*AllocatePage)(struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle);
	RC (*SetIfOpen)(int bln, struct PF_FileHandle *fileHandle);
	RC (*GetIfOpen)(struct PF_FileHandle *fileHandle);
	RC (*SetNpage)(PageNum pn, struct PF_FileHandle *fileHandle);
	RC (*MarkDirty)(PageNum pageNum,struct PF_FileHandle *fileHandle);
	RC (*UnpinPage)(PageNum pageNum,struct PF_FileHandle *fileHandle);
	RC (*ForcePages)(PageNum pageNum,struct PF_FileHandle *fileHandle);
	PageNum (*GetNpage)(struct PF_FileHandle *fileHandle);
} PF_FileHandle;
RC initPF_FileHandle( struct PF_FileHandle *fh);

//    void PF_FileHandle(struct PF_FileHandle *fileHandle);    // Copy constructor


//    RC DisposePage(PageNum pageNum);                   // Dispose of a page
//    RC MarkDirty(PageNum pageNum) const;               // Mark a page as dirty
//    RC UnpinPage(PageNum pageNum) const;               // Unpin a page
//    RC ForcePages(PageNum pageNum = ALL_PAGES) const;  // Write dirty page(s)
//                                                       //   to disk


//pf manager//
typedef struct PF_Manager {
	RC (*CreateFile)(const char *fileName); // Create a new file
	RC (*DestroyFile)(const char *fileName); // Destroy a file
	RC (*OpenFile)(const char *fileName, struct PF_FileHandle *fileHandle);

	RC (*CloseFile)(struct PF_FileHandle *fileHandle); // Close a file
	RC (*AllocateBlock)(char *buffer); // Allocate a new scratch page in buffer
	RC (*DisposeBlock)(char *buffer); // Dispose of a scratch page
} PF_Manager;

RC initPF_Manager( struct PF_Manager * pfm);

//pf page handle//
typedef struct PF_PageHandle {

	char filename[MAX_FILENAME];
	PageNum pagenum;
	char *page;

	RC (*GetData)(char **pData, struct PF_PageHandle *pageHandle);
	RC (*GetPageNum)(PageNum *pageNum, struct PF_PageHandle *pageHandle);
} PF_PageHandle;
RC initPF_PageHandle( struct PF_PageHandle * ph);

#endif /* PF_H_ */
