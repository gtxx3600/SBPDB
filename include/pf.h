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
#include "hashmap.h"

typedef int PageNum;
#define MAX_FILENAME   10
#define  NIL  -2
#define  ALL_PAGES  -1
#define PF_PAGE_SIZE  4092
#define PF_BUFFER_SIZE 40
#define OTHER  1
#define PASS_BREAK  3
#define ALL_PAGE_SIZE  4096

struct PF_PageHandle;

struct PF_FileHandle {

	char filename[MAX_FILENAME];

	int if_open;
	PageNum npage;


};

//    void PF_FileHandle(struct PF_FileHandle *fileHandle);    // Copy constructor

int GetFirstPage(struct PF_PageHandle *pageHandle); // Get the first page
int GetLastPage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle); // Get the last page
int GetNextPage(PageNum current, struct PF_PageHandle *pageHandle); // Get the next page
int GetPrevPage(PageNum current, struct PF_PageHandle *pageHandle);
int GetThisPage(PageNum pageNum, struct PF_PageHandle *pageHandle); // Get a specific page
int AllocatePage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle);
//    RC DisposePage(PageNum pageNum);                   // Dispose of a page
//    RC MarkDirty(PageNum pageNum) const;               // Mark a page as dirty
//    RC UnpinPage(PageNum pageNum) const;               // Unpin a page
//    RC ForcePages(PageNum pageNum = ALL_PAGES) const;  // Write dirty page(s)
//                                                       //   to disk


int SetIfOpen(int bln, struct PF_FileHandle *fileHandle);
int GetIfOpen(struct PF_FileHandle *fileHandle);
int SetNpage(PageNum pn, struct PF_FileHandle *fileHandle);
PageNum GetNpage(struct PF_FileHandle *fileHandle);

//pf manager//

int CreateFile(const char *fileName); // Create a new file
int DestroyFile(const char *fileName); // Destroy a file
int OpenFile(const char *fileName, struct PF_FileHandle *fileHandle);

int CloseFile(struct PF_FileHandle *fileHandle); // Close a file
int AllocateBlock(char *buffer); // Allocate a new scratch page in buffer
int DisposeBlock(char *buffer); // Dispose of a scratch page
//
//pf page handle//
struct PF_PageHandle {

	char filename[MAX_FILENAME];
	PageNum pagenum;
	char *page;

};

int GetData(char **pData, struct PF_PageHandle *pageHandle);
int GetPageNum(PageNum *pageNum, struct PF_PageHandle *pageHandle);

struct Buffer_Data {
	char Buffer_Pool[PF_BUFFER_SIZE * (ALL_PAGE_SIZE)];
	int Buffer_Chain[PF_BUFFER_SIZE * (2 + OTHER)];
	int LRU;
	int MRU;
	 hash_map hm;
//map<string, int> hist;
};

struct Buffer_Data InitBufferData();
//int getdirty(int num);
int getfname(int num);
int getdata(int num);
int delMRU(struct Buffer_Data *bd);
int delLRU(struct Buffer_Data *bd);
int addLRU(int num, struct Buffer_Data *bd);
int addMRU(int num, struct Buffer_Data *bd);
int delChain(int num, struct Buffer_Data *bd);
int writeBack(int num);
int writeBackWithDel(int num, struct Buffer_Data *bd);
int addMap (char* str , int num,struct Buffer_Data *bd);
int delMap (char* str ,struct Buffer_Data *bd);
//int delMap(string str, int num);
//int getMap(string str);
int getMRU(struct Buffer_Data bd);
int getLRU(struct Buffer_Data bd);
int setMRU(int num, struct Buffer_Data *bd);
int setLRU(int num, struct Buffer_Data *bd);

struct Buffer_Data * getBuffer_Data();

#endif /* PF_H_ */
