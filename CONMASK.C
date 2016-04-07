/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arcady Klimov            */
/*       conmask.c    - container functions for masks and numbers       */
/************************************************************************/
#include "refcom.h"
#include "rfalloc.h"
#define NODEFTAIL
typedef long tail;
#include "contain.h"
#include "arithm.h"
#include "rbarm.h"

#define GetBit(a,n) ((((a)[(n)/BitsShort])>>(n%BitsShort))&1)
#define PutBit(a,n,b) { if(b) (a)[(n)/BitsShort] |= 1<<(n%BitsShort); \
                        else  (a)[(n)/BitsShort] &= (-1)^(1<<(n%BitsShort)); }
#define MBLEN(h)  (BitsShort*(MLEN(h)))
#define MBSIZE(l) (((l)+(BitsShort-1))/BitsShort)

static int isetpos(addr thi, tail * at, int n)
{
 iseglen ln = MBLEN(PFROMFAR(thi));
      if (n>ln) { *at = ln; return(n-ln); }
 else if (n>=0) { *at =  n; return(0);    }
 else {
      n = ln+n+1;
      if (n<0)  { *at =  0; return(n);    }
      else      { *at =  n; return(0);    }
 }    }

static LOGICAL igetn (addr thi, tail * at, cvalue * ac)
{
 headptr h=PFROMFAR(thi);
 seglen t,ln;
 ln = MBLEN(h);
 if((t=(seglen)(*at))>=ln)
	 { *ac=CMKSIM(CHARVIR,TYPECHAR); return(FALSE); }
 (*at)++;
 *ac=CMKSIM('0'+GetBit(MFIRST(h),t),TYPECHAR);
 return(TRUE);
 }

static LOGICAL irepn   (addr thi, tail * at, cvalue c)
{
 headptr h=PFROMFAR(thi);
 int bit;
 seglen t,i,s,l;
 MPOS A;
 l=MLEN(h);
 cvalfree(c);
 t=(seglen)(*at);
 s=MBSIZE(t+1)+1;
 if(s>l) { ECALL(REALLOC(AD(h),MSIZE(s))!=NOADR); }
 else s=l;
 A=MFIRST(h);
 for(i=l;i<s;i++) A[i]=(l>0? A[l-1] : 0);
 (*at)++;
 bit = (CTYPE(c)==TYPECHAR ? CVAL(c)&1 : 0);
 PutBit(A,t,bit);
 return(TRUE);
 }

static int igetlen(addr thi, tail t)
{
 seglen ln;
 ln = MBLEN(PFROMFAR(thi));
 return(ln-(seglen)t);
 }

static LOGICAL isetlen(addr thi, tail * at, seglen l)
{
  headptr h=PFROMFAR(thi);
  seglen t,i,ln,s,tl,tm;
  MPOS A;
  int sign,bsign;
  ln = MLEN(h);
  t=(seglen)(*at);
  tl=t+l;            /* desired result length in bits */
  s=MBSIZE(t+l);
  if(s) s++;         /* result size in macrodigits    */
  tm=BitsShort*s;    /* actual result length in bits  */
  if(s!=ln) ECALL(REALLOC(AD(h),MSIZE(s))!=NOADR);
  A=MFIRST(h);
  sign = (ln>0? A[ln-1] : 0);
  bsign = sign&1;
  /* Clear extension bits using old sign */
  if(s>ln)  for(i=ln;i<s;i++) A[i]=sign;
  else      for(i=tl; i<tm; i++) PutBit(A,i,bsign);
 return(TRUE);
 }

static LOGICAL ishift  (addr thi, tail * at, int n)
{
 headptr h=PFROMFAR(thi);
 seglen t,i,i1,ln,ln1,ln1m,l1;
 int n1,bit;
 seglen  l = MLEN(h);
 MPOS    A = MFIRST(h);
 int bsign = A[l-1] & 1;

 ln= MBLEN(h);
 t=(seglen)(*at);
 ln1 = (n>=-(int)(ln-t)? ln+n : t);
 l1=MBSIZE(ln1)+1;
 ln1m= BitsShort*l1;
 n1=ln-ln1;
 if(n1>0) /* length decreeses */
	for(i=t; i!=ln1m; i++) {
       i1=i+n1;
       bit=(i1<ln? GetBit(A,i1) : bsign) ;
       PutBit(A,i,bit);
       };
 if(l1!=l) { ECALL(REALLOC(AD(h),MSIZE(l1))!=NOADR); A=MFIRST(h); }
 if(n1<0)  /* length increeses */ {
	for(i=ln1m; i>t; i--) {
	   seglen j=i-1;
	   i1=j+n1;
       bit=(i1<ln? (i1<t? 0: GetBit(A,i1)) : bsign) ;
	   PutBit(A,j,bit);
       }
	}
 return(TRUE);
 }

ifunc p_mask(headptr h, int m, addr * aa)
{
   *aa = PTOFAR(h);
   switch(m) {
	 case _SETPOS: return((ifunc)isetpos);
	 case _GETN:   return((ifunc)igetn);
	 case _REPN:   return((ifunc)irepn);
	 case _GETLEN: return((ifunc)igetlen);
	 case _SETLEN: return((ifunc)isetlen);
	 case _SHIFT:  return((ifunc)ishift);
	 case _END:	{  /* Normalize number */
                  MPOS A = MFIRST(h);
                  seglen l = MLEN(h), lA=l;
		  ra_norm(A,&lA);
		  if(lA!=l) REALLOC(AD(h),MSIZE(lA)); /* here memory error can't occur */
		  }
	 default: return(p_allocont(h,m,aa));
}	 }

ifunc p_real(headptr h, int m, addr * aa)
{
   switch(m) {
	 case _NEW: { real z=0.0; real2head(z,h); }
	 default: return(p_object(h,m,aa));
}	 }
