#include <stdlib.h>
#include <unistd.h>

#include "array.h"
#include "rm.h"
#include "sm.h"

#define RELARRCHECK(smm, size) \
	ARRCHECK(smm->relRecords, RelCat, size, smm->relmax)

#define ATTRARRCHECK(smm, size) \
	ARRCHECK(smm->attrRecords, AttrCat, size, smm->attrmax)

int checkInViews(SM_Manager *self, char *name) {
	ViewList *p;
	for (p = self->views; p; p = p->next) {
		if (!strcmp(p->name, name))
			return 1;
	}
	return 0;
}

int checkInTables(SM_Manager *self, char *name) {
	if (access(name, 0) == 0)
		return 1;
	return 0;
}

int checkRelAttr(SM_Manager *self, RelAttr *a) {
	int i;
	if (a == NULL) return 1;
	for (i = 0; i < self->attrCount; i++) {
		if (!strcmp(self->attrRecords[i].relName, a->relName) &&
				!strcmp(self->attrRecords[i].attrName, a->attrName))
			return 1;
	}
	return 0;
}

AttrCat *getAttrCat(SM_Manager *self,
		char *relName, char *attrName) {
	int i;
	for (i = 0; i < self->attrCount; i++) {
		if (!strcmp(self->attrRecords[i].attrName, attrName)
				&& !strcmp(self->attrRecords[i].relName, relName))
			return &self->attrRecords[i];
	}
	return NULL;
}

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
		RELARRCHECK(self, i);
		self->relRecords[i].rid = rec.rid;
		memcpy(&(self->relRecords[i]), rec.data, RELCAT_RSIZE);
	}
	relScan.CloseScan(&relScan);
	attrScan.OpenScan(&attrScan, &(self->attrFile), 
			INT, 0, 0, NO_OP, NULL, NO_HINT);
	while (attrScan.GetNextRec(&attrScan, &rec) == NORMAL) {
		int i = self->attrCount++;
		ATTRARRCHECK(self, i);
		self->attrRecords[i].rid = rec.rid;
		memcpy(&(self->attrRecords[i]), rec.data, ATTRCAT_RSIZE);
	}
	attrScan.CloseScan(&attrScan);
}

void SM_CloseCat(SM_Manager *self) {
	self->rmm->CloseFile(self->rmm, &self->relFile);
	self->rmm->CloseFile(self->rmm, &self->attrFile);
}

RC SM_GetAttrCats(SM_Manager *self, char *relName,
		AttrCat **ac, int *size) {
	int i, attrNum, offset = 0, isFound = 0;
    for (i = 0; i < self->relCount; i++)
        if (strcmp(self->relRecords[i].relName, relName) == 0) {
        	attrNum = self->relRecords[i].attrCount;
        	isFound = 1;
        	break;
        }
    if (!isFound) return SM_NOREL;
    *size = attrNum;
    *ac = calloc(sizeof(AttrCat), attrNum);
    for (i = 0; i < self->attrCount; i++)
    	if (strcmp(self->attrRecords[i].relName, relName) == 0) {
    		memcpy(*ac + offset, &self->attrRecords[i], sizeof(AttrCat));
    		offset++;
    	}
    return NORMAL;
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
	if (self->dbname) {
		SM_CloseCat(self);
		free(self->dbname);
		self->dbname = NULL;
		chdir("..");
	}
    return NORMAL;
}

#define MAXSCP(dst, s, max) do {\
	strncpy(dst, s, max); \
	dst[max] = 0;\
} while (0)

RC AttrCatSetRelName(AttrCat *ac, char *s) {
	MAXSCP(ac->relName, s, MAXNAME);
    return NORMAL;
}
RC AttrCatSetAttrName(AttrCat *ac, char *s) {
	MAXSCP(ac->attrName, s, MAXNAME);
    return NORMAL;
}
RC RelCatSetRelName(RelCat *rc, char *s) {
	MAXSCP(rc->relName, s, MAXNAME);
    return NORMAL;
}

RC SM_UseDatabase(SM_Manager *self, char *name) {
	int ret;
	SM_CloseDb(self);
	self->dbname = strdup(name);
	if ((ret = SM_OpenDb(self, name)) != NORMAL) {
		free(self->dbname);
		self->dbname = NULL;
	}
	return ret;
}

#define MKDIR "mkdir "
RC SM_CreateDatabase(SM_Manager *self, char *name) {
	char *command;

	if (access(name, 0) == 0) {
		return SM_DIREXIST;
	}

	command = malloc(strlen(MKDIR)+strlen(name)+1);
	strcpy(command, MKDIR);

	system(strcat(command, name));
	free(command);
	if (chdir(name) < 0) {
		fprintf(stderr, "chdir error to %s\n", name);
		return -1;
	}
	self->rmm->CreateFile(self->rmm, "relcat", RELCAT_RSIZE);
	self->rmm->CreateFile(self->rmm, "attrcat", ATTRCAT_RSIZE);
	chdir("..");

	return NORMAL;
}

#define RMDIR "rm -rf "
RC SM_DropDatabase(SM_Manager *self, char *name) {
	char *command;

	if (access(name, 0)) {
		return SM_NODIR;
	}

	if (self->dbname && !strcmp(name, self->dbname)) {
		SM_CloseDb(self);
	}

	command = malloc(strlen(RMDIR)+strlen(name)+1);
	strcpy(command, RMDIR);
	system(strcat(command, name));
	free(command);
	return NORMAL;
}

RC SM_CreateTable(SM_Manager *self, char *relName, AttrInfo *attributes) {
	int recordSize = 0, attrNum = 0, ret;
	AttrInfo *p, *pk = NULL;

	if (self->dbname == NULL) {
		return SM_NODBSELECTED;
	}

	if (checkInViews(self, relName)) {
		return SM_VIEWEXIST;
	}

    for (p = attributes; p; p = p->next) {
    	AttrInfo *p0;
    	for (p0 = p->next; p0; p0 = p0->next) {
    		if (!strcmp(p->name, p0->name))
    			return SM_DUPLICATEATTR;
    	}
    }

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
		if (!checkRelAttr(self, p->foriegnKey)) {
			return SM_NOREL;
		}
		if (p->foriegnKey) {
			MAXSCP(actmp.fkrelName, p->foriegnKey->relName, MAXNAME);
			MAXSCP(actmp.fkattrName, p->foriegnKey->attrName, MAXNAME);

			char *fname = malloc(2*MAXNAME+3);
			strcpy(fname, ".");
			strcat(fname, p->foriegnKey->relName);
			strcat(fname, ".");
			strcat(fname, p->foriegnKey->attrName);
			RM_FileHandle fh;
			initRM_FileHandle(&fh);
			self->rmm->CreateFile(self->rmm, fname, FKINFO_RSIZE);
			self->rmm->OpenFile(self->rmm, fname, &fh);
			fkInfo fki;
			strcpy(fki.relName, p->foriegnKey->relName);
			strcpy(fki.attrName, p->foriegnKey->attrName);
			RID rid;
			fh.InsertRec(&fh, (char *)&fki, &rid);
		} else {
			strcpy(actmp.fkrelName, "");
			strcpy(actmp.fkattrName, "");
		}
		if (p->isPrimaryKey) {
			if (pk) {
				return SM_DUPRIMARYKEY;
			} else {
				pk = p;
			}
		}
		if (p->check) {
			actmp.op = p->check->op;
			memcpy(&actmp.rvalue, p->check->value->data, p->size);
		} else {
			actmp.op = NO_OP;
		}
        self->attrFile.InsertRec(&(self->attrFile), (char *)&actmp,
				&actmp.rid);
        self->attrFile.ForcePages(&(self->attrFile), ALL_PAGES);
		ATTRARRCHECK(self, j);
        memcpy(&(self->attrRecords[j]), &actmp, sizeof(AttrCat));
        recordSize += p->size;
    }

    if ((ret = self->rmm->CreateFile(self->rmm, relName, recordSize))
    		!= NORMAL) {
    	return ret;
    }

    int k = self->relCount++;
    RelCat rctmp;
    RelCatSetRelName(&rctmp, relName);
    rctmp.tupleLength = recordSize;
    rctmp.attrCount = attrNum;
    rctmp.indexCount = 0;
    if (pk) {
    	strcpy(rctmp.primaryKey, pk->name);
    } else {
    	strcpy(rctmp.primaryKey, "");
    }
    self->relFile.InsertRec(&(self->relFile), (char *)&rctmp, &rctmp.rid);
    self->relFile.ForcePages(&(self->relFile), ALL_PAGES);
	RELARRCHECK(self, k);
    memcpy(&(self->relRecords[k]), &rctmp, sizeof(RelCat));
	return NORMAL;
}

RC SM_DropTable(SM_Manager *self, char *relName) {
	int i, ret;

	if (self->dbname == NULL) {
		return SM_NODBSELECTED;
	}

    if ((ret = self->rmm->DestroyFile(self->rmm, relName)) != NORMAL) {
    	return ret;
    }

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

RC SM_CreateView(SM_Manager *self, char *viewName, Expression *query) {
	ViewList *p = self->views;


	if (self->dbname == NULL) {
		return SM_NODBSELECTED;
	}
	if (checkInViews(self, viewName))
		return SM_VIEWEXIST;
	if (checkInTables(self, viewName))
		return SM_TABLEEXIST;

	if (self->views) {
		p = GET_LAST(ViewList, self->views, next);
		p->next = NEW(ViewList);
		p->next->name = viewName;
		p->next->exp = query;
		p->next->next = NULL;
	} else {
		self->views = NEW(ViewList);
		self->views->name = viewName;
		self->views->exp = query;
		self->views->next = NULL;
	}

	return NORMAL;
}

RC SM_DropView(SM_Manager *self, char *viewName) {
	ViewList *p;

	if (self->dbname == NULL) {
		return SM_NODBSELECTED;
	}
	if (self->views == NULL)
		return SM_VIEWNOTEXIST;
	if (!strcmp(self->views->name, viewName)) {
		self->views = self->views->next;
		return NORMAL;
	}
	for (p = self->views; p->next; p = p->next) {
		if (!strcmp(p->next->name, viewName)) {
			p->next = p->next->next;
			return NORMAL;
		}
	}
	return SM_VIEWNOTEXIST;
}

RC SM_Help(SM_Manager *self) {
	printf("qs-IHS-IN\n");
	return NORMAL;
}

RC SM_Exit(SM_Manager *self) {
	self->isExit = 1;
	return NORMAL;
}

RC initSM_Manager(SM_Manager *self, IX_Manager *ixm, RM_Manager *rmm) {
	self->ixm = ixm;
	self->rmm = rmm;
	self->relmax = 0;
	self->attrCount = 0;
	self->attrmax = 0;
	self->relCount = 0;
	self->isExit = 0;
	self->dbname = NULL;
	self->relRecords = NULL;
	self->attrRecords = NULL;
	self->views = NULL;
	initRM_FileHandle(&self->relFile);
	initRM_FileHandle(&self->attrFile);
	return 0;
}
