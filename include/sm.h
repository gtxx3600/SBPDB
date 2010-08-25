#ifndef SM_H
#define SM_H

#include "sbpdb.h"
#include "planner.h"

#include "rm.h"
#include "ixm.h"

typedef struct AttrInfo AttrInfo;
typedef struct SM_Manager SM_Manager;

#define RELCAT_RSIZE (MAXNAME + 1 + sizeof(int) * 3)
#define ATTRCAT_RSIZE ((MAXNAME+1) * 2 + sizeof(int) * 3 + sizeof(AttrType))

struct AttrInfo {
	char *name;
	AttrType type;
	int size;
	AttrInfo *next;
};

struct SM_Manager {
	IX_Manager *ixm;
	RM_Manager *rmm;
	RM_FileHandle attrFile, relFile;
	RM_Record *relRecord, *attrRecord;
	int relCount, attrCount;

	RC (*OpenDb)(SM_Manger *self, char *dbName);
	RC (*CloseDb)(SM_Manger *self);
	RC (*CreateTable)(SM_Manger *self, char *relName, AttrInfo *attributes);
	RC (*DropTable)(SM_Manger *self, char *relName);
	RC (*CreateView)(SM_Manger *self, char *viewName, Expression query);
	RC (*DropView)(SM_Manager *self, char *viewName);
	RC (*Help)();
	RC (*Exit)(SM_Manger *self);
};

RC initSM_Manager(SM_Manager *smm, IX_Manager *ixm, RM_Manager *rmm);

#endif
