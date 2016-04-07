/************************************************************************/
/*                                                                      */
/*   R E F A L - 6      Copyright (C) IPS AN 1991                       */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/*       refelem.h    - element type declarations                       */
/*                                                                      */
/************************************************************************/

#ifndef __REFELEM_H
#define __REFELEM_H

/* #include <dos.h> */

#define AND          &&
#define OR           ||
#define NOT          !

enum LOGICAL { FALSE, TRUE, FAIL };
#define LOGICAL int

#define ALLONES      037777777777

#ifndef FAST
#define MCALL
#endif

#ifdef HUGE32
#define LARGE
#endif

#ifdef LARGE
#define INFO_M "LARGE"
#else
#ifdef SMALL
#define INFO_M "SMALL"
#else
#define SPLIT
#define SMALL
#define INFO_M "SPLIT"
#endif
#endif

#ifdef DEFINE_EXTCOM
#define EXTCOM
#else
#define EXTCOM  extern
#endif

#define     uchar         unsigned char








/************************************************************************/
/*                    Memory models                                     */
/*                                                                      */
/*   LARGE - 12-byte element, all pointers are large,                   */
/*            more then 32 K elements (app. 36 K)                       */
/*                                                                      */
/*   SMALL - 8-byte element, all pointers are small, 8 K elements       */
/*                                                                      */
/*   SPLIT - 4 arrays, each of 32 K words, 32 K elements,               */
/*           elemptr is integer, all other pointers are large           */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/*   Element is an abstract data type                                   */
/*   elemptr is the type of refference to element                       */
/*                                                                      */
/*   Public attributes of element refferenced by p are                  */
/*      TYPE(p) or STYPE(p) - type of element                           */
/*         TYPE may be used only when element is of simple type         */
/*      VAL(p) - value of simple type                                   */
/*      REF(p) or REF00(p) - value of refference type                   */
/*         REF may be used only for brackets                            */
/*      NEXT - next element                                             */
/*      PREV - previos element                                          */
/*                                                                      */
/*   TYPE, VAL, NEXT and PREV are directly assignable                   */
/*   but STYPE and REF[00] are not                                      */
/*                                                                      */
/*   There is an open type for value of element:                        */
/*       typedef struct { int flag; int val } pvalue;                   */
/*                                                                      */
/*   To compare or to assign values of elements use macros:             */
/*   (p,q are refferences to elements, c is a 'constant' value          */
/*                                                                      */
/*      EQINF(p,q)       " *p == *q "                                   */
/*---   EQINFC(p,c)      " *p == c "                                    */
/*      MKINF(p,q)       " *p = *q "                                    */
/*---   MKINFC(p,c)      " *p = c "                                     */
/*---   MKCINF(c,p)      " c = *p "                                     */
/*      SETREF(elemptr p, elemptr v, int f)   " *p = (v,f) "            */
/*      SETBRAC(p,v)      the same when f = TYPEBRAC                    */
/*---   PREF00(c)         ref-value of the constant                     */
/*   Make new copy of *p at front end of free element list              */
/*      copp(p)                                                         */
/*                                                                      */
/*  There is an type of a value as a c-value:                           */
/*     typedef void * cvalue;                                           */
/*  To operate these values use macros                                  */
/*       c=DATA(p);                                                     */
/*       SETDATA(p,c);                                                  */
/*       int CTYPE(c), int CVAL(c),                                     */
/*       elemptr CREF(c), elemptr CREF00(c),                            */
/*       cvalue CMKSIM(v,f), cvalue CMKREF(v,f)                         */
/*       EQDATC(p,c)                                                    */
/************************************************************************/





/************************************************************************/
/*                        Element access                                */
/************************************************************************/
#define  REFSEL      1 /* TYPEREF & TYPELVAL */
#define  TYPESEL     3
#define  CONOBJSEL   2
#define  REALSEL     7
#define  WORDSEL    15
#define  MODESEL    63
#define  MODIFY    256
#define  TYPEBRAC    0
#define  TYPEREF     1
#define  TYPESIMPL   2
#define  TYPELVAL    3
#define  TYPEREAL    (TYPESIMPL+4)
#define  TYPESWORD(L) (TYPESIMPL+8+(L<<4))
#define  TYPESNUMB   (TYPESIMPL+16)
#define  TYPECHAR    (TYPESIMPL+64)
#define  TYPEVIR     (TYPESIMPL+128)
enum mode { EMPTY, CHAR, FSTORE, EXPR,
			CHAIN, VECTOR, FUNC, STRING, MASK, TABLE,
			REAL, FILEIO
			};
#define  HD(T,M)     ((T<<2)+M)
#define  MODE(T)     ((T>>2) & MODESEL)
#define  CON         2
#define  OBJ         3
#define  HEADSEL     3
#define  ACTSEL      1
#define  NOTHEADSEL  (ALLONES-HEADSEL)
#define  HEADACT     0
#define  HEADVACT    1
#define  HEADINITACT 3
#define  HEADVF      1
#define  HEADSIMPL   HD(EMPTY,CON) /* == TYPESIMPL !!! */
#define  HEADEMPTY   HD(EMPTY,OBJ)
#define  HEADSTOR    HD(FSTORE,OBJ) /* Unique object Afreestor */
#define  HEADBRAC    HD(CHAIN,CON)
#define  HEADBOX     HD(CHAIN,OBJ)
#define  HEADEXPR    HD(EXPR,OBJ)
#define  HEADVECT    HD(VECTOR,OBJ)
#define  HEADFUNC    HD(FUNC,OBJ)
#define  HEADWORD    HD(STRING,CON)
#define  HEADSTR     HD(STRING,OBJ)
#define  HEADNUMB    HD(MASK,CON)
#define  HEADMASK    HD(MASK,OBJ)
#define  HEADTABLE   HD(TABLE,OBJ)
#define  HEADREAL    HD(REAL,OBJ)












#define  STYPE(p)    S_TYPE(TYPE(p))
#define  ISSYMB(p)   IS_SYMB(TYPE(p))
#define  ISBRAC(p)   IS_BRAC(TYPE(p))
#define  ISREF(p)    IS_REF(TYPE(p))
#define  ISSIMPL(p)  IS_SIMPL(TYPE(p))
#define  ISCHAR(p)   IS_CHAR(TYPE(p))
#define  ISSNUMB(p)  IS_SNUMB(TYPE(p))
#define  ISSWORD(p)  IS_SWORD(TYPE(p))
#define  ISREAL(p)   IS_REAL(TYPE(p))
#define  ISLVAL(p)   IS_LVAL(TYPE(p))
#define  IS_SYMB(t)  S_TYPE(t)
#define  IS_BRAC(t)  (NOT IS_SYMB(t))
#define  IS_REF(t)   ((t) & REFSEL)     /* TYPEREF & TYPELVAL */
#define  IS_SIMPL(t) ((t & TYPESEL) == TYPESIMPL)
#define  IS_CHAR(t)  ((t) == TYPECHAR)
#define  IS_SNUMB(t) ((t) == TYPESNUMB)
#define  IS_SWORD(t) (((t) & WORDSEL) == TYPESWORD(0))
#define  SWD_LEN(t)  ((t>>4)&3)
#define  IS_REAL(t)  (((t) & REALSEL) == TYPEREAL)
#define  IS_SVAL(t)  ((t+1)&2)          /* TYPESIMPL & TYPEREF */
#define  IS_LVAL(t)  (NOT IS_SVAL(t))   /* TYPEBRAC & TYPELVAL */
#define  IS_TABLE(t) (t == HEADTABLE)
#define  IS_CON(t)   ((t & HEADSEL) == CON)
#define  IS_OBJ(t)   ((t & HEADSEL) == OBJ)

#define  ISR(p,t)    (IS_REF(TYPE(p)) AND (TYPE(REF00(p)) == t))
#define  ISBOX(p)    ISR(p,HEADBOX)
#define  ISWORD(p)   (IS_SWORD(TYPE(p)) OR ISR(p,HEADWORD))
#define  ISLNUMB(p)  ISR(p,HEADNUMB)
#define  ISNUMB(p)   (IS_SNUMB(TYPE(p)) OR ISLNUMB(p))

#define  ISTABLE(h)     IS_TABLE(TYPE(h))
#define  ISCON(h)       IS_CON(TYPE(h))
#define  ISOBJ(h)       IS_OBJ(TYPE(h))
#define  ISMODIFIED(h)  (TYPE(h) & MODIFY)
#define  SETMODIFIED(h) TYPE(h)|=MODIFY;
#define  MAKESIMPL(h)   SETSIM(h,1,TYPESIMPL)

#define  EQSYM(p,q)   (EQINF(p,q) )
/*	     OR (ISLVAL(p) AND ISLVAL(q) AND rf_eqlong(p,q))) */
#define  EQSYMC(p,c)   (EQDATC(p,c) )
/*	     OR (ISLVAL(p) AND ISLVAL(q) AND rf_eqlongc(p,c))) */
#define  EQTERM(p,q)  (EQINF(p,q) \
   	     OR (ISBRAC(p) AND ISBRAC(q) AND rf_eqobj(REF(p),REF(q))) )
#define  EQTERMC(p,c) (EQDATC(p,c) \
   	     OR (ISBRAC(p) AND IS_BRAC(CTYPE(c)) AND rf_eqobj(REF(p),CREF(c))) )
#define  GETHEAD(p,h,t) (ISREF(p) AND (TYPE(h=REF00(p))==t))
#define  GETEMPTY(p,h)  (ISREF(p) AND rf_getempty(h=REF00(p)))
#define  GETNUMB(p,n)   (rf_getnumb(p,&(n)))
#define  rf_getempty(h) rf_get_empty(h,HEADEMPTY)
#define  rf_mkempty(ah) rf_mk_empty(ah,HEADEMPTY)

/*#define ISLNUMB(p) ((STYPE(p) == TYPELVAL) & (TYPE(REF00(p))==HEADNUMB))*/
/* #define  ISNUMB(p)   (ISSNUMB(p) OR ISLNUMB(p)) */
/* #define  GETOBJ(p,o)    (rf_getobj(p,&(o))) */
/* #define  ADJACENT(p,q)  NEXT(p)==q */




/*        CLASSES OF CHARACTERS                                   */
#define  ISTERMS(c)  ((c) == ' ' OR (c) == '\n' OR (c) == '(' OR \
                      (c) == ')' OR (c) == '\t')
#define  ISWORDS(c)  ( isalpha(c) OR isdigit(c) OR \
                       (c) == '_' OR (c) == '-' OR \
                       (c) == '?' OR (c) == '!' OR (c) == '$')


/************************************************************************/
/*                        Element of list                               */
/************************************************************************/
typedef   void * cvalue;
#ifdef BIGENDIAN
typedef   struct { short val; short flag; } pvalue;
#else
typedef   struct { short flag; short val; } pvalue;
#endif

#ifdef  SPLIT

typedef  unsigned  elemptr;

EXTCOM   unsigned * BM[4];
#define  FIELD(k,p,typ) (*(typ*)(((char*)(BM[k]))+(p)))
#define  TYPE(p)     FIELD(0,p,unsigned)
#define  VAL(p)      FIELD(1,p,int)
#define  NEXT(p)     FIELD(2,p,elemptr)
#define  PREV(p)     FIELD(3,p,elemptr)
#define  REF(p)      FIELD(1,p,elemptr)

#define  DATA(p)     MK_FP(VAL(p),TYPE(p))
#define  SETDATA(p,c) { VAL(p) = CVAL(c); TYPE(p) = CTYPE(c); }
#define  EQDATC(p,c) (VAL(p) == CVAL(c) AND TYPE(p) == CTYPE(c))

#define  EQINF(p,q)  (VAL(p) == VAL(q) AND TYPE(p) == TYPE(q))
#define  MKINF(p,q)  {VAL(p) = VAL(q); TYPE(p) = TYPE(q);}

#define  NOELEM 0
#define  NOHEAD 0
#define  NOTABLE 0

#define  PTOFAR(p) MK_FP(0,p)
#define  PFROMFAR(a) FP_OFF(a)

#endif

#ifdef SMALL
#define  S_TYPE(t)      (t)
#define  SETSIM(p,v,f)  { VAL(p) = v; TYPE(p) = f; }
#define  SETREF(p,v,f)  { REF(p) = v; TYPE(p) = f; }
#define  SETREF00(p,v)  { REF(p) = v; }
#define  REF00(p)       REF(p)
#define  SETBRAC(p,v)   { REF(p) = v; TYPE(p) = TYPEBRAC;}
#define  SETACT(r,v)    { REF(r) = v; TYPE(r) = HEADACT;}
#define  ISACT(r)       (TYPE(r) == HEADACT)
/* #define  PSETREF(c,v,f) { PVAL(c) = v; PTYPE(c) = f; } */
#define  BADPTR(p)      FALSE
#endif






#ifndef SPLIT
#define  INFOX(C)  INFO((elemptr)&(C))

typedef struct elemstr {
   union {
      cvalue cval;
      pvalue data;
      struct {
#ifdef BIGENDIAN
           struct elemstr *ref;
#endif
#ifdef SMALL
           int flag;
#endif
#ifndef BIGENDIAN
           struct elemstr *ref;
#endif
           } r;
      } info;
   struct elemstr *prec;
   struct elemstr *foll;
   } elem;
typedef elem * elemptr;

#define  NEXT(p)     ((p) -> foll)
#define  PREV(p)     ((p) -> prec)
#define  REF(p)      ((p) -> info.r.ref)
#define  INFO(p)     ((p) -> info.cval)
#define  TYPE(p)     ((p) -> info.data.flag)
#define  VAL(p)      ((p) -> info.data.val)
#define  DATA(p)     (INFO(p))
#define  SETDATA(p,c) DATA(p)=c;
#define  EQDATC(p,c) (DATA(p)==c)

#define  EQINF(p,q)  (INFO(p) == INFO(q))
#define  MKINF(p,q)  INFO(p) = INFO(q);
#define  NOELEM  NULL
#define  NOHEAD  NULL
#define  NOTABLE NULL

#define  PTOFAR(p)   (void*)p
#define  PFROMFAR(a) (headptr)a
#endif

#ifdef LARGE
#define  S_TYPE(t)      (t & TYPESEL)
#define  SETSIM(p,v,f)  { VAL(p) = v; TYPE(p) = f; }
#define  SETREF(p,v,f)  { REF(p) = (elemptr) (((uchar*)v) + (f)); }
#define  SETREF00(p,v)  SETREF(p,v,STYPE(p))
#define  SETBRAC(p,v)   {REF(p) = v;}
#define  SETACT(r,v)    {REF(r) = v;}
#define  ISACT(r)       ((TYPE(r) & HEADSEL) == HEADACT)
/* #define  PSETREF(c,v,f) {INFOX(c) = ((long)(v)) + (f); } */
#define  REF00(p)       ((elemptr) (((long) REF(p)) & NOTHEADSEL ) )
#define  BADPTR(p)      ((long)p & TYPESEL) != 0
#endif





typedef elemptr headptr;

/************************************************************************/
/*   Definition of pvalue - the value of refal element                  */
/************************************************************************/

/* #define PTYPE(c)     ((c).flag) */
/* #define PVAL(c)      ((c).val) */
/* #define PISREF(c)    PTYPE(c) & TYPEREF */
/* #define PISWORD(c)   (PISREF(c) AND (TYPE(PREF00(c))==HEADBOX)) */
/* #define PISSNUMB(c)  (PTYPE(c) == TYPESNUMB) */

/************************************************************************/
/*  Macros for cvalue, using FP_OFF, FP_SEG, MK_FP                      */
/************************************************************************/

#define CTYPE(c)     (FP_OFF(c))
#define CVAL(c)      (FP_SEG(c))
#define CISWORD(c)   ((IS_REF(CTYPE(c)) AND (TYPE(CREF00(c))==HEADWORD))
#define CMKSIM(v,f)  (MK_FP(v,f))
#ifdef BIGENDIAN
#define SWDBODY(c)   (((uchar*)&(c))+0)  /* c must be L-value */
#else
#define SWDBODY(c)   (((uchar*)&(c))+1)  /* c must be L-value */
#endif
#define NOCVALUE     CMKSIM(0,TYPEVIR)
#define ADATA(p)     adata(p) /* make actual cvalue from virtual elemptr */

#ifdef SMALL
#define CMKREF(v,f)  CMKSIM(v,f)
#define CREF(c)      CVAL(c)
#define CREF00(c)    CREF(c)
#else
#define CMKREF(v,f)  ((void*) (((uchar*)(v))+(f)))
#define CREF(c)      ((elemptr)(c))
#define CREF00(c)    ((elemptr) (((long) (c)) & NOTHEADSEL ) )
#endif

/************************************************************************/
/*   Type definition for EV-, ST-, and C-variables:                     */
/************************************************************************/

typedef struct { elemptr  L,  R; } expr;
typedef struct { elemptr *L, *R; } aexpr;
typedef union  { elemptr  L,  R; } term;
typedef union  { elemptr *L, *R; } aterm;
typedef cvalue symb;
typedef symb* asymb;

/************************************************************************/
/*   Definition for REAL MODE                                           */
/************************************************************************/
#ifdef SMALL
typedef float real;
#else
typedef double real;
#endif
#endif
