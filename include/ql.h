#ifndef QL_H
#define QL_H

#include "sbpdb.h"
#include "planner.h"

#include "rm.h"
#include "sm.h"
#include "ix.h"

typedef struct IDList IDList;
typedef struct RelAttr RelAttr;
typedef struct RelAttrList RelAttrList;
typedef struct Value Value;
typedef struct ValueList ValueList;
typedef struct RelAttrValue RelAttrValue;
typedef struct RelAttrValueList RelAttrValueList;
typedef struct Condition Condition;
typedef struct InCondition InCondition;
typedef struct CompOpCondition CompOpCondition;
typedef struct AndCondition AndCondition;
typedef struct OrCondition OrCondition;
typedef struct NotCondition NotCondition;

typedef struct QL_Manager QL_Manager;
typedef struct QL_Tuple QL_Tuple;

#define NEW(type) \
	calloc(sizeof(type), 1)

#define GET_LAST(type, first, next) ({ \
	(type *) __p = first; \
	while (__p->next) __p = __p->next; \
	__p; })

typedef struct AttrSel AttrSel
struct AttrSel {
	char *relName;
	char *attrName;
	int offset;
	AttrType attrType;
	int attrLength;
	AttrSel *next;
};

struct QL_Tuple {
	RM_Record *rmr;
	AttrSel *as;
};

struct IDList {
	char *id;
	IDList *next;
};

struct RelAttr {
	char *relName;
	char *attrName;
};

struct RelAttrList {
	RelAttr *a;
	RelAttrList *next;
};

struct Value {
	AttrType type;
	void *data;
};

struct ValueList {
	Value *v;
	ValueList *next;
};

struct RelAttrValue {
	int isValue;
	union {
		RelAttr *a;
		Value *v;
	} u;
};

struct RelAttrValueList {
	RelAttrValue *av;
	RelAttrValueList *next;
};

struct InCondition {
	RelAttrValueList *avl;
	Expression *rel;
};

struct CompOpCondition {
	CompOp op;
	RelAttrValue *left;
	RelAttrValue *right;
};

struct AndCondition {
	Condition *left;
	Condition *right;
};

struct OrCondition {
	Condition *left;
	Condition *right;
};

struct NotCondtion {
	Condition *cond;
};

struct Condition {
	enum {
		InCond,
		CompOpCond,
		AndCond,
		OrCond,
		NotCond,
	} kind;
	union {
		InCondition *ic;
		CompOpCondition *coc;
		AndCondition *ac;
		OrCondition *oc;
		NotCondition *nc;
	} u;
};

struct QL_Manager {
	SM_Manager *smm;
	IX_Manager *ixm;
	RM_Manager *rmm;

	RC (*Select)(QL_Manager *self, Expression *exp);
	RC (*Insert)(QL_Manager *self, char *relName, ValueList *values);
	RC (*Delete)(QL_Manager *self, char *relName, Expression *exp);
	RC (*Update)(QL_Manager *self, char *relName, RelAttr *upAttr,
			int isValue, RelAttr *rRelAttr, Value *rValue,
			Expression *exp);
};

RC initQL_Manager(QL_Manager *self, SM_Manager *smm,
		IX_Manager *ixm, RM_Manager *rmm);

#endif
