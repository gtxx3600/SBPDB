/*
 * PF_Manager.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "pf.h"
RC CreateFile (const char *fileName); // Create a new file
RC DestroyFile (const char *fileName); // Destroy a file
RC OpenFile (const char *fileName, struct PF_FileHandle *fileHandle);

RC CloseFile (struct PF_FileHandle *fileHandle); // Close a file
RC AllocateBlock (char *buffer); // Allocate a new scratch page in buffer
RC DisposeBlock (char *buffer); // Dispose of a scratch page


RC CreateFile(const char *fileName) {
	FILE *efile = fopen(fileName, "rb+");

	if (efile == NULL) {
		PageNum num = 0;
		FILE *cfile = fopen(fileName, "wb");
		char numc[4];
		sprintf(numc, "%d", num);
		fwrite(numc, 4, 1, cfile);
		fclose(cfile);
		return NORMAL;//normal return
	} else {
		fclose(efile);
		printf("file exist");
		return (PF_EXIST);//file exist
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
