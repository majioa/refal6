/************************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
       rfgo.c     - RASL interpreter CALLER
************************************************************************/

#include <stdio.h>

#define PRX(s,l,r) { printlv(""); fprintf(stdtrc,"%s: ",s); rf_outs(l,r); fprintf(stdtrc,"\n"); }
#define PRH(s,h)
/* { headptr hh; hh=h;\
    if(NEXT(hh)!=hh) PRX(s,NEXT(hh),PREV(hh))\
    else { printlv(""); fprintf(stdtrc,"%s is empty\n",s);} } */

#define PP(n)
/* fprintf(stdtrc,"point %d\n",n); */

#define EXTINT

#include "refcom.h"
#include "rfstor.h"
#include "refint.h"
#include "rfalloc.h"
#include "rftable.h"
#include "contain.h"
#include "rfio.h"
#include "fileio.h"

extern char * blttxt[];
extern int    bltcnt;

FILE *stdtrc = NULL;

static void init_debugout(void) __attribute__((constructor));
static void init_debugout(void)
{
    stdtrc = stderr;
}

/* Create a list of builtin functions on the free memory list:
        CARD *CARD LESS *LESS ... */
static LOGICAL initblt()
{  int i;
   tail t;
   headptr h;
   Trepn repn;
   addr thi;
   for (i=0; i< bltcnt; i++) {
    if(NOT iniextbox(blttxt[i],&h))  return(FALSE);
    if(NOT rf_get_empty(h,HEADFUNC)) return(FALSE);
    START(h);
    EVAL(h,_SETPOS,Tsetpos,(THIS, &t, 0));
    repn = (Trepn)IFUNC(h,_REPN); thi = THIS;
     INS1(BUILTIN,i);
     INS0(EST);
    END(h);
    if(--VAL(h)!=2) return(FALSE); /* check: h is new and actual */
    }
  return(TRUE);
}

/* Print level-prefixed message */
void printlv(char* s)
{    int i;
    ALL(i,level) fprintf(stdtrc,"^");
    fprintf(stdtrc,"%s",s);
    };








#define MAXNAME 60
static char ini_name[MAXNAME+4];

static LOGICAL rfopi(char** anamelist, headptr h) {
   FILE * f;
   int l;
   ALL(l,MAXNAME) {  /* take first name until "+" */
      char s = **anamelist;
      if(s!=0) (*anamelist)++;
      if(s=='+' OR l==MAXNAME) s=0;
      ini_name[l]=s;
      if(s==0) break;
      }
   if (strchr(ini_name,'.')==NULL) strcat(ini_name, ".rex");
   f = rf_open(ini_name,"r");
   if (f == NULL) {
      fprintf(stdtrc, "Unable to open %s\n",ini_name);
      return(FALSE);
      }
   if(NOT rf_getempty(h)) return(FALSE);
   mk_fileio(h,f);
   return(TRUE);
}

void rf_trace1(elemptr T)
{  if (NOT tracefl  OR step < trstep) return;
   trstep = 0;
   printlv("STEP: ");
   fprintf(stdtrc,"%ld\n",step);
   printlv("AT: ");
   rf_oute(T,T);
   fprintf(stdtrc,"\n");
}

void rf_trace2(elemptr Lx, elemptr R)
{  if (NOT tracefl OR step < trstep) return;
   trstep = 0;
   printlv("RS: ");
   if (R != Lx) rf_oute(NEXT(Lx),R);
   fprintf(stdtrc,"\n");
}

static LOGICAL inisysinfo()
{   headptr h;
    tail t;
    int i;
    cvalue W[3];
    if(NOT iniextbox("SYSINFO",&h))  return(FALSE);
    if(NOT rf_get_empty(h,HEADVECT)) return(FALSE);
    if(NOT rf_mk_sword(INFO_M,W)) return(FALSE);
    if(NOT rf_mk_sword(INFO_E,W+1)) return(FALSE);
    if(NOT rf_mk_sword(INFO_C,W+2)) return(FALSE);
    START(h);
    EVAL(h,_SETPOS,Tsetpos,(THIS, &t, 0));
    ALL(i,3) if (NOT EVAL(h,_REPN,Trepn,(THIS, &t, W[i]))) return(FALSE);
    END(h);
    return(TRUE);
    }


static void rf_cnt_start()
{  int i;
   account = NULL;
#ifdef DEBUG
   dbgfl   = arglong(flags,'d',&trstep);
   if (dbgfl) tracefl = TRUE;
   if (strchr(flags,'c') != NULL) {
      account = (long *) halloc(64,sizeof(long *));
      for (i=0; i<64; i++) account[i] = 0;
      }
   if (tracefl OR (account != NULL)) testfl = TRUE;
   else  testfl = FALSE;
#endif
}

static void rf_cnt_end()
{ int i;
#ifdef DEBUG
   if (account != NULL) {
      long sum; char * s, * t;
      s = "--------------------";
      t = "OPC   Count   %      ";
      sum = 0;
      for (i=0; i < 64; i = i++) sum = sum+account[i];
      fprintf(stdtrc,"\nRASL operators accounting\n");
      fprintf(stdtrc,"%s%s%s%.19s\n",s,s,s,s);
      fprintf(stdtrc,"%s%s%s%.16s\n",t,t,t,t);
      fprintf(stdtrc,"%s%s%s%.19s\n",s,s,s,s);
      for (i=0; i < 48; i = i+4) {
        fprintf(stdtrc, "%3d: %7ld %2ld%% |   %3d: %7ld %2ld%% |   %3d: %7ld %2ld%% |   %3d: %7ld %2ld%%\n",
                i  ,account[i  ], account[i]*100/sum,
                i+1,account[i+1], account[i+1]*100/sum,
                i+2,account[i+2], account[i+2]*100/sum,
                i+3,account[i+3], account[i+3]*100/sum);
      }  }
#endif
}























#ifdef DEBUG
void rf_dbg(elemptr b1, elemptr b2, int ins) {
   if(ins < 0 OR ins >= 64) {
      fprintf(stdtrc,"=== INVALID RASL COUNTER\n");
      fprintf(stdtrc,"STEP: %ld\n",step);
      fprintf(stdtrc,"AT: ");
      rf_oute(tbel(0),tbel(0));
      fprintf(stdtrc,"\n");
      fprintf(stdtrc,"FUNCTION: ");
      rf_oute(NEXT(REF00(tbel(3))),PREV(REF00(tbel(3))));
      fprintf(stdtrc,"\n");
      fprintf(stdtrc,"RASL: %d",ins);
      rf_exit(3);
      }
   if(NOT dbgfl OR step < trstep) return;
   fprintf(stdtrc,"=== RASL OP: %3d\n",ins);
   if (NEXT(b1) != b2) {
      fprintf(stdtrc,"=== HOLE : ");
      rf_oute(NEXT(b1),PREV(b2));
      fputc('\n',stdtrc);
      }
   if (b != Afreestor AND lbrac == NULL) {
      fprintf(stdtrc,"=== RESULT : ");
      rf_oute(NEXT(Afreestor),b);
      fputc('\n',stdtrc);
}      }


































#define ERROR_MSG(N) {fprintf(stdtrc,"\n*** Data check %d Step %ld ***\n",N,step);\
            rf_error = ERRSTOR; spe = savspe; }
#define PCVAL(p) fprintf(stdtrc,"\nCvalue(%p):  %u:%u",p,VAL(p),TYPE(p));
#define PHEAD(h) fprintf(stdtrc,"\nHeadder(%p): %u:%u",h,VAL(h),TYPE(h));

LOGICAL rf_chck(elemptr p1, elemptr p2)               /* rf_chck */
{  elemptr * savspe = spe;
   if (PREV(p1) == p2) return(TRUE);
   for (;;p1 = NEXT(p1)) {
      if (BADPTR(p1)) { ERROR_MSG(0);  return (FALSE);}
      if (PREV(NEXT(p1)) != p1) { ERROR_MSG(1);  return (FALSE);}
      if (p1 == p2) return(TRUE);
}     }

LOGICAL rf_chckf (elemptr p1, elemptr p2)
{  elemptr * savspe = spe;
   headptr h;
  if (p1 == NULL) { spe = savspe; return(TRUE); }
  for (;;) {
    if (BADPTR(p1)) { ERROR_MSG(2); return (FALSE); }
    else if (ISBRAC(p1)) {
        h = REF(p1);
        if (PREV(h) != h) {
            pushe(p1,p2);
            if(spe > spemax) rf_err(ERCSTKO);
            p2 = PREV(h);
            if (NEXT(p2) != h) { ERROR_MSG(5); return (FALSE);}
            p1 = h;
          }    }
    else if (ISSIMPL(p1)) ;
    else if (ISREF(p1)) {
        h=REF00(p1);
        if (IFUNC(h,_OBJECT)==inull) { PHEAD(h); ERROR_MSG(11); return (FALSE); }
        }
    else { PCVAL(p1); ERROR_MSG(12);  return (FALSE);}
    while (p1 == p2) {
         if (savspe == spe) { spe = savspe; return(TRUE); }
         pope(p1,p2);
         }
    if (PREV(NEXT(p1)) != p1)  { ERROR_MSG(9); return (FALSE);}
    p1 = NEXT(p1);
  }    }

headptr chkref(headptr h)  /* Only for SPLIT model */
 {  elemptr * savspe = spe;
#ifdef SPLIT
    if((h&1)!=0) { ERROR_MSG(21); return NULL; }
#endif
    if(TYPE(h)==HEADSIMPL)  { PHEAD(h); ERROR_MSG(22); return NULL; }
    return(h);
 };

#endif


ifunc work_function(headptr std_load, addr *work_pointer)
{
   int flag = ((std_load->info.data.flag ) >>2 ) & 63;
   pfunc func = *pfunc_table[ flag ];
   return func(std_load, 24, work_pointer);
}


int work_eof(headptr std_load, addr *work_pointer)
{
   ifunc func = *((Tfeof)work_function(std_load, work_pointer));
   return func(*(FILE *)work_pointer);
}




LOGICAL rg_GO()
{  elemptr p, bstart;
   headptr stdload; /* io object for load loop */
   LOGICAL res;
   LOGICAL flag_s = strchr(flags,'s')!=NULL;
   LOGICAL flag_r = strchr(flags,'r')!=NULL;
   long tesize,savestep;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(p,p); /* Empty argument */ p++;
   if (NOT arglong(flags,'T', &tesize)) tesize = TESIZE;
   if ((te = (elemptr *)halloc(tesize, sizeof(elemptr))) == NULL)
      rf_err(ERCSTOR);
   step = 0;
   level = 0;
   tel = te;
   telmax = tel+((int)tesize-50);
   modacts=(arglong(flags,'m',&trstep)?0:1); /* if (flag -m) 0 else 1 */
   dbgfl = FALSE;
   tracefl = arglong(flags,'t',&trstep);
   BRKINIT;
   rf_cnt_start();
/* Register FUNC mode */
   if(NOT vregtype(P_FUNC,  "FUNC",  'F', FUNC  )) rf_err(ERCINIT);
/* make s.Boxtable */
   if(NOT inisysinfo()) rf_err(ERCINIT);
   if(NOT initblt()) rf_err(ERCINIT);
   if(NOT iniextbox("APPLY_OBJECT",&applyobject)) rf_err(ERCINIT);
/* Start input loop */
   bstart = b; /* This will be the left bound of the result */
/* make standard load channel STDLOAD */
   if(NOT iniextbox("STDLOAD",&stdload)) rf_err(ERCINIT);
   /* ininame = "name1+name2+..." */
   while (strlen(ininame) != 0 && rf_exitcd < 0 && rf_error != ERREXIT) {
     if (NOT rfopi(&ininame,stdload))
       rf_err(ERCINIT);
     chan[3] = stdload;
     VAL(stdload)++;   /* make chan[3] actual */
     if (flag_s)
       rf_out_stat (stdtrc);
     while (! work_eof(stdload, &wrkptr)) {
       rf_error = 0;
       savestep = step;
       res = rf_inpc(stdload, Afreestor, linefeed, systable);  /* INTER */
       if (NOT res OR tracefl OR flag_s OR flag_r) {
         if (rf_error == 0) {
/*          printlv("RES: ");                  */
           if (bstart != b)
             rf_oute(NEXT(bstart),b);
           fprintf(stdtrc, "\n");
         } else {
           printlv("MAIN: ");
           rf_errmsg(rf_error);
         }
       }
       if (savestep != step AND flag_s)
         rf_out_stat(stdtrc);
       b = bstart;
       if (rf_error == ERREXIT) {
         rf_error = 0;
         break;
       }
       if (rf_exitcd >= 0)
         break;
     }
   }
   rf_cnt_end();
   *AeresL = *AeresR = NOELEM;
   return(TRUE);
}
