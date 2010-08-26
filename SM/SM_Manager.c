#include <stdlib.h>
#include <unistd.h>

#include "array.h"
#include "rm.h"
#include "sm.h"

#define RELARRCHECK(size) \
	ARRCHECK(self->relRecords, RelCat, size, self->relmax)

#define ATTRARRCHECK(size) \
	ARRCHECK(self->attrRecords, AttrCat, size, self->attrmax)

void LoadCat(SM_Manager *self) {
	RM_FileScan relScan, attrScan;
	initRM_FileScan(&relScan);
	initRM_FileScan(&attrScan);
	relScan.OpenScan(&relScan, &(self->relFile), 
			INT, 0, 0, NO_OP, NULL, NO_HINT);
	RM_Record rec;
	initRM_Record(&rec);
	while (relScan.GetNextRec(&relScan, &rec) == NORMAL) {
		int i = self->relCount++;
		RELARRCHECK(i);
		self->relRecords[i].rid = rec.rid;
		memcpy(&(self->relRecords[i]), rec.data, RELCAT_RSIZE);
	}
	relScan.CloseScan(&relScan);
	attrScan.OpenScan(&attrScan, &(self->attrFile), 
			INT, 0, 0, NO_OP, NULL, NO_HINT);
	while (attrScan.GetNextRec(&attrScan, &rec) == NORMAL) {
		int i = self->attrCount++;
		ATTRARRCHECK(i);
		self->attrRecords[i].rid = rec.rid;
		memcpy(&(self->attrRecords[i]), rec.data, ATTRCAT_RSIZE);
	}
	attrScan.CloseScan(&attrScan);
}

void SM_CloseCat(SM_Manager *self) {
	self->rmm->CloseFile(self->rmm, &self->relFile);
	self->rmm->CloseFile(self->rmm, &self->attrFile);
}

RC SM_OpenDb(SM_Manager *self, char *dbName) {
	if (chdir(dbName) < 0) {
		return SM_NODIR;
	} else {
		self->rmm->OpenFile(self->rmm, "relcat", &(self->relFile));
		self->rmm->OpenFile(self->rmm, "attrcat", &(self->attrFile));
		LoadCat(self);
		return NORMAL;
	}
}

RC SM_CloseDb(SM_Manager *self) {
	SM_CloseCat(self);
}

#define MAXSCP(dst, s, max) do {\
	strncpy(dst, s, max); \
	dst[max] = 0;\
} while (0)

RC AttrCatSetRelName(AttrCat *ac, char *s) {
	MAXSCP(ac->relName, s, MAXNAME);
}
RC AttrCatSetAttrName(AttrCat *ac, char *s) {
	MAXSCP(ac->attrName, s, MAXNAME);
}
RC RelCatSetRelName(RelCat *rc, char *s) {
	MAXSCP(rc->relName, s, MAXNAME);
}

RC SM_CreateTable(SM_Manager *self, char *relName, AttrInfo *attributes) {
	int recordSize = 0, attrNum = 0;
	AttrInfo *p;
	for (p = attributes; p; p = p->next) {
		int j = self->attrCount++;
		attrNum++;
		AttrCat actmp;
		AttrCatSetRelName(&actmp, relName);
		AttrCatSetAttrName(&actmp, p->name);
		actmp.offset = recordSize;
		actmp.attrType = p->type;
		actmp.attrLength = p->size;
		actmp.indexNo = -1;
        self->attrFile.InsertRec(&(self->attrFile), (char *)&actmp,
				&actmp.rid);
        self->attrFile.ForcePages(&(self->attrFile), ALL_PAGES);
		ATTRARRCHECK(j);
        memcpy(&(self->attrRecords[j]), &actmp, sizeof(AttrCat));
        recordSize += p->size;
    }

    self->rmm->CreateFile(self->rmm, relName, recordSize);

    int k = self->relCount++;
    RelCat rctmp;
    RelCatSetRelName(&rctmp, relName);
    rctmp.tupleLength = recordSize;
    rctmp.attrCount = attrNum;
    rctmp.indexCount = 0;
    self->relFile.InsertRec(&(self->relFile), (char *)&rctmp, &rctmp.rid);
    self->relFile.ForcePages(&(self->relFile), ALL_PAGES);
	RELARRCHECK(k);
    memcpy(&(self->relRecords[k]), &rctmp, sizeof(RelCat));
	return NORMAL;
}

RC SM_DropTable(SM_Manager *self, char *relName) {
	int i;
    self->rmm->DestroyFile(self->rmm, relName);
    for (i = 0; i < self->relCount; i++)
        if (strcmp(self->relRecords[i].relName,
					relName) == 0) {
            self->relFile.DeleteRec(&(self->relFile), 
					&self->relRecords[i].rid);
			int j;
            for (j = 0; j < self->relRecords[i].indexCount; j++)
                self->ixm->DestroyIndex(self->ixm, relName, j);
        }
    self->relFile.ForcePages(&(self->relFile), ALL_PAGES);
    for (i = 0; i < self->attrCount; i++)
        if (strcmp(self->attrRecords[i].relName, relName) == 0)
            self->attrFile.DeleteRec(&(self->attrFile), 
					&self->attrRecords[i].rid);
    self->attrFile.ForcePages(&(self->attrFile), ALL_PAGES);
    LoadCat(self);
	return NORMAL;
}

RC SM_Help(SM_Manager *self) {
	printf("qs-IHS-IN\n");
	return NORMAL;
}

RC SM_Exit(SM_Manager *self) {
	return NORMAL;
}

RC initSM_Manager(SM_Manager *self, IX_Manager *ixm, RM_Manager *rmm) {
	self->ixm = ixm;
	self->rmm = rmm;
	self->relmax = 0;
	self->attrmax = 0;
	return 0;
}
