/*
 * Buffer_Data.c
 *
 *  Created on: 2010-8-23
 *      Author: cxy
 */

#include "bufferdata.h"
#include "hashmap.h"
#include "sbpdb.h"
Buffer_Data thebuffer;
int writeBack(Buffer_Data *this, Page_Buffer *pb);
int copyBack(Buffer_Data *this, Page_Buffer *pb);
int pinPage(Buffer_Data *this, Page_Buffer *pb);
int addMap(Buffer_Data *this, char* key, Page_Buffer *pb);
int delMap(Buffer_Data *this, char* key);
Page_Buffer* getMap(Buffer_Data *this, char* key);
int disposePB(Buffer_Data *this, Page_Buffer *pb, char* tmp);
int unpinPage(Buffer_Data *this, Page_Buffer *pb);
int allocPage(Buffer_Data *this, char* filename, int pagenum);
//int getpData(Page_Buffer *this);

Page_Buffer* getMap(Buffer_Data *this, char* key) {
	return ((Page_Buffer*) hmap_search(this->pagemap, key));
}

int addMap(Buffer_Data *this, char* key, Page_Buffer *pb) {
	hmap_insert(this->pagemap, key, -1, pb);
	return 0;
}

int delMap(Buffer_Data *this, char* key) {
	hmap_delete(this->pagemap, key);
	return 0;
}

int writeBack(Buffer_Data *this, Page_Buffer *pb) {
	if (pb == NULL) {
		return 1;
	}
	char* fname = pb->filename;
	FILE *wfile = fopen(fname, "rb+");
	if (wfile == NULL) {
		//		printf("file not exist");
		return 1;
	} else {
		fseek(wfile, (pb->pagenum + 1) * ALL_PAGE_SIZE, SEEK_SET );
		int suc = fwrite(pb->pagedata, ALL_PAGE_SIZE, 1, wfile);
		printf("writeback succeed: %d\n", suc);
		fclose(wfile);
	}
	free(pb);
	return 0;
}

int copyBack(Buffer_Data *this, Page_Buffer *pb) {
	if (pb == NULL) {
		return 1;
	}
	char* fname = pb->filename;
	FILE *wfile = fopen(fname, "rb+");
	if (wfile == NULL) {
		//		printf("fname: %s \n",pb->filename);
		//		printf("file not exist");
		return 1;
	} else {
		if (pb->pagedata == NULL) {
			printf("copyback: page data null \n");
		}
		printf("copy back pagenum %d\n", pb->pagenum);
		fseek(wfile, (pb->pagenum + 1) * ALL_PAGE_SIZE, SEEK_SET );
		int suc = fwrite(pb->pagedata, ALL_PAGE_SIZE, 1, wfile);
		printf("copyback succeed: %d\n", suc);
		fclose(wfile);
	}
	return 0;
}

int unpinPage(Buffer_Data *this, Page_Buffer *pb) {
	if (pb == NULL) {
		return 1;
	}
	if (pb->pinned > 1) {
		pb->pinned--;
		printf("pinnned !!!!!!!!!!!!!!!!!!!!\n");
		return 0;
	}
	if (this->lunpin_page == pb)
		return 0;
	if (pb->next_page != NULL) {
		pb->prev_page->next_page = pb->next_page;
	} else {
		pb->prev_page->next_page = NULL;
	}
	if (this->lpin_page == pb) {
		this->lpin_page = pb->prev_page;
	}

	if (pb->next_page != NULL) {
		pb->next_page->prev_page = pb->prev_page;
	}
	this->lunpin_page->next_page = pb;
	pb ->prev_page = this->lunpin_page;
	this->lunpin_page = pb;
	pb->next_page = NULL;
	if (this->unpin_num == 0) {
		this->funpin_page = pb;
	}
	if (pb->pinned == 1) {
		this->pin_num--;
		this->unpin_num++;
		pb->pinned = 0;
	}
	return 0;
}

int pinPage(Buffer_Data *this, Page_Buffer *pb) {
	if (pb == NULL) {
		return 1;
	}
	if (this->lpin_page == pb)
		return 0;
	if (pb->next_page != NULL) {
		pb->prev_page->next_page = pb->next_page;
		pb->next_page->prev_page = pb->prev_page;
	} else {
		pb->prev_page->next_page = NULL;
	}
	if (this->lunpin_page == pb) {
		this->lunpin_page = pb->prev_page;
	}
	if (this->funpin_page == pb) {
		if (this->unpin_num != 1) {
			this->funpin_page = pb->next_page;
		} else {
			this->funpin_page = this->unpin_header;
		}
	}
	this->lpin_page->next_page = pb;
	pb ->prev_page = this->lpin_page;
	this->lpin_page = pb;
	pb->next_page = NULL;
	if (pb->pinned == 0) {
		this->pin_num++;
		this->unpin_num--;
		pb->pinned = 1;//pb->pinned + 1;
	}
	return 0;
}

int allocPage(Buffer_Data *this, char* filename, int pagenum) {
	Page_Buffer *pb;
	pb = (Page_Buffer*) malloc(sizeof(Page_Buffer));
	pb->dirty = 0;
	pb->pagenum = pagenum;
	pb->filename = filename;
	pb->pagedata = (char*) malloc(ALL_PAGE_SIZE);
	pb->prev_page = this->lpin_page;
	pb->prev_page->next_page = pb;
	pb->next_page = NULL;
	pb->pinned = 1;
	pb->key = (char*) malloc(KEY_LENGTH);
	char strpagenum[KEY_LENGTH];
	sprintf(strpagenum, "%d", pagenum);
	strcat(strpagenum, filename);
	pb->key = strpagenum;
	printf("key of pb : %s\n", pb->key);
	addMap(this, strpagenum, pb);
	this->lpin_page = pb;
	this->page_num++;
	this->pin_num++;
	if (this->page_num > MIN_POOL_SIZE) {
		if (this->unpin_num > 0) {
			delMap(this, this->funpin_page->key);
			writeBack(this, this->funpin_page);
			//		free(this->funpin_page);
			if (this->unpin_num > 1) {
				this->funpin_page = this->funpin_page->next_page;
			}
			this->page_num--;
			this->unpin_num--;
		}

	}
	return 0;
}
int createpage(Page_Buffer *pb, char* filename, int pagenum) {
	pb = (Page_Buffer*) malloc(sizeof(Page_Buffer));
	pb->dirty = 0;
	pb->pagenum = pagenum;
	pb->filename = filename;
	pb->pagedata = (char*) malloc(ALL_PAGE_SIZE);
	pb->pinned = 1;

	return 0;
}
int disposePB(Buffer_Data *this, Page_Buffer *pb, char* tmp) {
	if (pb->pinned == 1) {
		printf("error in dispose pined page \n");
		return 1;
	}
	if (pb == this->lunpin_page) {
		this->lpin_page = pb->prev_page;
	}
	if (this->funpin_page == pb) {
		if (this->unpin_num != 1) {
			this->funpin_page = pb->next_page;
		} else {
			this->funpin_page = this->unpin_header;
		}
	}
	if (pb->next_page != NULL) {
		pb->prev_page->next_page = pb->next_page;
		pb->next_page->prev_page = pb->prev_page;
	} else {
		pb->prev_page->next_page = NULL;
	}

	delMap(this, tmp);

	free(pb);
	this->unpin_num--;
	this->page_num--;
	return 0;

}
int initBuffer_Data(Buffer_Data *this) {
	this->addMap = addMap;
	this->allocPage = allocPage;
	this->delMap = delMap;
	this->getMap = getMap;
	this->unpin_header = (Page_Buffer*) malloc(sizeof(Page_Buffer));
	this->pin_header = (Page_Buffer*) malloc(sizeof(Page_Buffer));
	this->lpin_page = this->pin_header;
	this->lunpin_page = this->unpin_header;
	this->funpin_page = this->unpin_header;
	this->page_num = 0;
	this->pin_num = 0;
	this->unpin_num = 0;
	this->writeBack = writeBack;
	hmap_create(&this->pagemap, 40000);
	this->unpinPage = unpinPage;
	this->pinPage = pinPage;
	this->copyBack = copyBack;
	this->disposePB = disposePB;
	return 0;
}

Buffer_Data *getBuffer_Data() {
	if (thebuffer.init != 1) {
		printf("init\n");
		initBuffer_Data(&thebuffer);
		thebuffer.init = 1;
	}
	return &thebuffer;
}

