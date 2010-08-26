/*
 * bufferdata.h
 *
 *  Created on: 2010-8-23
 *      Author: cxy
 */

#ifndef BUFFERDATA_H_
#define BUFFERDATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "sbpdb.h"
#define MIN_POOL_SIZE 40
#define KEY_LENGTH 20

typedef struct Buffer_Data Buffer_Data;
typedef struct Page_Buffer Page_Buffer;


struct Buffer_Data {
	Page_Buffer *lpin_page;
	Page_Buffer *lunpin_page;
	Page_Buffer *funpin_page;
	Page_Buffer *pin_header;
	Page_Buffer *unpin_header;
	int pin_num;
	int unpin_num;
	int page_num;
	hash_map pagemap;
	int init;
	int (*writeBack)(Buffer_Data *this,Page_Buffer *pb);
	int (*copyBack)(Buffer_Data *this,Page_Buffer *pb);

	int (*addMap)(Buffer_Data *this, char* key, Page_Buffer *pb);
	int (*delMap)(Buffer_Data *this, char* key);
	Page_Buffer* (*getMap)(Buffer_Data *this, char* key);
	int (*unpinPage)(Buffer_Data *this, Page_Buffer *pb);
	int (*pinPage)(Buffer_Data *this, Page_Buffer *pb);
	int (*disposePB)(Buffer_Data *this, Page_Buffer *pb,char* tmp);
	int (*allocPage)(Buffer_Data *this, char* filename, int pagenum);
};
int initBuffer_Data(Buffer_Data *this);
Buffer_Data *getBuffer_Data();
struct Page_Buffer {
	int dirty;
	int pagenum;
	char* filename;
	Page_Buffer *next_page;
	Page_Buffer *prev_page;
	char* pagedata;
	char* key;
	int pinned;
};
int createpage(Page_Buffer *pb, char* filename, int pagenum);
int initPage_Buffer();
#endif /* BUFFERDATA_H_ */
