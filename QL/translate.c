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
		case InCond:
		case CompOpCond:
			return exp;
		case AndCond:
			Expression *e = calloc(sizeof(Expression), 1);
			e.kind = SelectionExp;
			
		case OrCond:
		case NotCond:
		}
	case ProductExp:
		exp->u.prode->left = tranExp(exp->u.prode->left);
		exp->u.prode->right = tranExp(exp->u.prode->right);
		return exp;
	case NaturalJoinExp:
		exp->u.natje->left = tranExp(exp->u.natje->left);
		exp->u.natje->right = tranExp(exp->u.natje->right);
		return exp;
	}
}
