#ifndef SBPDB_H
#define SBPDB_H

enum AttrType { INT, STRING, FLOAT };

enum CompOp { EQ_OP, LT_OP, GT_OP, LE_OP, GE_OP, NE_OP, NO_OP };

enum ClientHint { NO_HINT };

typedef int RC;


#define   NORMAL  0
    // Positive
#define   PF_EOF  				1             // end of file
#define   PF_PAGEPINNED			2       // page pinned in buffer
#define	PF_PAGENOTINBUF		3    // page to be unpinned is not in buffer
#define    PF_PAGEUNPINNED		4    // page already unpinned
#define    PF_PAGEFREE			5        // page already free
#define    PF_INVALIDPAGE		6     // invalid page number
#define    PF_FILEOPEN			7        // file handle already open
#define    PF_CLOSEDFILE			8      // file is closed
#define	PF_UNEXIST				9			// file is not exist
#define	PF_EXIST				10			// file is exist
#define	PF_NOSCRATCH			11
#define	PF_PAGENOTEXIST		12

#define	RM_OUTOFTABLESIZE		13
#define	RM_OUTOFREC			14
#define	RM_ATTR_LENGTH_NOTRIGHT		15
#define	RM_EOF					16

#define    IX_TYPENOTSUPPORT	17
#define    IX_EXIST				18
#define    IX_NOTEXIST			19

#define    SM_NODIR				20

#define    QL_WRONGVALUENUM		21
#define    QL_WRONGVALUETYPE	22
#define    QL_WRONGATTRTYPE		23
    // Negative
#define    PF_NOMEM 				-15     // out of memory
#define    PF_NOBUF				-16           // out of buffer space
#define    PF_INCOMPLETEREAD	-17  // incomplete read of page from file
#define    PF_INCOMPLETEWRITE	-18 // incomplete write of page to file
#define    PF_HDRREAD			-19         // incomplete read of header from file
#define    PF_HDRWRITE			-20        // incomplete write of header to file
    // Internal PF errors:
#define    PF_PAGEINBUF			-21       // new allocated page already in buffer
#define    PF_HASHNOTFOUND		-22    // hash table entry not found
#define    PF_HASHPAGEEXIST		-23   // page already exists in hash table
#define    PF_INVALIDNAME		-24     // invalid file name
#define    PF_UNIX				-25            // Unix error


#endif
