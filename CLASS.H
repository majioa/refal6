/******************************************************************
   R E F A L - 6      Copyright (C) 1992 by Arkady Klimov 
   Definitions for building classes and objects
*******************************************************************/
/*   Object is a special kind of element == headder.
The TYPE of headder contains an index to the PFUNC table.
The HDPFUN(h) is a p-function of an object.
An object body is stored in fields NEXT and PREV of the headder.
In general, an adress of a segment is in the NEXT field (see rfalloc.h).
The P-function accepts a message code and an object headder
and returns a pointer to a certain i-function (which is able
to execute the inherent reaction of the object for the message).
As an auxiliary result the p-function returns a pointer
which should be passed to the i-function as an argument.
As usual, an i-function returns int.
*/

#ifndef __CLASS_H
#define __CLASS_H
#define _CLASS

#ifndef __REFELEM_H
#include "refelem.h"
#endif

#ifndef CLASS_EXT
#define CLASS_EXT extern
#endif

typedef void * addr;
typedef int (*ifunc) ();    /* i-function */
typedef ifunc (*pfunc) (headptr obj, int code, addr *res);

/* p-function */

#define IFUNC(H,M)      ((*PFUNC(H))(H,M,&wrkptr))
#define EVAL(H,M,T,P)   (wrkfun=IFUNC(H,M), (*((T)wrkfun)) P )
#define EVALS(H,M)      IFUNC(H,M)  /* EVAL immediate */
#define ICALL(H,M,T,P)  ((wrkfun=IFUNC(H,M))==inull? FAIL:(*((T)wrkfun)) P)
/* #define OBJADDR(H,M)   (IFUNC(H,M)==inull? NULL : wrkptr) */
#define PFUNC(H)        pfunc_table[MODE(TYPE(H))]
#define PNAME(H)        pname_table[MODE(TYPE(H))]
#define PWORD(H)        pword_table[MODE(TYPE(H))]
#define THIS     wrkptr

/* To get i-function of object H for message M use the form
       IFUNC(H,M)
   To apply i-function of type T named M of object H
   to parlist (P1,P2,...) use the form
       EVAL(H,M,T,(P1,P2,...))
   Note that the pointer to i-function evaluates before
   the parameter list (P1,P2,...).
   After IFUNC has executed, the global variable THIS helds a pointer
   which one of Pi can depend on.





   For an unknown message code, p-function returns inull.
   Different messages of all classes must have different codes.
*/
#define MAXMODE 64
CLASS_EXT  pfunc   pfunc_table[MAXMODE];
CLASS_EXT  char *  pname_table[MAXMODE];
CLASS_EXT  char    pchar_table[MAXMODE]; /* Unique first letter */
CLASS_EXT  cvalue  pword_table[MAXMODE];
CLASS_EXT  int     pfunc_free;
CLASS_EXT  addr    wrkptr; /* global variable for resulting pointer */
CLASS_EXT  ifunc   wrkfun; /* global variable for resulting i-function */

#define HDVAL(pfid) (pfid)<<2+2
#define HDOBJ(pfid) (pfid)<<2+3

extern int  iregtype(pfunc,char *,char);
extern LOGICAL vregtype(pfunc,char *,char, int);
LOGICAL class_init();

/* The module CLASS defines a unique object of class EMPTY */
extern void mk_empty(headptr);
#define ISNULL(H) (TYPE(H)==HEADEMPTY)

/* common i-functions: */
int inull();               /* i-function: no function       */
int itrue();               /* returns TRUE                  */
int ifalse();              /* returns FALSE                 */

LOGICAL icopy_alloc(addr thi, headptr from);

/* The common methods */
#define _OBJECT      0    /* message to test object */
#define _DONE        1    /* terminate object */
#define _NEW         2    /* initiate empty object */
/* Methods for movable objects */
#define _START       3    /* start operation */
#define _END         4    /* end operation */
#define _LOAD        5    /* load from channel */
#define _SAVE        6    /* save into channel */
#define _MOVE        7    /* move contents from another object */
#define _COPY        8   /* copy contents from another object */
#define _SEND        9    /* send a message in the refal format */

typedef int (*Tmove) (addr thi, headptr from);
typedef int (*Tsend) (addr thi, expr eM);

/* Macros for common methods */
#define DONE(H)       EVALS(H,_DONE)
#define START(H)      EVALS(H,_START)
#define END(H)        EVALS(H,_END)
#define NEW(H)        EVALS(H,_NEW)
#define CLASS_ID(H)   MODE(TYPE(H))
#define CLASS_NAME(H) pname_table[CLASS_ID(H)]
#define LOAD(H,f,gtc) EVAL(H,_LOAD,Tload,(THIS,f,gtc))
#define SAVE(H,f,ptc) EVAL(H,_SAVE,Tsave,(THIS,f,ptc))

/* Comon p-functions */
extern ifunc p_alloc(headptr h, int m, addr* aa);
extern ifunc p_object(headptr h, int m, addr* aa);
#endif