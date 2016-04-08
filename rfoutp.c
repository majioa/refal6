/***********************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
       rfoutp.c      - output to channel
***********************************************************************/

#include "refcom.h"
#include "fileio.h"
#include "rftable.h"
#include "rfio.h"
#include "rfalloc.h"
#include "rbarm.h"
#include <string.h>

/* macro to insert space after number or word */
/* #define PUTSPACE(f) ptc(' ',f); */
#define PUTSPACE(f)
#define SPEOFF 0  /* Earlier it was 6 [x4] */

static char * errmsg[] = {
   "",
   "Unexpected FAIL",
   "No storage",
   "Invalid function",
   "Break !!!",
   "Break on step",
   "Normal end",
   "Fail of main function",
   "End of computation",
   "Syntax error in input",
   "SYSTEM: Invalid parenthesis type in output",
   "SYSTEM: Invalid element type in output",
   "SYSTEM: Initialisation error",
   "SYSTEM: Invalid data structures",
   "SYSTEM: Invalid back link",
   "SYSTEM: Element table overflow",
   "SYSTEM: Call stack overflow",
   "No storage for words"
   "Illegal access function"
   };

void rf_errmsgs(char * s)
{ fprintf(stdtrc,"*** %s\n",s); }

void rf_errmsg(int n)
{ if (n != 0) rf_errmsgs(errmsg[n]); };

void rf_err(int n)
{ rf_errmsg(n); rf_termst(); exit(n); }

headptr mkh_fileio(FILE* f)
{ headptr h;
  if (NOT rf_mkempty(&h)) return(NOHEAD);
  mk_fileio(h,f);
  return(h);
  }

static LOGICAL IoInit = FALSE;


void initio()
{  int i;
   for (i=0; i<MAXFILES; i++) chan[i] = NOHEAD;
   chan[0] = mkh_fileio(stdin);
   chan[1] = mkh_fileio(stdout);
   chan[2] = mkh_fileio(stdtrc);
   IoInit = TRUE;
}

void rf_close(int n)
{  if (n>=0 AND n < MAXFILES AND chan[n]!=NOHEAD) {
      headptrfree(chan[n]);
      chan[n] = NOHEAD;
}     }

void endio()
{  int i;
   if (IoInit) for (i=0; i<MAXFILES; i++) rf_close(i);
}

/* Open file appending current directory by the directory
   which the RF.EXE was taken from
   (Note: mod is char, not char*) */
FILE * rf_open(char * fn, char * mod)
{  FILE * afile;
   int i;
   char fulname[80];
   extern char *exename;
   strcpy(fulname,fn);
   afile = fopen(fulname,mod);
   if (afile == NULL) {
      if(strchr(fulname,'\\')==NULL AND strchr(fulname,'/')==NULL) {
      /* try looking for the file in the directory
           which the EXE-file was taken from  */
         strcpy(fulname,exename);
       /* cut off the filename after the last symbol '\' or '/'*/
         for (i=strlen(fulname); i>0; i--)
           if (fulname[i-1]=='\\' OR fulname[i-1]=='/') break;
         fulname[i] = 0;
         strcat(fulname,fn);
     afile = fopen(fulname,mod);
      }     }
   return(afile);
}

/* Test whether word text c is not standard */
static LOGICAL rfspec(WDPOS c, int l)
{  int i;
   if (NOT (isalpha(WDCHAR(c)))) return(TRUE);
   for (i = l; i--; c = WDNEXT(c))
      if (NOT (ISWORDS(WDCHAR(c)))) return(TRUE);
   return (FALSE);
}

static void rfpts(char *s, FILE * f, Tputc ptc)
{ int i;
  for (i=0; s[i] !=0; i++) ptc(s[i],f);
};


static void rfptc(char c, FILE * f, Tputc ptc)
{
  if(c=='\r') rfpts ("\\r",f,ptc);
  else {
    if((c)=='\\' OR (c)=='\'' OR (c)=='\"') ptc('\\',f);
    ptc(c,f);
}   }

static LOGICAL outnumb(FILE * f, Tputc ptc, headptr h)
{
   char buf[12];
   MPOS A,C,W; ulong* D;
   adr adC,adW,adD;
   seglen lA,lC,lD,mlD;
   int i;
   START(h);
   A=MFIRST(h);
   lA=MLEN(h);

   /* abs(A) -> C */
   lC=lA+1;
   ECALL(ALLOC(adC,MSIZE(lC))!=NOADR);
   C=(MPOS)SEGINF(adC);
   if(A[lA-1]!=0) {
      ra_minus(A,lA,C,lC,&lC);
      ptc('-',f);
      }
   else { ALL(i,lA) C[i]=A[i]; lC=lA; };

   ECALL(ALLOC(adW,MSIZE(lC))!=NOADR);
   W=(MPOS)SEGINF(adW);

   mlD=lC+lC/10+1;
   ECALL(ALLOC(adD,MSIZE(mlD))!=NOADR);
   D=(ulong*)SEGINF(adD);

   ra_cvd(C,lC,W,D,mlD,&lD);

   ALL(i,lD) {
      sprintf(buf, (i==0 ? "%lu\0" : "%09lu\0"), D[lD-i-1]);
      rfpts(buf,f,ptc);
      }

   REALLOC(adC,0);
   REALLOC(adW,0);
   REALLOC(adD,0);
   return(TRUE);
   }

static void outreal(FILE * f, Tputc ptc, headptr h)
{
   char buf[20];
   real r;
   head2real(h,r);
   sprintf(buf, "%E\0" , r );
   rfpts(buf,f,ptc);
   };



static void outwds(FILE* f, Tputc ptc, uchar* c, int l, LOGICAL isexp)
{  int i;
   if (isexp AND rfspec(c,l)) {
      ptc('\"',f);
      ALL(i,l) rfptc(c[i],f,ptc);
      ptc('\"',f);
      }
   else ALL(i,l) ptc(c[i],f);
   }

void outwdh (FILE* f, Tputc ptc, headptr h, LOGICAL isexp)
{  seglen ln;
   START(h);
   ln = WDLEN(h);
/*  if(ln>100) { fprintf(stdtrc,"#%u#",ln); ln=10; } */
   outwds(f,ptc,WDFIRST(h),ln,isexp);
   END(h);
   }
extern void outref(FILE* f, Tputc ptc, headptr hx,  /* object headder */
         LOGICAL isexp, headptr table);

void outsymb( FILE* f, Tputc ptc, cvalue c,
         LOGICAL isexp, headptr table)
{ int t;
  elemptr h;
  char buf[20];
  t = CTYPE(c);
  if (IS_CHAR(t))
        if(isexp) {
          ptc('\'',f);
          rfptc(CVAL(c),f,ptc);
          ptc('\'',f);
          }
        else ptc(CVAL(c),f);
  else if (IS_SNUMB(t)) {
       sprintf(buf,"%d\0",(int)(short)CVAL(c));
       rfpts(buf,f,ptc);
       }
  else if (IS_REAL(t)) {
       float x;
       cvalue2float(c,x);
       sprintf(buf,"%e\0",x);
       rfpts(buf,f,ptc);
       }
  else if (IS_SWORD(t)) outwds(f,ptc,SWDBODY(c),SWD_LEN(t),isexp);
  else if (IS_REF(t))
    switch (TYPE(h=CREF00(c))) {
      case HD(MASK,CON)  : outnumb(f,ptc,h); break;
      case HD(REAL,CON)  : outreal(f,ptc,h); break;
      case HD(STRING,CON): outwdh(f,ptc,h,isexp); break;
      default:
         if (isexp) ptc('*',f);
         outref(f,ptc,h,isexp,table);
         break;
      }
  else if(t==TYPEVIR) ;
  else if(t==TYPESIMPL) { ptc('~',f); ptc('0'+CVAL(c),f); }



  else if(t==TYPEBRAC) { h=CREF00(c);
      ptc('(',f);
      ptc('\"',f);
      if(NEXT(h)!=h) rf_outm (f, ptc, NEXT(h), PREV(h), FALSE,table);
      ptc('\"',f);
      ptc(')',f);
      }
  else { fprintf(stdtrc,"\nCvalue(%d,%d) ",CVAL(c),CTYPE(c)); rf_err(ERCSYSO);}
}

void outref(FILE* f, Tputc ptc,
                  headptr hx,  /* object headder */
                  LOGICAL isexp, headptr table)
{  char buf[12];
   cvalue name;
   if(table!=NOTABLE AND
      rf_tab_nam(table,'O',CMKREF(hx,TYPEREF),&name) AND
      name != NOCVALUE ) {
         outsymb(f,ptc,name,isexp,NOTABLE);
         cvalfree(name);
         }
   else
      if (NOT isexp AND (TYPE(hx) == HEADSTR))
          /* put out contents of the master string */
          outwdh(f,ptc,hx,FALSE);
      else {  /* print pointer as number */
         sprintf(buf,"X$%lx\0",(long)hx);
         rfpts(buf,f,ptc);
}        }

/* Buffered output for pretty-print (see also rfstor.h) */
static int LastEnd, LastEndDepth, StartDepth, Pos;
static FILE* Ef;
static Tputc Eptc;

static void bBreakLine(int end, int pos, int depth)
{  int i,j;
   ALL(i,end) Eptc(Buf[i],Ef);
   Eptc('\n',Ef);
   StartDepth=depth;
   ALL(i,StartDepth) Buf[i] = ' ';
   for(i=StartDepth, j=end; j<pos; i++,j++) Buf[i]=Buf[j];
   Pos=i;
   LastEnd=0;
}

static int bputc(int c, uchar* dummy)
{ int i;
  if (Pos==LBUF) {
    if (LastEnd==0) { /* Output all and make pos>Mbuf */
       ALL(i,Pos) Eptc(Buf[i],Ef);
       Pos = LBUF+1;
       }
    else bBreakLine(LastEnd,Pos,LastEndDepth);
    }
  if (Pos<LBUF) Buf[Pos++]=c; else Eptc(c,Ef);
  return c;
  };


static void bStartTerm(int Depth)
{
  if(Depth>0) {
    if(Pos>LBUF) { if(Depth<LBUF) bBreakLine(0,0,Depth); }
    else { if (Depth<StartDepth) bBreakLine(Pos,Pos,Depth); }
}   }

static void bEndTerm(int Depth)
{
  if(LastEnd==0 OR Depth<=LastEndDepth OR
      LastEnd<Pos-LastEnd AND LastEnd<LBUF-Pos)
     if(Depth>0) { LastEnd=Pos; LastEndDepth=Depth; }
  }

static void bStart(FILE* f, Tputc ptc)
{
   LastEnd=StartDepth=Pos=0;
   Ef = f; Eptc = ptc;
   };

static void bEnd()
{ int i;
  if(Pos<=LBUF) ALL(i,Pos) Eptc(Buf[i],Ef);
  };

void rf_outm (FILE * f, Tputc ptc,
              elemptr p1, elemptr p2,
              LOGICAL isexp, headptr table)
{  elemptr * savspe;
   headptr h;
   int Depth=0;
   spe = savspe = spe+SPEOFF;
   if (p1 == NOELEM) {
      spe = savspe-SPEOFF;
      return;
      }
   if(isexp) { bStart(f,ptc); ptc = (Tputc) bputc; }
   for (;;) {
    bStartTerm(Depth);
    if (BADPTR(p1)) {rf_err(ERCSYSO);}
    else if (ISBRAC(p1)) {
      h = REF(p1);
      if (PREV(h) == h) {
        if (TYPE(h) == HEADBRAC) rfpts("()",f,ptc);
        else if ((TYPE(h) & ACTSEL) == HEADVF) rfpts("[]",f,ptc);
        else if ((TYPE(h) & ACTSEL) == HEADACT) rfpts("<>",f,ptc);
        else rf_err(ERCSYSB);
        }
      else if(spe > spemax) {
          rfpts(" *** NOT ENOUGH STACK SPACE FOR OUTPUT *** ",f,ptc);
          }









      else {
        pushe(p1,p2); Depth++;
        p1 = REF(p1);
        if (TYPE(p1) == HEADBRAC) ptc('(',f);
        else if ((TYPE(p1) & ACTSEL) == HEADVF) ptc('[',f);
        else if ((TYPE(p1) & ACTSEL) == HEADACT) ptc('<',f);
        else rf_err(ERCSYSB);
        p2 = PREV(p1);
        if (NEXT(p2) != p1) rf_err(ERCLINK);
        if (PREV(NEXT(p1)) != p1) rf_err(ERCLINK);
        p1 = NEXT(p1);
        continue;
    }    }
    else if (ISCHAR(p1))
        if(isexp) {
          ptc('\'',f);
          for (;;) {
             rfptc(VAL(p1),f,ptc);
             if ((p1 == p2) OR (NOT (ISCHAR(NEXT(p1))))) break;
             p1 = NEXT(p1);
             }
          ptc('\'',f);
          }
        else ptc(VAL(p1),f);
    else outsymb(f,ptc,DATA(p1),isexp,table);
    while (p1 == p2) {
        if (isexp) bEndTerm(Depth);
         if (savspe == spe) {
            spe = savspe-SPEOFF;
                if (isexp) bEnd();
                return;
                }
        pope(p1,p2); Depth--;
        if (TYPE(REF(p1)) == HEADBRAC) ptc(')',f);
        else if ((TYPE(REF(p1)) & ACTSEL) == HEADVF) ptc(']',f);
        else if ((TYPE(REF(p1)) & ACTSEL) == HEADACT) ptc('>',f);
        else rf_err(ERCSYSB);
        }
    if (isexp) { ptc(' ',f); bEndTerm(Depth); }
    if (PREV(NEXT(p1)) != p1) rf_err(ERCLINK);
    p1 = NEXT(p1);
}     }

void rf_outc (headptr cp,
     elemptr p1, elemptr p2,
     LOGICAL isexp, headptr table)
{ Tputc ptc;
  FILE * f;
  ptc = (Tputc) IFUNC(cp,_PUTC); f = (FILE*) THIS;
  rf_outm (f, ptc, p1,p2,isexp,table);
  }
