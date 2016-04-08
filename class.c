/******************************************************************
   R E F A L - 6      Copyright (C) IPS AN 1991
   class.c - function definitions for building classes and objects
*******************************************************************/

#define CLASS_EXT
#include "rfalloc.h"
#include "rfio.h"
#include <stdio.h>
#include <stdlib.h>

int  iregtype(pfunc f,char * n, char ch)
{  if(++pfunc_free >= MAXMODE) {
      fprintf(stdtrc,"No room to register type %s", n);
      rf_exit(20);
      };
   if(NOT vregtype(f,n,ch,pfunc_free)) return(0);
   return(pfunc_free);
   };

LOGICAL vregtype(pfunc f,char * n, char ch, int i)
{ if(pname_table[i] != NULL)
     fprintf(stdtrc,"Register type %s at code %d instead of type %s",
           n,i, pname_table[i]);
  pname_table[i] = n;
  pchar_table[i] = ch;
  pfunc_table[i] = f;
  return (rf_mk_sword(n, &(pword_table[i])));
  };

/* extern ifunc p_null   (headptr h, int m, addr* aa); */
/* extern ifunc p_char   (headptr h, int m, addr* aa); */
/* extern ifunc p_object (headptr h, int m, addr* aa); */
extern ifunc p_chain  (headptr h, int m, addr* aa);
extern ifunc p_store  (headptr h, int m, addr* aa);
extern ifunc p_expr   (headptr h, int m, addr* aa);
extern ifunc p_vector (headptr h, int m, addr* aa);
extern ifunc p_contain(headptr h, int m, addr* aa);
extern ifunc p_string (headptr h, int m, addr* aa);
extern ifunc p_mask   (headptr h, int m, addr* aa);
extern ifunc p_real   (headptr h, int m, addr* aa);
extern ifunc p_fileio (headptr h, int m, addr* aa);

addr getwrkptr(ifunc f)
{ return(wrkptr); };

int inull()
{ fprintf(stdtrc,"Illegal object access function\n");
  return(FALSE);
  };

int itrue() { return(TRUE); };

int ifalse() { return(FALSE); };

static LOGICAL imove(addr thi, headptr from)
{  headptr h;
   h = PFROMFAR(thi);
   NEXT(h) = NEXT(from);
   PREV(h) = PREV(from);
   return(TRUE);
   }

ifunc p_char(headptr h,int m, addr* aa)  { return (inull); }

/* p-function of pra-object */
ifunc p_object(headptr h, int m, addr* aa)
{
  *aa = PTOFAR(h);
  switch(m) {
    case _DONE: if (TYPE(h)!=HEADSIMPL) break;
            fprintf(stdtrc,"ERROR: REF-TO-SIMPL %p ",h);
            outref(stdout,fputc,h,TRUE,sysid);
            rf_exit(30);
    case _NEW:  break;
    case _COPY:
    case _MOVE: return((ifunc) imove);
    case _OBJECT: return(itrue);
    default:    return(inull);
    }
    return(itrue);
};

LOGICAL icopy_alloc(addr thi, headptr from)
{  headptr h = PFROMFAR(thi);
   seglen lf = SEGLEN(AD(from));
   HDSETA(h,ANULL);
   if(REALLOC(AD(h),lf)==NOADR) return(FALSE);
   memmove(SEGINF(AD(h)),SEGINF(AD(from)),lf);
   return(TRUE);
   }

/* p-function abstract class of objects which use rf_alloc */
ifunc p_alloc(headptr h, int m, addr* aa)
{
  switch(m) {
    case _START: LOCK(h); break;
    case _END:   UNLOCK(h); break;
    case _DONE:  LOCK(h); rf_realloc(AD(h),0); break;
    case _NEW:   AD(h)=anull; break;
    case _COPY:  return(icopy_alloc);
    default:   return p_object(h,m,aa);
    } return(itrue);
};

ifunc p_null(headptr h, int m, addr* aa)
{ fprintf(stdtrc,"Illegal object of type %d\n", TYPE(h));
  return(inull);
  };

LOGICAL class_init()
{ int i;
 pfunc_free = MAXMODE/2;
  for (i=0;i<MAXMODE;i++) {
    pfunc_table[i]=p_null;
    pname_table[i]=NULL;
    };
  /* mode STRING must be first */
 if(NOT vregtype(p_string, "STRING",'S', STRING)) return(FALSE);
 if(NOT vregtype(p_char,   "CHAR",   0 , CHAR  )) return(FALSE);
 if(NOT vregtype(p_object, "EMPTY", 'E', EMPTY )) return(FALSE);
 if(NOT vregtype(p_store,  "FSTORE", 0 , FSTORE)) return(FALSE);
 if(NOT vregtype(p_chain,  "CHAIN", 'C', CHAIN )) return(FALSE);
 if(NOT vregtype(p_chain,  "TABLE", 'T', TABLE )) return(FALSE);
 if(NOT vregtype(p_expr,   "EXPR",   0 , EXPR  )) return(FALSE);
 if(NOT vregtype(p_vector, "VECTOR",'V', VECTOR)) return(FALSE);
 if(NOT vregtype(p_mask,   "MASK",  'M', MASK  )) return(FALSE);
 if(NOT vregtype(p_real,   "REAL",  'R', REAL  )) return(FALSE);
 if(NOT vregtype(p_fileio, "FILEIO", 0 , FILEIO)) return(FALSE);
 return(TRUE);
 };
