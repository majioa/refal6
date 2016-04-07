/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arkady Klimov            */
/*       concha.c    - CHAIN mode container functions                   */
/************************************************************************/
#include "refcom.h"
#include "rfalloc.h"
#define NODEFTAIL
typedef elemptr tail;
#include "contain.h"
#include "channel.h"
#include "refgen.h"

static int isetpos(addr thi, tail * at, int n)
{
 elemptr p,h,p1,p2;
 h = PFROMFAR(thi);
 p=h;
 p1=NEXT(h); p2=PREV(h);
 if(n>0)
   for (;n>0;p=NEXT(p),n--) { if(p==p2) break; }
 else if(n<0) { n++;
   for (;n<0;p=PREV(p),n++) if(p==p1) break;
   p=(p==p1? h : PREV(p)); /* Conditional for virtual expr */
   }
 *at=p;
 return(n);
 }

static LOGICAL igetn (addr thi, tail * at, cvalue * ac)
{
 elemptr h;
 h = PFROMFAR(thi);
 if(*at==PREV(h)) /* PREV for virtual expr */
	{ *ac=CMKSIM(CHARVIR,TYPECHAR); return(FALSE); }
 *at=NEXT(*at);
 *ac=DATA(*at);
 return(TRUE);
 }

static LOGICAL irepn   (addr thi, tail * at, cvalue c)
{
 elemptr p,h;
 h = PFROMFAR(thi);
 if((p=NEXT(*at))==h) {
	movbx(b,p);
	AFTERX(*at,p,p);
	}
 *at=p;
 elemfree(p);
 SETDATA(p,c);
 return(TRUE);
 }

static int igetlen(addr thi, tail t)
{ int ln;
  elemptr p,pend;
  pend = PREV(PFROMFAR(thi));
  for (ln=0, p=t; p!=pend; p=NEXT(p), ln++);
  return(ln);
  };

static LOGICAL isetlen(addr thi, tail * at, int l)
{ elemptr p,p1,h;
  h = PFROMFAR(thi);
  for( p=*at ; l>0 AND (p1=NEXT(p))!=h ; l--, p = p1 );
  if(l>0) {
	 p1=b;
	 for ( ; l>0 ; l-- ) { movb; SETSIM(b,CHARVIR,TYPECHAR); }
	 AFTERX(p,NEXT(p1),b);
	 b=p1;
	 }
  else if((p1=NEXT(p))!=h) AFTERX(b,p1,PREV(h));
  return(TRUE);
  };

static LOGICAL ishift  (addr thi, tail * at, int n)
{ elemptr p,p1,h;
  h = PFROMFAR(thi);
  if(n>0) {
	 p1=b;
	 for ( ; n>0 ; n-- ) { movb; SETSIM(b,CHARVIR,TYPECHAR); }
	 AFTERX(*at,NEXT(p1),b);
	 b=p1;
	 }
  else if (n<0) { tail t=*at;
	 for( p=t ; n<0 AND (p1=NEXT(p))!=h ; n++, p = p1 );
	 if(p!=t) AFTERX(b,NEXT(t),p);
	 };
  return(TRUE);
  };

static LOGICAL imove(addr thi, headptr from)
{  headptr h;
   h = PFROMFAR(thi);
   NEXT(h) = PREV(h) = h;
   if(NEXT(from)!=from) AFTERX(h,NEXT(from),PREV(from));
   return(TRUE);
   }

static LOGICAL icopy(addr thi, headptr from)
{  
   elemptr savb = b;
   headptr h = PFROMFAR(thi);
   NEXT(h) = PREV(h) = h;
   if(from!=NEXT(from)) copvg(NEXT(from),PREV(from));
   if(savb!=b) AFTERX(h,NEXT(savb),b);
   b=savb;
   return(TRUE);
   }











ifunc p_chain(headptr h, int m, addr * aa)
{  *aa = PTOFAR(h);
   switch(m) {
	 case _START:
	 case _END:    return(itrue);
	 case _SETPOS: return((ifunc)isetpos);
	 case _GETN:   return((ifunc)igetn);
	 case _REPN:   return((ifunc)irepn);
	 case _GETLEN: return((ifunc)igetlen);
	 case _SETLEN: return((ifunc)isetlen);
	 case _SHIFT:  return((ifunc)ishift);
	 case _DONE:   if(NEXT(h)!=h) AFTERX(b,NEXT(h),PREV(h)); break;
	 case _NEW:    NEXT(h)=PREV(h)=h; break;
	 case _MOVE:   return((ifunc)imove);
	 case _COPY:   return((ifunc)icopy);
	 default: return(p_contain(h,m,aa));
	 }
	 return(itrue);
}

static int igetc(addr thi)
{ headptr h;
  elemptr L;
  h = PFROMFAR(thi);
  if((L=NEXT(h))==NOELEM OR TYPE(L)!=TYPECHAR) return(-1);
  if(L==PREV(h)) NEXT(h)=NOELEM;
  else NEXT(h)=NEXT(L);
  return(VAL(L));
  }

static int iungetc(int c, addr thi)
{ headptr h;
  elemptr L;
  h = PFROMFAR(thi);
  L=NEXT(h);
  if(L==NOELEM) L=PREV(h);
  else L=PREV(L);
  if(L==h OR NOT ISCHAR(L) OR VAL(L)!=c) return(EOF);
  NEXT(h)=L;
  return(c);
  }

ifunc p_expr(headptr h, int m, addr * aa)
{  *aa = PTOFAR(h);
   switch(m) {
	 case _START:
	 case _END:   break;
	 case _SETPOS: return((ifunc)isetpos);
	 case _GETN:   return((ifunc)igetn);
	 case _GETLEN: return((ifunc)igetlen);
	 case _GETC:   return((ifunc)igetc);
	 case _UNGETC: return((ifunc)iungetc);
	 case _NEW:    NEXT(h)=PREV(h)=NOELEM; break;
	 default: return(p_object(h,m,aa));
	 }
	 return(itrue);
}