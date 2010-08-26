/*
 * map.c
 *
 *  Created on: 2010-8-26
 *      Author: cxy
 */
#include"map.h"
#include"bufferdata.h"

int initMap(map* this) {
	this->kheader = (node*) malloc(sizeof(node));
	this->kheader->data = (void*) malloc(20);
	this->kheader->next =NULL;
	this->ktail = this->kheader;

	this->vheader = (node*) malloc(sizeof(node));
	this->vheader->data = (void*) malloc(200);
	this->vheader->next =NULL;
	this->vtail = this->vheader;
	return 0;
}

int mapinsert(map* this, char* key, void* value) {
	node* tmp = (node*) malloc(sizeof(node));
	tmp->data = (void*) malloc(20);
//	tmp->data = key;
	memcpy(tmp->data,key,20);
	tmp->prev = this->ktail;
	this->ktail->next = tmp;
	tmp->next = NULL;
	this->ktail = tmp;
	node* vtmp = (node*) malloc(sizeof(node));
	vtmp->data = (void*) malloc(sizeof(Page_Buffer));
	memcpy(vtmp->data,value,sizeof(Page_Buffer));
	vtmp->prev = this->vtail;
	this->vtail->next = vtmp;
	vtmp->next = NULL;
	this->vtail = vtmp;
	//printf("data %s \n",(char*)this->kheader->next->data);

	return 0;
}

int mapdelete(map* this, char* key) {
	node* ktmp = this->kheader;
	node* vtmp = this->vheader;
	while (ktmp) {
		if (strcmp(key,(char*)(ktmp->data)) == 0) {
			if (ktmp->next != NULL) {
				ktmp->prev->next = ktmp->next;
				ktmp->next->prev = ktmp->prev;
				vtmp->prev->next = vtmp->next;
				vtmp->next->prev = vtmp->prev;
			} else {
				ktmp->prev->next = NULL;
				this->ktail = ktmp->prev;
				vtmp->prev->next = NULL;
				this->vtail = vtmp->prev;
			}

		}
		ktmp = ktmp->next;
		vtmp = vtmp->next;
		//free(ktmp);
		//free(vtmp);
	}
	return 0;
}

void* getvalue(map* this, char* key) {
	node* ktmp = this->kheader;
	node* vtmp = this->vheader;
	while (ktmp) {
		//printf("data %s\n",(char*)ktmp->data);
		if (strcmp(key,(char*)(ktmp->data)) == 0) {
			return vtmp->data;
		}
		ktmp = ktmp->next;
		vtmp = vtmp->next;
	}
	return NULL;
}
