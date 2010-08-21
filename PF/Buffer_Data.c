/*
 * Buffer_Data.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "bufferdata.h"
#include "hashmap.h"


int addMap (char* str , int num,struct Buffer_Data *bd)
{
	hmap_insert(bd->hm,str,-1,(void*)(num+1));
	return 0;
}

int delMap (char* str ,struct Buffer_Data *bd)
{
	hmap_delete(bd->hm,str);
	return 0;
}

int getMap (char* str,struct Buffer_Data *bd)
{
	hmap_search(bd->hm,str);
	return 0;
}
int getMRU(struct Buffer_Data bd) {
	return (bd.MRU);
}

int getLRU(struct Buffer_Data bd) {
	return (bd.LRU);
}

int setLRU(int num, struct Buffer_Data *bd) {
	bd->LRU = num;
	return 0;
}

int setMRU(int num, struct Buffer_Data *bd) {
	bd->MRU = num;
	return 0;
}

int getdata(int num) {
	if (num != -1) {
		return (num * ALL_PAGE_SIZE + 4);
	} else {
		return (-1);
	}
}

int delMRU(struct Buffer_Data *bd) {
	int NMRU = bd->Buffer_Chain[bd->MRU * (2 + OTHER)];
	bd->Buffer_Chain[bd->MRU * (2 + OTHER)] = -1;
	bd->MRU = NMRU;
	if (bd->MRU != -1) {
		bd->Buffer_Chain[bd->MRU * (2 + OTHER) + 2] = -1;
	} else {
		bd->LRU = -1;
	}
	return (bd->MRU);
}
int delLRU(struct Buffer_Data *bd) {
	int NLRU = bd->Buffer_Chain[bd->LRU * (2 + OTHER) + 2];
	bd->Buffer_Chain[bd->LRU * (2 + OTHER) + 2] = -1;
	bd->LRU = NLRU;
	if (bd->LRU != -1) {
		bd->Buffer_Chain[bd->LRU * (2 + OTHER)] = -1;
	} else {
		bd->MRU = -1;
	}
	return (bd->LRU);
}

int addLRU(int num, struct Buffer_Data *bd) {
	bd->Buffer_Chain[num * (2 + OTHER) + 2] = bd->LRU;
	if (bd->LRU != -1) {
		bd->Buffer_Chain[bd->LRU * (2 + OTHER)] = num;
	} else {
		bd->MRU = num;
	}
	bd->LRU = num;
	return (bd->LRU);
}

int addMRU(int num, struct Buffer_Data *bd) {
	if (bd->MRU != -1) {
		bd->Buffer_Chain[bd->MRU * (2 + OTHER) + 2] = num;
	}
	bd->Buffer_Chain[num * (2 + OTHER)] = bd->MRU;
	bd->MRU = num;
	return (bd->MRU);
}

int delChain(int num,struct Buffer_Data *bd){
	if ( bd->MRU == num){
		return( delMRU(bd) );
	}
	else if(bd-> LRU == num ){
		return( delLRU(bd) );
	}
	else{
		int last = bd->Buffer_Chain[num * (2 + OTHER)];
		int next = bd->Buffer_Chain[num * (2 + OTHER) + 2];
		bd->Buffer_Chain[last * (2 + OTHER) + 2] = next;
		bd->Buffer_Chain[next * (2 + OTHER)] = last;
		bd->Buffer_Chain[num * (2 + OTHER) + 0] = -1;
		bd->Buffer_Chain[num * (2 + OTHER) + 2] = -1;
		return (num);
	}
}

int  writeBackWithDel(int num,struct Buffer_Data *bd){
	char* filename;
	filename = bd->fname[num];
	int intPageNum = bd->Buffer_Chain[(2 + OTHER) * num + 1];
	char strPageNum[10];
	sprintf(strPageNum, "%d", intPageNum);
	strcat(filename,strPageNum);
	delMap(filename,bd) ;
	FILE *outfile =fopen(filename,"wb");
	if ( outfile == NULL){
			printf("file not exist");
			return 1;
		}else
		{
			fseek(outfile,intPageNum * ALL_PAGE_SIZE,SEEK_SET );
			int suc = fwrite(&(bd->Buffer_Pool[getdata(num)]), PF_BUFFER_SIZE, 1, outfile);
			fclose(outfile);
			return suc;
		}

	return 0;
}

int  writeBack(int num,struct Buffer_Data *bd){
	char* filename;
	filename = bd->fname[num];
	int intPageNum = bd->Buffer_Chain[(2 + OTHER) * num + 1];
	char strPageNum[10];
	sprintf(strPageNum, "%d", intPageNum);
	strcat(filename,strPageNum);
	FILE *outfile =fopen(filename,"wb");
	if ( outfile == NULL){
			printf("file not exist");
			return 1;
		}else
		{
			fseek(outfile,intPageNum * ALL_PAGE_SIZE,SEEK_SET );
			int suc = fwrite(&(bd->Buffer_Pool[getdata(num)]), PF_BUFFER_SIZE, 1, outfile);
			fclose(outfile);
			return suc;
		}

	return 0;
}


RC initBuffer_Data(struct Buffer_Data *bd){
	int i = 0;
	for (i = 0; i < PF_BUFFER_SIZE; i++) {
		bd->dirty[i]=0;
		bd->fname[i]='\0';
		int j = 0;
		for (j = 0; j < PF_PAGE_SIZE; j++) {
			bd->Buffer_Pool[getdata(i) + j] ='\0';
		}
	}
	for (i = 0; i < PF_BUFFER_SIZE; i++) {
		bd->Buffer_Chain[i * (2 + OTHER)] = i - 1;
		bd->Buffer_Chain[i * (2 + OTHER) + 1] = 0;
		bd->Buffer_Chain[i * (2 + OTHER) + 2] = i + 1;
	}
	bd->Buffer_Chain[PF_BUFFER_SIZE * (2 + OTHER) - 1] = -1;
	bd->LRU = 0;
	bd->MRU = PF_BUFFER_SIZE - 1;
	hmap_create(&(bd->hm), 1024);
	return (NORMAL);
}
