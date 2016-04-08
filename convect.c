/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1991 by Arcady Klimov            */
/*       convect.c    - container functions for vectors                 */
/************************************************************************/
#include "refcom.h"
#include "rfalloc.h"
#define NODEFTAIL
typedef long tail;
#include "contain.h"

#define VLEN(h) ((SEGLEN(AD(h)))/sizeof(cvalue))
#define VSIZE(vl) ((vl)*sizeof(cvalue))
#define VECFIRST(h) ((cvalue*) SEGINF(AD(h)))

static int isetpos(addr thi, tail * at, int n)
{
 iseglen ln = VLEN(PFROMFAR(thi));
      if (n>ln) { *at = ln; return(n-ln); }
 else if (n>=0) { *at =  n; return(0);    }
 else {
      n = ln+n+1;
      if (n<0)  { *at =  0; return(n);    }
      else      { *at =  n; return(0);    }
 }    }
/*
static LOGICAL isetpos(addr thi, tail * at, int n)
{
 seglen t,ln;
 ln = VLEN(PFROMFAR(thi));
 t = (seglen) (n>=0? n: ln+n+1);
 if (t>ln) { *at=ln; return(FALSE); }
 else
 if (t<0L) { *at=0; return(FALSE); }
 *at = (tail) t;
 return(TRUE);
 }
*/
static LOGICAL igetn (addr thi, tail * at, cvalue * ac)
{
 headptr h;
 seglen t,ln;
 ln = VLEN(h=PFROMFAR(thi));
 if((t=(seglen)(*at))>=ln)
     { *ac=CMKSIM(CHARVIR,TYPECHAR); return(FALSE); }
 (*at)++;
 *ac=VECFIRST(h)[t];
 return(TRUE);
 }

static LOGICAL irepn   (addr thi, tail * at, cvalue c)
{
 headptr h;
 seglen t,ln;
 ln = VLEN(h=PFROMFAR(thi));
 if((t=(seglen)(*at))>=ln)
     { if(REALLOC(AD(h),VSIZE(t+1))==NOADR) return(FALSE); }
 else cvalfree(VECFIRST(h)[t]);
 (*at)++;
 VECFIRST(h)[t] = c;
 return(TRUE);
 }

static int igetlen(addr thi, tail t)
{ seglen ln;
 ln = VLEN(PFROMFAR(thi));
 return(ln-(seglen)t);
 }



static LOGICAL isetlen(addr thi, tail * at, seglen l)
{ headptr h;
 seglen t,i,ln, ln1;
 ln = VLEN(h=PFROMFAR(thi));
 t=(seglen)(*at);
 ln1 = t+l;
 if(ln1<ln) for (i=ln1; i<ln; i++) cvalfree(VECFIRST(h)[i]);
 if(REALLOC(AD(h),VSIZE(ln1))==NOADR) return(FALSE);
 if(ln1>ln) for (i=ln; i<ln1; i++) VECFIRST(h)[i]=CMKSIM(CHARVIR,TYPECHAR);
 return(TRUE);
 }
static LOGICAL ishift  (addr thi, tail * at, int n)
{ headptr h;
 seglen t,ln,ln1,i;
 int n1;
 ln = VLEN(h=PFROMFAR(thi));
 t=(seglen)(*at);
 ln1 = (n>=-(int)(ln-t)? ln+n : t);
 n1=ln-ln1;
 if(n1>0)  /* length decreeses */ {
    ALL(i,n1) cvalfree(VECFIRST(h)[t+i]);
    for(i=t; i!=ln1; i++) VECFIRST(h)[i]=VECFIRST(h)[i+n1];
    };
 if(ln1!=ln) if(REALLOC(AD(h),VSIZE(ln1))==NOADR) return(FALSE);
 if(n1<0)  /* length increeses */ {
    for(i=ln1; i!=t; i--) VECFIRST(h)[i-1]=VECFIRST(h)[i+n1-1];
    ALL(i,n) VECFIRST(h)[t+i]=CMKSIM(CHARVIR,TYPECHAR);
    }
 return(TRUE);
 }
static LOGICAL icopy(addr thi, headptr from)
{  cvalue c;
   headptr h = PFROMFAR(thi);
   seglen i,ln = VLEN(from);
   if(NOT icopy_alloc(thi,from)) return(FALSE);
   ALL(i,ln) {
     cvalue vc = VECFIRST(h)[i];
     if (NOT IS_SIMPL(CTYPE(vc))) ++VAL(CREF00(vc));
     };
   return(TRUE);
   }

ifunc p_vector(headptr h, int m, addr * aa)
{  *aa = PTOFAR(h);
   switch(m) {
     case _SETPOS: return((ifunc)isetpos);
     case _GETN:   return((ifunc)igetn);
     case _REPN:   return((ifunc)irepn);
     case _GETLEN: return((ifunc)igetlen);
     case _SETLEN: return((ifunc)isetlen);
     case _SHIFT:  return((ifunc)ishift);
     case _COPY:   return((ifunc)icopy);
     case _DONE: {  seglen ln,i;
          LOCK(h);
          ln = VLEN(h);
          ALL(i,ln) cvalfree(VECFIRST(h)[i]);
          UNLOCK(h);
          }
     default: return(p_allocont(h,m,aa));
}     }