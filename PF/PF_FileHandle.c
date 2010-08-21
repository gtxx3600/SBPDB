/*
 * PF_FileHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */
#include "pf.h"
#include "bufferdata.h"

RC GetFirstPage(struct PF_PageHandle *pageHandle); // Get the first page
RC GetLastPage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle); // Get the last page
RC GetNextPage(PageNum current, struct PF_PageHandle *pageHandle); // Get the next page
RC GetPrevPage(PageNum current, struct PF_PageHandle *pageHandle);
RC GetThisPage(PageNum pageNum, struct PF_PageHandle *pageHandle); // Get a specific page
RC AllocatePage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle);
RC SetIfOpen(int bln, struct PF_FileHandle *fileHandle);
RC GetIfOpen(struct PF_FileHandle *fileHandle);
RC SetNpage(PageNum pn, struct PF_FileHandle *fileHandle);
PageNum GetNpage(struct PF_FileHandle *fileHandle);
RC DisposePage (PageNum pageNum) ;
RC GetFirstPage(struct PF_PageHandle *pageHandle) {
	return GetThisPage(0, pageHandle);
}

RC GetLastPage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle) {
	return GetThisPage(fileHandle->npage - 1, pageHandle);
}

RC GetNextPage(PageNum current, struct PF_PageHandle *pageHandle) {
	return GetThisPage(current + 1, pageHandle);
}

RC GetPrevPage(PageNum current, struct PF_PageHandle *pageHandle) {
	return GetThisPage(current - 1, pageHandle);
}

RC GetThisPage(PageNum pageNum, struct PF_PageHandle *pageHandle) {

	return 0;
}

RC AllocatePage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle) {
	fileHandle->npage++;
	//	string str(filename);
	//	char* p = &str[0];
	//	fstream ff(p);
	//	ff.seekp(0, ios::beg);
	//	ff << npage << endl;
	//	ff.close();
	return (GetThisPage(fileHandle->npage - 1, pageHandle));
}

RC DisposePage (PageNum pageNum)
{
	return 0;
}

RC MarkDirty(PageNum pageNum,PF_FileHandle *fileHandle) {
	Buffer_Data *theBD;
	char strPageNum[10];
	sprintf(strPageNum, "%d", pageNum);
	strcat(fileHandle->filename,strPageNum);

	int num = theBD->getMap(fileHandle->filename,theBD);

	theBD->dirty[num] = 1;
	return (NORMAL);
}
RC SetIfOpen(int bln, struct PF_FileHandle *fileHandle) {
	fileHandle->if_open = bln;
	return NORMAL;
}

RC GetIfOpen(struct PF_FileHandle *fileHandle) {
	return (fileHandle->if_open);
}

RC SetNpage(PageNum pn, struct PF_FileHandle *fileHandle) {
	fileHandle->npage = pn;
	return NORMAL;
}

PageNum GetNpage(struct PF_FileHandle *fileHandle) {
	return (fileHandle->npage);
}
