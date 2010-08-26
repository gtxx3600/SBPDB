/*
 * NODENL.h
 *
 *  Created on: 2010-8-25
 *      Author: hhf
 */

#include "ix.h"
typedef struct NODEENTRY{
	PageNum page;
	char value[attrLength];
}NODEENTRY;
int ENTRYSINBTNODE = (ALL_PAGE_SIZE-2*sizeof(PageNum)-2*sizeof(int))/sizeof(NODEENTRY);
typedef struct NODE{
	int level;
	int totalEntry;
	PageNum brother;
	NODEENTRY entrys[ENTRYSINBTNODE];
	char reserved[4096 - ENTRYSINBTNODE*sizeof(NODEENTRY) - 2*sizeof(PageNum) - 2*sizeof(int)];
}NODE;
