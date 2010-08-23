/*
 * PF_FileHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */
#include "pf.h"
#include "bufferdata.h"
#include "sbpdb.h"

RC GetFirstPage(PF_FileHandle *this, PF_PageHandle *pageHandle); // Get the first page
RC GetLastPage(PF_FileHandle *this, PF_PageHandle *pageHandle); // Get the last page
RC GetNextPage(PF_FileHandle *this,
		struct PF_PageHandle *pageHandle); // Get the next page
RC GetPrevPage(PF_FileHandle *this,
		struct PF_PageHandle *pageHandle);
RC GetThisPage(PF_FileHandle *this, PageNum pageNum,
		struct PF_PageHandle *pageHandle); // Get a specific page
RC AllocatePage(PF_FileHandle *this, struct PF_PageHandle *pageHandle);
RC SetIfOpen(PF_FileHandle *this, int bln);
RC GetIfOpen(PF_FileHandle *this);
RC SetNpage(PF_FileHandle *this, PageNum pn);
RC MarkDirty(PF_FileHandle *this, PageNum pageNum);
RC UnpinPage(PF_FileHandle *this, PageNum pageNum);
RC ForcePages(PF_FileHandle *this, PageNum pageNum);
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
	this->currentPage=pageNum;
Buffer_Data *theBD = getBuffer_Data();
char strPageNum[20];
sprintf(strPageNum, "%d", pageNum);
strcat(strPageNum, this->filename);
int MRU = theBD->getMRU(theBD);
FILE *fio = fopen(this->filename, "r");
printf("MRU %d\n", MRU);
if (this->npage > pageNum && pageNum >= 0) {
	if (theBD->getMap(strPageNum, theBD) == -1) {

		fseek(fio, pageNum * ALL_PAGE_SIZE + PASS_BREAK, SEEK_SET);
		if (MRU != -1) {
			if (theBD ->dirty[MRU] != 0) {
				theBD->writeBackWithDel(MRU, theBD);
			}
			theBD->fname[MRU] = this->filename;
			theBD->dirty[MRU] = 0;
			char *c = &(theBD->Buffer_Pool[theBD->getdata(MRU)]);
			fread(c, ALL_PAGE_SIZE, 1, fio);

			theBD->Buffer_Chain[MRU * (2 + OTHER) + 1] = pageNum;

			pageHandle->page = &(theBD->Buffer_Pool[theBD->getdata(MRU)]);
			pageHandle->pagenum = pageNum;
			strcpy(pageHandle->filename, this->filename);
			this->mapnum[MRU] = MRU;
			theBD->addMap(strPageNum, &(this->mapnum[MRU]), theBD);
			printf("%d%s\n", theBD->getMap(strPageNum, theBD), strPageNum);
			theBD->delChain(MRU, theBD);

			return (NORMAL);
		} else {
			return (PF_NOBUF);
		}
	} else {
		//在buffer中
		//分类已经pined还是在链中
		int num = theBD->getMap(strPageNum, theBD);
		if ((*theBD).Buffer_Chain[num * (2 + OTHER)] == -1
				&& (*theBD).Buffer_Chain[num * (2 + OTHER) + 2] == -1) {
			//分类已经pined
			pageHandle->page = &(theBD->Buffer_Pool[theBD->getdata(MRU)]);
			pageHandle->pagenum = pageNum;
			int i = 0;
			for (i = 0; i < MAX_FILENAME; i++) {
				pageHandle->filename[i] = this->filename[i];
			}
		} else {
			//还是在链中
			pageHandle->page = &(theBD->Buffer_Pool[theBD->getdata(MRU)]);
			pageHandle->pagenum = pageNum;
			int i = 0;
			for (i = 0; i < MAX_FILENAME; i++) {
				pageHandle->filename[i] = this->filename[i];
			}
			//调整链表
			theBD->delChain(num, theBD);
		}
		return (NORMAL);
	}
} else {
	return (PF_EOF);
}

}

RC AllocatePage(PF_FileHandle *this, struct PF_PageHandle *pageHandle) {
this->npage++;
FILE *wfile = fopen(this->filename, "rb+");
fseek(wfile, 0, SEEK_SET);
fwrite(&this->npage, 4, 1, wfile);
fclose(wfile);
return (GetThisPage(this, this->npage - 1, pageHandle));
}

RC DisposePage(PF_FileHandle *this, PageNum pageNum) {
return 0;
}

RC MarkDirty(PF_FileHandle *this, PageNum pageNum) {
Buffer_Data *theBD = getBuffer_Data();
char* strPageNum;
sprintf(strPageNum, "%d", pageNum);
strcat(strPageNum, this->filename);
int num = theBD->getMap(strPageNum, theBD);
theBD->dirty[num] = 1;
return (NORMAL);
}

RC UnpinPage(PF_FileHandle *this, PageNum pageNum) {
Buffer_Data *theBD = getBuffer_Data();
char strPageNum[20];
sprintf(strPageNum, "%d", pageNum);
strcat(strPageNum, this->filename);
int num = theBD->getMap(strPageNum, theBD);
if (num >= 0) {
	theBD->addLRU(num, theBD);
	return (NORMAL);
} else {
	return (PF_PAGENOTEXIST);
}
}

RC ForcePages(PF_FileHandle *this, PageNum pageNum) {
Buffer_Data *theBD = getBuffer_Data();
char strPageNum[10];
if (pageNum == ALL_PAGES) {
	int i = 0;
	for (i = 0; i < this->npage; i++) {
		sprintf(strPageNum, "%d", pageNum);
		strcat(strPageNum, this->filename);
		int num = theBD->getMap(strPageNum, theBD);
		if (theBD->dirty[num] == 1) {
			theBD->writeBack(num, theBD);
			theBD->dirty[num] = 0;
		}
	}
} else {
	sprintf(strPageNum, "%d", pageNum);
	strcat(strPageNum, this->filename);
	int num = theBD->getMap(strPageNum, theBD);
	if (num >= 0) {
		if (theBD->dirty[num] == 1) {
			theBD->writeBack(num, theBD);
			theBD->dirty[num] = 0;
		}
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
int i = 0;
for (i = 0; i < MAX_FILENAME; i++) {
	this->filename[i] = '\0';
}
return NORMAL;
}
