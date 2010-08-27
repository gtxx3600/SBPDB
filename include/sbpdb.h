#ifndef SBPDB_H
#define SBPDB_H

#define BASEDIR		"data"

#define MAX_FILENAME   20

#define ALL_PAGES					-1
#define  NIL  -2
#define  ALL_PAGES  -1
#define PF_PAGE_SIZE  4092
#define OTHER  1
#define PASS_BREAK  4
#define ALL_PAGE_SIZE  4096
/*AttrType*/
#define TYPE_NUM	3
typedef int AttrType;
#define INT 		0
#define STRING	1
#define FLOAT		2
#define INT_SIZE	sizeof(int)
#define FLOAT_SIZE	sizeof(float)
#define STRING_SIZE	MAXSTRINGLEN+1
#define MAXSTRINGLEN	255
/*Compare Operator*/
#define OP_NUM	9
typedef int CompOp;
#define EQ_OP		0
#define LT_OP		1
#define GT_OP		2
#define LE_OP		3
#define GE_OP		4
#define NE_OP		5
#define NO_OP		6
#define INC_OP	7
#define DEC_OP	8

#define MAXNAME 25

#define NEED_SPLIT	99
#define NEED_UNION	100
/*Client Hint*/
typedef enum ClientHint {
	NO_HINT
}ClientHint;

typedef int RC;

#define   NORMAL					0
// Positive
#define	PF_EOF  				1             // end of file
#define	PF_PAGEPINNED			2       // page pinned in buffer
#define	PF_PAGENOTINBUF		3    // page to be unpinned is not in buffer
#define	PF_PAGEUNPINNED		4    // page already unpinned
#define	PF_PAGEFREE			5        // page already free
#define	PF_INVALIDPAGE		6     // invalid page number
#define	PF_FILEOPEN			7        // file handle already open
#define	PF_CLOSEDFILE			8      // file is closed
#define	PF_NOTEXIST			9			// file is not exist
#define	PF_EXIST				10			// file is exist
#define	PF_NOSCRATCH			11
#define	PF_PAGENOTEXIST		12

#define	RM_OUTOFTABLESIZE		13
#define	RM_OUTOFREC			14
#define	RM_ATTR_LENGTH_NOTRIGHT		15
#define	RM_EOF					16

#define	IX_TYPENOTSUPPORT		17
#define	IX_EXIST				18
#define	IX_NOTEXIST			19
#define 	IX_EOF					20
#define	IX_FULL				21

#define	SM_NODIR				50
#define SM_NOREL				51
#define SM_DIREXIST				52
#define SM_NODBSELECTED			53
#define SM_DUPLICATEATTR		54

#define	QL_WRONGVALUENUM		60
#define	QL_WRONGTYPE			61
#define	QL_WRONGATTRTYPE		62
#define QL_ATTRNOTFOUND			63
#define QL_ATTRALREADYFOUND		64
#define QL_TUPLENOTFOUND		65
#define QL_NOTSUPPORT			66
// Negative
#define	DB_PARAM				-1
#define	DB_INTERNAL			-2
#define	DB_FATAL				-3

#define	PF_NOMEM 				-15     // out of memory
#define	PF_NOBUF				-16           // out of buffer space
#define	PF_INCOMPLETEREAD		-17  // incomplete read of page from file
#define	PF_INCOMPLETEWRITE	-18 // incomplete write of page to file
#define	PF_HDRREAD				-19         // incomplete read of header from file
#define	PF_HDRWRITE			-20        // incomplete write of header to file
// Internal PF errors:
#define	PF_PAGEINBUF			-21       // new allocated page already in buffer
#define	PF_HASHNOTFOUND		-22    // hash table entry not found
#define	PF_HASHPAGEEXIST		-23   // page already exists in hash table
#define	PF_INVALIDNAME		-24     // invalid file name
#define	PF_UNIX				-25            // Unix error
#endif
