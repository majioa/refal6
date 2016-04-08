/************************************************************************/
/*  R E F A L - 6           Copyright (C) 1992 by Arkady Klimov         */
/*       rfstor.c    - storage control routines                         */
/************************************************************************/
#define STOR_OWN
#include "refint.h"
#include "rfalloc.h"
#include "rftable.h"
#include "channel.h"
#include "contain.h"
#include "rfio.h"
#include <string.h>

#define MKSIMPL(p,n) SETSIM(p,n,TYPESIMPL)

LOGICAL arglong(char* f, int a, long * N)
{ char *pos;
  if ((pos=strchr(f,a)) == NULL) return(FALSE);
  *N = 0L;
  while (isdigit(*++pos)) *N = *N * 10L + (long) (*pos - '0');
  return(TRUE);
}

#ifdef SPLIT

#define  DP sizeof(elemptr)
static LOGICAL rf_getstor(LOGICAL first)
{
 elemptr p,p1;
 long blk,i;
 if (first) {
   if (NOT arglong(flags,'B', &blk)) blk = STORBLK;
   ALL(i,4)
      if ((BM[i]=(unsigned*)halloc(blk,DP)) == NULL) return(FALSE);
   p = NOELEM;
   for (i=blk,p1=p; i; i--,p1+=DP) {
      PREV(p1) = p1-DP;
      NEXT(p1) = p1+DP;
      TYPE(p1) = TYPEVIR;
      }
   p1=p+(blk-1)*DP;
   totstor = 0L;
   totword = 0L;
   p+=DP;
   weld(p1,p);
   Afreestor = p;
   totstor = (long)(blk-1);
   return(TRUE);
   };
 rf_error = ERRSTOR;
 return (FALSE);
 }
extern headptr chkref(headptr h);
#define CHKREF(h)  chkref(h)







#else
static LOGICAL rf_getstor(LOGICAL first)
{  elemptr p,p1;
   long i;
   static long blk;
   if (first) if (NOT arglong(flags,'B', &blk)) blk = STORBLK;
   p = (elemptr) halloc((long)blk, sizeof(elem));
   if (p==NOELEM) {
      rf_error = ERRSTOR;
      return(FALSE);
      }
   for (i=blk,p1=p; i; i--,p1++) {
      PREV(p1) = p1-1;
      NEXT(p1) = p1+1;
      TYPE(p1) = TYPEVIR;
      }
   p1=p+(blk)-1;
   if (first) {
      totstor = 0L;
      totword = 0L;
      weld(p1,p);
      Afreestor = p;
      }
   else {
      weld(PREV(Afreestor),p);
      weld(p1,Afreestor);
      }
   totstor += (long)blk;
   return (TRUE);
   }

#define CHKREF(h) h
#endif

static void headfree(headptr h)
{/*if(MODE(TYPE(h))==CHAIN) {if(NEXT(h)!=h) AFTER(b,NEXT(h),PREV(h));}else */
  DONE(h);
  AFTER1(Zfreestor,h);
  MKSIMPL(h,1);
};
void headptrfree(headptr h) { if(--VAL(h)==0) headfree(h); }
void elemfree(elemptr p) /* Recursive function (via headfree) */
{ elemptr h;
  if (NOT (ISSIMPL(p)))
    if (--VAL(h=CHKREF(REF00(p))) == 0)
      headfree(h);
};
void cvalfree(cvalue c) /* Recursive function (via headfree) */
{ headptr hh;
  if (NOT IS_SIMPL(CTYPE(c))) {
        hh = CHKREF(CREF00(c));
        if (--VAL(hh) == 0)
            headfree(hh);
}       }

void cvalact(cvalue c) /* Reverse to cvalfree */
 { if (NOT IS_SIMPL(CTYPE(c))) ++VAL(CREF00(c)); }

LOGICAL rf_mk_hword (uchar* nm, seglen l, headptr * ah)
       /* result *ah is actual and STARTED */
{
   headptr h;
   int i;
   WDPOS cw;
   ECALL( rf_mk_empty(&h,HEADWORD) AND WDREALLOC(h,l)!=NOADR );
   for (i=0,cw=WDFIRST(h); i<l; i++, cw = WDNEXT(cw)) WDCHAR(cw) = nm[i];
   *ah=h;
   return (TRUE);
}

LOGICAL rf_mk_word (uchar* nm, seglen l, cvalue * aw, LOGICAL simplify)
       /* result *aw is actual */
{
   headptr w;
   unsigned s,i;
   WDPOS cw;
   if(simplify) {
     if(l<4) {
       *aw = CMKSIM(0,TYPESWORD(l));
       ALL(i,l) SWDBODY(*aw)[i] = nm[i];
       return(TRUE);
       };
   /* evaluate hash code s */
     s = 0;
     ALL(i,l) s += nm[i];
     s = (s+(unsigned)l)%HASHWD;
   /* look up for the same word */
     w = hashwd[s];
     while (w != NOHEAD) {
       START(w);
       if (WDLEN(w) == l) {
         for (cw=WDFIRST(w),i=0; i<l; i++, cw=WDNEXT(cw) )
            if (nm[i] != WDCHAR(cw)) break;
         if (i == l) break;
         }
       END(w);
       w = PREV(w);
     } }
   else w=NOHEAD;
 /* make new word or "actual copy" of old one */
   if (w == NOHEAD) {
      if(NOT rf_mk_hword(nm,l,&w)) return(FALSE);
/*
      if( (NOT rf_mk_empty(&w,HEADWORD)) OR  (WDREALLOC(w,l)==NOADR))
                    return(FALSE);
      for ( i=0,cw=WDFIRST(w); i<l; i++, cw = WDNEXT(cw) )
          WDCHAR(cw) = nm[i];
*/
      if(simplify) {  /* include new word into the hashtable */
        PREV(w) = hashwd[s];
        hashwd[s] = w;
        }
      } else VAL(w)++;
   END(w);
   *aw=CMKREF(w,TYPEREF);
   return (TRUE);
}

LOGICAL rf_mk_sword(char* nm, cvalue * ac)
{ return( rf_mk_word((uchar*)nm, strlen(nm), ac, TRUE)); };

LOGICAL newword(char* nm)
{  cvalue c;
   if(NOT rf_mk_word((uchar*)nm,strlen(nm), &c, TRUE)) return(FALSE);
   movb;
   SETDATA(b,c);
   return (TRUE);
}






void fromhashlist(headptr h) /* h is STARTed, before and after */
{ uchar * nm;
  unsigned i,s; seglen l;
  headptr w, * aw ;
/*   START(h); */
  nm = WDFIRST(h);
  l = WDLEN(h);
    /* evaluate hash code s */
  s = 0;  ALL(i,l) s += nm[i];  s = (s+(unsigned)l)%HASHWD;
/*   END(h);   */
   /* look up for the same word */
  aw=&(hashwd[s]) ;
  w = *aw;
  while (w != NOHEAD) { if (w==h) break; aw = &PREV(w); w = *aw ; }
  if(w==NOHEAD) {
         fprintf(stdtrc,"DONE: No word \"");
         ALL(i,l) fprintf(stdtrc,"%c",nm[i]);
         fprintf(stdtrc,"\" in hash list [%d]",s);
         rf_exit(30);
         };
  *aw = PREV(w);
 }

void tohashlist(headptr* ah)
{ uchar * nm, *cw;
  unsigned i,s; seglen l;
  headptr h,w;
  h = *ah;
  START(h);
  nm = WDFIRST(h);
  l = WDLEN(h);
    /* evaluate hash code s */
  s = 0;  ALL(i,l) s += nm[i];  s = (s+(unsigned)l)%HASHWD;
  /* look up for the same word */
  w = hashwd[s];
  while (w != NOHEAD) {
       START(w);
       if (WDLEN(w) == l) {
         for (cw=WDFIRST(w),i=0; i<l; i++, cw=WDNEXT(cw) )
            if (nm[i] != WDCHAR(cw)) break;
         if (i == l) break;
         }
       END(w);
       w = PREV(w);
       }
  END(h);
  if(w==NOHEAD) /* insert h into the hashlist */
     { PREV(h) = hashwd[s]; hashwd[s] = h; }
   else {  /* use w instead of h */
     TYPE(h) = HEADSTR;
     headptrfree(h);
     *ah = w;
     VAL(w)++;
 }     };






int initstor(int argc, char** argv)
{  int i;
   long stsize,par;
 /* init flags */
   flags[0] = '\0';
   ALL(i,argc) if(*argv[i]=='-') strcat(flags,argv[i]+1);
 /* initialize b as 0 to denote that store is not initialized yet */
   b=0;
 /* init heap memory */
   rf_mem_init();
 /* init output buffer */
   if (NOT arglong(flags,'W', &par)) par = MBUF; LBUF=par;
   if ((Buf = (char*) halloc(LBUF, 1)) == NULL) rf_err(ERCSTOR);
 /* init stack */
   if (NOT arglong(flags,'S', &stsize)) stsize = STSIZE;
   if ((stk = (elemptr*) halloc(stsize, sizeof(elemptr))) == NULL)
         rf_err(ERCSTOR);
   spe = stk;
   spemax = stk+(stsize-100);

   if (NOT rf_getstor(TRUE)) rf_err(ERCSTOR);
   TYPE(Afreestor) = HEADSTOR;
   VAL(Afreestor) = 0;
   b = Afreestor;
   ALL(i,HASHWD) hashwd[i] = NOHEAD;
   if(NOT class_init()) rf_err(ERCINIT);
   if(NOT INIWORDS(NWDS,COMWORD,WORDS)) rf_err(ERCINIT);
   if (NOT rf_mk_empty(&blbr,HEADBRAC)) rf_err(ERCINIT);
   if ( NOT rf_mk_hbox(&sysid) OR
    NOT tbox_init(sysid)   OR
    NOT rf_tab_link(sysid,COMWORD[5],CMKREF(sysid,TYPEREF))
    ) rf_err(ERCINIT);

   if ( NOT rf_mk_hboxn(COMWORD[_SYSTABLE], &systable) OR
    NOT tbox_init(systable)                OR
    NOT rf_mk_hboxn(COMWORD[_SYSARG],  &sysarg)
    ) rf_err(ERCINIT);

/*  box SYSTABLE := TABLE SYSTABLE *SYSTABLE SYSARG *SYSARG SYSID *SYSID */
 if ( NOT rf_tab_link(systable,COMWORD[_SYSTABLE],CMKREF(systable,TYPEREF))
     OR NOT rf_tab_link(systable,COMWORD[  _SYSARG],CMKREF(sysarg,TYPEREF))
     OR NOT rf_tab_link(systable,COMWORD[   _SYSID],CMKREF(sysid,TYPEREF))
    ) rf_err(ERCINIT);

/* Make list of arguments */
   if(NOT rf_get_empty(sysarg,HEADBOX)) rf_err(ERCINIT);
   ALL(i,argc)
      if (*argv[i] != '-')
           {bl; rf_mkstr((uchar*)(argv[i]), strlen(argv[i])); br;}
   if (b != Afreestor) AFTERX(sysarg,NEXT(Afreestor),b);
   b=Afreestor;

   lbrac = NOELEM;
   rf_error = ERRNO;
   rf_exitcd = -1;
   exename = argv[0];
   ininame = (argc>1 AND (argv[1][0]!='-') ? argv[1] : "default");
   return(TRUE);
}









LOGICAL rf_movb()
{  elemptr h;
   if (b == Afreestor) {
      b = PREV(b);
      if (NOT rf_getstor(FALSE)) return (FALSE);
      b = NEXT(b);
      return (TRUE);
      }
   h=REF00(b);
   if(ISBRAC(b) AND TYPE(h)!=HEADBRAC) {
     SETSIM(h,1,HEADBRAC);
     }
   if (--VAL(h) == 0) headfree(h);
   MKSIMPL(b,2);
   return (TRUE);
}

LOGICAL rf_movbx(elemptr * y)
{
   elemptr h,savb;
   if (*y == Afreestor) {
      *y = PREV(*y);
      if (NOT rf_getstor(FALSE)) return (FALSE);
      *y = NEXT(*y);
      return (TRUE);
      }
   h=REF00(*y);
   if(ISBRAC(*y) AND TYPE(h)!=HEADBRAC) {
     fprintf(stdtrc,"Free AT or VF: %u : %d, step=%lu\n",h,STYPE(h),step);
     SETSIM(h,1,HEADBRAC);
     }
   if (--VAL(h) == 0) { savb=b; b=*y; headfree(h); b=savb;}
   MKSIMPL(*y,3);
   return (TRUE);
}

LOGICAL testmem (long * al)
{ /* b==0: store is not initialized yet */
  if (b==0) { *al = 0L; return(FALSE); }
  for( *al = 0L ; ; ) {
    if(b!=PREV(NEXT(b))) {
       fprintf(stdtrc,"Testmem: invalid back link: %u <-> %u \n",b,NEXT(b));
       break;
       }
      if (NEXT(b)==Afreestor) return(TRUE);
      movb;
      (*al)++;
}      }

long int memleft()
{ long N=0L;
  elemptr savb;
  int saverr;
  saverr=rf_error;
  savb = b;
  testmem(&N);
  b = savb;
  rf_error=saverr;
  return(N);
}







void countstor(long* t, long* r, long* u)
{  elemptr p;
   long virgin=0;
   for(p=NEXT(b); p!=Afreestor; p=NEXT(p))
      if(TYPE(p)==TYPEVIR) virgin++;
   *t = totstor;
   *r = memleft();
   *u = totstor-virgin;
};

void rf_mem_stat(FILE* f)
{
 long tot, rest, used;
 countstor(&tot,&rest,&used);
 fprintf(f,"STORAGE elements: total %ld, rest %ld, maxused %ld\n",
               tot, rest, used);
}



void printallwords(FILE* f)
{  int i,n=0;
   headptr h;
   ALL(i, HASHWD)
      if (hashwd[i]!=NOHEAD) {
         fprintf(stdtrc,"\n:%d:%u: ",i,hashwd[i]);
         for (h = hashwd[i]; h!=NOHEAD; h=PREV(h)) {
                        if(TYPE(h)!=HEADWORD)
                { fprintf(stdtrc,"[%u:%u]",VAL(h),TYPE(h)); break; }
            else outwdh(f,fputc,h,TRUE);
            putc(' ',f);
            n++;
         }  }
   fprintf(stdtrc,"\n   total: %d words\n",n);
   }
