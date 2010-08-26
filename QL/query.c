#include "sbpdb.h"
#include "ql.h"

AttrSel *attrToAttrSel(RelAttrList *al) {
	AttrSel *as = NULL, *p;
	while (al) {
		if (as == NULL) {
			as = NEW(AttrSel);
			if (al->a->relName)
				as->relName = strdup(al->a->relName);
			else
				as->relName = NULL;
			as->attrName = strdup(al->a->attrName);
			as->next = NULL;
			p = as;
		} else {
			p->next = NEW(AttrSel);
			if (al->a->relName)
				p->next->relName = strdup(al->a->relName);
			else
				p->next->relName = NULL;
			p->next->attrName = strdup(al->a->attrName);
			p->next->next = NULL;
			p = p->next;
		}
	}
	return ai;
}

AttrSel *catAttrSel(AttrSel *left, AttrSel *right, int leftsize) {
	AttrSel *p = GET_LAST(AttrSel, left, next);
	p->next = right;
	for (p = right; p; p = p->next) {
		p->offset += leftsize;
	}
	return left;
}

RM_Record *catRecord(RM_Record *left, RM_Record *right) {
	RM_Record *ret = NEW(RM_Record);
	initRM_Record(ret);
	ret->recordSize = left->recordSize + right->recordSize;
	ret->data = malloc(ret->recordSize);
	memcpy(ret->data, left->data, left->recordSize);
	memcpy(ret->data + left->recordSize, right->data, right->recordSize);
	return ret;
}

RC *QL_RelGetNext(QL_Manager *qlm, struct relation *exp,
		QL_Tuple *qlt) {
	qlt->rmr = NEW(RM_Record);
	if (exp->u.fs->GetNextRec(exp->u.fs, qlt->rmr) != NORMAL)
		return QL_TUPLENOTFOUND;
	qlt->as = exp->as; // TODO
	return NORMAL;
}

RC *QL_ProjGetNext(QL_Manager *qlm, struct projection_exp *exp,
		QL_Tuple *qlt) {
	AttrSel *as = attrToInfo(exp->al);
	RC re = QL_GetNext(exp->exp, qlt);
	if (re != NORMAL) return re;
	while (as) {
		AttrSel *p;
		int is_found = 0;
		for (p = qlt->as; p; p = p->next) {
			if ((!as->relName || !strcmp(as->relName, p->relName)) &&
					!strcmp(as->attrName, p->attrName)) {
				if (is_found) {
					fprintf(stderr, "Already Found: %s\n", as->attrName);
					return QL_ATTRARLRADYFOUND;
				}
				is_found = 1;
				as->attrType = p->attrType;
				as->offset = p->offset;
				as->attrLength = p->attrLength;
			}
		}
		if (!is_found) {
			fprintf(stderr, "Not Found: %s\n", as->attrName);
			return QL_ATTRNOTFOUND;
		}
	}
	qlt->as = as;
	return NORMAL;
}

RC *QL_SelGetNext(QL_Manager *qlm, struct selection_exp *exp,
		QL_Tuple *qlt) {

}

RC *QL_ProdGetNext(QL_Manager *qlm, struct product_exp *exp,
		QL_Tuple *qlt) {
	QL_Tuple t1, t2;
	QL_GetCurrent(qlm, exp->right, &t2);

	if (QL_GetNext(qlm, exp->left, &t1) == QL_TUPLENOTFOUND) {
		QL_ExpScanClose(qlm, exp->left);
		if (QL_GetNext(qlm, exp->right, &t2) == QL_TUPLENOTFOUND)
			return QL_TUPLENOTFOUND;
		QL_ExpScanOpen(qlm, exp->left);
		QL_GetNext(qlm, exp->left, &t1);
	}
	qlt->as = catAttrSel(t1.as, t2.as, t1.rmr->recordSize);
	qlt->rmr = catRecord(t1.rmr, t2.rmr);
	return NORMAL;
}

RC *QL_GetNext(QL_Manager *qlm, Expression *exp, QL_Tuple *qlt) {
	switch (exp->kind) {
		case Relation:
			return QL_RelGetNext(qlm, exp->u.rel, qlt);
		case ProjectionExp:
			return QL_ProjGetNext(qlm, exp->u.proje, qlt);
		case SelectionExp:
			return QL_SelGetNext(qlm, exp->u.sele, qlt);
		case ProductExp:
			return QL_ProdGetNext(qlm, exp->u.prode, qlt);
		case UnionExp:
		case IntersectionExp:
		case DifferenceExp:
		case NaturalJoinExp:
		default:
			fprintf(stderr, "not support\n");
			return QL_NOTSUPPORT;
	}
}
