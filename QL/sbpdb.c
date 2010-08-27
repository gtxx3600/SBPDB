#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ql.h"

extern FILE *yyin;

int main() {
	PF_Manager pfm;
	RM_Manager rmm;
	IX_Manager ixm;
	SM_Manager smm;
	QL_Manager qlm;
//	FILE *input = fopen("sk.sbp", "r");
	initPF_Manager(&pfm);
	initRM_Manager(&rmm, &pfm);
	initIX_Manager(&ixm, &pfm);
	initSM_Manager(&smm, &ixm, &rmm);
	initQL_Manager(&qlm, &smm, &ixm, &rmm);


//    int i;
//	rmm.CreateFile(&rmm, "test", 259);
//    for (i = 0; i < 20; i++) {
//        RM_FileHandle rmFile;
//        RID rid;
//        char pData[256];
//        initRM_FileHandle(&rmFile);
//        initRID(&rid, 0, 0);
//    	printf("%d\n", i);
//    	sprintf(pData, "asdf%d", i);
//		rmm.OpenFile(&rmm, "test", &rmFile);
//		rmFile.InsertRec(&rmFile, pData, &rid);
//		rmm.CloseFile(&rmm, &rmFile);
//    }
//    return 0;

//	RM_FileHandle fh;
//	initRM_FileHandle(&fh);
//	rmm.CreateFile(&rmm, "test", 10);
//	rmm.OpenFile(&rmm, "test", &fh);
//	RM_FileScan fs;
//	initRM_FileScan(&fs);
//	char *rctmp = malloc(10);
//	RID rid;
//	fh.InsertRec(&fh, rctmp, &rid);
//	fs.OpenScan(&fs, &fh,
//			INT, 0, 0, NO_OP, NULL, NO_HINT);
//	RM_Record rec;
//	initRM_Record(&rec);
//	while (fs.GetNextRec(&fs, &rec) == NORMAL);

	if (access(BASEDIR, 0))
		system("mkdir "BASEDIR);
	chdir(BASEDIR);
//	system("ls");
//	yyin = input;
	while (!smm.isExit) {
		printf("> ");
		fflush(stdout);
		sbp_parse(&pfm, &smm, &qlm);
	}
	SM_CloseDb(&smm);
	printf("Bye\n");
	return 0;
}
