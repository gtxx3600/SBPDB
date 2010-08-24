#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "ql.h"

typedef struct expression Expression;

struct relation {
	char *id;
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
	RelAttrValueList *avl;
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
		struct natural_join_exp natje;
	} u;
};

#endif
