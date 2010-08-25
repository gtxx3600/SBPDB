#include <stdlib.h>

#include "rm.h"
#include "sm.h"

void LoadCathe(SM_Manager *self) {
	RM_FileScan relScan, attrScan;
	initRM_FileScan(&relScan);
	initRM_FileScan(&attrScan);
	relScan.OpenScan(&relScan, &(self->relFile), 
			INT, 0, 0, NO_OP, NULL, NO_HINT);
	RM_Record rec;
	initRM_Record(&rec);
	while (relScan.GetNextRec(&rec) == NORMAL) {
		int i = relCount++;
		self->relRecords[i].rid = rec.rid;
		self->relRecords[i].pData = malloc(RELCAT_RSIZE);
		memcpy(self->relRecords[i].pData, rec.pData, RELCAT_SIZE);		
	}
	relScan.CloseScan();
	attrScan.OpenScan(&attrScan, &(self->attrFile), 
			INT, 0, 0, NO_OP, NULL, NO_HINT);
	while (attrScan.GetNextRec(&rec) == NORMAL) {
		int i = attrCount++;
		self->attrRecords[i].rid = rec.rid;
		self->attrRecords[i].pData = malloc(ATTRCAT_RSIZE);
		memcpy(self->attrRecords[i].pData, rec.pData, ATTRCAT_RSIZE);
	}
	attrScan.CloseScan();
}

RC SM_OpenDb(SM_Manager *self, char *dbName) {
	if (chdir(dbName) < 0) {
		return SM_NODIR;
	} else {
		self->rmm->OpenFile("relcat", relFile);
		self->rmm->OpenFile("attrcat", attrFile);
		LoadCathe(self);
		return NORMAL;
	}
}

RC SM_CreateTable(SM_Manager *self, char *relName, AttrInfo *attributes) {
	RID rid;
	int recordSize = 0;
	AttrInfo *p;
	for (p = attributes; p; p = p->next) {
		int j = self->attrCount++;
		self->attrRecords[j].pData = malloc(ATTRCAT_RSIZE);
		AttrSetRelName(&(self->attrRecords[j]), relName);
        AttrSetAttrName(&(self->attrRecords[j]), p->attrName);
        AttrSetOffset(&(self->attrRecords[j]), recordSize);
        AttrSetAttrType(&(self->attrRecords[j]), p->attrType);
        AttrSetAttrLength(&(self->attrRecords[j]), p->attrLength);
        AttrSetIndexNo(&(self->attrRecords[j]), -1);
        self->attrFile.InsertRec(&(self->attrFile), attrRecords[j].pData, 
				rid);
        self->attrFile.ForcePages(&(self->attrFile));
        self->attrRecords[j].rid = rid;

        recordSize += p->attrLength;
    }

    self->rmm->CreateFile(self->rmm, relName, recordSize);

    int k = relCount++;
    self->relRecords[k].pData = malloc(RELCAT_RSIZE);
    RelSetRelName(&(self->relRecords[k]), relName);
    RelSetTupleLength(&(self->relRecords[k]), recordSize);
    RelSetAttrCount(&(self->relRecords[k]), self->attrCount);
    RelSetIndexCount(&(self->relRecords[k]), 0);
    self->relFile.InsertRec(&(self->relFile), relRecords[k].pData, rid);
    self->relFile.ForcePages(&(self->relFile));
    self->relRecords[k].rid = rid;
	return NORMAL;
}

RC SM_DropTable(SM_Manager *self, char *relName) {
	int i;
    self->rmm->DestroyFile(relName);
    for (i = 0; i < self->relCount; i++)
        if (strcmp(RelGetRelName(&relRecords[i]), 
					relName) == 0) {
            self->relFile.DeleteRec(&(self->relFile), 
					self->relRecords[i].rid);
			int j;
            for (j = 0; j < RelGetIndexCount(
					&(self->relRecords[i])); j++)
                self->ixm->DestroyIndex(self->ixm, relName, j);
        }
    self->relFile.ForcePages(&(self->relFile));
    for (i = 0; i < self->attrCount; i++)
        if (strcmp(AttrGetRelName(&(self->attrRecords[i])), relName) == 0)
            self->attrFile.DeleteRec(&(self->attrFile), 
					self->attrRecords[i].rid);
    self->attrFile.ForcePages(&(self->attrFile));
    LoadCat(self);
	return NORMAL;
}

RC SM_Help() {
	printf("qs-IHS-IN\n");
	return NORMAL;
}

RC SM_Exit(SM_Manager *self) {
	return NORMAL;
}

RC initSM_Manager(SM_Manager *smm, IX_Manager *ixm, RM_Manager *rmm) {
	smm->ixm = ixm;
	smm->rmm = rmm;
	smm->OpenDb = SM_OpenDb;
	smm->CreateTable = SM_CreateTable;
	smm->DropTable = SM_DropTable;
	smm->CreateView = SM_CreateView;
	smm->DropView = SM_DropView;
	smm->Help = SM_Help;
	smm->Exit = SM_Exit;
	return 0;
}
