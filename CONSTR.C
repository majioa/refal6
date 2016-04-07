/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arcady Klimov            */
/*       constr.c    - container functions for strings and words        */
/************************************************************************/
#include "refcom.h"
#include "rfalloc.h"
#define NODEFTAIL
typedef long tail;
#include "contain.h"

static int isetpos(addr thi, tail * at, int n)
{
 iseglen ln = WDLEN(PFROMFAR(thi));
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
 ln = WDLEN(PFROMFAR(thi));
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
 ln = WDLEN(h=PFROMFAR(thi));
 if((t=(seglen)(*at))>=ln)
	 { *ac=CMKSIM(CHARVIR,TYPECHAR); return(FALSE); }
 (*at)++;
 *ac=CMKSIM(WDFIRST(h)[t],TYPECHAR);
 return(TRUE);
 }

static LOGICAL irepn   (addr thi, tail * at, cvalue c)
{
 headptr h;
 seglen t,ln;
 ln = WDLEN(h=PFROMFAR(thi));
 cvalfree(c);
 if((t=(seglen)(*at))>=ln)
	 if(REALLOC(AD(h),t+1)==NOADR) return(FALSE);
 (*at)++;
 WDFIRST(h)[t] = (CTYPE(c)==TYPECHAR ? CVAL(c) : CHARVIR);
 return(TRUE);
 }

static int igetlen(addr thi, tail t)
{
 seglen ln;
 ln = WDLEN(PFROMFAR(thi));
 return(ln-(seglen)t);
 }





static LOGICAL isetlen(addr thi, tail * at, seglen l)
{
 headptr h;
 seglen t,i,ln, ln1;
 ln = WDLEN(h=PFROMFAR(thi));
 t=(seglen)(*at);
 ln1 = t+l;
 if(REALLOC(AD(h),ln1)==NOADR) return(FALSE);
 if(ln1>ln) for (i=ln; i<ln1; i++) WDFIRST(h)[i]=CHARVIR;
 return(TRUE);
 }

static LOGICAL ishift  (addr thi, tail * at, int n)
{
 headptr h;
 seglen t,i,ln,ln1;
 int n1;
 ln = WDLEN(h=PFROMFAR(thi));
 t=(seglen)(*at);
 ln1 = (n>=-(int)(ln-t)? ln+n : t);
 n1=ln-ln1; 
 if(n1>0)  /* length decreeses */ {
	for(i=t; i!=ln1; i++) WDFIRST(h)[i]=WDFIRST(h)[i+n1];
	};
 if(ln1!=ln) if(REALLOC(AD(h),ln1)==NOADR) return(FALSE);
 if(n1<0)  /* length increeses */ {
	for(i=ln1; i!=t; i--) WDFIRST(h)[i-1]=WDFIRST(h)[i+n1-1];  
	ALL(i,n) WDFIRST(h)[t+i]=CHARVIR;
	}
 return(TRUE);
 }

ifunc p_string(headptr h, int m, addr * aa)
{
   *aa = PTOFAR(h);
   switch(m) {
	 case _SETPOS: return((ifunc)isetpos);
	 case _GETN:   return((ifunc)igetn);
	 case _REPN:   return((ifunc)irepn);
	 case _GETLEN: return((ifunc)igetlen);
	 case _SETLEN: return((ifunc)isetlen);
	 case _SHIFT:  return((ifunc)ishift);
	 case _DONE:   if(TYPE(h)==HEADWORD) {START(h);fromhashlist(h);END(h);}
	 default: return(p_allocont(h,m,aa));
}	 }