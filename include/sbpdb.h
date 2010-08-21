#ifndef SBPDB_H
#define SBPDB_H

enum AttrType { INT, STRING, FLOAT };

enum CompOp { EQ_OP, LT_OP, GT_OP, LE_OP, GE_OP, NE_OP, NO_OP };

enum ClientHint { NO_HINT };

enum RC {
    NORMAL = 0,
    // Positive
    PF_EOF,             // end of file
    PF_PAGEPINNED,      // page pinned in buffer
    PF_PAGENOTINBUF,    // page to be unpinned is not in buffer
    PF_PAGEUNPINNED,    // page already unpinned
    PF_PAGEFREE,        // page already free
    PF_INVALIDPAGE,     // invalid page number
    PF_FILEOPEN,        // file handle already open
    PF_CLOSEDFILE,      // file is closed
	PF_UNEXIST,			// file is not exist
	PF_EXIST,			// file is exist
	PF_NOSCRATCH,
	PF_PAGENOTEXIST,

	RM_OUTOFTABLESIZE,
	RM_OUTOFREC,
	RM_ATTR_LENGTH_NOTRIGHT,
	RM_EOF,

    IX_TYPENOTSUPPORT,
    IX_EXIST,
    IX_NOTEXIST,

    SM_NODIR,

    QL_WRONGVALUENUM,
    QL_WRONGVALUETYPE,
    QL_WRONGATTRTYPE,

    // Negative
    PF_NOMEM = -15,     // out of memory
    PF_NOBUF,           // out of buffer space
    PF_INCOMPLETEREAD,  // incomplete read of page from file
    PF_INCOMPLETEWRITE, // incomplete write of page to file
    PF_HDRREAD,         // incomplete read of header from file
    PF_HDRWRITE,        // incomplete write of header to file
    // Internal PF errors:
    PF_PAGEINBUF,       // new allocated page already in buffer
    PF_HASHNOTFOUND,    // hash table entry not found
    PF_HASHPAGEEXIST,   // page already exists in hash table
    PF_INVALIDNAME,     // invalid file name
    PF_UNIX,            // Unix error

};

#endif
