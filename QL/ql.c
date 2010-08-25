#include "ql.h"

RC QL_Select(QL_Manager *self, Expression *exp) {

}

RC QL_Insert(QL_Manager *self, char *relName, ValueList *values) {
	char *pData;
	int i, nValues = 0;
	ValueList *p = values;
	while (p) nValues++;
	for (i = 0; i < self->smm->relCount; i++)
        if (strcmp(RelGetRelName(&(self->smm->relRecords[i])), relName) == 0) {
            if (RelGetAttrCount(&(self->smm->relRecords[i])) != nValues)
                return QL_WRONGVALUENUM;
            pData = malloc(RelGetTupleLength(&(self->smm.relRecords[i])));
        }
	p = values;
    for (i = 0; i < self->smm->attrCount; i++)
        if (strcmp(AttrRelGetRelName(&(self->smm->attrRecords[i])), 
				relName) == 0) {
            if (AttrGetAttrType(&(self->smm->attrRecords[i])) != 
					p->type)
                return QL_WRONGVALUETYPE;
            memcpy(pData + AttrGetOffset(&(self->smm->attrRecords[i])),
                   p->data, AttrGetAttrLength(&(self->smm->attrRecords[i])));
            p = p->next;
        }
    RM_FileHandle rmFile;
    RID rid;
    self->rmm->OpenFile(self->rmm, relName, rmFile);
    rmFile.InsertRec(pData, rid);
    self->rmm->CloseFile(self->rmm, rmFile);
    // TODO: insert index
    free(pData);
}

RC QL_Delete(QL_Manager *self, char *relName, Expression *exp) {

}

RC QL_Update(QL_Manager *self, char *relName, RelAttr *upAttr,
		int isValue, RelAttr *rRelAttr, Value *rValue,
		Expression *exp) {

}

RC initQL_Manager(QL_Manager *self, SM_Manager *smm,
		IX_Manager *ixm, RM_Manager *rmm) {
	self->smm = smm;
	self->ixm = ixm;
	self->rmm = rmm;
	return NORMAL;
}
