/*
 * PF_FileHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */
#include "../include/pf.h"

int GetFirstPage(struct PF_PageHandle *pageHandle) {
	return GetThisPage(0, pageHandle);
}

int GetLastPage(struct PF_PageHandle *pageHandle,struct PF_FileHandle *fileHandle) {
	return GetThisPage(fileHandle->npage - 1, pageHandle);
}

int GetNextPage(PageNum current, struct PF_PageHandle *pageHandle) {
	return GetThisPage(current + 1, pageHandle);
}

int GetPrevPage(PageNum current, struct PF_PageHandle *pageHandle) {
	return GetThisPage(current - 1, pageHandle);
}

int GetThisPage(PageNum pageNum,struct PF_PageHandle *pageHandle) {


	return 0;
}

int AllocatePage(struct PF_PageHandle *pageHandle,struct PF_FileHandle *fileHandle){
	fileHandle->npage++;
//	string str(filename);
//	char* p = &str[0];
//	fstream ff(p);
//	ff.seekp(0, ios::beg);
//	ff << npage << endl;
//	ff.close();
	return (GetThisPage(fileHandle->npage - 1, pageHandle));
}

int SetIfOpen(int bln, struct PF_FileHandle *fileHandle) {
	fileHandle->if_open = bln;
	return 0;
}

int GetIfOpen(struct PF_FileHandle *fileHandle) {
	return (fileHandle->if_open);
}

int SetNpage(PageNum pn, struct PF_FileHandle *fileHandle) {
	fileHandle->npage = pn;
	return 0;
}

PageNum GetNpage(struct PF_FileHandle *fileHandle) {
	return (fileHandle->npage);
}
