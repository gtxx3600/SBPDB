#include "ql.h"

void QL_PrintTuple(QL_Tuple *tuple, int h) {
	AttrSel *p = tuple->as;
	if (h) {
		while (p) {
			printf("%s", p->attrName);
			p = p->next;
			if (p == NULL) break;
			printf("\t");
		}
		printf("\n");
		p = tuple->as;
	}
	while (p) {
		switch (p->attrType) {
		case INT:
			printf("%d INT", *(int *)(tuple->rmr->data+p->offset));
			break;
		case STRING:
			printf("%s STRING", (char *)(tuple->rmr->data+p->offset));
			break;
		case FLOAT:
			printf("%f FLOAT", *(float *)(tuple->rmr->data+p->offset));
			break;
		}
		p = p->next;
		if (p == NULL) break;
		printf("\t");
	}
	printf("\n");
}

void QL_PrintSummary(int count) {
	printf("Total %d\n", count);
}

RC QL_Select(QL_Manager *self, Expression *exp) {
	QL_Tuple tuple;
	int count = 0, ret, h = 1;
	exp = QL_transExp(exp);
//	prtExpression(exp, 0);
	if ((ret = QL_ExpScanOpen(self, exp)) != NORMAL) {
		return ret;
	}
	while ((ret = QL_GetNext(self, exp, &tuple)) == NORMAL) {
		QL_PrintTuple(&tuple, h);
		h = 0;
		count++;
	}
	QL_ExpScanClose(self, exp);
	QL_PrintSummary(count);
	return NORMAL;
}

RC QL_Insert(QL_Manager *self, char *relName, ValueList *values) {
	char *pData;
	int i, nValues = 0, ret;
	ValueList *p = values;
	while (p) {
		nValues++;
		p = p->next;
	}
	for (i = 0; i < self->smm->relCount; i++)
        if (strcmp(self->smm->relRecords[i].relName, relName) == 0) {
            if (self->smm->relRecords[i].attrCount != nValues)
                return QL_WRONGVALUENUM;
            pData = malloc(self->smm->relRecords[i].tupleLength);
        }
	p = values;
    for (i = 0; i < self->smm->attrCount; i++)
        if (strcmp(self->smm->attrRecords[i].relName, relName) == 0) {
            if (self->smm->attrRecords[i].attrType != p->v->type)
                return QL_WRONGTYPE;
            memcpy(pData + self->smm->attrRecords[i].offset,
                   p->v->data, self->smm->attrRecords[i].attrLength);
            p = p->next;
        }
    RM_FileHandle rmFile;
    RID rid;
    initRM_FileHandle(&rmFile);
    initRID(&rid, 0, 0);
    self->rmm->OpenFile(self->rmm, relName, &rmFile);
    ret = rmFile.InsertRec(&rmFile, pData, &rid);
    printf("rid:    %d, %d\n", rid.pageNum, rid.slotNum);
    self->rmm->CloseFile(self->rmm, &rmFile);
    // TODO: insert index
    free(pData);
    return NORMAL;
}

RC QL_Delete(QL_Manager *self, char *relName, Expression *exp) {
	QL_Tuple tuple;
	RM_FileHandle fh;
	int count = 0, ret, h = 1;
	exp = QL_transExp(exp);
	if ((ret = QL_ExpScanOpen(self, exp)) != NORMAL) {
		return ret;
	}
	initRM_FileHandle(&fh);
	self->rmm->OpenFile(self->rmm, relName, &fh);
	while ((ret = QL_GetNext(self, exp, &tuple)) == NORMAL) {
		QL_PrintTuple(&tuple, h);
		fh.DeleteRec(&fh, &tuple.rmr->rid);
		h = 0;
		count++;
	}
	self->rmm->CloseFile(self->rmm, &fh);
	QL_ExpScanClose(self, exp);
	QL_PrintSummary(count);
	return NORMAL;
}

RC QL_Update(QL_Manager *self, char *relName, AssignmentList *agl,
		Expression *exp) {
	QL_Tuple tuple;
	RM_FileHandle fh;
	int count = 0, ret, h = 1;
	exp = QL_transExp(exp);
	if ((ret = QL_ExpScanOpen(self, exp)) != NORMAL) {
		return ret;
	}
	initRM_FileHandle(&fh);
	self->rmm->OpenFile(self->rmm, relName, &fh);
	while ((ret = QL_GetNext(self, exp, &tuple)) == NORMAL) {
		AssignmentList *p;
		QL_PrintTuple(&tuple, h);
		for (p = agl; p; p = p->next) {
			AttrSel *as = findAttrSel(tuple.as, p->left);
			int len = as->attrLength;
			char *left = tuple.rmr->data + as->offset;
			char *right = getDataFromRecord(tuple.as,
					tuple.rmr, p->right);
			memcpy(left, right, len);
			fh.UpdateRec(&fh, tuple.rmr);
		}
		h = 0;
		count++;
	}
	self->rmm->CloseFile(self->rmm, &fh);
	QL_ExpScanClose(self, exp);
	QL_PrintSummary(count);
	return NORMAL;
}

RC initQL_Manager(QL_Manager *self, SM_Manager *smm,
		IX_Manager *ixm, RM_Manager *rmm) {
	self->smm = smm;
	self->ixm = ixm;
	self->rmm = rmm;
	return NORMAL;
}
