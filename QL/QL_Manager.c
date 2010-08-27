#include "ql.h"
#include "AttrOpFunc.h"

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

int checkPrimaryKey(QL_Manager *self, char *relName,
		void *data, int type, int length, int offset, RID *rid) {
	RM_FileHandle fh;
	initRM_FileHandle(&fh);
	self->rmm->OpenFile(self->rmm, relName, &fh);
	RM_FileScan fs;
	initRM_FileScan(&fs);
	fs.OpenScan(&fs, &fh, type, length, offset, EQ_OP, data, NO_HINT);
	RM_Record rmr;
	initRM_Record(&rmr);
	while (fs.GetNextRec(&fs, &rmr) == NORMAL) {
		if (rid && rmr.rid.pageNum == rid->pageNum &&
				rmr.rid.slotNum == rid->slotNum) {
			continue;
		}
		return 1;
	}
	return 0;
}

int checkCheck(QL_Manager *self, AttrCat *ac, void *data) {
	int b;
	if (ac->op == NO_OP) return 0;
	b = typeOP[ac->attrType][ac->op](data, ac->rvalue,
			ac->attrLength);
	return !b;
}

int isUniqueInRel(QL_Manager *self, char *relName, char *attrName,
		void *data) {
	AttrCat *ac = getAttrCat(self->smm, relName, attrName);
	RM_FileHandle fh;
	RM_FileScan fs;
	RM_Record rmr;
	int count = 0;
	initRM_Record(&rmr);
	initRM_FileHandle(&fh);
	initRM_FileScan(&fs);
	self->rmm->OpenFile(self->rmm, relName, &fh);
	fs.OpenScan(&fs, &fh, ac->attrType, ac->attrLength, ac->offset,
			EQ_OP, data, NO_HINT);
	while (fs.GetNextRec(&fs, &rmr) == NORMAL) {
		count++;
	}
	return (count == 1);
}

int isExistInRel(QL_Manager *self, char *relName, char *attrName,
		void *data) {
	AttrCat *ac = getAttrCat(self->smm, relName, attrName);
	RM_FileHandle fh;
	RM_FileScan fs;
	RM_Record rmr;
	initRM_Record(&rmr);
	initRM_FileHandle(&fh);
	initRM_FileScan(&fs);
	self->rmm->OpenFile(self->rmm, relName, &fh);
	fs.OpenScan(&fs, &fh, ac->attrType, ac->attrLength, ac->offset,
			EQ_OP, data, NO_HINT);
	while (fs.GetNextRec(&fs, &rmr) == NORMAL) {
		return 1;
	}
	return 0;
}

int checkForiegnKey(QL_Manager *self, AttrCat *ac, void *data) {
	int b;
	if (!strcmp(ac->fkrelName, "") || !strcmp(ac->fkattrName, ""))
		return 0;
	b = isExistInRel(self, ac->fkrelName, ac->fkattrName,
			data);
	return !b;
}

int checkForiegnKey2(QL_Manager *self, AttrSel *as, void *data) {
	int b = isUniqueInRel(self, as->relName, as->attrName, data);
	if (!b) return 0;

	char *fname = malloc(2*MAXNAME+3);

	strcpy(fname, ".");
	strcat(fname, as->relName);
	strcat(fname, ".");
	strcat(fname, as->attrName);

	RM_FileHandle fh;
	RM_FileScan fs;
	RM_Record rmr;
	initRM_Record(&rmr);
	initRM_FileHandle(&fh);
	initRM_FileScan(&fs);
	self->rmm->OpenFile(self->rmm, fname, &fh);
	fs.OpenScan(&fs, &fh, 0, 0, 0, NO_OP, NULL, NO_HINT);
	while (fs.GetNextRec(&fs, &rmr) == NORMAL) {
		fkInfo *fkp = (fkInfo *)rmr.data;
		b = isExistInRel(self, fkp->relName, fkp->attrName,
				data);
		if (b) return 1;
	}
	free(fname);
	return 0;
}

RC QL_Insert(QL_Manager *self, char *relName, ValueList *values) {
	char *pData, *pk = NULL;
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
            pk = self->smm->relRecords[i].primaryKey;
        }
	p = values;
	int b;
    for (i = 0; i < self->smm->attrCount; i++)
        if ((b=strcmp(self->smm->attrRecords[i].relName, relName)) == 0) {
        	if (strcmp(self->smm->attrRecords[i].attrName, pk) == 0) {
        		if (checkPrimaryKey(self, relName, p->v->data,
        				self->smm->attrRecords[i].attrType,
        				self->smm->attrRecords[i].attrLength,
        				self->smm->attrRecords[i].offset,
        				NULL)) {
        			return QL_DUPLICATE;
        		}
        	}
            if (self->smm->attrRecords[i].attrType != p->v->type)
                return QL_WRONGTYPE;
            if (checkCheck(self, &self->smm->attrRecords[i],
            		p->v->data)) {
            	return QL_CHECKFAIL;
            }
            if (checkForiegnKey(self, &self->smm->attrRecords[i],
            		p->v->data)) {
            	return QL_FOREIGNNOFOUND;
            }
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
		if (checkForiegnKey2(self, tuple.as,
				tuple.rmr->data + tuple.as->offset)) {
			return QL_ONLYFK;
		}
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
			if (checkPrimaryKey(self, relName, right, as->attrType,
					len, as->offset, &tuple.rmr->rid)) {
				return QL_DUPLICATE;
			}
			AttrCat *ac = getAttrCat(self->smm,
					as->relName, as->attrName);
			if (checkCheck(self, ac, right)) {
				return QL_CHECKFAIL;
			}
            if (checkForiegnKey(self, ac, right)) {
            	return QL_FOREIGNNOFOUND;
            }
            if (checkForiegnKey2(self, as, left)) {
            	return QL_ONLYFK;
            }
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
