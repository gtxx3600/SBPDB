#include <stdio.h>

#include "sm.h"
#include "ql.h"
#include "planner.h"

#define __QL_DEBUG
#ifdef __QL_DEBUG
#define DEPRT(...) \
	fprintf(stderr, __VA_ARGS__)
#else
#define DEPRT(...)
#endif

#define DD 2

void indent(int d) {
	while (d--) DEPRT("  ");
}

void prtAttrInfo(AttrInfo *ai, int d) {
	indent(d);
	DEPRT("%s, %d\n", ai->name, ai->type);
}

void prtAttrInfoList(AttrInfo *ai, int d) {
	indent(d);
	DEPRT("AttrInfoList\n");
	d += DD;
	while (ai) {
		prtAttrInfo(ai, d);
		ai = ai->next;
	}
}

void prtIDList(IDList *il, int d) {
	indent(d);
	DEPRT("IDList\n");
	d += DD;
	while (il) {
		indent(d);
		DEPRT("%s\n", il->id);
		il = il->next;
	}
}

void prtRelAttr(RelAttr *a, int d) {
	indent(d);
	if (a->relName) {
		DEPRT("%s.%s\n", a->relName, a->attrName);
	} else {
		DEPRT("%s\n", a->attrName);
	}
}

void prtRelAttrList(RelAttrList *al, int d) {
	indent(d);
	DEPRT("RelAttrList\n");
	d += DD;
	while (al) {
		prtRelAttr(al->a, d);
		al = al->next;
	}
}

void prtValue(Value *v, int d) {
	indent(d);
	switch (v->type) {
	case INT:
		DEPRT("INT %d\n", *(int *)(v->data));
		break;
	case STRING:
		DEPRT("STRING %s\n", (char *)(v->data));
		break;
	case FLOAT:
		DEPRT("FLOAT %f\n", *(float *)(v->data));
		break;
	default:
		DEPRT("prtValue\n");
	}
}

void prtValueList(ValueList *vl, int d) {
	indent(d);
	DEPRT("ValueList\n");
	d += DD;
	while (vl) {
		prtValue(vl->v, d);
		vl = vl->next;
	}
}

void prtRelAttrValue(RelAttrValue *av, int d) {
	if (av->isValue) {
		prtValue(av->u.v, d);
	} else {
		prtRelAttr(av->u.a, d);
	}
}

void prtRelAttrValueList(RelAttrValueList *avl, int d) {
	indent(d);
	DEPRT("RelAttrValueList\n");
	d += DD;
	while (avl) {
		prtRelAttrValue(avl->av, d);
		avl = avl->next;
	}
}

void prtExpression(Expression *exp, int d);
void prtCondition(Condition *cond, int d);

void prtInCondition(InCondition *ic, int d) {
	indent(d);
	DEPRT("InCondition\n");
	d += DD;
	prtRelAttrValueList(ic->avl, d);
	prtExpression(ic->rel, d);
}

void prtCompOpCondition(CompOpCondition *coc, int d) {
	indent(d);
	DEPRT("CompOpCondition\n");
	d += DD;
	indent(d);
	DEPRT("CompOp: %d\n", coc->op);
	prtRelAttrValue(coc->left, d);
	prtRelAttrValue(coc->right, d);
}

void prtAndCondition(AndCondition *ac, int d) {
	indent(d);
	DEPRT("AndCondition\n");
	d += DD;
	prtCondition(ac->left, d);
	prtCondition(ac->right, d);
}

void prtOrCondition(OrCondition *oc, int d) {
	indent(d);
	DEPRT("OrCondition\n");
	d += DD;
	prtCondition(oc->left, d);
	prtCondition(oc->right, d);
}

void prtNotCondition(NotCondition *nc, int d) {
	indent(d);
	DEPRT("NotCondition\n");
	d += DD;
	prtCondition(nc->cond, d);
}

void prtCondition(Condition *cond, int d) {
	if (cond == NULL) {
		indent(d);
		DEPRT("Condition NULL\n");
		return;
	}
	switch (cond->kind) {
	case InCond:
		prtInCondition(cond->u.ic, d);
		break;
	case CompOpCond:
		prtCompOpCondition(cond->u.coc, d);
		break;
	case AndCond:
		prtAndCondition(cond->u.ac, d);
		break;
	case OrCond:
		prtOrCondition(cond->u.oc, d);
		break;
	case NotCond:
		prtNotCondition(cond->u.nc, d);
		break;
	default:
		DEPRT("prtCondition\n");
		break;
	}
}

void prtUnionExp(struct union_exp *ue, int d) {
	indent(d);
	DEPRT("UnionExp\n");
	d += DD;
	prtExpression(ue->left, d);
	prtExpression(ue->right, d);
}

void prtIntersectionExp(struct intersection_exp *ie, int d) {
	indent(d);
	DEPRT("IntersectionExp\n");
	d += DD;
	prtExpression(ie->left, d);
	prtExpression(ie->right, d);
}

void prtDifferenceExp(struct difference_exp *de, int d) {
	indent(d);
	DEPRT("DifferenceExp\n");
	d += DD;
	prtExpression(de->left, d);
	prtExpression(de->right, d);
}

void prtProjectionExp(struct projection_exp *pe, int d) {
	indent(d);
	DEPRT("ProjectionExp\n");
	d += DD;
	prtRelAttrList(pe->al, d);
	prtExpression(pe->exp, d);
}

void prtSelectionExp(struct selection_exp *se, int d) {
	indent(d);
	DEPRT("SelectionExp\n");
	d += DD;
	prtCondition(se->cond, d);
	prtExpression(se->exp, d);
}

void prtProductExp(struct product_exp *pe, int d) {
	indent(d);
	DEPRT("ProductExp\n");
	d += DD;
	prtExpression(pe->left, d);
	prtExpression(pe->right, d);
}

void prtNaturalJoinExp(struct natural_join_exp *nje, int d) {
	indent(d);
	DEPRT("NaturalJoinExp\n");
	d += DD;
	prtExpression(nje->left, d);
	prtExpression(nje->right, d);
}

void prtExpression(Expression *exp, int d) {
	switch (exp->kind) {
	case Relation:
		indent(d);
		DEPRT("Relation %s\n", exp->u.rel->id);
		break;
	case UnionExp:
		prtUnionExp(exp->u.unie, d);
		break;
	case IntersectionExp:
		prtIntersectionExp(exp->u.inte, d);
		break;
	case DifferenceExp:
		prtDifferenceExp(exp->u.dife, d);
		break;
	case ProjectionExp:
		prtProjectionExp(exp->u.proje, d);
		break;
	case SelectionExp:
		prtSelectionExp(exp->u.sele, d);
		break;
	case ProductExp:
		prtProductExp(exp->u.prode, d);
		break;
	case NaturalJoinExp:
		prtNaturalJoinExp(exp->u.natje, d);
		break;
	default:
		DEPRT("prtExpression\n");
		break;
	}
}
