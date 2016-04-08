/************************************************************************/
/*  R E F A L - 6           Copyright (C) 1992 by Arkady Klimov         */
/*       constor.c    - free storage mode container                     */
/************************************************************************/

#include "refcom.h"
#include "contain.h"
#include "channel.h"


/* Free store as a container to be written into */

static int isetpos(addr thi, tail * at, cvalue c, int n)
/* All arguments are dummy */
{ return(0);}

LOGICAL irepnfree(addr thi, tail * at, cvalue c)
/* thi and at are dummy arguments */
{ movb;
  SETDATA(b,c);
  return(TRUE);
}

/* Free store as output channel */

static int iputc(int c, addr thi)
{
   movb;
   SETSIM(b,c,TYPECHAR);
   return(TRUE);
   };

ifunc p_store(headptr h, int m, addr * aa)
{
   switch(m) {
     case _SETPOS: return((ifunc)isetpos);
     case _SETLEN: return((ifunc)isetpos);
     case _REPN:   return((ifunc)irepnfree);
     case _PUTC:   return((ifunc)iputc);
     default: return(p_object(h,m,aa));
}     }
