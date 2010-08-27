/*
 * IX_IndexScan.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */
#include "ix.h"
#include "BTNode.h"
extern int (**typeOP[TYPE_NUM])(void* left,void*right,int len);
RC IX_OpenScan(IX_IndexScan* this, IX_IndexHandle *idxh, CompOp op, void* value, ClientHint ch)
{
	this->op = op;
	this->value = value;
	this->idxh = idxh;
	this->ch = ch;
	this->inEQregin = 0;
	firstOP(idxh,value,&this->left,&this->left_offset,EQ_OP);
	firstOP(idxh,value,&this->right,&this->right_offset,GT_OP);


	switch(op)
	{
	case LT_OP:
	case LE_OP:
	case NE_OP:
		this->curr = findStartPage(idxh);
		this->curr_offset = 0;
		break;
	case GT_OP:
		this->curr = this->right;
		this->curr_offset = this->right_offset;
		break;
	case GE_OP:
	case EQ_OP:
		this->curr = this->left;
		this->curr_offset = this->left_offset;
		break;
	default:
		this->curr = this->left;
		this->curr_offset = this->left_offset;
		break;
	}
	return NORMAL;
}
void checkCurr(IX_IndexScan* this)
{
	int attrLength = this->idxh->head.attrLength;
#include "NODEL.h"
	PF_FileHandle *pffh = &(this->idxh->pffh);
	PF_PageHandle cph;
	initPF_PageHandle(&cph);
	PageNum tmp = this->curr;
	pffh->GetThisPage(pffh, this->curr, &cph);
	NODE* n = (NODE*)cph.page;
	if(this->curr_offset == n->totalEntry)
	{
		this->curr = n->pointers[ENTRYSINBTNODE].page;
		this->curr_offset = 0;
	}
	if(this->curr == -1)
	{
		this->end = 1;
	}
	pffh->UnpinPage(pffh, tmp);
}
RC IX_GetNextEntry(IX_IndexScan* this, RID *rid)
{
	if(this->end)return IX_EOF;

	int attrLength = this->idxh->head.attrLength;
	checkCurr(this);
	#include "NODEL.h"
	PF_FileHandle *pffh = &(this->idxh->pffh);
	PF_PageHandle cph;
	initPF_PageHandle(&cph);
	int ret = NORMAL;
	pffh->GetThisPage(pffh, this->curr, &cph);
	NODE* n = (NODE*)cph.page;
	printf("getNextEntry: currpage:%d, curr offset:%d\n",this->curr,this->curr_offset);
	if(typeOP[this->idxh->head.attrType][this->op](&n->values[this->curr_offset], this->value, attrLength))
	{
		rid->pageNum = n->pointers[this->curr_offset].page;
		rid->slotNum = n->pointers[this->curr_offset].slot;
	}else
	{
		this->end = 1;
		ret = IX_EOF;
	}
	this->curr_offset ++;
	checkCurr(this);
	if(this->op == NE_OP && this->curr == this->left && this->curr_offset == this->left_offset)
	{
		this->curr = this->right;
		this->curr_offset = this->right_offset;
	}else if(this->op == LT_OP && this->curr == this->left && this->curr_offset == this->left_offset)
	{
		this->end = 1;
	}else if(this->op == LE_OP && this->curr == this->right && this->curr_offset == this->right_offset)
	{
		this->end = 1;
	}
	else if(this->op == EQ_OP && this->curr == this->right && this->curr_offset == this->right_offset)
	{
		this->end = 1;
	}
	return ret;
}
RC IX_CloseScan(IX_IndexScan* this)
{
	return NORMAL;
}

RC initIX_IndexScan(IX_IndexScan *this)
{
	this->OpenScan = IX_OpenScan;
	this->GetNextEntry = IX_GetNextEntry;
	this->CloseScan = IX_CloseScan;
	this->end = 0;
	return NORMAL;
}
