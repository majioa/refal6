/******************************************************************/
/*                                                                */
/*   R E F A L - 6      Copyright (C) IPS AN 1991                 */
/*                                                                */
/*  REFAL - C COMPILER CODE GENERATOR MACROS'S        refgen.h    */
/*                                                                */
/******************************************************************/
/*                                                                */
/*      Mnemonics:  S,T,U - symbol, term,                         */
/*                  E,F - expression,                             */
/*                  V,W - nonempty expression                     */

#ifndef __REFGEN_H
#define __REFGEN_H

#include "refcom.h"

/*          STACK OPERATIONS                                      */
#define  PUSHE(E)  pushe((E).L,(E).R)
#define  PUSHAE(AE) pushae((AE).L,(AE).R)
#define  POPE(E)  pope((E).L,(E).R)
#define  POPAE(AE) popae((AE).L,(AE).R)
#define  PUSHT(T)  pusht((T).L)
#define  PUSHAT(AT) pushat((AT).L)
#define  POPT(T)  popt((T).L)
#define  POPAT(AT) popat((AT).L)

#define  PUSHAAE(AE) pushae(*((AE).L),*((AE).R))

/*        CONTRACTIONS                                            */
#define  setempty(E)  (E).L=NOELEM;
#define  empty(E)  ((E).L==NOELEM)
#define  one(E)  ((E).L==(E).R)
#define  first(E,T)  (T).L=(E).L;
#define  last(E,T)  (T).R=(E).R;
#define  butfirst(E)  if(one(E)) setempty(E) \
                       else (E).L = NEXT((E).L);
#define  butfirst2(E,F)  if(one(E)) setempty(F) \
                          else {(F).L = NEXT((E).L); (F).R = (E).R;}
#define  butlast(E)  if(one(E)) setempty(E) \
                       else (E).R = PREV((E).R);
#define  butlast2(E,F)  if(one(E)) setempty(F) \
                          else {(F).R = PREV((E).R); (F).L = (E).L;}
#define  cont(T,E)  {(E).R=REF(T.L);\
              if(((E).L=NEXT((E).R))==(E).R) setempty(E)\
                      else (E).R=PREV((E).R);}
#define  eqSS(S,T)  EQSYM((S).L,T.L)
#define  eqSC(S,C)  EQDATC((S).L,C)
#define  eqCS(C,S)  EQDATC((S).L,C)
#define  eqSL(S,N)  (ISLNUMB((S).L) AND rf_eqlong(N,S) )
#define  eqLS(N,S)  (ISLNUMB((S).L) AND rf_eqlong(N,S) )
/* right end of the term pointed by (elemptr)p: */
#define  RE(p) p






/*  APPENDING via free storage                                      */
/*            by copying (MUL) or by transplanting (MOV)            */

#define  initnew  oldb=b;
#define  endnew  nextb=NEXT(b);
#define  endmov  { weld(oldb,nextb); b=oldb; }
#define  savnew(E) if(NEWEMP) setempty(E) else {(E).L=NEWL; (E).R=NEWR;}

#define  FREE(L,R) after(b,L,R)
#define  TPLV(L,R) {after(b,L,R) b=R;}
#define  NEWEMP b==oldb
#define  NEWL  NEXT(oldb)
#define  NEWR  b

EXTCOM  LOGICAL copp(elemptr p);
EXTCOM  LOGICAL copvg(elemptr L, elemptr R);
EXTCOM  LOGICAL takeout(elemptr *L, elemptr *R);

#define  COPP(p)  if(NOT copp(p)) return(FALSE);

#define  COPTN(T)  COPP(T.L)
#define  COPSN(T)  COPP(T.L)
#define  COPVN(V)  { if(NOT copvg(V.L,V.R)) return(FALSE); }
#define  COPEN(E)  { if(NOT empty(E)) COPVN(E) }

#define  COPTG(T)  COPP(T.L)
#define  COPSG(T)  COPP(T.L)
#define  COPVG(V)  { if(NOT copvg(V.L,V.R)) return(FALSE); }
#define  COPEG(E)  { if (NOT empty(E)) COPVG(E) }

#define  TAKEOUT(L,R)  { if(NOT takeout(&L,&R)) return(FALSE); }
/* COPY symbol constant or variable given as cvalue */
#define  pconst  cvalue
#define  COPSYMB(c) { if(NOT copsymb(c)) return(FALSE); }
#define  COPCHAR(C) {movb; SETDATA(b,C);}
#define  COPNUMB(N) COPCHAR(N)
#define  COPLONG(L) COPSYMB(L)
#define  COPWORD(W) COPSYMB(W)
#define  COPBOX(B)  { movb; SETDATA(b,B); ++VAL(REF00(b)); }
#define  COPEXTBOX(B)  COPBOX(B)
#define  COPALFA(A) rf_mkstr(A,strlen(A));
#define  COPREF(PTR) { movb; SETREF(b,PTR,TYPEREF); VAL(PTR)++; }


















/*          MISCELANEOUS                                        */
#define  CALL(f)  { if(NOT f()) return(FALSE); }
#define  SCALL(f) { if(NOT f()) return(FALSE); }
#define  STOP return(TRUE);
#define  ERROR(N) { rf_error=N; return(FALSE); }

#define  CIMP(s) return(rc_imp(s));
#define  GOTO(f) return(GO_TO(f));
#define  RFUNC LOGICAL
#define  RFERCINIT rf_err(ERCINIT);
#define  CHKSTK(s,m) {if(sp+m>spmax)return(rc_break(s,ERRSTKO));}
#define  MAXSTK 0
#define  LOC static

#define CVALFREE(c) if(NOT IS_SIMPL(CTYPE(c))) headptrfree(CREF00(c));
#define CVALACT(c)  if(NOT IS_SIMPL(CTYPE(c))) ++VAL(CREF00(c));

#ifdef TRACE
#define  PRX(L,R)  rf_out(stdout,L,R);
#define  PRE(E)  PRX((E).L,(E).R)
#define  PRT(T)  PRX(T.L,T.R)
#define  PRI(I)  PRX((elemptr)&I,(elemptr)&I)
#define  PRAE(AE)  PRX(*(AE.L),*(AE.R))
#define  PRAT(AT)  PRX(*(AT.L),*(AT.R))
#define  PRAI(AI)  PRX((elemptr)AI,(elemptr)AI)
#define  CALLP(n,f) {fprintf(stdtrc,"\ncall%d",n); CALL(f); fprintf(stdtrc,"\nexit%d",n);}
#else
#define  PRE(E)
#define  PRT(T)
#define  PRI(I)
#define  PRAE(AE)
#define  PRAT(AT)
#define  PRAI(AI)
#define  CALLP(n,f) CALL(f)
#endif

extern LOGICAL rc_imp(char * s);
extern LOGICAL rc_ovstk(char * s);

/************************************   Entries from refini.c  */
LOGICAL  iniextbox(char * string, headptr * ah);
LOGICAL  inibox(char * string, headptr * ah);
LOGICAL  INIWORDS(int, cvalue*, char**);
LOGICAL  INILONGS(int, cvalue*, long*);
LOGICAL  INIBOXS (int, cvalue*, char**);
LOGICAL  INIEXTBOXS(int, cvalue*, char**);

















/*  Weld two elements together */
void WELD(elemptr p1, elemptr p2);

/*  Exchange next elements of the two given elements */
void CROSS(elemptr p1, elemptr p2);

/* Insert segment (pl,pr) between elements px and py */
void BETWEEN(elemptr px, elemptr py, elemptr pl, elemptr pr);

/* Insert segment (pl,pr) after element px */
void AFTER( elemptr px, elemptr pl, elemptr pr);

/* Take segment (pl,pr) out of its place and insert it after px */
void AFTERX( elemptr px, elemptr pl, elemptr pr);

/* Move list of AT's and VF's making them usual (.)'s */
void AFTERXS( elemptr px, elemptr pl, elemptr pr);

/* Insert element p after element px */
void AFTER1( elemptr px, elemptr p);

/* Move nonempty list to free storage, converting ATs & VFs to ()s */
void FREEX(int n, elemptr l, elemptr r); /* the result is always TRUE */

/* 'To copy' means to make new sequence of elements
   which is equal to the original one
   on the free list after element b,
   b being set to the last element of new sequence. */
/* Copy the single element p */
LOGICAL copp( elemptr p);

/* Copy the single cvalue element c (c is virtual) */
LOGICAL copsymb( cvalue c);

/* Move the single cvalue element c (c is actual) */
LOGICAL movsymb( cvalue c);

/* make actual cvalue from virtual elemptr */
cvalue adata(elemptr p);

/* Copy the segment (l,r) */
LOGICAL copvg( elemptr l, elemptr r);

/* Copy the single cvalue element c */
LOGICAL copsymb( cvalue c);

/* make actual cvalue from virtual elemptr */
cvalue adata(elemptr p);

/* Make an actual copy of the ghost value (l,r) */
LOGICAL takeout( elemptr * L, elemptr * R);

LOGICAL rc_imp (char* s);

LOGICAL rc_break(char* s, int n);








LOGICAL CBL();  /* { bl;  return(TRUE); }  */
LOGICAL CBR();  /* { br;  return(TRUE); }  */
LOGICAL CBLR(); /* { blr; return(TRUE); }  */
LOGICAL MOVB(); /* { movb; return(TRUE); } */

/* Convert any box or object to empty object of type typ */
LOGICAL rf_get_empty(elemptr h, int typ);

/* Compare two objects for equality */
LOGICAL rf_eqobj(headptr h1, headptr h2); /* -> TRUE/FALSE */

/*  Check that object h is of the class cls, otherwise
    if object's EMPTY convert it to empty object of type typ
LOGICAL rf_defobj(elemptr h, int cls, int typ);
*/

/* Test type of p and get long integer to n */
LOGICAL rf_getnumb ( elemptr p, long * n);
/************************************   Entries from rbcont.c  */
/* Make a valid copy for modification */
LOGICAL rf_mk_copy (cvalue c, headptr* ah, int mincount, LOGICAL enablobj);
/* c is virtual; if returned TRUE result *ah is actual and STARTED */
/* mincount is ref. count. for c when it is allowed to be modified */

LOGICAL rf_norm_cont(cvalue c, headptr *ah, int mincont);

void rf_simp_cont(headptr h, cvalue * ac);

LOGICAL rf_get(cvalue *av, cvalue c, int mincount);

LOGICAL rf_set(headptr h, cvalue c, int mincount);

/************************************   Entries from rfserv.c  */
LOGICAL rf_eqlongc (elemptr p, cvalue c);
LOGICAL rf_eqlong (elemptr p1, elemptr p2);
LOGICAL rf_mkstr (uchar* s,int l);
int ra_cmpl(headptr h1, headptr h2);  /* сравнение двух длинных чисел */
int ra_sign(headptr h);               /* знак длинного числа: 0 или -1 */
LOGICAL rf_cmp (elemptr p1, elemptr p2, elemptr p3, elemptr p4);

/* make box head given a name */
LOGICAL rf_mk_hboxn (cvalue name, headptr * ah);
         /* name is ghost, result *ah is actual */

/* make box head without name */
LOGICAL rf_mk_hbox(headptr * ah);

/* make new object headder */
LOGICAL  rf_mk_empty
   (headptr * ah,     /* result *ah is actual unless result is FALSE */
    int typ           /* type typ must accept message _NEW */
    );
#ifdef DEBUG
LOGICAL rf_chck(elemptr p1, elemptr p2);
LOGICAL rf_chckf(elemptr p1, elemptr p2);
LOGICAL rf_chckf(elemptr p1, elemptr p2);
#endif
void handint(int n);
void rf_exit(int n);
void rf_exitmsg(int n, char * s);
LOGICAL rf_mk_cnumb(long n, cvalue * ac, LOGICAL simplify);
LOGICAL rf_mknumb(long n);
LOGICAL rf_mk_numb(ushort * an, seglen l, cvalue * ac, LOGICAL simplify);
LOGICAL rf_mk_hnumb(ushort* an, seglen l, headptr *ah);
LOGICAL rf_mk_hword (uchar* nm, seglen l, headptr * ah); /* in rfstor.c */
void rf_out_stat(FILE* f) ;

void rf_errmsgs(char * s);
void rf_errmsg(int n);
void rf_err(int n);

LOGICAL rf_inist();
LOGICAL rf_termst();

#endif
