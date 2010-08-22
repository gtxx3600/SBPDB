/*
 * PF_FileHandle.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */
#include "pf.h"
#include "bufferdata.h"

RC GetFirstPage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle); // Get the first page
RC GetLastPage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle); // Get the last page
RC GetNextPage(PageNum current, struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle); // Get the next page
RC GetPrevPage(PageNum current, struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle);
RC GetThisPage(PageNum pageNum, struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle); // Get a specific page
RC AllocatePage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle);
RC SetIfOpen(int bln, struct PF_FileHandle *fileHandle);
RC GetIfOpen(struct PF_FileHandle *fileHandle);
RC SetNpage(PageNum pn, struct PF_FileHandle *fileHandle);
PageNum GetNpage(struct PF_FileHandle *fileHandle);
RC DisposePage(PageNum pageNum);
RC MarkDirty(PageNum pageNum, PF_FileHandle *fileHandle);
RC UnpinPage(PageNum pageNum, PF_FileHandle *fileHandle);
RC ForcePages(PageNum pageNum, PF_FileHandle *fileHandle);

RC GetFirstPage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle) {
	return GetThisPage(0, pageHandle, fileHandle);
}

RC GetLastPage(struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle) {
	return GetThisPage(fileHandle->npage - 1, pageHandle, fileHandle);
}

RC GetNextPage(PageNum current, struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle) {
	return GetThisPage(current + 1, pageHandle, fileHandle);
}

RC GetPrevPage(PageNum current, struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle) {
	return GetThisPage(current - 1, pageHandle, fileHandle);
}

RC GetThisPage(PageNum pageNum, struct PF_PageHandle *pageHandle,
		struct PF_FileHandle *fileHandle) {
	Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[20];
	sprintf(strPageNum, "%d", pageNum);
	strcat(strPageNum, fileHandle->filename);
	int MRU = theBD->getMRU(theBD);
	FILE *fio = fopen(fileHandle->filename, "r");
	printf("MRU %d\n",MRU);
	if (fileHandle->npage > pageNum && pageNum >= 0) {
		if (theBD->getMap(strPageNum, theBD) == -1) {

			fseek(fio, pageNum * ALL_PAGE_SIZE + PASS_BREAK, SEEK_SET);
			if (MRU != -1) {
				if (theBD->dirty[MRU] != 0) {
					theBD->writeBackWithDel(MRU, theBD);
				}
				theBD->fname[MRU] = fileHandle->filename;
				theBD->dirty[MRU] = 0;
				char *c = &(theBD->Buffer_Pool[theBD->getdata(MRU)]);
				fread(c, PF_PAGE_SIZE, 1, fio);

				theBD->Buffer_Chain[MRU * (2 + OTHER) + 1] = pageNum;

				pageHandle->page = &(theBD->Buffer_Pool[theBD->getdata(MRU)]);
				pageHandle->pagenum = pageNum;
				int i = 0;
				for (i = 0; i < MAX_FILENAME; i++) {
					pageHandle->filename[i] = fileHandle->filename[i];
				}
				fileHandle->mapnum[MRU]=MRU;
				theBD->addMap(strPageNum, &(fileHandle->mapnum[MRU]), theBD);
				printf("%d%s\n",theBD->getMap(strPageNum, theBD),strPageNum);
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
					pageHandle->filename[i] = fileHandle->filename[i];
				}
			} else {
				//还是在链中
				pageHandle->page = &(theBD->Buffer_Pool[theBD->getdata(MRU)]);
				pageHandle->pagenum = pageNum;
				int i = 0;
				for (i = 0; i < MAX_FILENAME; i++) {
					pageHandle->filename[i] = fileHandle->filename[i];
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

RC AllocatePage(struct PF_PageHandle *pageHandle,
	struct PF_FileHandle *fileHandle) {
	fileHandle->npage++;
	FILE *wfile = fopen(fileHandle->filename, "rb+");
	fseek(wfile, 0, SEEK_SET);
	char strnpage[4];
	sprintf(strnpage, "%d", fileHandle->npage);
	fwrite(strnpage, 4, 1, wfile);
	fclose(wfile);
	return (GetThisPage(fileHandle->npage - 1, pageHandle, fileHandle));
}

RC DisposePage(PageNum pageNum) {
	return 0;
}

RC MarkDirty(PageNum pageNum, PF_FileHandle *fileHandle) {
	Buffer_Data *theBD = getBuffer_Data();
	char* strPageNum;
	sprintf(strPageNum, "%d", pageNum);
	strcat(strPageNum, fileHandle->filename);
	int num = theBD->getMap(strPageNum, theBD);
	theBD->dirty[num] = 1;
	return (NORMAL);
}

RC UnpinPage(PageNum pageNum, PF_FileHandle *fileHandle) {
	Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[20];
	sprintf(strPageNum, "%d", pageNum);
	strcat(strPageNum, fileHandle->filename);
	int num = theBD->getMap(strPageNum, theBD);
	printf("NUM IN BUFFER %d\n",num);
	if (num >= 0) {
		theBD->addLRU(num, theBD);
		return (NORMAL);
	} else {
		return (PF_PAGENOTEXIST);
	}
}

RC ForcePages(PageNum pageNum, PF_FileHandle *fileHandle) {
	Buffer_Data *theBD = getBuffer_Data();
	char strPageNum[10];
	if (pageNum == -1) {
		int i = 0;
		for (i = 0; i < fileHandle->npage; i++) {
			sprintf(strPageNum, "%d", pageNum);
			strcat(strPageNum,fileHandle->filename);
			int num = theBD->getMap(strPageNum, theBD);
			if (theBD->dirty[num] == 1) {
				theBD->writeBack(num, theBD);
				theBD->dirty[num] = 0;
			}
		}
	} else {
		sprintf(strPageNum, "%d", pageNum);
		strcat( strPageNum,fileHandle->filename);
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
RC SetIfOpen(int bln, struct PF_FileHandle *fileHandle) {
	fileHandle->if_open = bln;
	return NORMAL;
}

RC GetIfOpen(struct PF_FileHandle *fileHandle) {
	return (fileHandle->if_open);
}

RC SetNpage(PageNum pn, struct PF_FileHandle *fileHandle) {
	fileHandle->npage = pn;
	return NORMAL;
}

PageNum GetNpage(struct PF_FileHandle *fileHandle) {
	return (fileHandle->npage);
}

RC initPF_FileHandle(struct PF_FileHandle * fh) {
	fh->GetThisPage = GetThisPage;
	fh->AllocatePage = AllocatePage;
	fh->GetFirstPage = GetFirstPage;
	fh->GetIfOpen = GetIfOpen;
	fh->GetLastPage = GetLastPage;
	fh->GetNextPage = GetNextPage;
	fh->GetNpage = GetNpage;
	fh->GetPrevPage = GetPrevPage;
	fh->ForcePages = ForcePages;
	fh->MarkDirty = MarkDirty;
	fh->SetNpage = SetNpage;
	fh->SetIfOpen = SetIfOpen;
	fh->UnpinPage = UnpinPage;
	fh->if_open = 0;
	fh->npage = 0;
	int i = 0;
	for (i = 0; i < MAX_FILENAME; i++) {
		fh->filename[i] = '\0';
	}
	return NORMAL;
}
