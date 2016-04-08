/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arcady Klimov            */
/*       rbcont.c      - container functions                            */
/************************************************************************/
/*E        <NEW [s.Word]> -> s.Ref         ,  rb_new()                  */
/*E        <GET tR>    -> tV               ,  rb_get=rb_get()           */
/*E        <SET tR tX> -> tR               ,  rb_set=rb_set()           */
/*E  old   <WTR sR eA> ->                  ,  rb_wtr=rb_setvr(FALSE)    */
/*E  old   <GTR sR> -> eA                  ,  rb_gtr=rb_getvr(TRUE)     */
/*E  old   <CPR sR> -> eA                  ,  rb_getv=rb_getvr(FALSE)   */
/*E        <GETV tR> -> eA                 ,  rb_getv=rb_getvr(FALSE)   */
/*E        <SETV tR eA> -> t.R1            ,  rb_setv=rb_setvr(TRUE)    */
/*E        <GETN tR s.Ind> -> t.Val        ,  rb_getn()                 */
/*E        <SETN tR s.Ind t.Val> -> t.R1   ,  rb_setn()                 */
/*E        <GETS tR sN sL> -> eS           ,  rb_gets()                 */
/*E        <SETS tR sN sL eS> -> t.R1      ,  rb_sets()                 */
/*E        <POS  tR tV> -> sN1...sNk       ,  rb_pos()                  */
/*E        <SHIFT tR sN sD> -> tR          ,  rb_shift()                */
/*           sN - offset, sD - shift ><0                                */
/*E  n/a   <CONCAT tR t1 t2 ...> -> t.R1   ,  rb_concat()               */
/*          concat: tR = t1+t2+...
/*E        <LENCONT tR> -> s.Len           ,  rb_lencont()              */
/*E        <TESTMEM>         -> s.Num      ,  rb_testmem()              */

#include "refgen.h"
#include "contain.h"
#include "rfalloc.h"
#include "rbarm.h"

#define CHARVIR ' '

#define rf_intlen(n) BITSSHORT
#ifndef rf_intlen
static int rf_intlen(int i)
{ int len;
  for(len=1; i!=0 AND i!=-1; i>>=1, len++);
  return(len);
  };
#endif

LOGICAL
rb_new()
{
   elemptr p; headptr h;
   cvalue cw;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL) cw = NOCVALUE;
     else if(EargL!=EargR OR NOT ISSYMB(EargL)) return(FAIL);
     else cw = DATA(EargL);
   ECALL( rf_mk_hboxn(cw, &h) );
   movbx(b,p);
   SETREF(p,h,TYPEREF);
   weld(b,NEXT(p));
   if(EargL!=NULL) AFTER(b,EargL,EargR);
   *AeresL=*AeresR=p;
   return(TRUE);
 }

/*  Check that (STARTED, actual) object h is of the class contain,
    otherwise if object is EMPTY convert it to empty BOX,
    otherwise return FALSE  */
LOGICAL rf_iscont(elemptr h)
{   if(IFUNC(h,_CONTAIN)!=inull) return(TRUE);
    if(TYPE(h)!=HEADEMPTY) return(FALSE);
    TYPE(h) = HEADBOX;
    NEW(h);
    return(TRUE);
}

/* Make a valid copy for modification */
LOGICAL rf_mk_copy (cvalue c, headptr* ah, int mincount, LOGICAL enablobj)
/* c is virtual; if returned TRUE result *ah is actual and STARTED */
/* mincount is ref. count. for c when it is allowed to be modified */
{
  int t = CTYPE(c);
  if (IS_SIMPL(t)) {
    if(IS_SWORD(t)) {
       ECALL( rf_mk_hword(SWDBODY(c),SWD_LEN(t),ah) );
       }
    else if (IS_SNUMB(t)) {
       /* long i = (int) CVAL(c); Was incorect for big-endian computers */
       ushort i[2];
       i[0] = CVAL(c);
       i[1] = ((short)(i[0])>=0? 0 : -1);
       ECALL( rf_mk_hnumb(/* (ushort*) */ i,2,ah) );
       }
    else if (IS_REAL(t)) {
       float f;
       cvalue2float(c,f);
       ECALL( rf_mk_hreal((real)f,ah) );
       }
    else return(FAIL);
    }
  else {
     headptr h=CREF00(c);
     t = TYPE(h);
     START(h);
     if(ISOBJ(h) AND enablobj) { VAL(h)++; *ah=h; return(TRUE); }
     if(VAL(h)==mincount) {
        if (t==HEADWORD) fromhashlist(h);
        VAL(h)++; *ah=h; return(TRUE);
        }
     ECALL( rf_mk_empty(ah,t) );
     /* copy h to *ah */
       ECALL( EVAL(*ah,_COPY,Tmove,(THIS,h)) );
     END(h);
     }
  return(TRUE);
}










LOGICAL rf_norm_cont(cvalue c, headptr *ah, int mincont)
{ LOGICAL res;
  RCALL( res,rf_mk_copy(c,ah,mincont,TRUE) );
  if(NOT rf_iscont(*ah)) { END(*ah); headptrfree(*ah); return(FAIL); }
  return TRUE;
  }

#define NORMCONT(c,ah,mincont) RCALL( res, rf_norm_cont(c,ah,mincont) );
#define ISCONT(h) rf_iscont(h)

void rf_simp_cont(headptr h, cvalue * ac)
/* h is actual and STARTED; result *ac is actual */
{ int t;
  int i,l;
  uchar * aw;
  t=TYPE(h);
  if(t==HEADNUMB) {
         ushort* an = MFIRST(h);
         l=MLEN(h);
         while (l>1 AND an[l-1]==an[l-2]) l--;
         if (l<2 OR l==2 AND ((short) (an[0]^an[1]))>=0) {
            *ac = CMKSIM(( l? *(MFIRST(h)) : 0 ),TYPESNUMB);
            END(h);
            headptrfree(h);
            }
         else {
            *ac = CMKREF(h,TYPELVAL);
            END(h);
            }
         }
  else if(t==HEADWORD) {
          if ((l=WDLEN(h)) < 4) {
              aw=WDFIRST(h);
              *ac = CMKSIM(0,TYPESWORD(l));
              ALL(i,l) SWDBODY(*ac)[i]=aw[i];
              TYPE(h) = HEADSTR;
              END(h);
              headptrfree(h);
              }
          else {
             END(h);
             tohashlist(&h);
             *ac = CMKREF(h,TYPEREF);
             }
          }
  else {
      *ac = CMKREF(h,(t==HD(CHAIN,CON)? TYPEBRAC:
                     (t==HD(REAL, CON)? TYPELVAL:
                     TYPEREF)));
      END(h);
      }
}








LOGICAL rf_set(headptr h, cvalue c, int mincount)
{ headptr g; LOGICAL res;
   RCALL( res,rf_mk_copy(c,&g,mincount,FALSE) );
   ECALL( rf_get_empty(h,TYPE(g)|1) );  /* Convert type to OBJ-type */
   /* move g to h */
   ECALL( EVAL(h,_MOVE,Tmove,(THIS,g)) );
   TYPE(g) = HEADEMPTY;
   headptrfree(g);
   END(h);
   return TRUE;
   }

LOGICAL rb_set()
{ elemptr p,h;
  cvalue c; LOGICAL res;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR NEXT(EargL) != EargR) return FAIL;
   if (NOT ISREF(EargL) OR NOT ISOBJ(h=REF00(EargL))) return FAIL;
   c = DATA(EargR);          /* c is virtual */
   RCALL(res,rf_set(h,c,1));       /* 1 is min. refcount. for c */
   VAL(h)++;
   AFTER(b,EargL,EargR);
   movbx(b,p);
   weld(b,NEXT(p));
   SETREF(p,h,TYPEREF);
   *AeresL = *AeresR = p;
   return(TRUE);
}

LOGICAL rf_get(cvalue *av, cvalue c, int mincount)
{  headptr h;
   LOGICAL res;
   RCALL( res,rf_mk_copy(c,&h,mincount,FALSE) );
   TYPE(h) &= -2; /* Convert to CONST */
   rf_simp_cont(h,av);
   return TRUE;
   }

LOGICAL rb_get()
{ elemptr p;
  cvalue c,v;
  LOGICAL res;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL != EargR) return FAIL;
   c = DATA(EargL);          /* c is virtual */
   RCALL( res,rf_get(&v,c,1) );    /* 1 is min. refcount. for c */
   AFTER(b,EargL,EargR);
   movbx(b,p);
   weld(b,NEXT(p));
   SETDATA(p,v);
   *AeresL = *AeresR = p;
   return(TRUE);
}

LOGICAL rb_setvr(LOGICAL ires)
{
  elemptr p,h;
  cvalue c;
  LOGICAL res;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL) return(FAIL);
   c = DATA(EargL);          /* c is virtual */
   NORMCONT(c,&h,1); /* 1 is min. refcount. for c */
   if (MODE(TYPE(h)) == CHAIN) { /* for efficiency */
       if (NEXT(h) != h) { AFTERX(b,NEXT(h),PREV(h)); }
       if (EargL != EargR) { AFTER(h,NEXT(EargL),EargR); }
       AFTER(b,EargL,EargL);
       }
   else {
       int len;
       tail t;
       Tsetlen isetlen; Tsetpos isetpos; Trepn irepn;
       addr thi;
         isetlen = (Tsetlen) IFUNC(h,_SETLEN);
         isetpos = (Tsetpos) IFUNC(h,_SETPOS);
         irepn   = (Trepn) IFUNC(h,_REPN);
         thi = THIS;
         /* Evaluate the length of Arg */
         for (p=EargL, len=0; p!=EargR; p=NEXT(p), len++);
         isetpos(thi,&t,0);
         ECALL( isetlen(thi,&t,len) );
         for (p=EargL; p!=EargR; ) {
             p=NEXT(p);
             irepn(thi,&t,ADATA(p));
             };
       AFTER(b,EargL,EargR);
       };
   rf_simp_cont(h, &c); /* h is actual and STARTED, result c is actual */
   if(ires) {
      movbx(b,p);
      weld(b,NEXT(p));
      SETDATA(p,c);
      }
   else {
      p = NOELEM;
      cvalfree(c);
      }
   *AeresL = *AeresR = p;
   return(TRUE);
}











LOGICAL c_getvr(cvalue c, LOGICAL iclr)
  /* Argument c is virtual, result in Free */
{
 int i,ct,l;
 elemptr p,q,h;
 cvalue cc;
 ct = CTYPE(c);
 if( IS_SIMPL(ct)) {
   if (IS_SWORD(ct))
      for( i=0, l=SWD_LEN(ct); i<l; i++) {
         movb;
         SETSIM(b,SWDBODY(c)[i],TYPECHAR);
         }
   else if(IS_SNUMB(ct))
      for (i = CVAL(c), l=rf_intlen(i) ; l-- > 0 ; i>>=1) {
         movb;
         SETSIM(b,'0'+(i&1),TYPECHAR);
         }
   else return(FAIL);
   }
 else {
   h = CREF00(c); /* c IS-REF */
   if(NOT ISCONT(h)) return(FAIL);
   if(MODE(TYPE(h)) == CHAIN) { /* for efficiency */
     if (iclr OR (VAL(h)==1)) {
       p = NEXT(h); q = PREV(h);
       if (p!=h) { AFTERX(b,p,q); b = q; }
       }
     else
       for (p = NEXT(h); p != h ; p = NEXT(p) ) {
         movb;  MKINF(b,p);
         if (NOT ISSIMPL(b)) ++VAL(REF00(b));
     }     }
   else /* (MODE!=CHAIN) */ {
       tail t;
       Tsetpos isetpos; Tgetn igetn;
       addr thi;
       START(h);
         isetpos  = (Tsetpos) IFUNC(h,_SETPOS);
         igetn    = (Tgetn)   IFUNC(h,_GETN);    thi = THIS;
         for (isetpos(thi,&t,0); igetn(thi,&t,&cc); ) {
            movb;
            cvalact(cc);
            SETDATA(b,cc);
            }
         if(iclr AND ISOBJ(h)) {
             isetpos(thi,&t,0);
             EVAL(h,_SETLEN,Tsetlen,(THIS,&t,0));
             }
       END(h);
   }   }
 return(TRUE);
}







LOGICAL rb_getvr(LOGICAL iclr)
{  LOGICAL res;
   elemptr EargL, EargR, savb=b;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL != EargR) return(FAIL);
   RCALL( res, c_getvr(DATA(EargL),iclr) );
   if (b == savb) *AeresL = *AeresR = NOELEM;
   else {
     *AeresL = NEXT(savb);
     *AeresR = b;
     weld(savb,NEXT(b));
     b = savb;
     }
   AFTER(b,EargL,EargR);
   return(TRUE);
   }

LOGICAL rb_setn()
{ elemptr p,h,pn; cvalue c;
  long N;  iseglen n; int n1;  LOGICAL res;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL == EargR) return(FAIL);
   pn=NEXT(EargL);
   if(NEXT(pn)!=EargR) return(FAIL);
   if (NOT rf_getnumb(pn, &N)) return(FAIL); n = (iseglen) N;
   c = DATA(EargL);          /* c is virtual */
   NORMCONT(c,&h,1); /* 1 is min. refcount. for c */
   {       addr thi;
       tail t;
       Tsetpos isetpos;
       isetpos = (Tsetpos) IFUNC(h,_SETPOS); thi = THIS;
       if((n1=isetpos(thi,&t,n)) > 0) {
        ECALL( EVAL(h,_SETLEN,Tsetlen,(THIS,&t,n1+1)) );
        isetpos(thi,&t,n);
        };
       if(n1>=0) ECALL( EVAL(h,_REPN,Trepn,(THIS,&t,ADATA(EargR))) );
       AFTER(b,EargL,EargR);
       };
   rf_simp_cont(h, &c); /* h is actual and STARTED, result c is actual */
   movbx(b,p);
   weld(b,NEXT(p));
   SETDATA(p,c);
   *AeresL = *AeresR = p;
   return(TRUE);
}










/*    <GETN s.Obj sN> == tX  */
LOGICAL rb_getn()
{ int ct;   char ch;
  cvalue c, cc;  elemptr h, savb = b;
  long N;  iseglen n,n1;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
  popae(AeresL,AeresR);
  pope(EargL,EargR);
  if (EargL == NULL OR NEXT(EargL) != EargR) return(FAIL);
  if (NOT rf_getnumb(EargR, &N)) return(FAIL); n = (iseglen) N;
  c = DATA(EargL);
  ct = CTYPE(c);
  if( IS_SIMPL(ct)) {
     if (IS_SWORD(ct))
        ch = ( (n>=0? (n1=n)<SWD_LEN(ct) : (n1=SWD_LEN(ct)+n+1)>=0)
                 ? SWDBODY(c)[n1] : CHARVIR);
     else if(IS_SNUMB(ct)) {
        int i=CVAL(c);
        ch = ( (n>=0? (n1=n)<rf_intlen(i) : (n1=rf_intlen(i)+n+1)>=0)
                ? '0'+((i>>n1)&1) : (i<0?'1':'0'));
        }
     else return(FAIL);
     cc = CMKSIM(ch,TYPECHAR);
     }
  else { tail t;
    h = CREF00(c); /* c IS-REF */
    if(NOT ISCONT(h)) return(FAIL);
    START(h);
      if(EVAL(h,_SETPOS,Tsetpos,(THIS,&t,n))<0 OR
         NOT EVAL(h,_GETN,Tgetn,(THIS,&t,&cc)))
                cc = CMKSIM(CHARVIR,TYPECHAR);
    END(h);
    }
  movb; cvalact(cc); SETDATA(b,cc);    /* cc must be actual !!! */
  *AeresL = NEXT(savb);
  *AeresR = b;
  weld(savb,NEXT(b));
  b = savb;
  AFTER(b,EargL,EargR);
  return(TRUE);
  }

LOGICAL rb_wtr  () { return rb_setvr(FALSE); }
LOGICAL rb_setv () { return rb_setvr(TRUE); }
LOGICAL rb_getv () { return rb_getvr(FALSE); }
LOGICAL rb_gtr  () { return rb_getvr(TRUE); }













/*    <POS s.Obj sV> == sN1 ... sNk */
LOGICAL rb_pos()
{ int i,ct;
  char ch;
  elemptr h, savb = b;
  cvalue c,d,dd;
  seglen k,l;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
  popae(AeresL,AeresR);
  pope(EargL,EargR);
  if (EargL == NULL OR NEXT(EargL) != EargR) return(FAIL);
  d = DATA(EargR);
  c = DATA(EargL);
  ct = CTYPE(c);
  if( IS_SIMPL(ct)) {
    if(ISCHAR(EargR)) {
      ch=VAL(EargR);
      if (IS_SWORD(ct)) {
        l=SWD_LEN(ct);
        ALL(k,l) if (SWDBODY(c)[k]==ch) ECALL( rf_mknumb(k) )
        }
      else if(IS_SNUMB(ct)) {
        i=CVAL(c);
        l = rf_intlen(i);
        ALL(k,l) {
                   if('0'+(i&1)==ch) ECALL( rf_mknumb(k) )
                   i>>=1;
        }  }
      else return(FAIL);
    } }
  else { tail t;
    int res = TRUE;
    h = CREF00(c); /* c IS-REF */
    if(NOT ISCONT(h)) return(FAIL);
    START(h);
/*       igetn = (Tgetn) IFUNC(h,_GETN); */
      EVAL(h,_SETPOS,Tsetpos,(THIS,&t,0));
      for(k=0; EVAL(h,_GETN,Tgetn,(THIS,&t,&dd)); k++)
         if(dd==d) if((res=rf_mknumb(k))!=TRUE) break;
    END(h);
    if(res!=TRUE)return(res);
    }
  if(b==savb) *AeresL=*AeresR=NOELEM;
  else {
    *AeresL = NEXT(savb);
    *AeresR = b;
    weld(savb,NEXT(b));
    b = savb;
    }
  AFTER(b,EargL,EargR);
  return(TRUE);
  }






/*    <GETS s.Obj sN sL> == eS  */
LOGICAL rb_gets()
{int i,ct;
 char ch;
 elemptr pn,pl,h,savb;
 cvalue c,cc;
 long N,L;
 iseglen n,n1,k,m; seglen l;
 elemptr EargL, EargR;
 elemptr *AeresL, *AeresR;
 popae(AeresL,AeresR);
 pope(EargL,EargR);
 if (EargL == NULL OR EargL==EargR OR
    (pn=NEXT(EargL)) == EargR OR (pl=NEXT(pn))!=EargR) return(FAIL);
 if (NOT rf_getnumb(pn,&N)) return(FAIL); else n=(iseglen)N;
 if (NOT rf_getnumb(pl,&L)) return(FAIL); else l=(seglen)L;
 savb = b;
 c = DATA(EargL);
 ct = CTYPE(c);
 if( IS_SIMPL(ct)) {
   if (IS_SWORD(ct)) {
       if (n<0) n=SWD_LEN(ct)+n+1;
       ALL(k,l) {
          m=n+k;
          ch=(m>=0 AND m<SWD_LEN(ct)? SWDBODY(c)[m] : CHARVIR);
          movb;
          SETDATA(b,CMKSIM(ch,TYPECHAR));
       }  }
   else if(IS_SNUMB(ct)) {
       i=CVAL(c);
       if (n<0) n=rf_intlen(i)+n+1;
       ALL(k,l) {
          m=n+k;
          ch=(m>=0 AND m<rf_intlen(i)? '0'+((i>>m)&1) : (i<0?'1':'0'));
          movb;
          SETDATA(b,CMKSIM(ch,TYPECHAR));
       }  }
   else return(FAIL);
   }
 else {
    tail t;
    Tgetn igetn;
    addr thi;
    h = CREF00(c); /* c IS-REF */
    if(NOT ISCONT(h)) return(FAIL);
    START(h);
      n1=EVAL(h,_SETPOS,Tsetpos,(THIS,&t,n));
      igetn = (Tgetn) IFUNC(h,_GETN); thi = THIS;
      ALL(k,l) {
        if(k<-n1) cc = CMKSIM(CHARVIR,TYPECHAR);
        else igetn(thi,&t,&cc);
        cvalact(cc);
        movb;
        SETDATA(b,cc);     /* cc is actual */
        }
    END(h);
    }



 if(b==savb) *AeresL=*AeresR=NOELEM;
 else {
    *AeresL = NEXT(savb);
    *AeresR = b;
    weld(savb,NEXT(b));
    b = savb;
    }
 AFTER(b,EargL,EargR);
 return(TRUE);
 }

/*    <SETS s.Obj sN sL eS> == s.Obj1 */
LOGICAL rb_sets()
{
  elemptr p,pn,pl,h;
  cvalue c,cc;
  long N,L;
  seglen l,k,ln;
  iseglen n;
  LOGICAL res;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL==EargR OR
      (pn=NEXT(EargL)) == EargR) return(FAIL);
   pl=NEXT(pn);
   if (NOT rf_getnumb(pn, &N)) return(FAIL); n = (iseglen) N;
   if (NOT rf_getnumb(pl, &L)) return(FAIL); l =  (seglen) L;
   c = DATA(EargL);          /* c is virtual */
   NORMCONT(c,&h,1); /* 1 is min. refcount. for c */
   {   addr thi;
       tail t;
       Tsetpos isetpos;
         isetpos = (Tsetpos) IFUNC(h,_SETPOS); thi = THIS;
         isetpos(thi,&t,0);
         ln = EVAL(h,_GETLEN,Tgetlen,(thi,t));
         if(n<0) n += ln+1;
         if(ln<n+l)    ECALL( EVAL(h,_SETLEN,Tsetlen,(THIS,&t,n+l)) );
         isetpos(thi,&t,max(n,0));
         p = pl;
         ALL(k,l) {
            if(pl==EargR) cc = CMKSIM(CHARVIR,TYPECHAR);
            else { if(p==EargR) p=pl; p=NEXT(p); cc = ADATA(p); }
            if(n<0) n++;
            else ECALL( EVAL(h,_REPN,Trepn,(THIS,&t,cc)) );
            }
       AFTER(b,EargL,EargR);
       };
   rf_simp_cont(h, &c); /* h is actual and STARTED, result c is actual */
      movbx(b,p);
      weld(b,NEXT(p));
      SETDATA(p,c);
   *AeresL = *AeresR = p;
   return(TRUE);
}




/*    <SHIFT s.Obj sN sL> == s.Obj1 */
LOGICAL rb_shift()
{
  elemptr p,h,pn,pl;
  cvalue c;
  long N,L;
  iseglen n, l;
  iseglen n1;
  LOGICAL res;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL==EargR OR
      (pn=NEXT(EargL)) == EargR OR (pl=NEXT(pn)) != EargR) return(FAIL);
   if (NOT rf_getnumb(pn, &N)) return(FAIL); n = (iseglen) N;
   if (NOT rf_getnumb(pl, &L)) return(FAIL); l = (iseglen) L;
   c = DATA(EargL);          /* c is virtual */
   NORMCONT(c,&h,1); /* 1 is min. refcount. for c */
   {   tail t;
       Tsetpos isetpos;
         isetpos =  (Tsetpos) IFUNC(h,_SETPOS);
         if((n1=isetpos(THIS,&t,n)) > 0) {
            ECALL( EVAL(h,_SETLEN,Tsetlen,(THIS,&t,n1)) );
            isetpos(THIS,&t,n);
            };
         ECALL( EVAL(h,_SHIFT,Tshift,(THIS,&t,l)) );
       AFTER(b,EargL,EargR);
       };
   rf_simp_cont(h, &c); /* h is actual and STARTED, result c is actual */
   movbx(b,p);
   weld(b,NEXT(p));
   SETDATA(p,c);
   *AeresL = *AeresR = p;
   return(TRUE);
}

LOGICAL rb_concat() { return(TRUE); };






















LOGICAL rb_lencont()
{
 headptr h; cvalue c;
 int t; long len;
 elemptr EargL, EargR;
 elemptr *AeresL, *AeresR;
 popae(AeresL,AeresR);
 pope(EargL,EargR);
 if (EargL == NULL OR EargL != EargR) return(FAIL);
 t=TYPE(EargL);
 if(IS_SIMPL(t)) {
     if(IS_SWORD(t)) len = SWD_LEN(t);
     else if(IS_SNUMB(t)) len = rf_intlen(VAL(EargL));
     else return(FALSE);
     }
 else {
     tail t;
     h = REF00(EargL);
     if(NOT ISCONT(h)) return(FAIL);
     START(h);
     EVAL(h,_SETPOS,Tsetpos,(THIS,&t,0));
     len = EVAL(h,_GETLEN,Tgetlen,(THIS,t));
     END(h);
     }
 elemfree(EargL);
 ECALL( rf_mk_cnumb(len,&c,TRUE) );
 SETDATA(EargL,c);
 *AeresL=*AeresR=EargL;
 return(TRUE);
}

LOGICAL rb_testmem()
{  long L;
   elemptr savb;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL != NULL) return(FAIL); EargR++;
   savb = b;
   testmem(&L);
   rf_error = ERRNO;
   b=savb;
   rf_mknumb(L);
   WELD(savb,NEXT(b));
   *AeresL=*AeresR=b;
   b=savb;
   return(TRUE);
};
