/*
 * PF_Manager.c
 *
 *  Created on: 2010-8-20
 *      Author: cxy
 */

#include "pf.h"
#include "bufferdata.h"
RC CreateFile (PF_Manager *this,const char *fileName ); // Create a new file
RC DestroyFile (PF_Manager *this,const char *fileName); // Destroy a file
RC OpenFile (PF_Manager *this,const char *fileName, struct PF_FileHandle *fileHandle);

RC CloseFile (PF_Manager *this,struct PF_FileHandle *fileHandle); // Close a file
RC AllocateBlock (PF_Manager *this,char *buffer); // Allocate a new scratch page in buffer
RC DisposeBlock (PF_Manager *this,char *buffer); // Dispose of a scratch page


RC CreateFile(PF_Manager *this,const char *fileName ) {
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
		printf("file exist\n");
		return (PF_EXIST);//file exist
	}
}

RC DestroyFile(PF_Manager *this,const char *fileName){
	if( remove(fileName) == -1 )
	{

		printf("file not exist");
		return PF_NOTEXIST;
	}
	else
		return NORMAL;

}

RC OpenFile(PF_Manager *this,const char *fileName, struct PF_FileHandle* fileHandle){
	FILE *infile = fopen(fileName, "rb+");
	if ( infile == NULL){
		printf("file not exist");
		return PF_NOTEXIST;
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
		fileHandle->SetIfOpen(fileHandle,1);
		PageNum pn;
		char numc[4];
		if(fread(numc, 4, 1, infile))
		{
				//printf("a");
		}
		pn = atoi(numc);
		fileHandle->SetNpage(fileHandle,pn);
		fclose(infile);
		return NORMAL;
	}
}

RC CloseFile(PF_Manager *this,struct PF_FileHandle *fileHandle){
	if(fileHandle->GetIfOpen(fileHandle)==1){
		return NORMAL;
	}
	else{
		return PF_CLOSEDFILE;
	}
}

RC AllocateBlock(PF_Manager *this,char *buffer){
	return 0;
}

RC DisposeBlock(PF_Manager *this,char *buffer)
{

	return 0;
}

RC initPF_Manager(PF_Manager *this)
{
	this->AllocateBlock = AllocateBlock;
	this->CloseFile = CloseFile;
	this->CreateFile = CreateFile;
	this->DestroyFile = DestroyFile;
	this->DisposeBlock = DisposeBlock;
	this->OpenFile = OpenFile;
	return NORMAL;
}
