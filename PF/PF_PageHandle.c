/*
 * PF_PageHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "../include/pf.h"


int GetData(char **pData,struct PF_PageHandle *pageHandle) {
	*pData = pageHandle->page;
	return 0;
}

int GetPageNum(PageNum *pageNum,struct PF_PageHandle *pageHandle) {
	*pageNum = pageHandle->pagenum;
	printf("%d",*pageNum);
	return 0;
}
