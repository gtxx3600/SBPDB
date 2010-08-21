/*
 * PF_PageHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "pf.h"


RC PF_GetData(char **pData,struct PF_PageHandle *pageHandle) {
	*pData = pageHandle->page;
	return NORMAL;
}

RC PF_GetPageNum(PageNum *pageNum,struct PF_PageHandle *pageHandle) {
	*pageNum = pageHandle->pagenum;
	printf("%d",*pageNum);
	return NORMAL;
}
