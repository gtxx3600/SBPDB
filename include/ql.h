#ifndef QL_H
#define QL_H

#include "planner.h"

typedef struct IDList IDList;
typedef struct RelAttr RelAttr;
typedef struct RelAttrList RelAttrList;
typedef struct Value Value;
typedef struct ValueList ValueList;
typedef struct Condition Condition;
typedef struct CompOpCondition CompOpCondition;
typedef struct AndCondition AndCondition;
typedef struct OrCondition OrCondition;
typedef struct NotCondition NotCondition;

typedef enum CompOp CompOp;

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

enum CompOp {
	EQ_OP,
	LT_OP,
	GT_OP,
	LE_OP,
	GE_OP,
	NE_OP,
};

struct InCondition {
	RelAttrValueList *avl;
	Expression *rel;
};

struct CompOpCondition {
	enum CompOp op;
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
