/************************************************************************/
/*   R E F A L - 6      Copyright (C) 1992 by Arkady Klimov             */
/*       refcom.h    - common Refal-6 declarations                      */
/************************************************************************/

#ifndef __REFCOM_H
#define __REFCOM_H

#include "config.h"
#include "refelem.h"
#include <stdio.h>
#include <ctype.h>
/* #include <signal.h> */

#ifdef AT286
#include <dos.h>
#else
#define FP_OFF(fp)	((unsigned short) (unsigned) (fp))
#define FP_SEG(fp)	((unsigned short)((unsigned long)(fp) >> 16))
#define MK_FP(seg,ofs)	((void*) \
	   (((unsigned long)(seg) << 16) | (unsigned short)(ofs)))
#define far
#endif

#ifdef DEFINE_EXTCOM
#define EXTCOM
#else
#define EXTCOM  extern
#endif

/************************************************************************/
/*                                                                      */
/*                        Constants                                     */
/*                                                                      */
/************************************************************************/

#define  MAXFLAG     64    /* MAX total flags length                    */
#define  TESIZE      2048  /* Size of element table (by default)        */
/*          x sizeof(elemptr)  Use -Tnnn switch to redefine TESIZE      */
#define  STSIZE      2048  /* Size of the call stack (by default)       */
/*          x sizeof(elemptr)  Use -Snnn switch to redefine STSIZE      */
#define  MAXFILES    50    /* MAX number of files                       */
#define  MAXSTATFILES 20   /* MAX number of static channels             */
#define  MAXWORD     127   /* MAX length of word                        */
#define  BITSSHORT   16    /* Length of short number in bits            */
#define  HASHWD      256   /* Size of word hash table                   */
#define  MAXFN       60    /* MAX length of file name                   */

#ifdef SPLIT
#define  STORBLK	 32766 /* Size of storage block in elements        */
#else
#define  STORBLK	 1024  /* Size of storage block in elements         */
#endif

#include "rfstor.h"
#include "class.h"















/************************************************************************/
/*                        Word structure                                */
/************************************************************************/

typedef uchar *   WDPOS;
#define WDLEN(h)    SEGLEN(AD(h))           /* int */
#define WDREALLOC(h,l) REALLOC(AD(h),l)
#define WDFIRST(h)  ((WDPOS) SEGINF(AD(h)))   /* WDPOS */
#define WDNEXT(c)   (c+1)          /* WDPOS -> WDPOS */
#define WDCHAR(c)   *(c)           /* WDPOS -> uchar (L-expression) */

/************************************************************************/
/*                  Number (Mask) structure                             */
/************************************************************************/
#define ushort myushort
typedef unsigned short int ushort;    /* тип макроцифры */
#define ulong  myulong
typedef unsigned long  int ulong;     /* тип двойной макроцифры */
typedef ushort*  MPOS;
#define MLEN(h)   ((SEGLEN(AD(h)))/sizeof(ushort))
#define MSIZE(l)  ((l)*sizeof(ushort))
#define MFIRST(h) ((MPOS) SEGINF(AD(h)))

/************************************************************************/
/*                        Pointer to call stack                         */
/************************************************************************/

typedef union {
/*   stel * stelp;  */
   elemptr * elp;
   elemptr ** aelp;
   cvalue * cvp;
   long * nump;
   int (** funp)();
   } stptr;
EXTCOM   elemptr*  stk;              /* Base of stack                   */
EXTCOM   stptr     stp;              /* Pointer to stack                */
EXTCOM   elemptr*  spemax;           /* Upper bound for stack           */
/* #define  st     stk.elp */
#define  spe       stp.elp
#define  spcv      stp.cvp           /* cvalue* */
/* #define  spmax  stpmax.stelp */

/************************************************************************/
/*                        Access to call stack                          */
/************************************************************************/

#define  pusht(T)      *(spe++) = T;
#define  popt(T)       T = *(--spe);
#define  pushe(L,R)  { pusht(L) pusht(R) }
#define  pope(L,R)   { popt(R)  popt(L)  }
#define  pushat(T)      *(stp.aelp++) = &T;
#define  popat(AT)       AT = *(--stp.aelp);
#define  pushae(L,R) { pushat(L) pushat(R) }
#define  popae(AL,AR)  { popat(AR)  popat(AL)  }
#define  pushc(F)       *(stp.funp++) = F;
#define  stcall         (**(--stp.funp))()




/************************************************************************/
/*                        Error codes                                   */
/************************************************************************/
typedef enum {
		 ERRNO,             /* No error                                  */
		 ERRIMP,            /* Recognition impossible                    */
		 ERRSTOR,           /* No storage                                */
		 ERRFUN,            /* Invalid function identificator            */
		 ERRBRK,            /* Cntrl-Break pressed                       */
		 ERRSTEP,           /* Break on step                             */
		 ERREND,            /* Normal end (No active terms)              */
		 ERRFAIL,           /* Normal fail                               */
		 ERREXIT,           /* End of computation (by <EXIT...>)         */
		 ERRSYNT,           /* Syntax error in input                     */
		 ERRSYSB,           /* Invalid bracket in output                 */
		 ERRSYSO,           /* Invalid element in output                 */
		 ERRINIT,           /* Error in initialisation                   */
		 ERRDATA,           /* Invalid data structures                   */
		 ERRLINK,           /* Invalid link                              */
		 ERRELTO,           /* Element table overflow                    */
		 ERRSTKO,           /* Transition stack overflow                 */
		 ERRWORD,           /* No storage for words                      */
		 ERRAFUN            /* Illegal access function                   */
  } ercodes;

/************************************************************************/
/*                        Error exit codes                              */
/************************************************************************/
typedef enum {
		 ERCNO,            /* No error                                  */
		 ERCIMP,           /* Recognition impossible                    */
		 ERCSTOR,          /* No storage                                */
		 ERCFUN,           /* Invalid function identificator            */
		 ERCBRK,           /* Break!!!                                  */
		 ERCSTEP,          /* Break on step                             */
		 ERCEND,           /* Normal end (No active terms)              */
		 ERCFAIL,          /* Normal fail                               */
		 ERCEXIT,          /* End of computation (by <EXIT...>)         */
		 ERCSYNT,          /* Syntax error in input                     */
		 ERCSYSB,          /* Invalid bracket in output                 */
		 ERCSYSO,          /* Invalid element in output                 */
		 ERCINIT,          /* Error in initialisation                   */
		 ERCDATA,          /* Invalid data structures                   */
		 ERCLINK,          /* Invalid link                              */
		 ERCELTO,          /* Element table overflow                    */
		 ERCSTKO,          /* Call stack overflow                       */
		 ERCWORD,          /* No storage for words                      */
		 ERCAFUN           /* Illegal access function                   */
   } erexcodes;











/************************************************************************/
/*                        Macros                                        */
/************************************************************************/

#define	 movb    { b=NEXT(b);\
				 if(NOT ISSIMPL(b) AND NOT rf_movb()) return(FALSE);}
#define  movbx(x,y) { y=NEXT(x);\
                 if(NOT ISSIMPL(y) AND NOT rf_movbx(&y)) return(FALSE);}
#ifdef MCALL
#define bl  if(NOT CBL()) return(FALSE);
#define br  if(NOT CBR()) return(FALSE);
#define blr if(NOT CBLR()) return(FALSE);
#define  weld(a1,a2) {WELD(a1,a2);}
#define  after(X,L,R) {AFTER(X,L,R);}
#else
#define  bl          { movb; REF(b) = lbrac; lbrac = b; }
#define  br          { elemptr xxx,yyy; yyy=b; movb;\
                       xxx = PREV(lbrac); weld(yyy,lbrac);\
                       weld(xxx,b); xxx = lbrac;\
                       lbrac = REF(xxx); SETBRAC(b, xxx);\
                       TYPE(xxx) = HEADBRAC; VAL(xxx) = 1;}
#define  blr         { movb; SETBRAC(b,blbr); VAL(blbr)++; }
/*         { elemptr xxx,yyy;\
					   movb; xxx = b; movb; yyy=NEXT(b);\
					   weld(xxx,yyy); weld(b,b);\
					   TYPE(b) = HEADBRAC; VAL(b) = 1;\
					   SETBRAC(xxx,b); b = xxx;} */
#define  weld(a1,a2) { NEXT(a1) = a2; PREV(a2) = a1;}
#define  after(X,L,R) {weld(R,NEXT(X)); weld(X,L);}
#endif

#define rf_outs(L,R)  rf_out(stdout,L,R)
#define rf_out(F,L,R)   rf_outm(F, fputc, L,R,TRUE,SYSID)
/* #define rf_inps(F,L,R)  rf_insm(F, fgetc, L,R) */
/* #define rf_inp(F,L,R)   rf_inpm(F, fgetc, L,R,SYSID) */
#define SYSID sysid
























/************************************************************************/
/*                        Common data                                   */
/************************************************************************/

EXTCOM   headptr  chan[MAXFILES];   /* Array of channels                */
EXTCOM   elemptr  tracefa;          /* Trace function address           */
EXTCOM   elemptr  EresL;            /* Left end of result for C func.   */
EXTCOM   elemptr  EresR;            /* Right end of result --""--       */

/*        GLOBAL VARIABLES FOR GENERATED C-FUNCTIONS                    */
EXTCOM expr EW;
EXTCOM elemptr oldb,nextb;

#ifdef EXIST_CONIO_H

/* #include <stdlib.h> */
#include <conio.h>
#define  BRKTEST       kbhit()
#define  GETCH         getch()
#define  ECHO(c)       putc(c,stdtrc);
#define  PUTCH(c)      putch(c);
#define  SKIPLINE      fprintf(stdtrc,"\n");

#else

#define  BRKTEST       0
#define  GETCH         getchar()
#define  ECHO(c)       if (stdtrc!=stderr) putc(c,stdtrc);
#define  PUTCH(c)      putchar(c);
#define  SKIPLINE      while (getchar()!='\n');

#endif

#ifdef TURBO_C
#define  CLRSCR        clrscr()
#else
#include <stdlib.h>
#define  CLRSCR        system("clear")
#endif

#define  BRKINIT       signal(SIGINT,handint);

#define ALL(v,n) for(v=0;v<n;v++)

#define RCALL(res,fun) { if((res=fun)!=TRUE) return res; }
#define ECALL(fun)     { if(NOT (fun)) return FALSE;     }

#endif
