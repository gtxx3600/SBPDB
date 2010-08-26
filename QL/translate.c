#include "planner.h"

Expression *QL_transExp(Expression *exp) {
	switch (exp->kind) {
	case Relation:
		return exp;
	case UnionExp:
		exp->u.unie->left = QL_transExp(exp->u.unie->left);
		exp->u.unie->right = QL_transExp(exp->u.unie->right);
		return exp;
	case IntersectionExp:
		exp->u.inte->left = QL_transExp(exp->u.inte->left);
		exp->u.inte->right = QL_transExp(exp->u.inte->right);
		return exp;
	case DifferenceExp:
		exp->u.dife->left = QL_transExp(exp->u.dife->left);
		exp->u.dife->right = QL_transExp(exp->u.dife->right);
		return exp;
	case ProjectionExp:
		exp->u.proje->exp = QL_transExp(exp->u.proje->exp);
		return exp;
	case SelectionExp:
		exp->u.sele->exp = QL_transExp(exp->u.sele->exp);
		switch (exp->u.sele->cond->kind) {
		case CompOpCond:
			exp->u.sele->exp = QL_transExp(exp->u.sele->exp);
			return exp;
		case AndCond: {
			Expression *e = NEW(Expression);
			e->kind = SelectionExp;
			e->u.sele = NEW(struct selection_exp);
			e->u.sele->cond = exp->u.sele->cond->u.ac->right;
			e->u.sele->exp = exp->u.sele->exp;
			exp->u.sele->cond = exp->u.sele->cond->u.ac->left;
			exp->u.sele->exp = e;
			exp = QL_transExp(exp);
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
		exp->u.prode->left = QL_transExp(exp->u.prode->left);
		exp->u.prode->right = QL_transExp(exp->u.prode->right);
		return exp;
	case NaturalJoinExp:
		exp->u.natje->left = QL_transExp(exp->u.natje->left);
		exp->u.natje->right = QL_transExp(exp->u.natje->right);
		return exp;
	default:
		fprintf(stderr, "not support\n");
		return NULL;
	}
}
