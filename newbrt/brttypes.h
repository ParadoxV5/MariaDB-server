#ifndef BRTTYPES_H
#define BRTTYPES_H

#ident "$Id$"
#ident "Copyright (c) 2007, 2008, 2009 Tokutek Inc.  All rights reserved."
#ident "The technology is licensed by the Massachusetts Institute of Technology, Rutgers State University of New Jersey, and the Research Foundation of State University of New York at Stony Brook under United States of America Serial No. 11/760379 and to the patents and/or patent applications resulting from it."

#include <sys/types.h>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif
#define _FILE_OFFSET_BITS 64

#include <db.h>
#include <inttypes.h>

typedef struct brt *BRT;
struct brt_header;
struct wbuf;

typedef unsigned int ITEMLEN;
typedef const void *bytevec;
//typedef const void *bytevec;

typedef int64_t DISKOFF;  /* Offset in a disk. -1 is the NULL pointer. */
typedef u_int64_t TXNID;
#define TXNID_NONE_LIVING ((TXNID)0)

typedef struct s_blocknum { int64_t b; } BLOCKNUM; // make a struct so that we will notice type problems.

static inline BLOCKNUM make_blocknum(int64_t b) { BLOCKNUM result={b}; return result; }

typedef struct {
    u_int32_t len;
    char *data;
} BYTESTRING;

/* Make the LSN be a struct instead of an integer so that we get better type checking. */
typedef struct __toku_lsn { u_int64_t lsn; } LSN;
#define ZERO_LSN ((LSN){0})
#define MAX_LSN  ((LSN){UINT64_MAX})

/* Make the FILEID a struct for the same reason. */
typedef struct __toku_fileid { u_int32_t fileid; } FILENUM;

#if !TOKU_WINDOWS && !defined(BOOL_DEFINED)
#define BOOL_DEFINED
typedef enum __toku_bool { FALSE=0, TRUE=1} BOOL;
#endif

typedef struct tokulogger *TOKULOGGER;
#define NULL_LOGGER ((TOKULOGGER)0)
typedef struct tokutxn    *TOKUTXN;
#define NULL_TXN ((TOKUTXN)0)

struct logged_btt_pair {
    DISKOFF off;
    int32_t size;
};

// The data that appears in the log to encode a brtheader. */
typedef struct loggedbrtheader {
    u_int32_t size;
    u_int32_t flags;
    u_int32_t nodesize;
    BLOCKNUM  free_blocks;
    BLOCKNUM  unused_blocks;
    BLOCKNUM  root;
    BLOCKNUM btt_size; // block translation table size
    DISKOFF  btt_diskoff;
    struct logged_btt_pair *btt_pairs;
} LOGGEDBRTHEADER;

typedef struct intpairarray {
    u_int32_t size;
    struct intpair {
	u_int32_t a,b;
    } *array;
} INTPAIRARRAY;

typedef struct cachetable *CACHETABLE;
typedef struct cachefile *CACHEFILE;

/* tree command types */
enum brt_cmd_type {
    BRT_NONE = 0,
    BRT_INSERT = 1,
    BRT_DELETE_ANY = 2,  // Delete any matching key.  This used to be called BRT_DELETE.
    BRT_DELETE_BOTH = 3,
    BRT_ABORT_ANY = 4,   // Abort any commands on any matching key.
    BRT_ABORT_BOTH  = 5, // Abort commands that match both the key and the value
    BRT_COMMIT_ANY  = 6,
    BRT_COMMIT_BOTH = 7
};

typedef struct xids_t *XIDS;
typedef struct fifo_msg_t *FIFO_MSG;
/* tree commands */
struct brt_cmd {
    enum brt_cmd_type type;
    XIDS         xids;
    union {
        /* insert or delete */
        struct brt_cmd_insert_delete {
            DBT *key;
            DBT *val;
        } id;
    } u;
};
typedef struct brt_cmd BRT_CMD_S, *BRT_CMD;

// TODO: replace brt_cmd_type when ready
typedef enum brt_cmd_type brt_msg_type;
// Message sent into brt to implement command (insert, delete, etc.)
// This structure supports nested transactions, and obsoletes brt_cmd.
typedef struct brt_cmd BRT_MSG_S, *BRT_MSG;

typedef int (*brt_compare_func)(DB *, const DBT *, const DBT *);

#define UU(x) x __attribute__((__unused__))

#endif

