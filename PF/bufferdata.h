/*
 * bufferdata.h
 *
 *  Created on: 2010-8-21
 *      Author: cxy
 */

#ifndef BUFFERDATA_H_
#define BUFFERDATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "sbpdb.h"
#define  NIL  -2
#define  ALL_PAGES  -1
#define PF_PAGE_SIZE  4092
#define PF_BUFFER_SIZE 40
#define OTHER  1
#define PASS_BREAK  3
#define ALL_PAGE_SIZE  4096
struct Buffer_Data {
	char Buffer_Pool[PF_BUFFER_SIZE * (ALL_PAGE_SIZE)];
	int Buffer_Chain[PF_BUFFER_SIZE * (2 + OTHER)];
	int LRU;
	int MRU;
	hash_map hm;
	char* fname[PF_BUFFER_SIZE];
	int dirty[PF_BUFFER_SIZE];
	int (*getdata)(int num);
	int (*delMRU)(struct Buffer_Data);
	int (*delLRU)(struct Buffer_Data *bd);
	int (*addLRU)(int num, struct Buffer_Data *bd);
	int (*addMRU)(int num, struct Buffer_Data *bd);
	int (*delChain)(int num, struct Buffer_Data *bd);
	int (*writeBack)(int num, struct Buffer_Data *bd);
	int (*writeBackWithDel)(int num, struct Buffer_Data *bd);
	int (*addMap)(char* str, int num, struct Buffer_Data *bd);
	int (*delMap)(char* str, struct Buffer_Data *bd);
	int (*getMap)(char* str, struct Buffer_Data *bd);
	int (*getMRU)(struct Buffer_Data bd);
	int (*getLRU)(struct Buffer_Data bd);
	int (*setMRU)(int num, struct Buffer_Data *bd);
	int (*setLRU)(int num, struct Buffer_Data *bd);
};

RC initBuffer_Data(struct Buffer_Data *bd);
#endif /* BUFFERDATA_H_ */
