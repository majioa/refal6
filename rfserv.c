/******************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
   rfserv.c    - service functions
*******************************************************************/
#include "refgen.h"
#include "rfio.h"
#include "rftable.h"
#include "rfalloc.h"
#include "rbarm.h"

/* Compare strings */
static int wcmps (uchar* s1, int l1, uchar* s2, int l2)      /* wcmps */
{  int d,k;
#define LEXICOGRAPHICALLY
#ifdef LEXICOGRAPHICALLY
/* Pure lexicographical order */
   for ( ; l1!=0 AND l2!=0 ; l1--, l2--, s1++, s2++ )
      { d = (*s1)-(*s2) ; if (d != 0) return(d); }
   return(l1-l2);
#else
/* the shortest string is the least string */
   if(l1!=l2) return(l1-l2);
   ALL(k,l1) { d=s1[k]-s2[k]; if (d!=0) return(d); }
   return(0);
#endif
}

int ra_cmpl(headptr h1, headptr h2)  /* сравнение двух длинных чисел */
{   int d,i;
    seglen l1,l2;
    ushort *a1, *a2;
    if(h1==h2) return 0;
    START(h1); START(h2);
      l1 = MLEN(h1);   l2 = MLEN(h2);
      a1 = MFIRST(h1); a2 = MFIRST(h2);
      if((d=a1[l1-1]-a2[l2-1])!=0) ;
      else
      if((d=l1-l2) != 0) { if (a1[l1-1]!=0) d = -d; }
      else
      for(d=0,i=l1-2; i>=0; i--)
        if(a1[i]!=a2[i]) { d = (a1[i]>a2[i] ? 1 : -1); break; }
    END(h1); END(h2);
    return d;
    };

int ra_sign(headptr h) /* знак длинного числа: 0 или -1 */
{  int d;
   START(h);
      d = (MFIRST(h))[MLEN(h)-1];
   END(h);
   return d;
   }

LOGICAL rf_eqlongc (elemptr p, cvalue c)              /* rf_eqlongc */
{  return(ra_cmpl(REF00(p),CREF00(c))==0); };

LOGICAL rf_eqlong (elemptr p, elemptr q)               /* rf_eqlong */
{  return(ra_cmpl(REF00(p),REF00(q))==0); };

int rf_cmp (elemptr p1, elemptr q1, elemptr p2, elemptr q2)
{
  int t1,t2;
  headptr h1,h2;
  elemptr* savspe;
  int res;
  res = 0;
  if (p2 == NULL)
    if (p1 == NULL) return (0);
    else return(1);
  else
    if (p1 == NULL) return (-1);
    else {
      savspe = spe;
      for (;; p1 = NEXT(p1), p2 = NEXT(p2)) {
        t1 = TYPE(p1);
        t2 = TYPE(p2);
        h1 = REF00(p1);
        h2 = REF00(p2);

        if (IS_CHAR(t1)) {
          if (NOT IS_CHAR(t2)) res = -1;
          else if (VAL(p1) != VAL(p2)) res = VAL(p1) - VAL(p2);
          }
        else if (IS_CHAR(t2)) res = 1;

    else if (ISWORD(p1)) {
          if (NOT ISWORD(p2)) res = -1;
          else {
             uchar *s1, *s2;
             int l1,l2;
             cvalue c1,c2;
             if (IS_REF(t1)) { START(h1);
                s1 = WDFIRST(h1);
                l1 = WDLEN(h1);
                }
             else {    c1 = DATA(p1);
                s1 = SWDBODY(c1);
                l1 = SWD_LEN(CTYPE(c1));
                };
             if (IS_REF(t2)) { START(h2);
                s2 = WDFIRST(h2);
                l2 = WDLEN(h2);
                }
             else { c2 = DATA(p2);
                s2 = SWDBODY(c2);
                l2 = SWD_LEN(CTYPE(c2));
                };
             res = wcmps(s1,l1,s2,l2);
             if(IS_REF(t1)) END(h1);
             if(IS_REF(t2)) END(h2);
          }     }
        else if (ISWORD(p2)) res = 1;







        else if (ISNUMB(p1)) {
          if (NOT ISNUMB(p2)) res = -1;
          else if (IS_SNUMB(t1)) {
            if (IS_SNUMB(t2)) {
               if (VAL(p1) != VAL(p2)) res =
                    ( (int) VAL(p1) < (int) VAL(p2) ? -1 : 1 );
               }
            else res = (ra_sign(h2) == 0 ? -1 : 1);
            }
          else if (ISLNUMB(p2)) res = ra_cmpl(h1,h2);
          else res = (ra_sign(h1) == 0 ? 1 : -1);
          }
        else if (ISNUMB(p2)) res = 1;

#define NormReal(p,r) {                             \
      headptr h; float f; cvalue c;                 \
      if (ISREF(p)) { h=REF00(p); head2real(h,r); } \
      else { c=DATA(p); cvalue2float(c,f); r=f; }   \
      }

#define ISANYREAL(p) (ISREAL(p) OR ISR(p,HD(REAL,CON)))

        else if (ISANYREAL(p1)) {
          if (NOT ISANYREAL(p2)) res = -1;
          else {
             real r1,r2;
             NormReal(p1,r1);
             NormReal(p2,r2);
             if(r1!=r2) res = (r1<r2? -1 : 1);
          }  }
        else if (ISANYREAL(p2)) res = 1;

        else if (ISOBJ(h1)) {
          if (NOT ISOBJ(h2)) res = -1; else res = h1-h2;
          }
        else if (ISOBJ(h2)) res = 1;

        else if (IS_BRAC(t1)) {
          if (NOT IS_BRAC(t2)) res = 1;
          else {
            pushe(p1,q1); pushe(p2,q2);
            if(spe > spemax) rf_err(ERCSTKO);
            p1 = h1;
            p2 = h2;
            q1 = PREV(p1);
            q2 = PREV(p2);
          } }
        else rf_err(ERCDATA);

        if (res != 0) {  spe = savspe;  return(res); }
        while (p1 == q1) {
          if (p2 != q2) { spe = savspe;    return(-1); }
          if (spe == savspe) return (0);
          pope(p2,q2); pope(p1,q1);
          }
        if (p2 == q2) {  spe = savspe; return (1); }
      } }
}


/* make box head given a name */
LOGICAL rf_mk_hboxn (cvalue name, headptr * ah)        /* rf_mk_hboxn */
         /* name is ghost, result *ah is actual */
{  if(NOT rf_mk_hbox(ah)) return(FALSE);
     if(name!=NOCVALUE) rf_tab_link(sysid,name,CMKREF(*ah,TYPEREF));
   return(TRUE);
   };

/* make box head without name */
LOGICAL rf_mk_hbox(headptr * ah)                   /* rf_mk_hbox */
{ return(rf_mk_empty(ah,HEADEMPTY)); }
/* {  headptr hb;
   movbx(b,hb);
   CROSS(b,hb);
   TYPE(hb) = HEADBOX;
   VAL(hb) = 1;
   *ah = hb;
   return(TRUE);
   }; */

/* make new object headder */
LOGICAL                                                rf_mk_empty
   (headptr * ah,     /* result *ah is actual unless result is FALSE */
    int typ           /* type typ must accept message _NEW */
    )
{  headptr hb;
   movbx(b,hb);
   weld(b,NEXT(hb));
   TYPE(hb) = typ;
   VAL(hb) = 1;
   NEW(hb);
   *ah = hb;
   return(TRUE);
   };

void rf_exit(int n)                             /* rf_exit */
{  rf_termst();
   if (n > rf_exitcd) exit(n);
   exit(rf_exitcd);
}

void rf_exitmsg(int n, char * s)                 /* rf_exitmsg */
{  rf_errmsgs(s); rf_exit(n); }

/* Print out statistics on memory usage */

void rf_out_stat(FILE* f) {
    rf_mem_stat (f);
    rf_heap_stat(f);
    }










LOGICAL rf_mk_hnumb(ushort* an, seglen l, headptr *ah)
/* number (an,l) is normalized; result *ah is actual and STARTED */
{  headptr h;
   int i;
   ushort * am;
   ECALL(rf_mk_empty(&h,HEADNUMB));
   ECALL(REALLOC(AD(h),MSIZE(l))!=NOADR);
   am=MFIRST(h);
   ALL(i,l) { am[i] = an[i]; }
   *ah = h;
   return(TRUE);
   }

LOGICAL rf_mk_numb(ushort* an, seglen l, cvalue * ac, LOGICAL simplify)
{
   headptr h;
   ra_norm(an,&l);
   if (simplify AND (l==1 OR l==2 AND (short)(an[0]^an[1])>=0))
        { *ac = CMKSIM(an[0],TYPESNUMB); return(TRUE);}
   ECALL(rf_mk_hnumb(an,l,&h));
   END(h);
   *ac = CMKREF(h,TYPELVAL);
   return(TRUE);
   }

LOGICAL rf_mk_cnumb(long l, cvalue * ac, LOGICAL simplify)
{
  /* long n[2];
  n[0] = l;
  n[1] = (n[0]>=0? 0:-1); Was incorrect for Big-endian processors */
  ushort n[3];
  n[0] = l;
  n[1] = l>>16;
  n[2] = (l>=0? 0:-1);
  return rf_mk_numb((ushort*) &n, 3, ac, simplify);
  }

LOGICAL rf_mknumb(long n)
{
  cvalue c;
  if(NOT rf_mk_cnumb(n,&c,TRUE)) return(FALSE);
  movb;
  SETDATA(b,c);
  return (TRUE);
  };

LOGICAL rf_mkstr (uchar* s,int l)                         /* rf_mkstr */
{   while (l--) {
      movb;
      TYPE(b) = TYPECHAR;
      VAL(b) = *s++;
      }
   return (TRUE);
}

