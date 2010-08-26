#include "sbpdb.h"
#include "ql.h"

RM_Record *rmrCopy(RM_Record *src) {
	RM_Record *dst;
	if (src == NULL) return NULL;
	dst = NEW(RM_Record);
	initRM_Record(dst);
	dst->recordSize = src->recordSize;
	dst->data = malloc(dst->recordSize);
	memcpy(dst->data, src->data, dst->recordSize);
	dst->rid = src->rid;
	return dst;
}

AttrSel *attrSelCopy(AttrSel *src) {
	AttrSel *dst;
	if (src == NULL) return NULL;
	dst = NEW(AttrSel);
	if (src->relName)
		dst->relName = strdup(src->relName);
	else
		dst->relName = NULL;
	dst->attrName = strdup(src->attrName);
	dst->attrType = src->attrType;
	dst->attrLength = src->attrLength;
	dst->offset = src->offset;
	dst->next = attrSelCopy(src->next);
	return dst;
}

void destroyAttrSel(AttrSel *as) {
	if (as == NULL) return;
	destroyAttrSel(as->next);
	if (as->relName) free(as->relName);
	free(as->attrName);
	free(as);
}

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
	ret->rid = left->rid;
	return ret;
}

RC QL_ExpScanOpen(QL_Manager *qlm, Expression *exp);
RC QL_ExpScanClose(QL_Manager *qlm, Expression *exp);
RC QL_GetTuple(QL_Manager *qlm, Expression *exp, int isNext,
		QL_Tuple *qlt);

RC QL_GetCurrent(QL_Manager *qlm, Expression *exp, QL_Tuple *qlt) {
	return QL_GetTuple(qlm, exp, 0, qlt);
}

RC QL_GetNext(QL_Manager *qlm, Expression *exp, QL_Tuple *qlt) {
	return QL_GetTuple(qlm, exp, 1, qlt);
}

RC QL_RelExpScanClose(QL_Manager *qlm, struct relation *exp) {
	if (exp->cur) {
		free(exp->cur);
		exp->cur = NULL;
	}
	if (exp->isIndexed) {
		//TODO
	} else {
		free(exp->u.fs);
		exp->u.fs = NULL;
	}
	destroyAttrSel(exp->as);
	exp->as = NULL;
	return NORMAL;
}

RC QL_RelExpScanOpen(QL_Manager *qlm, struct relation *exp) {
	AttrCat *ac;
	int size, i;
	AttrSel *as = NULL, *asp;
	int ret = SM_GetAttrCats(qlm->smm, exp->id, &ac, &size);
	if (ret != NORMAL) return SM_NOREL;
	for (i = 0; i < size; i++) {
		AttrSel *tmp = NEW(AttrSel);
		tmp->attrName = strdup(ac[i].attrName);
		tmp->relName = strdup(ac[i].relName);
		tmp->attrType = ac[i].attrType;
		tmp->attrLength = ac[i].attrLength;
		tmp->offset = ac[i].offset;
		tmp->next = NULL;
		if (as == NULL) {
			asp = as = tmp;
		} else {
			asp = asp->next = tmp;
		}
	}
	free(ac);
	exp->as = as;
	exp->cur = NULL;
	exp->isIndexed = 0;
	exp->u.fs = NEW(RM_FileScan);
	initRM_FileScan(exp->u.fs);
}

RC QL_RelGetTuple(QL_Manager *qlm, struct relation *exp, int isNext,
		QL_Tuple *qlt) {
	if (isNext) {
		if (exp->cur == NULL) exp->cur = NEW(RM_Record);
		if (exp->u.fs->GetNextRec(exp->u.fs, exp->cur) != NORMAL)
			return QL_TUPLENOTFOUND;
		qlt->rmr = rmrCopy(exp->cur);
		qlt->as = attrSelCopy(exp->as);
	} else {
		if (exp->cur == NULL) return QL_TUPLENOTFOUND;
		qlt->rmr = rmrCopy(exp->cur);
		qlt->as = attrSelCopy(exp->as);
	}
	return NORMAL;
}

RC QL_ProjExpScanClose(QL_Manager *qlm, struct projection_exp *exp) {
	return QL_ExpScanClose(exp->exp);
}

RC QL_ProjExpScanOpen(QL_Manager *qlm, struct projection_exp *exp) {
	return QL_ExpScanOpen(exp->exp);
}

RC QL_ProjGetTuple(QL_Manager *qlm, struct projection_exp *exp, int isNext,
		QL_Tuple *qlt) {
	AttrSel *as = attrToAttrSel(exp->al);
	RC re = QL_GetTuple(exp->exp, isNext, qlt);
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
				if (!as->relName)
					as->relName = strdup(p->relName);
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

RC QL_SelExpScanClose(QL_Manager *qlm, struct selection_exp *exp) {
	return QL_ExpScanClose(exp->exp);
}

RC QL_SelExpScanOpen(QL_Manager *qlm, struct selection_exp *exp) {
	return QL_ExpScanOpen(exp->exp);
}

RC QL_SelGetTuple(QL_Manager *qlm, struct selection_exp *exp, int isNext,
		QL_Tuple *qlt) {

}

RC QL_ProdExpScanClose(QL_Manager *qlm, struct product_exp *exp) {
	RC ret;
	if ((ret = QL_ExpScanClose(qlm, exp->left)) != NORMAL) {
		return ret;
	}
	return QL_ExpScanClose(qlm, exp->right);
}

RC QL_ProdExpScanOpen(QL_Manager *qlm, struct product_exp *exp) {
	RC ret;
	if ((ret = QL_ExpScanOpen(qlm, exp->left)) != NORMAL) {
		return ret;
	}
	return QL_ExpScanOpen(qlm, exp->right);
}

RC QL_ProdGetTuple(QL_Manager *qlm, struct product_exp *exp, int isNext,
		QL_Tuple *qlt) {
	QL_Tuple t1, t2;

	if (isNext) {
		if (QL_GetCurrent(qlm, exp->right, &t2) == QL_TUPLENOTFOUND) {
			if (QL_GetNext(qlm, exp->right, &t2) == QL_TUPLENOTFOUND)
				return QL_TUPLENOTFOUND;
		}
		if (QL_GetNext(qlm, exp->left, &t1) == QL_TUPLENOTFOUND) {
			if (QL_GetNext(qlm, exp->right, &t2) == QL_TUPLENOTFOUND)
				return QL_TUPLENOTFOUND;
			QL_ExpScanClose(qlm, exp->left);
			QL_ExpScanOpen(qlm, exp->left);
			QL_GetNext(qlm, exp->left, &t1);
		}
	} else {
		if (QL_GetCurrent(qlm, exp->right, &t2) == QL_TUPLENOTFOUND) {
			return QL_TUPLENOTFOUND;
		}
		if (QL_GetCurrent(qlm, exp->left, &t1) == QL_TUPLENOTFOUND) {
			return QL_TUPLENOTFOUND;
		}
	}
	qlt->as = catAttrSel(t1.as, t2.as, t1.rmr->recordSize);
	qlt->rmr = catRecord(t1.rmr, t2.rmr);
	return NORMAL;
}

RC QL_ExpScanClose(QL_Manager *qlm, Expression *exp) {
	switch (exp->kind) {
		case Relation:
			return QL_RelExpScanClose(qlm, exp->u.rel);
		case ProjectionExp:
			return QL_ProjExpScanClose(qlm, exp->u.proje);
		case SelectionExp:
			return QL_SelExpScanClose(qlm, exp->u.sele);
		case ProductExp:
			return QL_ProdExpScanClose(qlm, exp->u.prode);
		case UnionExp:
		case IntersectionExp:
		case DifferenceExp:
		case NaturalJoinExp:
		default:
			fprintf(stderr, "not support\n");
			return QL_NOTSUPPORT;
	}
}

RC QL_ExpScanOpen(QL_Manager *qlm, Expression *exp) {
	switch (exp->kind) {
		case Relation:
			return QL_RelExpScanOpen(qlm, exp->u.rel);
		case ProjectionExp:
			return QL_ProjExpScanOpen(qlm, exp->u.proje);
		case SelectionExp:
			return QL_SelExpScanOpen(qlm, exp->u.sele);
		case ProductExp:
			return QL_ProdExpScanOpen(qlm, exp->u.prode);
		case UnionExp:
		case IntersectionExp:
		case DifferenceExp:
		case NaturalJoinExp:
		default:
			fprintf(stderr, "not support\n");
			return QL_NOTSUPPORT;
	}
}

RC QL_GetTuple(QL_Manager *qlm, Expression *exp, int isNext,
		QL_Tuple *qlt) {
	switch (exp->kind) {
		case Relation:
			return QL_RelGetTuple(qlm, exp->u.rel, isNext, qlt);
		case ProjectionExp:
			return QL_ProjGetTuple(qlm, exp->u.proje, isNext, qlt);
		case SelectionExp:
			return QL_SelGetTuple(qlm, exp->u.sele, isNext, qlt);
		case ProductExp:
			return QL_ProdGetTuple(qlm, exp->u.prode, isNext, qlt);
		case UnionExp:
		case IntersectionExp:
		case DifferenceExp:
		case NaturalJoinExp:
		default:
			fprintf(stderr, "not support\n");
			return QL_NOTSUPPORT;
	}
}
