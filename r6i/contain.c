/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arkady Klimov            */
/*       contain.c    - container generic functions                     */
/************************************************************************/
#include "refcom.h"
#include "rfalloc.h"
#include "contain.h"
#include "refgen.h"
#include "rfio.h"

static LOGICAL isave (addr thi, void far * f, ifunc ptc)
{ elemptr h;
  h=PFROMFAR(thi);
  return(TRUE);
  }

ifunc p_contain(headptr h, int m, addr * aa)
{
   *aa = PTOFAR(h);
   switch(m) {
     case _START:
     case _END:   return(itrue);
     case _LOAD:  return((ifunc)iload);
     case _SAVE:  return((ifunc)isave);
/*     case _DONE:  if(TYPE(h)==HEADNUMB AND NEXT(h)!=h)
                    AFTERX(b,NEXT(h),PREV(h));
                  return(itrue); */
     case _CONTAIN: return(itrue);
     default: return(p_object(h,m,aa));
}     }

ifunc p_allocont(headptr h, int m, addr * aa)
{  ifunc res;
   res = p_alloc(h,m,aa);
   if(res==inull) return(p_contain(h,m,aa)); else return(res);
   };
