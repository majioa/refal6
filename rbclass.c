/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arkady Klimov            */

/*       rfclass.c   -  built-in functions for objects and classes      */
/************************************************************************/

/*E        <MODE s.Ref> -> s.Mode          ,  rb_mode                   */
/*E        <CONST s.Ref> -> T | F          ,  rb_const                  */
/*E        <MAKE s.Mode [s.Ref]> -> s.Ref  ,  rb_make                   */
/*E        <MOVE s.Src s.Dst> -> s.Dst1    ,  rb_movcop(_MOVE)          */
/*E        <COPY s.Src s.Dst> -> s.Dst1    ,  rb_movcop(_COPY)          */
/*E        <REFCOUNT s.Ref> -> sN          ,  rb_refcount               */
/*E        <SEND s.Obj e.Mes> -> e.Reply   ,  rb_refcount               */
/*   where s.Mode = EMPTY | CHAIN | VECTOR | STRING | MASK  etc.        */

#include "refgen.h"

LOGICAL rb_mode()
{
   int m;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL != EargR) return(FAIL);
   if (NOT ISSIMPL(EargL)) m=MODE(TYPE(REF00(EargL)));
   else if (ISSNUMB(EargL)) m=MASK;
   else if (ISSWORD(EargL)) m=STRING;
   else if (ISCHAR(EargL)) m=CHAR;
   else if (ISREAL(EargL)) m=REAL;
   else return FAIL;
   elemfree(EargL);
   SETDATA(EargL, pword_table[m]);
   if(NOT ISSIMPL(EargL)) ++VAL(REF00(EargL));
   *AeresL = *AeresR = EargL;
   return(TRUE);
   };

LOGICAL rb_const()
{
   int m;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL != EargR) return(FAIL);
   m = (ISSIMPL(EargL) OR ISCON(REF00(EargL)) ? _T : _F );
   elemfree(EargL);
   SETDATA(EargL, COMWORD[m]);
   if(NOT ISSIMPL(EargL)) ++VAL(REF00(EargL));
   *AeresL = *AeresR = EargL;
   return(TRUE);
   };









LOGICAL rb_make()
{
   int mode,typ;
   elemptr p;
   headptr h;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL) return(FAIL);
   ALL(mode,MAXMODE) { if(EQDATC(EargL,pword_table[mode])) goto M; }
                  return(FAIL); M:;
   typ = HD(mode,OBJ);
   if(EargL==EargR) { if(NOT rf_mk_empty(&h,typ)) return(FALSE); }
   else if(NEXT(EargL)==EargR AND ISREF(EargR) )
      { if(NOT rf_get_empty(h=REF00(EargR),typ)) return(FALSE); ++VAL(h); }
   else return(FAIL);
   movbx(b,p);
   BETWEEN(b,NEXT(p),EargL,EargR);
   SETREF(p,h,TYPEREF);
   *AeresL=*AeresR=p;
   return(TRUE);
   }

static LOGICAL rb_movcop(int MovCop)
{
   headptr g,h;  cvalue c;       /* g -> h */
   elemptr p;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL) return(FAIL);
   if (NEXT(EargL)!=EargR) return(FAIL);
   if (NOT ISREF(EargL) OR NOT ISOBJ(g=REF00(EargL))) return(FAIL);
   if (ISREF(EargR) AND rf_get_empty(h=REF00(EargR),HEADBOX)) VAL(h)++;
   else return(FAIL); /* Move shouldn't produce values */
   TYPE(h) = TYPE(g);
     START(g);
       if(NOT EVAL(h,MovCop,Tmove,(THIS,g))) return FALSE;
     END(h);
   if(MovCop==_MOVE) TYPE(g) = HEADEMPTY; else END(g);
   c = CMKREF(h,TYPEREF);
   movbx(b,p);
   BETWEEN(b,NEXT(p),EargL,EargR);
   SETDATA(p,c);
   *AeresL=*AeresR=p;
   return TRUE;
   }

/*   <MOVE s.Src s.Dst> -> s.Dst1   */
/*  Object s.Src changes its location for s.Dst1  */
/*  The old state of s.Dst is missing             */
/*  s.Src becomes empty object                    */
LOGICAL rb_move() { return rb_movcop(_MOVE); }

/*   <COPY s.Src s.Dst> -> s.Dst1   */
/*  A copy of s.Src is created and put to location s.Dst1  */
/*  The old state of s.Dst is missing             */
/*  s.Src remains unchanged                       */
LOGICAL rb_copy() { return rb_movcop(_COPY); }

LOGICAL rb_refcount()
{  int m;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL != EargR) return(FAIL);
   if (ISSIMPL(EargL)) return(FAIL);
   m=VAL(REF00(EargL))--;
   SETSIM(EargL, m, TYPESNUMB);
   *AeresL = *AeresR = EargL;
   return(TRUE);
   };

LOGICAL rb_send()
{  elemptr bstart=b;
   int res;
   headptr h;
   expr eM;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL) return(FAIL);
   if (NOT ISREF(EargL) OR NOT ISOBJ(h=REF00(EargL))) return(FAIL);
   if (EargL == EargR) setempty(eM) else { eM.L=NEXT(EargL); eM.R=EargR; }
   START(h);
     res=ICALL(h,_SEND,Tsend,(THIS,eM));  /* eM is virtual here */
   END(h);
   if(res != TRUE) { b=bstart; return(res); }
   else if(b==bstart) *AeresL = *AeresR = NOELEM;
   else {
        *AeresL = NEXT(bstart);
        *AeresR= b;
        weld(bstart,NEXT(b));
        b = bstart;
        }
   AFTER(b,EargL,EargR);
   return(TRUE);
   };
