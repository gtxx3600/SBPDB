#ifndef SM_H
#define SM_H

#include "sbpdb.h"
#include "planner.h"

#include "rm.h"
#include "ix.h"

typedef struct SM_Manager SM_Manager;
typedef struct AttrInfo AttrInfo;
typedef struct RelCat RelCat;
typedef struct AttrCat AttrCat;

#define RELCAT_RSIZE (sizeof(RelCat) - sizeof(RID))
#define ATTRCAT_RSIZE (sizeof(AttrCat) - sizeof(RID))

struct AttrInfo {
	char *relname;
	char *name;
	AttrType type;
	int size;
	AttrInfo *next;
};

struct RelCat {
	char relName[MAXNAME+1];
	int tupleLength;
	int attrCount;
	int indexCount;
	RID rid;
};

struct AttrCat {
	char relName[MAXNAME+1];
	char attrName[MAXNAME+1];
	int offset;
	AttrType attrType;
	int attrLength;
	int indexNo;
	RID rid;
};

struct SM_Manager {
	IX_Manager *ixm;
	RM_Manager *rmm;
	RM_FileHandle attrFile, relFile;
	RelCat *relRecords;
	AttrCat *attrRecords;
	int relCount, attrCount;
	int relmax, attrmax;
};

RC SM_OpenDb(SM_Manager *self, char *dbName);
RC SM_CloseDb(SM_Manager *self);
RC SM_GetAttrCats(SM_Manager *self, char *relName,
		AttrCat **ac, int *size);
RC SM_CreateTable(SM_Manager *self, char *relName, AttrInfo *attributes);
RC SM_DropTable(SM_Manager *self, char *relName);
RC SM_CreateView(SM_Manager *self, char *viewName, Expression query);
RC SM_DropView(SM_Manager *self, char *viewName);
RC SM_Help(SM_Manager *self);
RC SM_Exit(SM_Manager *self);
RC initSM_Manager(SM_Manager *smm, IX_Manager *ixm, RM_Manager *rmm);

#endif
