#ifndef QL_H
#define QL_H

#include "sbpdb.h"
#include "planner.h"

#include "rm.h"
#include "sm.h"
#include "ix.h"

typedef struct QL_Manager QL_Manager;
typedef struct QL_Tuple QL_Tuple;

RC QL_GetCurrent(QL_Manager *qlm, Expression *exp, QL_Tuple *qlt);
RC QL_GetNext(QL_Manager *qlm, Expression *exp, QL_Tuple *qlt);

struct QL_Manager {
	SM_Manager *smm;
	IX_Manager *ixm;
	RM_Manager *rmm;
};

RC QL_Select(QL_Manager *self, Expression *exp, RelAttrList *al);
RC QL_Insert(QL_Manager *self, char *relName, ValueList *values);
RC QL_Delete(QL_Manager *self, char *relName, Expression *exp);
RC QL_Update(QL_Manager *self, char *relName, RelAttr *upAttr,
		int isValue, RelAttr *rRelAttr, Value *rValue,
		Expression *exp);

RC initQL_Manager(QL_Manager *self, SM_Manager *smm,
		IX_Manager *ixm, RM_Manager *rmm);

#endif
