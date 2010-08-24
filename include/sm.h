#ifndef SM_H
#define SM_H

#include "sbpdb.h"

typedef struct AttrInfo AttrInfo;
typedef struct SM_Manager SM_Manager;

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

	RC (*OpenDb)(SM_Manger *self, char *dbName);
	RC (*CloseDb)(SM_Manger *self);
	RC (*CreateTable)(SM_Manger *self, char *relName, AttrInfo *attributes);
	RC (*DropTable)(SM_Manger *self, char *relName);
	//TODO
	RC (*CreateView)(SM_Manger *self, char viewName);
	RC (*Help)();
	RC (*Exit)(SM_Manger *self);
};

#endif
