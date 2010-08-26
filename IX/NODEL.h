/*
 * NODEL.h
 *
 *  Created on: 2010-8-25
 *      Author: hhf
 */

#include "ix.h"
typedef struct EntryValue{
	char value[attrLength];
}EntryValue;
typedef struct EntryPointer{
	PageNum page;
	SlotNum slot;
}EntryPointer;
int ENTRYSINBTNODE = (ALL_PAGE_SIZE - sizeof(EntryPointer) - 2*sizeof(int))/(sizeof(EntryValue) + sizeof(EntryPointer));
typedef struct NODE{
	int level;
	int totalEntry;
	EntryValue values[ENTRYSINBTNODE];
	EntryPointer pointers[ENTRYSINBTNODE];
	char reserved[4096 - ENTRYSINBTNODE*(sizeof(EntryValue) + sizeof(EntryPointer)) - sizeof(EntryPointer) - 2*sizeof(int)];
}NODE;

