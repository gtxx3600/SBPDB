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


struct PF_PageHandle;

struct PF_FileHandle {

	char filename[MAX_FILENAME];

	int if_open;
	PageNum npage;
	RC (*GetFirstPage)(struct PF_PageHandle *pageHandle); // Get the first page
	RC (*GetLastPage)(struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle); // Get the last page
	RC (*GetNextPage)(PageNum current, struct PF_PageHandle *pageHandle); // Get the next page
	RC (*GetPrevPage)(PageNum current, struct PF_PageHandle *pageHandle);
	RC (*GetThisPage)(PageNum pageNum, struct PF_PageHandle *pageHandle); // Get a specific page
	RC (*AllocatePage)(struct PF_PageHandle *pageHandle,
			struct PF_FileHandle *fileHandle);
	RC (*SetIfOpen)(int bln, struct PF_FileHandle *fileHandle);
	RC (*GetIfOpen)(struct PF_FileHandle *fileHandle);
	RC (*SetNpage)(PageNum pn, struct PF_FileHandle *fileHandle);
	PageNum (*GetNpage)(struct PF_FileHandle *fileHandle);
};

//    void PF_FileHandle(struct PF_FileHandle *fileHandle);    // Copy constructor


//    RC DisposePage(PageNum pageNum);                   // Dispose of a page
//    RC MarkDirty(PageNum pageNum) const;               // Mark a page as dirty
//    RC UnpinPage(PageNum pageNum) const;               // Unpin a page
//    RC ForcePages(PageNum pageNum = ALL_PAGES) const;  // Write dirty page(s)
//                                                       //   to disk




//pf manager//
struct PF_MANAGER{
RC (*CreateFile)(const char *fileName); // Create a new file
RC (*DestroyFile)(const char *fileName); // Destroy a file
RC (*OpenFile)(const char *fileName, struct PF_FileHandle *fileHandle);

RC (*CloseFile)(struct PF_FileHandle *fileHandle); // Close a file
RC (*AllocateBlock)(char *buffer); // Allocate a new scratch page in buffer
RC (*DisposeBlock)(char *buffer); // Dispose of a scratch page
};

//pf page handle//
struct PF_PageHandle {

	char filename[MAX_FILENAME];
	PageNum pagenum;
	char *page;

	RC (*GetData)(char **pData, struct PF_PageHandle *pageHandle);
	RC (*GetPageNum)(PageNum *pageNum, struct PF_PageHandle *pageHandle);
};







#endif /* PF_H_ */
