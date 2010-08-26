#include "ql.h"

void QL_PrintHeader(RelAttrList *al) {

}

void QL_PrintTuple(QL_Tuple *tuple) {

}

void QL_PrintSummary(int count) {

}

RC QL_Select(QL_Manager *self, Expression *exp, RelAttrList *al) {
	QL_Tuple tuple;
	int count = 0, ret;
	QL_ScanOpen(self, exp);
	QL_PrintHeader(al);
	while ((ret = QL_GetNext(self, exp, &tuple)) == NORMAL) {
		QL_PrintTuple(&tuple);
		count++;
	}
	QL_ScanClose(self, exp);
	QL_PrintSummary(count);
	return NORMAL;
}

RC QL_Insert(QL_Manager *self, char *relName, ValueList *values) {
	char *pData;
	int i, nValues = 0;
	ValueList *p = values;
	while (p) nValues++;
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
                return QL_WRONGVALUETYPE;
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
