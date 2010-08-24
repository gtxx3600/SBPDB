/*
 * AttrOpFunc.c
 *
 *  Created on: 2010-8-24
 *      Author: hhf
 */
#include "rm.h"
#include <string.h>
int defaultOP(void* left, void* right, int len)
{
	return 0;
}
/*    INT    */
int intEQ(void* left,void* right,int len)
{
	return *(int*)left == *(int*)right;
}
int intLT(void* left,void* right,int len)
{
	return *(int*)left < *(int*)right;
}
int intGT(void* left,void* right,int len)
{
	return *(int*)left > *(int*)right;
}
int intLE(void* left,void* right,int len)
{
	return *(int*)left <= *(int*)right;
}
int intGE(void* left,void* right,int len)
{
	return *(int*)left >= *(int*)right;
}
int intNE(void* left,void* right,int len)
{
	return *(int*)left != *(int*)right;
}
int (*intOP[OP_NUM])(void* left,void*right,int len) =
{
		intEQ,
		intLT,
		intGT,
		intLE,
		intGE,
		intNE,
		defaultOP,
};

/*    STRING    */
int strEQ(void* left,void* right,int len)
{
	return strncmp((char*)left,(char*)right,len) == 0;
}
int strLT(void* left,void* right,int len)
{
	return strncmp((char*)left,(char*)right,len) < 0;
}
int strGT(void* left,void* right,int len)
{
	return strncmp((char*)left,(char*)right,len) > 0;
}
int strLE(void* left,void* right,int len)
{
	return strncmp((char*)left,(char*)right,len) <= 0;
}
int strGE(void* left,void* right,int len)
{
	return strncmp((char*)left,(char*)right,len) >= 0;
}
int strNE(void* left,void* right,int len)
{
	return strncmp((char*)left,(char*)right,len) != 0;
}

int (*strOP[OP_NUM])(void* left,void*right,int len) =
{
		strEQ,
		strLT,
		strGT,
		strLE,
		strGE,
		strNE,
		defaultOP,
};

/*    FLOAT    */
int floatEQ(void* left,void*right,int len)
{
	return *(float*)left == *(float*)right;
}
int floatLT(void* left,void*right,int len)
{
	return *(float*)left < *(float*)right;
}
int floatGT(void* left,void*right,int len)
{
	return *(float*)left > *(float*)right;
}
int floatLE(void* left,void*right,int len)
{
	return *(float*)left <= *(float*)right;
}
int floatGE(void* left,void*right,int len)
{
	return *(float*)left >= *(float*)right;
}
int floatNE(void* left,void*right,int len)
{
	return *(float*)left != *(float*)right;
}

int (*floatOP[OP_NUM])(void* left,void*right,int len) =
{
		floatEQ,
		floatLT,
		floatGT,
		floatLE,
		floatGE,
		floatNE,
		defaultOP,
};
