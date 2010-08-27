#include "ql.h"

void QL_PrintHeader(RelAttrList *al) {
	RelAttrList *p = al;
	while (p) {
		printf("%s", p->a->attrName);
		p = p->next;
		if (p == NULL) break;
		printf("\t");
	}
	printf("\n");
}

void QL_PrintTuple(QL_Tuple *tuple) {
	AttrSel *p = tuple->as;
	while (p) {
		switch (p->attrType) {
		case INT:
			printf("%d", *(int *)(tuple->rmr->data+p->offset));
		case STRING:
			printf("%s", (char *)(tuple->rmr->data+p->offset));
		case FLOAT:
			printf("%f", *(float *)(tuple->rmr->data+p->offset));
		}
		p = p->next;
		if (p == NULL) break;
		printf("\t");
	}
	printf("\n");
}

void QL_PrintSummary(int count) {
	printf("summary\n");
}

RC QL_Select(QL_Manager *self, Expression *exp, RelAttrList *al) {
	QL_Tuple tuple;
	int count = 0, ret;
	exp = QL_transExp(exp);
	if ((ret = QL_ExpScanOpen(self, exp)) != NORMAL) {
		return ret;
	}
	QL_PrintHeader(al);
	while ((ret = QL_GetNext(self, exp, &tuple)) == NORMAL) {
		QL_PrintTuple(&tuple);
		count++;
	}
	QL_ExpScanClose(self, exp);
	QL_PrintSummary(count);
	return NORMAL;
}

RC QL_Insert(QL_Manager *self, char *relName, ValueList *values) {
	char *pData;
	int i, nValues = 0;
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
    self->rmm->OpenFile(self->rmm, relName, &rmFile);
    rmFile.InsertRec(&rmFile, pData, &rid);
    self->rmm->CloseFile(self->rmm, &rmFile);
    // TODO: insert index
    free(pData);
    return NORMAL;
}

RC QL_Delete(QL_Manager *self, char *relName, Expression *exp) {
	return NORMAL; // TODO
}

RC QL_Update(QL_Manager *self, char *relName, RelAttr *upAttr,
		int isValue, RelAttr *rRelAttr, Value *rValue,
		Expression *exp) {
	return NORMAL; // TODO
}

RC initQL_Manager(QL_Manager *self, SM_Manager *smm,
		IX_Manager *ixm, RM_Manager *rmm) {
	self->smm = smm;
	self->ixm = ixm;
	self->rmm = rmm;
	return NORMAL;
}
