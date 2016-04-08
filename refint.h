/******************************************************************
   R E F A L - 6      Copyright (C) 1990 IPS AN 1992 Arkady Klimov
       refint.h    -  declarations for RASL-interpreter
*******************************************************************/
#ifndef __REFINT_H
#define __REFINT_H

#include <time.h>
#include <string.h>
#include <signal.h>
/* #include <mem.h> */

#define DEBUG
#include "refgen.h"
#include "rfalloc.h"

#ifndef EXTINT
#define EXTINT extern
#endif

/************************************************************************/
/*                        Macros                                        */
/************************************************************************/
#define  movb1       { b1 = NEXT(b1); if(b1==b2) fail; }
#define  movb2       { b2 = PREV(b2); if(b1==b2) fail; }
#define  tbel(x)     (*(tel + (x)))
#define  mult(n)     { movb; MKINF(b,tbel(n));\
                       if(NOT ISSIMPL(b)) ++VAL(REF00(b));}
#define  tpl(m,n)    { if(PREV(tbel(m))!=tbel(n))\
                           pushtpl(tbel(m),tbel(n));}
#define  pushtpl(vl,vr) \
                     { stt[pst].l = vl;\
                       stt[pst].r = vr;\
                       stt[pst++].b = b;}
#define  poptpl(vl,vr,vb) \
                     { vl = stt[--pst].l;\
                       vr = stt[pst].r;\
                       vb = stt[pst].b;}
#define  extpl       { movb;\
                       while(pst) {\
                           elemptr xl,xr,xb;\
                           poptpl(xl,xr,xb);\
                           AFTERX(xb,xl,xr);}\
                       b = PREV(b);}
#define AFTERXS(x,l,r) FREEXS(l,r)

/************************************************************************/
/*                        Element of transplantation stack              */
/************************************************************************/
typedef struct tplels {
   elemptr l;
   elemptr r;
   elemptr b;
   } tplel;
typedef tplel * tplelptr;





/************************************************************************/
/*            Type for the reaction-on-fail field (in nel)              */
/*                 of the item of transition stack                      */
/************************************************************************/

typedef enum {
    _nofail, /* neutral code */
    _vfld,   /* auxiliary view field pointer */
    _error,  /* strong call failed */
    _fail,   /* soft call failed*/
    _failinit, /* initial active term failed */
    _alt,    /* block level ('{') */
    _stake,  /* a stake, level mark ('\') */
    _cut     /* remove stack up to last stake */
    } sysnels; /* Nel codes in case of spc=0 */

/*********************************************************************/
/*                     RASL operators codes                          */
/*********************************************************************/

typedef enum { NOP,
    PS,      PSR,        /* AB */
    CSYM,     CSYMR,      /* CD */
    SYMS,    SYMSR,      /* EF */
    SYM,    SYMR,        /* GH */
    OSYM,    OSYMR,        /* IJ */
    TERM,    TERMR,      /* KL */
    OTERM,    OTERMR,        /* MN */
    EXP,      EMP,        /* OP */
    OEXP,    OEXPR,      /* QR */
    SETB,    CHKB,        /* ST */
    PLEN,    PLENS,      /* UV */
    LEN,    LENP,         /* WX */
    LENS,    LENOS,      /* YZ */
    NOP6,    NOP5,    NOP4,    NOP3,    NOP2,    NOP1,

    BL,      BR,      BLR,    /* abc */
    MULT,    MULE,    MUL,    /* def */
    TPLE,     TPL,        /* gh */
    NS,      TEXT,        /* ij */
    BLNS,    BRACT,        /* kl */
    BUILTIN,RETURN,        /* mn */
    EVAL,    EST,        /* op */
    STAKE,    CUT,           /* qr */
    DO,     UNDO,       /* st */
    BRANCH, BRANCHR,    /* uv */
    ALT,    END,           /* wx */
    FAILS,  BLOCK        /* yz */
    } opcode;











/************************************************************************/
/*                        Interpreter data                              */
/************************************************************************/

EXTINT   elemptr  *te;              /* Table of elements                */
EXTINT   tplel    stt[128];         /* Stack of transplantations        */
EXTINT   long     step;             /* Number of steps                  */
EXTINT   long     trstep;           /* No. of step when start trace     */
EXTINT   elemptr  *tel;             /* Pointer to te                    */
EXTINT   elemptr  *telmax;          /* Upper bound for tel              */
EXTINT   LOGICAL  dbgfl;            /* Debug flag                       */
EXTINT   LOGICAL  tracefl;          /* RASL Interpreter internal trace  */
EXTINT   LOGICAL  testfl;           /* Debug OR Trace OR Account is set */
EXTINT   long *   account;          /* RASL istr. counter               */
EXTINT   unsigned modacts;          /* if (flag -m) 0 else 1            */
EXTINT   headptr  applyobject;      /* Function "Apply Object"          */
EXTINT   int      level;            /* Level of recursion of rf_eval    */


/************************************************************************/
/*               Common functions                                       */
/************************************************************************/
/* Print level-prefixed message */
void printlv(char* s);

/* Put out expression to stderr */
#define rf_oute(L,R)  rf_out(stdtrc,L,R)

/* interpreter (in rfinter.c */
LOGICAL rf_inter(elemptr newact, long * steps);

/* Functions from rfgo.c */
void rf_dbg(elemptr b1, elemptr b2, int ins);
void rf_trace1(elemptr T);
void rf_trace2(elemptr Lx, elemptr R);

























/************************************************************************/
/*              RASL function mode/code                                 */
/************************************************************************/
/* #define CHAIN_FUN         ###   CHAIN/VECTOR function mode */
/* #define NUMB_CODE         ###   NUNB/WORD code */

#ifdef  CHAIN_FUN

#define INFO_E    "CHAIN"
extern  ifunc     p_chain  (headptr h, int m, addr* aa);
#define P_FUNC    p_chain
#define pctype    elemptr
#define NOPC      NOELEM
#define GETCOD    VAL(pc)            /* for NUMB_CODE  */
#define GETINS    DATA(pc)           /* for WORD_CODE  */
#define NEXTOP    pc = NEXT(pc);
#define EQSA(x)   EQSYM(x,pc)
#define MKTA      MKINF(b,pc)
#define BRANCH1   NEXT(REF(pc))
#define BRANCH2   NEXT(pc)
#define savecfh   pusht(cfh)
#define unsavecfh popt(cfh)
#define startf    ++VAL(cfh);
#define endf      --VAL(cfh);
#define expc      pc=NEXT(cfh);
#define chkpc     ( pc!=cfh /* AND ISSIMPL(pc) */)
#define TEST_BRANCH (IS_BRAC(TYPE(pc)))

#else  /* VECTOR_FUN */

#define INFO_E    "VECTOR"
extern  ifunc     p_vector (headptr h, int m, addr* aa);
#define P_FUNC    p_vector
#define pctype    seglen
#define NOPC      0
#define PC        (*(cvalue*) MEMOFF(fbas,pc))
#define GETCOD    CVAL(PC)                 /*  for NUMB_CODE  */
#define GETINS    PC                       /*  for WORD_CODE  */
#define NEXTOP    pc=pc+sizeof(cvalue);
#define EQSA(x)   EQSYMC(x,PC)
#define MKTA      SETDATA(b,PC)
#define BRANCH1   (pc+sizeof(cvalue))
#define BRANCH2   (pc+sizeof(cvalue)*CVAL(PC))
#define savecfh   { END(cfh); pusht(cfh) }
#define unsavecfh { popt(cfh); START(cfh); HDGETA(cfh,fbas);}
#define SAFECNT   256
#define startf    { VAL(cfh)+=SAFECNT; START(cfh); }
#define endf      { END(cfh); VAL(cfh)-=SAFECNT; }
#define expc      { HDGETA(cfh,fbas); pc = HEADSIZE; }
#define chkpc     ( TRUE /* SEGLEN(AD(cfh)) > pc-HEADSIZE AND IS_SIMPL(CTYPE(PC)) */ )
#define TEST_BRANCH (IS_SYMB(CTYPE(PC)))
#endif







/* Make command insertion endianity-independent */
/* #define PUTINS(v,f) if(NOT repn(thi,&t,CMKSIM(v,f))) return(FALSE); */
#define PUTINS(cv) if(NOT repn(thi,&t,cv)) return(FALSE);
                             /* (For use in INSi in RFGO) */
#ifdef NUMB_CODE
#define INFO_C    "NUMBER"
#define EXINS     { ins = GETCOD ; NEXTOP }
#define ARG1(n)   { n   = GETCOD; NEXTOP }
#define ARG2(n)   { n   = GETCOD; NEXTOP }
#define INS1(c,n)  { INS0(c); INS0(n); }
#define INS0(c)    PUTINS(CMKSIM(c,TYPESNUMB));

#else    /* WORD_CODE  */
#define INFO_C    "WORD"
EXTINT  cvalue    cins;  /* current instruction */
#define ARG(i)    (SWDBODY(cins)[i]-COFF)
#define EXINS     { cins = GETINS; ins=ARG(0); NEXTOP }
#define ARG1(n)   n = ARG(1);
#define ARG2(n)   n = ARG(2);
/* Make command insertion endianity-independent */
/* #define INS1(c,n)  PUTINS(UPC(n),(UPC(c)<<8)+TYPESWORD(2));
   #define INS0(c)    PUTINS(   0  ,(UPC(c)<<8)+TYPESWORD(1)); */
#define INS1(c,n)  { cvalue cc; uchar ins[2];\
                     ins[0] = UPC(c); ins[1] = UPC(n);\
                     rf_mk_word(ins,2,&cc,TRUE);\
                     PUTINS(cc); }
#define INS0(c)    { cvalue cc; uchar ins[1];\
                     ins[0] = UPC(c);\
                     rf_mk_word(ins,1,&cc,TRUE);\
                     PUTINS(cc); }
#define UPC(n)     (n+COFF)
#define COFF       64
#endif

/************************************************************************/
/*                        Transition stack                              */
/************************************************************************/

typedef struct stels {
   union {
      struct { elemptr svb1; elemptr svb2;} sb;
      struct stels * sstp;
      } u;
   int snel;
   pctype spc;
   } stel;
typedef stel * stelptr;

#define sb1 u.sb.svb1
#define sb2 u.sb.svb2
#define ssp u.sstp

#define sp ((stelptr)spe)

#define  pushst(vb1,vb2,vnel,vpc) \
                     { sp->sb1 = vb1;\
                       sp->sb2 = vb2;\
                       sp->snel = vnel;\
                       sp->spc =  vpc;}
#define  popst(vb1,vb2,vnel,vpc) \
                     { vb1 = sp->sb1;\
                       vb2 = sp->sb2;\
                       vnel = sp->snel;\
                       vpc = sp->spc;}
#endif
