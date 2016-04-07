/******************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
   Memory management: interface
*******************************************************************/
#ifndef __RFALLOC_H
#define __RFALLOC_H
#include "refgen.h"
#include <stdio.h>
#include <string.h>

/* This module implements functions to allocate dynamic memory segments
It's main features are:
    The memory segment address can be stored in a headder (field NEXT).
    Each (unlocked) memory segment is connected with it's own headder.
    The application accesses the segment via the headder.
    The segment when accessed is locked to prevent moving.
    When no propper hole exist a squeeze operation is performed,
    during which all unlocked segments move downmost. */

/*  To access a segment via the headder h use a variable a of the type adr
and macro HDGETA. To store the adr a into the headder h use macro HDSETA. */

/* Memory model dependencies */
#ifdef SPLIT
typedef unsigned adr;
#define MEM(a) ((block *) (MK_FP(a,0)))
#define MEMOFF(a,o) ((uchar*)(MK_FP(a,o)))
#define NOADR 0
#else
typedef void * adr;
#define MEM(a) ((block *) a)
#define MEMOFF(a,o) (((uchar*) a) + o)
#define NOADR NULL
#endif

/* Interface */

void rf_mem_init();

/* the structure of starting block of memory segment
   or free segment or hole */
/* typedef unsigned seglen; (see rfstor.h) */
typedef union {
   struct {
	  headptr head;   /* NULL when segment is LOCKED */
	  seglen  slen;   /* size of user information in bytes */
      } tit;
   headptr zzz[2];
   } headblk;

typedef union {
	uchar whole[16];
	struct {
        headblk hd;
		uchar info[8];  /* start of user information */
		} segm;
	} block;





/* Types asumed below are: headptr h; adr a; size_t size;  */

#define SEGINF(a) ((void *) (MEM(a)->segm.info)) /* pointer to user info */
#define SEGLEN(a) (MEM(a)->segm.hd.tit.slen)     /* the length of user info */
#define REALLOC(a,size)  (a=rf_realloc(a,size))
#define ALLOC(a,size)    (a=rf_realloc(anull,size))
#define ANULL anull /* this is an empty segment (size=0) */
adr rf_realloc(adr A, seglen size);
void* rmalloc(seglen size);
void rfree(void*);
void rf_heap_stat(FILE * f);
void rf_prholes(FILE* f);
extern adr anull;
#define halloc(l,s) rmalloc(((seglen)l)*((seglen)s))  /* turboc */

/* macros to store adr in object headder (using NEXT field) */
#define HDGETA(h,a)  { a = AD(h); };
#define HDSETA(h,a)  { AD(h) = a; };

/* macros to store far pointer (i.g. FILE*) in object headder
   (using NEXT and PREV fields) */
#ifdef SPLIT
#define HDGETFAR(h,a) a = MK_FP(PREV(h),NEXT(h));
#define HDSETFAR(h,a) { PREV(h) = FP_SEG(a); NEXT(h) = FP_OFF(a); }
#else
#define HDGETFAR(h,a) (elemptr)a = NEXT(h);
#define HDSETFAR(h,a) NEXT(h) = (elemptr)a;
#endif

/*******************************************************************/
/* Private macros */
#define SEGHEAD(a) (MEM(a)->segm.hd.tit.head) /* pointer to headder */
#define SEGSIZE(l) (HEADSIZE+l)
#define HEADSIZE sizeof(headblk)
#define AD(h) (adr)NEXT(h)    /* headptr  -> adr */
#define   LOCK(h)  SEGHEAD(AD(h))=NOHEAD;
#define UNLOCK(h)  SEGHEAD(AD(h))=h;
#define LOCKED(a)  (SEGHEAD(a)==NOHEAD)

/* The function rf_realloc tries to reallocate the previously
   allocated segment. If A is anull, the segment is newly allocated.
   It returns ANULL if size=0, and NULL if the segment cannot be allocated.
   The state of new segment is LOCKED.
   When size=0 it is a FREE operation.
*/

#endif