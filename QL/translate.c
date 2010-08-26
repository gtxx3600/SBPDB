#include "planner.h"

Expression *tranExp(Expression *exp) {
	switch (exp->kind) {
	case Relation:
		return exp;
	case UnionExp:
		exp->u.unie->left = tranExp(exp->u.unie->left);
		exp->u.unie->right = tranExp(exp->u.unie->right);
		return exp;
	case IntersectionExp:
		exp->u.inte->left = tranExp(exp->u.inte->left);
		exp->u.inte->right = tranExp(exp->u.inte->right);
		return exp;
	case DifferenceExp:
		exp->u.dife->left = tranExp(exp->u.dife->left);
		exp->u.dife->right = tranExp(exp->u.dife->right);
		return exp;
	case ProjectionExp:
		exp->u.proje->exp = tranExp(exp->u.proje->exp);
		return exp;
	case SelectionExp:
		exp->u.sele->exp = tranExp(exp->u.sele->exp);
		switch (exp->u.sele->cond->kind) {
		case CompOpCond:
			exp->u.sele->exp = transExp(exp->u.sele->exp);
			return exp;
		case AndCond: {
			Expression *e = calloc(sizeof(Expression), 1);
			e->kind = SelectionExp;
			e->u.sele->cond = exp->u.sele->cond->u.ac->left;
			exp->u.sele->cond = exp->u.sele->cond->u.ac->right;
			exp->u.sele->exp = e;
			exp = transExp(exp);
			return exp;
		}
		case InCond:
		case OrCond:
		case NotCond:
		default:
			fprintf(stderr, "not support\n");
			return NULL;
		}
	case ProductExp:
		exp->u.prode->left = tranExp(exp->u.prode->left);
		exp->u.prode->right = tranExp(exp->u.prode->right);
		return exp;
	case NaturalJoinExp:
		exp->u.natje->left = tranExp(exp->u.natje->left);
		exp->u.natje->right = tranExp(exp->u.natje->right);
		return exp;
	default:
		fprintf(stderr, "not support\n");
		return NULL;
	}
}
