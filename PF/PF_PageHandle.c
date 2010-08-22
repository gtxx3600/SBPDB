/*
 * PF_PageHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "pf.h"
RC PF_GetData(PF_PageHandle *this,char **pData);
RC PF_GetPageNum(PF_PageHandle *this,PageNum *pageNum);

RC PF_GetData(PF_PageHandle *this,char **pData){
	*pData = this->page;
	return NORMAL;
}

RC PF_GetPageNum(PF_PageHandle *this,PageNum *pageNum){
	*pageNum = this->pagenum;
	printf("%d", *pageNum);
	return NORMAL;
}

RC initPF_PageHandle(struct PF_PageHandle * this) {
	this->GetData = PF_GetData;
	this->GetPageNum = PF_GetPageNum;
	this->page = (char*) (malloc(4096));
	int i =0;
	for (i = 0; i < MAX_FILENAME; i++) {
			this->filename[i] = '\0';
	}
	this->pagenum = 0;
	return NORMAL;
}
