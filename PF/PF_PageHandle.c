/*
 * PF_PageHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "pf.h"
RC PF_GetData(char **pData, struct PF_PageHandle *pageHandle);
RC PF_GetPageNum(PageNum *pageNum, struct PF_PageHandle *pageHandle);

RC PF_GetData(char **pData, struct PF_PageHandle *pageHandle) {
	*pData = pageHandle->page;
	return NORMAL;
}

RC PF_GetPageNum(PageNum *pageNum, struct PF_PageHandle *pageHandle) {
	*pageNum = pageHandle->pagenum;
	printf("%d", *pageNum);
	return NORMAL;
}

RC initPF_PageHandle(struct PF_PageHandle * ph) {
	ph->GetData = PF_GetData;
	ph->GetPageNum = PF_GetPageNum;
	ph->page = (char*) (malloc(4096));
	int i =0;
	for (i = 0; i < MAX_FILENAME; i++) {
			ph->filename[i] = '\0';
	}
	ph->pagenum = 0;
	return NORMAL;
}
