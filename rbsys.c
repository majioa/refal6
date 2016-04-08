/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1990 by IPS-AN                   */
/*       rbsys.c    - system functions and miscelaneous                 */
/************************************************************************/

/*E       <ARG sN> == e.Chars                 ,     rb_arg              */
/*E       <EXPLODE s.Word> == e.Chars         ,     rb_expl             */
/*E       <IMPLODE e.Chars> == s.Word         ,     rb_impl             */
/*E       <FIRST sN eX> == (eN) eY            ,     rb_first            */
/*E       <LAST  sN eX> == (eY) eN            ,     rb_last             */
/*E       <LENGTH t1 ... tl> == l             ,     rb_length           */
/*E  old  <LENW eX> == sL eX                  ,     rb_lenw             */
/*E       <TYPE eA> == 'L' | 'l' | 'D' | 'O' |
                       'F' | 'R' | 'B' | '*'  ,     rb_type             */
/*E       <CHR s.Num> == s.Char               ,     rb_chr              */
/*E       <ORD s.Char> == s.Num               ,     rb_ord              */
/*E       <LOWER eA> == eB                    ,     rb_lower            */
/*E       <UPPER eA> == eB                    ,     rb_upper            */
/*E       <EXIT [s.Code]>                     ,     rb_exit             */
/*E       <EXITCD s.Code>                     ,     rb_exitcd           */

#include "refgen.h"

LOGICAL rb_arg()
{  elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr p;
   int n,i;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL != EargR) return(FALSE);
   if (NOT ISSNUMB(EargL) OR (n=VAL(EargL))<0) return(FALSE);
   for(p = NEXT(sysarg),i=0; ; i++,p=NEXT(p))
     if (NOT ISBRAC(p)) {
        *AeresL = *AeresR = NOELEM;
        break;
        }
     else if (i==n) {
        p = REF(p);
        *AeresL=NEXT(p);
        *AeresR=PREV(p);
        if(NOT takeout(AeresL,AeresR)) return(FALSE);
        break;
        };
   AFTER(b,EargL,EargR);
   return(TRUE);
   };












LOGICAL rb_impl()
{
   char wbuf[MAXWORD+1];
   elemptr savb,p,p1;
   int i;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb=b;
/*   if (EargL == NOELEM OR NOT ISCHAR(EargL)) return(FALSE); */
   p = EargL;
   p1 = NOELEM;
   i = 0;
   if (p != NOELEM)
   while (p1 != EargR) {
      if (NOT ISCHAR(p) OR i == MAXWORD) return(FALSE);
      wbuf[i++] = VAL(p);
      p1 = p;
      p = NEXT(p);
      }
   wbuf[i] = '\0';
   if (NOT newword(wbuf)) return(FALSE);
   *AeresL = *AeresR = b;
   weld(savb,NEXT(b));
   if (EargL!=NOELEM) AFTER(savb,EargL,EargR);
   b=savb;
   return (TRUE);
}































LOGICAL rb_first()
{
   elemptr p1,p2, p3,p4, savb;
   long n;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR NOT GETNUMB(EargL,n)) return(FALSE);
   if (n < 0) return(FALSE);
   savb = b;
   p1 = p2 = p3 = p4 = NOELEM;
   if (EargL != EargR) {
      p4 = EargR;
      p3 = NEXT(EargL);
      if (n != 0) {
         for (p1 = p3; n--; p3 = NEXT(p3)) {
            if (p3 == EargR) {
               p2 = p3;
               p3 = p4 = NOELEM;
               break;
               }
            p2 = p3;
            }
         }
      }
   bl;
   if (p1 != NOELEM) {
      weld(p2,NEXT(b));
      weld(b,p1);
      b = p2;
      }
   br;
   if (p3 != NOELEM) {
      weld(p4,NEXT(b));
      weld(b,p3);
      b = p4;
      }
   *AeresL = NEXT(savb);
   *AeresR = b;
   BETWEEN(savb,NEXT(b),EargL,EargL);
   b = savb;
   return (TRUE);
}
















LOGICAL rb_last()
{
   elemptr p1,p2, p3,p4, savb;
   long n;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR NOT GETNUMB(EargL,n)) return(FALSE);
   if (n < 0) return(FALSE);
   savb = b;
   p1 = p2 = p3 = p4 = NOELEM;
   if (EargL != EargR) {
      p1 = NEXT(EargL);
      p2 = EargR;
      if (n != 0) {
         for (p4 = p2; n--; p2 = PREV(p2)) {
            if (p2 == NEXT(EargL)) {
               p3 = p2;
               p1 = p2 = NOELEM;
               break;
               }
            p3 = p2;
            }
         }
      }
   bl;
   if (p1 != NOELEM) {
      weld(p2,NEXT(b));
      weld(b,p1);
      b = p2;
      }
   br;
   if (p3 != NOELEM) {
      weld(p4,NEXT(b));
      weld(b,p3);
      b = p4;
      }
   *AeresL = NEXT(savb);
   *AeresR = b;
   BETWEEN(savb,NEXT(b),EargL,EargL);
   b = savb;
   return (TRUE);
}
















LOGICAL rb_type()
{  int c,t;
   uchar n;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM) n = '*';
   else {
      t = TYPE(EargL);
      if (IS_SIMPL(t)) {
         if (IS_SWORD(t)) n = 'F';
         else if (IS_SNUMB(t)) n = 'N';
         else if (IS_CHAR(t)) {
            if (isalpha(c=VAL(EargL))) n = (isupper(c) ? 'L' : 'l');
            else if (isdigit(c)) n = 'D';
            else n = 'O';
         }  }
      else switch (TYPE(REF00(EargL))) {
         case HEADWORD: n = 'F'; break;
         case HEADNUMB: n = 'N'; break;
         case HEADBRAC: n = 'B'; break;
               default: n = 'R';
      }     }
   movb;
   SETSIM(b,n,TYPECHAR);
   *AeresL = b;
   b = PREV(b);
   weld(b,NEXT(*AeresL));
   if (EargL == NOELEM) *AeresR = *AeresL;
   else {
      weld(*AeresL,EargL);
      *AeresR = EargR;
      }
   return (TRUE);
}

LOGICAL rb_len(int old)
{  long n;
   elemptr p;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM) n=0;
   else for (n = 1, p = EargL; p != EargR; n++) p = NEXT(p);
   if (NOT rf_mknumb(n)) return(FALSE);
   *AeresL = *AeresR = b;
   weld(PREV(b), NEXT(b));
   b = PREV(b);
   if(EargL!=NOELEM)
     if (old) { weld(*AeresR,EargL); *AeresR=EargR; }
     else AFTER(b,EargL, EargR);
   return (TRUE);
}





LOGICAL rb_lenw()   { return(rb_len( TRUE)); };
LOGICAL rb_length() { return(rb_len(FALSE)); };

LOGICAL rb_chr()
{
   elemptr p1;
   long n;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
 if (EargL != NOELEM) {
   p1 = EargL;
/*   for (;;) {
      if (ISBRAC(p1)) return(FALSE);
      if (p1 == EargR) break;
      p1 = NEXT(p1);
      }
   p1 = EargL; */
   for (;;) {
      if (GETNUMB(p1,n)) {
        elemfree(p1);
        TYPE(p1) = TYPECHAR;
        VAL(p1) = (unsigned)n;
        }
      if (p1 == EargR) break;
      p1 = NEXT(p1);
      }
   }
 *AeresL = EargL;
 *AeresR = EargR;
 return (TRUE);
}

LOGICAL rb_ord()
{
   elemptr p;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
 if (EargL != NOELEM) {
   p = EargL;
   for (;;) {
      if (ISCHAR(p)) { TYPE(p) = TYPESNUMB; }
      if (p == EargR) break;
      p = NEXT(p);
      }
   }
  *AeresL = EargL;
  *AeresR = EargR;
  return (TRUE);
}







LOGICAL rb_lower()
{
   elemptr p;
   int c;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
 if (EargL != NOELEM) {
   p = EargL;
   for (;;) {
      if (ISCHAR(p) AND (c=VAL(p), isalpha(c) )) {
         VAL(p) = tolower(VAL(p));
         }
      if (p == EargR) break;
      p = NEXT(p);
      }
   }
 *AeresL = EargL;
 *AeresR = EargR;
 return (TRUE);
}

LOGICAL rb_upper()
{
   elemptr p;
   int c;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
 if (EargL != NOELEM) {
   p = EargL;
   for (;;) {
      if (ISCHAR(p) AND (c=VAL(p), isalpha(c) )) {
         VAL(p) = toupper(VAL(p));
         }
      if (p == EargR) break;
      p = NEXT(p);
      }
   }
 *AeresL = EargL;
 *AeresR = EargR;
 return (TRUE);
}















LOGICAL rb_exitcd()
{
   int n;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR EargL != EargR OR NOT ISSNUMB(EargL)) return(FALSE);
   n = VAL(EargL);
   if(n > rf_exitcd) rf_exitcd = n;
   *AeresR = *AeresL = NOELEM;
   AFTER(b,EargL,EargR);
   return (TRUE);
}

LOGICAL rb_exit()
{
   int n;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM) rf_exit(0);
   if (EargL != EargR OR NOT ISSNUMB(EargL)) return(FALSE);
   n = VAL(EargL);
   *AeresL = *AeresR = NOELEM;
   rf_exit(n);
   return(TRUE);
}
