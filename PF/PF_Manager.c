/*
 * PF_Manager.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "../include/pf.h"

int main() {
//	hash_map hm;
//	hmap_create(&hm,3);
//	assert(hm);
//	int n =20;
//	void* mydata=malloc(n);
//	hmap_insert(hm, "a", -1,1);
//	hmap_insert(hm, "b", -1,2);
//	hmap_insert(hm, "c", -1,3);
//	int a =(int) hmap_search(hm,"c");
//	printf("%d\n",a);
//	hmap_delete(hm,"a");
//	a =(int) hmap_search(hm,"a");
//	printf("%d\n",a);

	struct PF_FileHandle test1;
//	struct PF_PageHandle a;
//	char* b="a";printf("%s",b);
//	GetData(&b,&a) ;

    CreateFile("r1");
    OpenFile("r1",&test1);
  //  printf("%s",test1.filename);
 //   printf("%d",test1.if_open);
    CloseFile(&test1);
    DestroyFile("r1");
    return 0;
}

RC CreateFile(const char *fileName) {
	FILE *efile = fopen(fileName, "rb+");

	if (efile == NULL) {
		PageNum num = 0;
		FILE *cfile = fopen(fileName, "wb");
		char numc[4];
		sprintf(numc, "%d", num);
		fwrite(numc, 4, 1, cfile);
		fclose(cfile);
		return 0;//normal return
	} else {
		fclose(efile);
		printf("file exist");
		return 1;//file exist
	}
}

RC DestroyFile(const char *fileName){
	if( remove(fileName) == -1 )
	{

		printf("file not exist");
		return 1;
	}
	else
		return 0;

}

RC OpenFile(const char *fileName, struct PF_FileHandle* fileHandle){
	FILE *infile = fopen(fileName, "rb+");
	if ( infile == NULL){
		printf("file not exist");
		return 1;
	}
	else{
//		Buffer_Data *theBD = getBuffer_Data();
		int i;
		for (i = 0; i < MAX_FILENAME; i++){
			if (fileName[i] == '\0') break;
			else
			{
				fileHandle->filename[i] = fileName[i];
				//printf("%c",fileName[i]);
			}
		}
		for (; i < MAX_FILENAME; i++){
			fileHandle->filename[i] = '\0';
		}
		SetIfOpen(1,fileHandle);
		PageNum pn;
		char numc[4];
		if(fread(numc, 4, 1, infile))
		{
				//printf("a");
		}
		pn = atoi(numc);
		SetNpage(pn,fileHandle);
		fclose(infile);
		return 0;
	}
}

RC CloseFile(struct PF_FileHandle *fileHandle){
	if(GetIfOpen(fileHandle)==1){
		return 0;
	}
	else{
		return 1;
	}
}

RC AllocateBlock(char *buffer){
	return 0;
}

RC DisposeBlock(char *buffer)
{

	return 0;
}
