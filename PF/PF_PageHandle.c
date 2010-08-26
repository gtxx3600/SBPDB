/*
 * PF_PageHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "pf.h"
#include <string.h>
RC PF_GetData(PF_PageHandle *this, char **pData);
RC PF_GetPageNum(PF_PageHandle *this, PageNum *pageNum);

RC PF_GetData(PF_PageHandle *this, char **pData) {
	*pData = this->page;
	return NORMAL;
}

RC PF_GetPageNum(PF_PageHandle *this, PageNum *pageNum) {
	*pageNum = this->pagenum;
	return NORMAL;
}

RC initPF_PageHandle(struct PF_PageHandle *this) {
	this->GetData = PF_GetData;
	this->GetPageNum = PF_GetPageNum;
	this->page = (char*) (malloc(ALL_PAGE_SIZE));
	bzero(this->filename, MAX_FILENAME);
	this->pagenum = 0;
	return NORMAL;
}
