/*
 * PF_FileHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */
#include "pf.h"
#include "bufferdata.h"
#include "sbpdb.h"
#define KEY_SIZE  20

RC GetFirstPage(PF_FileHandle *this, PF_PageHandle *pageHandle); // Get the first page
RC GetLastPage(PF_FileHandle *this, PF_PageHandle *pageHandle); // Get the last page
RC GetNextPage(PF_FileHandle *this, struct PF_PageHandle *pageHandle); // Get the next page
RC GetPrevPage(PF_FileHandle *this, struct PF_PageHandle *pageHandle);
RC GetThisPage(PF_FileHandle *this, PageNum pageNum,
		struct PF_PageHandle *pageHandle); // Get a specific page
RC AllocatePage(PF_FileHandle *this, struct PF_PageHandle *pageHandle);
RC SetIfOpen(PF_FileHandle *this, int bln);
RC GetIfOpen(PF_FileHandle *this);
RC SetNpage(PF_FileHandle *this, PageNum pn);
RC MarkDirty(PF_FileHandle *this, PageNum pageNum);
RC UnpinPage(PF_FileHandle *this, PageNum pageNum);
RC ForcePages(PF_FileHandle *this, PageNum pageNum);
RC DisposePages(PF_FileHandle *this);

PageNum GetNpage(PF_FileHandle *this);

RC GetFirstPage(PF_FileHandle *this, PF_PageHandle *pageHandle) {
	return GetThisPage(this, 0, pageHandle);
}

RC GetLastPage(PF_FileHandle *this, struct PF_PageHandle *pageHandle) {
	return GetThisPage(this, this->npage - 1, pageHandle);
}

RC GetNextPage(PF_FileHandle *this, struct PF_PageHandle *pageHandle) {
	return GetThisPage(this, this->currentPage + 1, pageHandle);
}

RC GetPrevPage(PF_FileHandle *this, struct PF_PageHandle *pageHandle) {
	return GetThisPage(this, this->currentPage - 1, pageHandle);
}

RC GetThisPage(PF_FileHandle *this, PageNum pageNum, PF_PageHandle *pageHandle) {
	this->currentPage = pageNum;
	Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[KEY_SIZE];
	sprintf(strPageNum, "%d", pageNum);
	strcat(strPageNum, this->filename);
	FILE *fio = fopen(this->filename, "r");
	//printf("MRU %d\n", MRU);
	if (this->npage >= pageNum && pageNum >= 0) {
		if (theBD->getMap(theBD, strPageNum) == NULL) {
			fseek(fio, (pageNum + 1) * ALL_PAGE_SIZE, SEEK_SET);

			theBD->allocPage(theBD, this->filename, pageNum);
			char *c = (theBD->lpin_page->pagedata);
			fread(c, ALL_PAGE_SIZE, 1, fio);

			pageHandle->page = theBD->lpin_page->pagedata;
			pageHandle->pagenum = pageNum;
			strcpy(pageHandle->filename, this->filename);
			//theBD->addMap(theBD, strPageNum, theBD->lpin_page);
			fclose(fio);
			//			printf("%d%s\n", theBD->getMap(strPageNum, theBD), strPageNum);
			return (NORMAL);

		} else {
			//在buffer中
			//分类已经pined还是在unpined链中
			Page_Buffer *pb = theBD->getMap(theBD, strPageNum);
			if (pb->pinned > 0) {
				//分类已经pined
				pageHandle->page = pb->pagedata;
				pageHandle->pagenum = pageNum;
				if (strlen(this->filename) < MAX_FILENAME)
					strcpy(pageHandle->filename, this->filename);
				else
					return DB_PARAM;
				theBD->pinPage(theBD, pb);
			} else {
				//还是在unpinned链中
				pageHandle->page = pb->pagedata;
				pageHandle->pagenum = pageNum;
				if (strlen(this->filename) < MAX_FILENAME)
					strcpy(pageHandle->filename, this->filename);
				else
					return DB_PARAM;
				theBD->pinPage(theBD, pb);
			}
			fclose(fio);
			return (NORMAL);
		}
	} else {
		return (PF_EOF);
	}

}

RC AllocatePage(PF_FileHandle *this, struct PF_PageHandle *pageHandle) {
	this->npage++;
	FILE *wfile = fopen(this->filename, "rb+");
	if (wfile == NULL) {
		printf("error in open file %s\n", this->filename);
	}
	fseek(wfile, 0, SEEK_SET);
	fwrite(&this->npage, 4, 1, wfile);
	fclose(wfile);
	//	printf("get page : %d\n",this->npage);
	return (GetThisPage(this, this->npage - 1, pageHandle));
}

RC DisposePages(PF_FileHandle *this) {
	//Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[KEY_SIZE];
	int i = 0;
	for (i = 0; i < this->npage; i++) {
		sprintf(strPageNum, "%d", i);
		strcat(strPageNum, this->filename);
		//Page_Buffer *pb = theBD->getMap(theBD, strPageNum);
		this->UnpinPage(this,i);
	}
	return NORMAL;
}

RC MarkDirty(PF_FileHandle *this, PageNum pageNum) {
	Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[KEY_SIZE];
	sprintf(strPageNum, "%d", pageNum);
	strcat(strPageNum, this->filename);
	Page_Buffer *pb = theBD->getMap(theBD, strPageNum);
	pb->dirty = 1;
	return (NORMAL);
}

RC UnpinPage(PF_FileHandle *this, PageNum pageNum) {
	Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[KEY_SIZE];
	sprintf(strPageNum, "%d", pageNum);
	strcat(strPageNum, this->filename);
	Page_Buffer *pb = theBD->getMap(theBD, strPageNum);
	if (pb != NULL) {
		theBD->unpinPage(theBD, pb);
		return (NORMAL);
	} else {
		return (PF_PAGENOTEXIST);
	}
}

RC ForcePages(PF_FileHandle *this, PageNum pageNum) {
	Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[KEY_SIZE];
	if (pageNum == ALL_PAGES) {
		int i = 0;
		for (i = 0; i < this->npage; i++) {
			sprintf(strPageNum, "%d", i);
			strcat(strPageNum, this->filename);
			Page_Buffer *pb = theBD->getMap(theBD, strPageNum);
			if (pb != NULL) {
				if (pb->dirty == 1)
					theBD->copyBack(theBD, pb);
				pb->dirty = 0;
			}
		}
	} else {
		sprintf(strPageNum, "%d", pageNum);
		strcat(strPageNum, this->filename);
		Page_Buffer *pb = theBD->getMap(theBD, strPageNum);
		if (pb != NULL) {
			if (pb->dirty == 1)
				theBD->copyBack(theBD, pb);
			pb->dirty = 0;

		} else {
			return (PF_PAGENOTEXIST);
		}
	}
	return (NORMAL);
}
RC SetIfOpen(PF_FileHandle *this, int bln) {
	this->if_open = bln;
	return NORMAL;
}

RC GetIfOpen(PF_FileHandle *this) {
	return (this->if_open);
}

RC SetNpage(PF_FileHandle *this, PageNum pn) {
	this->npage = pn;
	return NORMAL;
}

PageNum GetNpage(PF_FileHandle *this) {
	return (this->npage);
}

RC initPF_FileHandle(PF_FileHandle *this) {
	this->GetThisPage = GetThisPage;
	this->AllocatePage = AllocatePage;
	this->GetFirstPage = GetFirstPage;
	this->GetIfOpen = GetIfOpen;
	this->GetLastPage = GetLastPage;
	this->GetNextPage = GetNextPage;
	this->GetNpage = GetNpage;
	this->GetPrevPage = GetPrevPage;
	this->ForcePages = ForcePages;
	this->MarkDirty = MarkDirty;
	this->SetNpage = SetNpage;
	this->SetIfOpen = SetIfOpen;
	this->UnpinPage = UnpinPage;
	this->if_open = 0;
	this->npage = 0;
	this->currentPage = 0;
	this->DisposePages = DisposePages;
	int i = 0;
	for (i = 0; i < MAX_FILENAME; i++) {
		this->filename[i] = '\0';
	}
	return NORMAL;
}
