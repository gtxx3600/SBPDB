/*
 * map.h
 *
 *  Created on: 2010-8-26
 *      Author: cxy
 */

#ifndef MAP_H_
#define MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node node;
typedef struct map map;
 struct node
{
	node* prev;
	node* next;
	void* data;
};
 struct map
{
	node* kheader;
	node* ktail;
	node* vheader;
	node* vtail;
};

int mapinsert(map* this,char* key,void* value);
int mapdelete(map* this,char* key);
void* getvalue(map* this,char* key);
int initMap(map* this);

#endif /* MAP_H_ */
