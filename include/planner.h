#ifndef PLANNER_H
#define PLANNER_H

#include "rm.h"

#define NEW(type) \
	calloc(sizeof(type), 1)

#define GET_LAST(type, first, next) ({ \
	type * __p = first; \
	while (__p->next) __p = __p->next; \
	__p; })

typedef struct IDList IDList;
typedef struct RelAttr RelAttr;
typedef struct RelAttrList RelAttrList;
typedef struct Value Value;
typedef struct ValueList ValueList;
typedef struct RelAttrValue RelAttrValue;
typedef struct RelAttrValueList RelAttrValueList;
typedef struct AssignmentList AssignmentList;
typedef struct Condition Condition;
typedef struct InCondition InCondition;
typedef struct CompOpCondition CompOpCondition;
typedef struct AndCondition AndCondition;
typedef struct OrCondition OrCondition;
typedef struct NotCondition NotCondition;
typedef struct expression Expression;
typedef struct AttrSel AttrSel;

Expression *QL_transExp(Expression *exp);

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

struct AssignmentList {
	RelAttr *left;
	RelAttrValue *right;
	AssignmentList *next;
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

struct NotCondition {
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

struct relation {
	char *id;
	AttrSel *as;
	int isIndexed;
	union {
		RM_FileScan *fs;
	} u;
	RM_FileHandle *fh;
	RM_Record *cur;
	Expression *exp;
};

struct union_exp {
	struct expression *left;
	struct expression *right;
};

struct intersection_exp {
	struct expression *left;
	struct expression *right;
};

struct difference_exp {
	struct expression *left;
	struct expression *right;
};

struct projection_exp {
	RelAttrList *al;
	struct expression *exp;
};

struct selection_exp {
	Condition *cond;
	struct expression *exp;
};

struct product_exp {
	struct expression *left;
	struct expression *right;
};

struct natural_join_exp {
	struct expression *left;
	struct expression *right;
};

struct expression {
	enum {
		Relation,
		UnionExp,
		IntersectionExp,
		DifferenceExp,
		ProjectionExp,
		SelectionExp,
		ProductExp,
		NaturalJoinExp,
	} kind;
	union {
		struct relation *rel;
		struct union_exp *unie;
		struct intersection_exp *inte;
		struct difference_exp *dife;
		struct projection_exp *proje;
		struct selection_exp *sele;
		struct product_exp *prode;
		struct natural_join_exp *natje;
	} u;
};

#endif
