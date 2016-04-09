/************************************************************************/
/*       R E F A L - 6       Copyright (C) 1992 by Arkady Klimov        */
/*       refgen.c - common functions for generated c-programs           */
/************************************************************************/
#define FAST

#include "refgen.h"
#include "rfio.h"

/*  Weld two elements together */
void WELD(elemptr p1, elemptr p2)
{ weld(p1,p2) }

/*  Exchange next elements of the two given elements */
void CROSS(elemptr p1, elemptr p2)
{ elemptr n1,n2;
  n1 = NEXT(p1);
  n2 = NEXT(p2);
  weld(p1,n2);
  weld(p2,n1);
/*  NEXT(p1)=n2; PREV(n2)=p1;
  NEXT(p2)=n1; PREV(n1)=p2;*/
}

/* Insert segment (pl,pr) between elements px and py */
void BETWEEN(elemptr px, elemptr py, elemptr pl, elemptr pr)
{
  weld(px,pl);
  weld(pr,py);
/*  NEXT(px)=pl; PREV(pl)=px;
  NEXT(pr)=py; PREV(py)=pr; */
}

/* Insert segment (pl,pr) after element px */
void AFTER( elemptr px, elemptr pl, elemptr pr)
{ elemptr py;
  py = NEXT(px);
  weld(px,pl);
  weld(pr,py);
/*  NEXT(px)=pl; PREV(pl)=px;
  NEXT(pr)=py; PREV(py)=pr; */
}

/* Take segment (pl,pr) out of its place and insert it after px */
void AFTERX( elemptr px, elemptr pl, elemptr pr)
{ elemptr py,pL,pR;
  pL = PREV(pl);
  pR = NEXT(pr);
  weld(pL,pR);
/* after (px,pl,pr) */
  py = NEXT(px);
  weld(px,pl);
  weld(pr,py);
}

/* Move nonempty list to free storage, converting ATs & VFs to ()s */
void FREEX(int n, elemptr l, elemptr r) /* the result is always TRUE */
{ elemptr savb=b, end=NEXT(b);
  elemptr pL,pR;
  pL = PREV(l);
  pR = NEXT(r);
  weld(pL,pR);                      /* take out */
  weld(savb,l);
  weld(r,end);                      /* after(b,l,r) */
  while ( b!=end ) {
     b=NEXT(b);
     if(NOT ISSIMPL(b)) {
        headptr h = REF00(b);
        /* Convert AT or VF to () */
        if(ISBRAC(b) AND TYPE(h)!=HEADBRAC) SETSIM(h,1,HEADBRAC);
        /* make element simple */
        headptrfree(h);
        SETSIM(b,n,TYPESIMPL)
     }  }
  b=savb;
  };

/* Move list of AT's and VF's making them usual (.)'s */
void AFTERXS( elemptr px, elemptr pl, elemptr pr)
{ elemptr py,pL,pR;
  pL = PREV(pl);
  pR = NEXT(pr);
  weld(pL,pR);
/* after (px,pl,pr) */
  py = NEXT(px);
  weld(px,pl);
  weld(pr,py);
  /* Make brackets of all AT's and VF's */
  for(py=pl;;py=NEXT(py)) {
    headptr h;
    h=REF(py);
    if(TYPE(py) !=TYPEBRAC OR STYPE(h)==HEADBRAC)
       fprintf(stdtrc,"AFTERXS: %p\n",h);
    SETSIM(h,1,HEADBRAC);
    if(py==pr) break;
    }
}

/* Insert element p after element px */
void AFTER1( elemptr px, elemptr p)
{ elemptr py;
  py = NEXT(px);
  weld(px,p);
  weld(p,py);
/*  NEXT(px)=p; PREV(p)=px;
  NEXT(p)=py; PREV(py)=p; */
}

/* 'To copy' means to make new sequence of elements
   which is equal to the original one
   on the free list after element b,
   b being set to the last element of new sequence.
*/
/* Copy the single element p */
LOGICAL copp( elemptr p)
{ movb;
  MKINF(b,p);
  if (NOT ISSIMPL(b)) ++VAL(REF00(b));
  return(TRUE);
}

/* Copy the segment (l,r) */
LOGICAL copvg( elemptr l, elemptr r)
{ elemptr p;
  for (p = l; ; p = NEXT(p)) {
    movb;
    MKINF(b,p);
    if (NOT ISSIMPL(b)) ++VAL(REF00(b));
    if (p==r) break;
    }
  return(TRUE);
}





/* Copy the single cvalue element c (c is virtual) */
LOGICAL copsymb( cvalue c)
{ movb;
  SETDATA(b,c);
  if (NOT IS_SIMPL(CTYPE(c))) ++VAL(CREF00(c));
  return(TRUE);
}

/* make actual cvalue from virtual elemptr */
cvalue adata(elemptr p)
{ cvalue c;
  c=DATA(p);
  if(NOT IS_SIMPL(CTYPE(c))) ++VAL(CREF00(c));
  return(c);
  };

/* Make an actual copy of the ghost value (l,r) */
LOGICAL takeout( elemptr * L, elemptr * R)
{ elemptr p,q,oldb;
  oldb = b;
  for (p = *L, q = *R ; ; p = NEXT(p)) {
    movb;
    MKINF(b,p);
    if (NOT ISSIMPL(b)) ++VAL(REF00(b));
    if (p==q) break;
    }
  *L = NEXT(oldb);
  *R = b;
  b = NEXT(b) ;
  weld(oldb,b);
  b = oldb;
  return(TRUE);
}

LOGICAL rc_imp (char* s)
{
  fprintf(stdtrc,"Error in %s \n",s);
  rf_errmsg(ERRIMP);
  return(FALSE);
}

LOGICAL rc_break(char* s, int n)
{ fprintf(stdtrc,"Break in %s\n",s);
  rf_errmsg(n);
  return(FALSE);
}

LOGICAL CBL()  { bl;  return(TRUE); }
LOGICAL CBR()  { br;  return(TRUE); }
LOGICAL CBLR() { blr; return(TRUE); }
LOGICAL MOVB() { movb; return(TRUE); }

/* Convert any box or object to empty object of type typ */
LOGICAL rf_get_empty(headptr h, int typ)
{    if(NOT ISOBJ(h)) return(FALSE);
    DONE(h);
    TYPE(h) = typ;
    NEW(h);
    return(TRUE);
}

/* Compare two objects for equality */
LOGICAL rf_eqobj(headptr h1, headptr h2) /* -> TRUE/FALSE */
{
  int t;
  if ((t=TYPE(h1))!=TYPE(h2)) return(FALSE);
  if (IS_OBJ(t)) return(h1==h2);
  if (t==HEADBRAC) {
     elemptr p1=NEXT(h1),p2=NEXT(h2);
     for (; p1!=h1 AND p2!=h2; p1=NEXT(p1), p2=NEXT(p2) ) {
        if(NOT EQTERM(p1,p2)) return(FALSE);
        }
     return (p1==h1 AND p2==h2);
     };
  return(FALSE);
  }

