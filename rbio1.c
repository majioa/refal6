/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1990 by IPS-AN                   */
/*       rbio.c    - input/output built-in functions                    */
/************************************************************************/
/*E     <READ_CHAR! s.Chan> == s.Char             ,     rb_read         */
/*E     <READ_LINE! s.Chan> == e.Chars            ,     rb_readln       */
/*E     <PRINT! s.Chan> ==                        ,     rb_write        */
/*E     <PRINTLN! s.Chan> ==                      ,     rb_writeln      */
/*E     <READ! t.Chan> == t.Term |                ,     rb_read         */
/*E     <READLN! t.Chan> == e.Expr                ,     rb_readln       */
/*E     <WRITE! t.Chan e.Expr> ==                 ,     rb_write        */
/*E     <WRITELN! t.Chan e.Expr> ==               ,     rb_writeln      */
/*E     <EOF s.Chan> == T | F                     ,     rb_eof          */
/*E     <OPEN_FILE s.Chan e.Name s.Mode> == T | F , rb_open_file        */
/*E     <ERASE_FILE e.Name> ==                    ,     rb_erase_file   */
/*E     <CLOSE_CHANNEL s.Chan> ==                 ,     rb_close_chan   */
/*E     <TO_CHARS e.Expr> == e.Chars              ,     rb_to_chars     */
/*E     <ENCODE s.Table e.Expr> == e.Chars        ,     rb_encode       */
/*E     <DECODE s.Table e.Chars> == e.Expr        ,     rb_decode       */
/*E     <NUMB e.Chars> == s.Num                   ,     rb_numb         */
/*E     <GETENV s.Word> == e.Chars                ,     rb_getenv       */
/*E      &STDIN, &STDOUT, &STDERR - standard channels                   */

#include "refgen.h"
#include "fileio.h"
#include "rfalloc.h"
#include "contain.h"
#include "rfio.h"
#include <stdlib.h>
#include <string.h>

static headptr STDIN, STDOUT, STDERR;

static LOGICAL inichan(char* n, headptr* ah, FILE* f)
{
   if(NOT iniextbox(n,ah)) return(FALSE); /* make new h <- *ah */
   if(NOT rf_getempty(*ah))return(FALSE);
   mk_fileio(*ah,f);
   return(NOT ISNULL(*ah));
   };

LOGICAL inirbio()
{
  if(NOT inichan("STDIN" ,&STDIN , stdin)) return(FALSE);
  if(NOT inichan("STDOUT",&STDOUT,stdout)) return(FALSE);
  if(NOT inichan("STDERR",&STDERR,stdtrc)) return(FALSE);
  return(TRUE);
}













/* Get channel from symbol (object or number) */
static LOGICAL tochan(elemptr p, headptr * ah)       /* p --> h */
{ int n;
  *ah = NOHEAD;
  if(ISSNUMB(p))
    { n = VAL(p);
      if (n>=0 AND n<=MAXFILES) *ah=chan[n];
      else return(FALSE);
      }
  else if (ISREF(p)) *ah=REF00(p);
  START(*ah);
  if(IFUNC(*ah,_CHANNEL)!=inull) return (TRUE);
  END(*ah);
  return(FALSE);
  }

/* Get new empty object from symbol (object or number) */
static LOGICAL toempty(elemptr p, headptr * ah)       /* p --> h */
{ int n;
  if(ISSNUMB(p))
    { n = VAL(p);
      if (n>=0 AND n<=MAXFILES) {
          if(chan[n]==NOHEAD)
             { if (NOT rf_mkempty(&(chan[n]))) return(FALSE); }
          else
             { if (NOT rf_getempty(chan[n])) return(FALSE); }
          *ah=chan[n]; return(TRUE);
          }
      else return(FALSE);
      }
  if (ISREF(p)) {
        *ah=REF00(p);
        return (rf_getempty(*ah));
        };
  return(FALSE);
  }

/* get channel and table from the argument term sC or (sC sT) */
static LOGICAL chantbl(elemptr pl, elemptr pr, headptr* ac, headptr* at)
{
 elemptr h;
 if (ISBRAC(pl)) {
    h = REF(pl);
    pl = NEXT(h);
    pr =  PREV(h);
    };
  if (NOT tochan(pl,ac)) return(FALSE);
  *at = NOTABLE;
  return (pl==pr OR (ISREF(pr) AND ISTABLE(*at=REF00(pr))));
};











static LOGICAL rb_printx(LOGICAL line)
{  headptr cp, table;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR NOT chantbl(EargL,EargL,&cp,&table)) return (FAIL);
   if (table==NOTABLE) table = SYSID;
   if (IFUNC(cp,_PUTC)==inull) return (FALSE);
   if (EargL != EargR) rf_outc(cp,NEXT(EargL),EargR,FALSE,table);
   *AeresL = *AeresR = NOELEM;
   if (line) PUTC('\n',cp);
   END(cp);
   AFTER(b,EargL,EargR);
   return (TRUE);
}
static LOGICAL rb_readx(LOGICAL line)
{  headptr cp, table;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR
        NOT chantbl(EargL,EargR,&cp,&table)) return(FAIL);
   if (IFUNC(cp,_GETC)==inull)  return (FALSE);
   oldb=b;
   if (NOT rf_inpc(cp,Afreestor,(line?linefeed:termfeed),table))
            return(FALSE);
   END(cp);
   if(b==oldb) *AeresL=*AeresR=NOELEM;
   else {
     *AeresL = NEXT(oldb);
     *AeresR = b;
     weld(oldb,NEXT(b));
     b=oldb;
     }
   AFTER(b,EargL,EargR);
   return (TRUE);
}
LOGICAL rb_writex(LOGICAL line)
{  headptr cp, table;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR NOT chantbl(EargL,EargL,&cp,&table)) return (FAIL);
   if (table==NOTABLE) table = SYSID;
   if (IFUNC(cp,_PUTC)==inull) return (FALSE);
   if (EargL != EargR) rf_outc(cp,NEXT(EargL),EargR,TRUE,table);
   END(cp);
   *AeresL = *AeresR = NOELEM;
   if (line) PUTC('\n',cp);
   AFTER(b,EargL,EargR);
   return (TRUE);
}





LOGICAL rb_println() {  return rb_printx(TRUE ); }
LOGICAL rb_print  () {  return rb_printx(FALSE); }
LOGICAL rb_readln() {  return rb_readx(TRUE ); }
LOGICAL rb_read  () {  return rb_readx(FALSE); }
LOGICAL rb_writeln() {  return rb_writex(TRUE ); }
LOGICAL rb_write  () {  return rb_writex(FALSE); }

LOGICAL rb_read_char()
{  headptr cp;
   Tgetc gtc;
   FILE * f;
   int c;
   elemptr p;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR EargL != EargR OR NOT tochan((p=EargL),&cp))
       return(FAIL);
   gtc = (Tgetc)IFUNC(cp,_GETC); f=(FILE*)THIS;
   if (gtc==(Tgetc)inull)  return (FALSE);
   c = gtc(f);
   elemfree(p);
   if(c) { TYPE(p)=TYPECHAR; VAL(p)=c;}
   else  { TYPE(p)=TYPESNUMB; VAL(p)=0;}
   END(cp);
   *AeresL=*AeresR=p;
   return (TRUE);
}

LOGICAL rb_read_line()
{  headptr cp;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR EargL != EargR OR NOT tochan(EargL,&cp))
       return(FAIL);
   if (IFUNC(cp,_GETC)==inull)  return (FALSE);
   oldb=b;
   if(NOT rf_insc(cp,Afreestor)) return(FALSE);
   END(cp);
   if(b==oldb) *AeresL=*AeresR=NOELEM;
   else {
     *AeresL = NEXT(oldb);
     *AeresR = b;
     weld(oldb,NEXT(b));
     b=oldb;
     }
   AFTER(b,EargL,EargR);
   return (TRUE);
}







/* <READ_BLOCK! s.Chan s.Len s.Loc> -> s.Loc */
LOGICAL rb_read_block()
{  headptr cp,h;
   seglen l;
   long L;
   elemptr pl,ps;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR NOT tochan(EargL,&cp)) return(FAIL);
   if (IFUNC(cp,_GETC)==inull)  return (FALSE);
   if(EargL==EargR OR (pl=NEXT(EargL))==EargR) return(FAIL);
   if (NOT rf_getnumb(pl,&L)) return(FAIL);
   l=(seglen)L;
   if((ps=NEXT(pl))!=EargR) return(FAIL);
   if( ISSIMPL(ps) ) return(FAIL);
   h=REF00(ps);
   if (TYPE(h)!=HEADSTR AND NOT rf_get_empty(h,HEADSTR)) return(FAIL);
   {   tail t;
       START(h);
         EVAL(h,_SETPOS,Tsetpos,(THIS,&t,0));
         if(NOT EVAL(h,_SETLEN,Tsetlen,(THIS,&t,l))) return(FALSE);
         if(NOT rf_read(cp,&l,SEGINF(AD(h)),FALSE))  return(FALSE);
         if(NOT EVAL(h,_SETLEN,Tsetlen,(THIS,&t,l))) return(FALSE);
       END(h);
   }
   *AeresL=*AeresR=ps;
   AFTER(b,EargL,pl);
   return (TRUE);
}





























LOGICAL rb_open_file()
{  elemptr p;
   int i; char mod[3];
   headptr cp,h;
   FILE * f;
   cvalue res;
   char fn[MAXFN+1];
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR EargL == EargR) return (FAIL);
   if (NOT toempty(EargL,&cp)) return (FAIL);
 /* Get mode symbol */
   if (ISCHAR(EargR)) {mod[0]=VAL(EargR); mod[1]=0;}
   else if (ISSWORD(EargR)) {
        cvalue c;
        c = DATA(EargR);
        ALL(i,2) mod[i] = tolower(SWDBODY(c)[i]);
        i=SWD_LEN(CTYPE(c));
        mod[(i>2? 2:i)]=0;
        }
   else return(FAIL);
 /* Get file name and open file */
   p=NEXT(EargL);
   if(NEXT(p)==EargR AND ISREF(p) AND MODE(TYPE(h=REF00(p)))==STRING) {
      START(h);
        i=WDLEN(h);
        if(WDREALLOC(h,i+1)==NOADR) return(FALSE);
        WDFIRST(h)[i] = '\0';
          f=rf_open((char*)WDFIRST(h),mod);
        WDREALLOC(h,i);
      END(h);
      }
   else {
      for (i=0; p!=EargR; p = NEXT(p)) {
          if (i>=MAXFN OR NOT ISCHAR(p)) return(FALSE);
         fn[i++] = VAL(p);
         }
      fn[i] = '\0';
      f=rf_open(fn,mod);
      }
 /**/
   elemfree(EargR);
   res = COMWORD[( f!=NULL? _T : _F )] ;
   SETDATA(EargR,res);
/* if(NOT IS_SIMPL(CTYPE(res)) VAL(CREF00(res))++; */
     mk_fileio(cp,f);
   *AeresL = *AeresR = EargR;
   AFTER(b,EargL,PREV(EargR));
   return (TRUE);
}








LOGICAL rb_eof()
{  headptr cp;
   Tfeof ifeof;
   FILE * f;
   cvalue res;
   elemptr p, EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR EargL != EargR OR NOT tochan((p=EargL),&cp))
       return(FAIL);
   ifeof = (Tfeof) IFUNC(cp,_FEOF); f = (FILE*)THIS;
   if ((ifunc)ifeof==inull)  return (FALSE);
   res = COMWORD[( ifeof(f)? _T : _F )] ;
   END(cp);
   elemfree(p);
   SETDATA(p,res);
/* if(NOT IS_SIMPL(CTYPE(res)) VAL(CREF00(res))++; */
   *AeresL=*AeresR=p;
   return (TRUE);
}

LOGICAL rb_erase_file()
{
   elemptr p;
   int i;
   char fn[MAXFN+1];
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM)
      return (FAIL);
   i = 0;
   for (p = EargL; ; p = NEXT(p)) {
         if (NOT ISCHAR(p)) return(FAIL);
         fn[i++] = VAL(p);
         if (i == MAXFN) break;
         if (p == EargR) break;
      }
   if (p != EargR) return(FAIL);
   fn[i] = '\0';
   AFTER(b,EargL,EargR);
   unlink(fn);
   *AeresL = *AeresR = NOELEM;
   return (TRUE);
}













LOGICAL rb_close_chan()
{
   headptr cp;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NOELEM OR EargL != EargR OR NOT toempty(EargL,&cp))
     return (FAIL);
   AFTER(b,EargL,EargR);
   *AeresL = *AeresR = NOELEM;
   return (TRUE);
}

static int lputc(int c, uchar * dummy)
{
 if(MOVB()) { TYPE(b)=TYPECHAR; VAL(b)=c; return(c); }
 else return(EOF);
 }

LOGICAL rb_to_chars()
{
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr savb;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb=b;
   if(EargL!=NOELEM) {
     rf_outm(NULL,(Tputc)lputc,EargL,EargR,FALSE,NOTABLE);
     if(rf_error==ERRSTOR) return(FALSE);
     };
   if(b==savb) { *AeresL=*AeresR=NOELEM; }
   else {
     *AeresL=NEXT(savb);
     *AeresR=b;
     BETWEEN(savb,NEXT(b),EargL,EargR);
     b=savb;
     }
   return (TRUE);
}

/*  <ENCODE s.Table e.Expr> == e.Chars  */
LOGICAL rb_encode()
{
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr savb;
   headptr table;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb=b;








   if (EargL == NOELEM) return FAIL;
   if (NOT ISREF(EargL) OR NOT ISTABLE(table=REF00(EargL))) table=NOTABLE;
   if(EargL!=EargR) {
     rf_outm(NULL,(Tputc)lputc,NEXT(EargL),EargR,TRUE,table);
     if(rf_error==ERRSTOR) return(FALSE);
     };
   if(b==savb) { *AeresL=*AeresR=NOELEM; }
   else {
     *AeresL=NEXT(savb);
     *AeresR=b;
     weld(savb,NEXT(b));
     b=savb;
     }
   AFTER(b,EargL,EargR);
   return (TRUE);
}

/*  <DECODE s.Table e.Chars> == T e.Expr | F e.Chars_rest  */
LOGICAL rb_decode()
{
   LOGICAL res;
   cvalue wres;
   elemptr fl;
   expr E,E1,E2;
   elemptr *AeresL, *AeresR;
   headptr h,table;
   popae(AeresL,AeresR);
   pope(E.L,E.R);
   if (E.L == NOELEM) return FAIL;
   if(NOT ISREF(E.L) OR NOT ISTABLE(table=REF00(E.L))) table=NOTABLE;
   butfirst2(E,E1);
   if(NOT rf_mk_empty(&h,HEADEXPR)) return(FALSE);
   NEXT(h)=E1.L; PREV(h)=E1.R;
   oldb=b;
   movb; fl=b;
   res = rf_inpc(h,Afreestor,linefeed,table);
   E2.L = NEXT(h); E2.R = PREV(h);
   AFTER(b,E.L,E.R);
   if (NOT res) {
      b=fl;
      if(NOT empty(E2)) { AFTERX(b,E2.L,E2.R); b=E2.R; }
          rf_error=0;
      };
   headptrfree(h);
   wres = COMWORD[( res? _T : _F )] ;
   SETDATA(fl,wres);
/* if(NOT IS_SIMPL(CTYPE(wres)) VAL(CREF00(wres))++; */
   *AeresL=NEXT(oldb);
   *AeresR=b;
   WELD(oldb,NEXT(b));
   b=oldb;
   return (TRUE);
}







/*  <NUMB e.Chars> == s.Int  */
LOGICAL  rb_numb()
{  elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   headptr h;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if(NOT rf_mk_empty(&h,HEADEXPR)) return(FALSE);
   NEXT(h)=EargL; PREV(h)=EargR;
   oldb=b;
   if(NOT rf_inpc(h,Afreestor,linefeed,NOTABLE) OR NEXT(h)!=NOELEM) return(FALSE);
   if(b==oldb) { movb; SETSIM(b,0,TYPESNUMB); }
   if(b!=NEXT(oldb) OR NOT ISNUMB(b)) return(FALSE);
   if(EargL!=NOELEM) AFTER(b,EargL,EargR);
   headptrfree(h);
   *AeresL=NEXT(oldb);
   *AeresR=b;
   weld(oldb,NEXT(b));
   b=oldb;
   return (TRUE);
}

sputc(uchar c, uchar * buf)
/* buf is a bounded buffer: buf[0]=length, buf[1]=size */
{ if (buf[0] < buf[1]) buf[(++buf[0])+1]=c;
  return(c);
  }
LOGICAL  rb_getenv()
{  elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr savb;
   char buf[64], * s;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb=b;
   if(EargL==NOELEM) s=NULL;
   else {
     buf[0]=0; buf[1]=61;
     rf_outm((FILE*)buf,(Tputc)sputc,EargL,EargR,FALSE,NOTABLE);
     if(buf[0]==buf[1]) return(FAIL);
     buf[buf[0]+2]=0;
     s = getenv(buf+2);
     }
   if(s==NULL) *AeresL=*AeresR=NOELEM;
   else {
     if(NOT rf_mkstr((uchar*)s,strlen(s))) return(FALSE);
     *AeresL=NEXT(savb);
     *AeresR=b;
     weld(savb,NEXT(b));
     }
   if(EargL!=NOELEM) AFTER(savb,EargL,EargR);
   b=savb;
   return (TRUE);
}