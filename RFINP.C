/***********************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
	   rfinp.c      - input from channel
***********************************************************************/
#include "refcom.h"
#include "rftable.h"
#include "contain.h"
#include "rfio.h"
#include "rfalloc.h"
#include "arithm.h"
#include "rbarm.h"

#define C (ain->c)
#define GTC ((ain->gtc)(ain->f))
#define UNGTC ((ain->ungtc)(ain->c,ain->f))
#define TenthMaxShort 3276

static LOGICAL inpstr(in* ain, Trepn irepn, addr current, tail * at)
{  int termc = C;
   if ((C=GTC) == termc) {
	  irepn(current,at,CMKSIM(C,TYPECHAR));
	  C=GTC;
      return (TRUE);
      }
   while (C != EOF) {
    while (C=='\r') C=GTC; /* \r must be '\r', \r must be skipped in UNIX */ 
	  if (C == termc)
	      if ((C=GTC) != termc) return(TRUE);
    if (C=='\\')
         if ((C=GTC)=='r') C='\r';
          else if (C=='n') C='\n';
          else if (C=='b') C='\b';
          else if (C=='t') C='\t';
          else if (C=='v') C='\v';
          else if (C=='f') C='\f';
	  irepn(current,at,CMKSIM(C,TYPECHAR));
	  C=GTC;
	  }
   return (FALSE);
}

static LOGICAL inpwds(in* ain, cvalue * ac)
{  int termc;
   uchar wbuf[MAXWORD+1];
   int i;
   i = 0;
   termc=C;
   C=GTC;
   while (C != EOF AND i<MAXWORD) {
	  if (C == termc) {
		 if ((C=GTC) != termc) return (rf_mk_word(wbuf, i, ac, TRUE));
		 wbuf[i++] = termc;
         }
      else  {
		if (C=='\\') if ((C=GTC)=='r') C='\r';
		wbuf[i++] = C;
        };
	  C=GTC;
      }
   return (FALSE);
}
static LOGICAL inpword(in* ain, cvalue * ac)
{  uchar wbuf[MAXWORD+1];
   int i;
   if(C=='.') C=GTC;
   if(C=='\"' OR C=='\'') return(inpwds(ain,ac));
   for (i=0; ISWORDS(C) AND i<MAXWORD; i++) { wbuf[i] = C; C = GTC; }
   return (rf_mk_word(wbuf, i, ac, TRUE));
}

/* extern int prm; */
#define POINT(n) /* if(prm) fprintf(stdtrc," POINT(%d) ",n); */

static LOGICAL inpnumb(in* ain, int sign, cvalue * ac)
{  int i,m;
   MPOS N; seglen lN, mlN; adr adN;
   double x,f; char t; int e;

   i = C - '0';
   while (C=GTC, isdigit(C) AND i < TenthMaxShort)
	  i = i*10 + (C - '0');
   if (C==' ') { *ac=CMKSIM(i*sign,TYPESNUMB); return (TRUE); }
   if(C=='E' OR C=='e' OR C=='.') { x = i*sign; goto real; }
   if (NOT isdigit(C)) {
	  *ac=CMKSIM(i*sign,TYPESNUMB);
	  return (TRUE);
	  }
   /* Allocate number in dynamic memory segment */
   POINT(10)
   mlN=6; ECALL(ALLOC(adN, MSIZE(mlN))!=NOADR);
   POINT(11)
   N = (MPOS)(SEGINF(adN));
   /* *((long*)N) = i; Was incorect for big-endian computers */
   N[0] = i;
   N[1] = 0;
   lN=2;

   while (isdigit(C)) {
      i = C - '0';
      m = 10;
      while (C = GTC, isdigit(C) AND m < TenthMaxShort) {
		 i = i*10 + (C - '0');
		 m = m*10;
		 }
      /* Append next m decimal digits to the number */
      { /* long D = i; Was incorect for big-endian computers */
        ushort D[2] ;
        D[0] = i; D[1] = 0;
        POINT(14)
		/* ra_addm((MPOS)&D,2,N,lN,m,1,N,mlN,&lN); */
        /* m>=10 */
        ra_adm(N,lN,m-1,1,N,mlN,&lN); /* N += N*(m-1) = N*m */
		ra_ads((MPOS)&D,2,1,N,mlN,&lN);  /* N = N + D */
        ra_norm(N,&lN);
        }
      if(lN>=mlN) {
          mlN = (mlN*4)/3+5;
          POINT(20)
          ECALL(REALLOC(adN,MSIZE(mlN))!=NOADR);
          N = (MPOS)(SEGINF(adN));
          POINT(21)
          }
	  }
   if(sign<0) { POINT(25) ra_minus( N,lN, N,mlN,&lN); }
   if(C=='E' OR C=='e' OR C=='.') {
      x = ra_int2double(N,lN);
      REALLOC(adN,0);
      goto real;
      }
   POINT(30)
   ECALL(rf_mk_numb(N,lN,ac,TRUE));
   POINT(31)
   REALLOC(adN,0);
   return (TRUE);
real:
   t='e';
   e=0;
   if(C=='.') while (C=GTC,isdigit(C)) { x=10*x+sign*(C-'0'); e--; }
   if(C=='e' OR C=='E') { int psign;
       t=C; psign=1; i=0; C=GTC;
            if (C=='-') { psign=-1; C=GTC; }
       else if (C=='+') C=GTC;
       while(isdigit(C)) { i=i*10+(C-'0'); C=GTC; }
       e+=psign*i;
       }
   f=(e>0? 10.0 : 0.1); e = abs(e);
   ALL(i,e) x*=f;
   return rf_mk_creal(x,ac,t=='e');
}

extern LOGICAL rf_eval(headptr hat, LOGICAL exitonfail);

/* Function rf_inpm is recursive over the input line bracket structure */
static LOGICAL rf_inpm(in* ain,
	Trepn irepn, addr current, tail * at,  /* object to output cvalues */
	termcond tcond)
{
	static LOGICAL res;
	static cvalue name,value,nc;
	static int mode,modchar;
	auto   headptr outh;
#define ERRS goto error;
#define CONTINUE { C=GTC; goto A; }
   /* first character is in the buffer C */
   res = TRUE;
A: while (C != EOF) {
   while (C==' ' OR C=='\t') C=GTC;
	 if (isdigit(C)) { if ( NOT(inpnumb(ain, 1, &value))) ERRS; }
	 else if ( C=='\'') {
		 if ( NOT inpstr(ain, irepn,current,at)) ERRS;
		 goto testend;
		 }
	 else if (C == '*' OR C == '&') {
		 C = GTC;
		 if (C != '=') {
			if (ain->table == NOTABLE) ERRS;
			if ( NOT(inpword(ain, &name))) ERRS;
			if ( NOT(rf_tab_val(ain->table,'U',name,&value))) ERRS;
			cvalfree(name);
			outh = CREF00(value);
			}
		 else if(NOT rf_mk_empty( &outh, HEADEMPTY)) ERRS;
		 if(C =='=') {
            C=GTC;
            if(isdigit(C) OR C=='+' OR C=='-') {
			  int sign = (C=='-'? -1 : 1);
              if(NOT isdigit(C)) C=GTC;
              if(NOT inpnumb(ain, sign, &nc)) ERRS;
              if(NOT rf_set(outh,nc,1)) ERRS; /* 1 is min.refcount for nc */
              }
            else if(C=='\"') {
              if ( NOT(inpword(ain, &nc))) ERRS;
              if(NOT rf_set(outh,nc,1)) ERRS; /* 1 is min.refcount for nc */
              }
            else {
		      if( C == '(') modchar='C'; /* CHAIN */
			  else { modchar = C; C = GTC; }
			  /* Convert modchar to mode */
			  ALL(mode,MAXMODE) if(pchar_table[mode]==modchar) break;
			  if(mode==MAXMODE) ERRS;
			  if(NOT rf_get_empty(outh,HD(mode,OBJ))) ERRS;
			  res=(EVAL(outh,_LOAD,Tload,(ain, THIS)));
			  END(outh);
			  value = CMKREF(outh,TYPEREF);
			  if(NOT res) ERRS;
			  C=GTC; /* Skip ')' */
		 }	} }
	 else if (C == '(' OR C == '<') {
		auto tail t;
		static int c1;
		movb;
		outh = b;
		c1 = C;
		C = GTC;








		if (c1=='<' AND (isalpha(C) OR C=='\"')) { UNGTC; C='&'; }
		res=rf_inpm(ain,irepnfree,THIS,&t,(c1=='('? rightbrac:anglebrac));
		oldb = PREV(outh);
		CROSS(oldb,b);
		b = oldb;
		SETSIM(outh,1,HEADBRAC);
		value = CMKREF(outh,TYPEBRAC);
		if(NOT res) ERRS;
		if(C=='>') {
			elemptr savb;
			static elemptr p;
			savb=b;
			if(rf_eval(outh,FALSE)!=TRUE) b=savb;
			if(irepn!=irepnfree) {
				for (p=savb; p!=b;) {
					p=NEXT(p);
					irepn(current,at,ADATA(p));
					}
				b=savb;
				}
			if(rf_error==ERREXIT) break;
			C=GTC;
			goto testend;
			}
		else if(C==')') C=GTC; else ERRS;
		}
	 else if (isalpha(C) OR C=='\"') { if ( NOT(inpword(ain, &value))) ERRS; }
	 else if (C == '.')      { C=GTC; if ( NOT(inpword(ain, &value))) ERRS; }
	 else if (C == '+' OR C == '-') {
		 static int sign;
		 sign = ( C == '+' ? 1 : -1 );
		 C = GTC;
		 if (NOT (isdigit(C) AND inpnumb(ain, sign,&value))) ERRS;
		 }
	 else if (C == ')' AND tcond == rightbrac) break;
	 else if (C == '>' AND tcond == anglebrac) break;
	 else if (C == '\n') { if (tcond==linefeed) break; else CONTINUE }
	 else if (C == '\t') CONTINUE
   else if (C == '\r') CONTINUE
	 else if (C == '/') {
          C=GTC;
          if (C == '/') {  /*   the rest of line after // is a comment  */
             while (C!='\n' AND C!=EOF) C=GTC;
             goto A;
             }
          else if (C == '*') {  /* all this is a comment up to */
             do {
                C = GTC;
                while (C=='*') {
                   C=GTC;
                   if(C=='/') CONTINUE
                   }
                } while (C!=EOF); ERRS
             }
          else ERRS
          }





	 else if (C == EOF) break;
   else if (C == '\x1A') break; /* Ctrl-Z = end of file */
	 else ERRS;
	 irepn(current,at,value);
testend: if (tcond == termfeed) { UNGTC; break; }
	 } /* end while */
   return(TRUE);
error:
    if(rf_error!=ERRNO) return(FALSE);
    ERROR(ERRSYNT);
}

LOGICAL rf_inpc(
	headptr inch, /* input channel object (STARTed) */
	headptr outcont, /* output container object (STARTed) */
	termcond tcond, headptr table)
{
  in IN; /* input object */
  addr current; tail t; Trepn irepn; /* output object */
  IN.gtc   = (Tgetc) IFUNC(inch,_GETC);
  IN.ungtc = (Tungetc) IFUNC(inch,_UNGETC);
  IN.f = (FILE*) THIS;
  IN.table = table;
  irepn = (Trepn) IFUNC(outcont,_REPN);
  EVAL(outcont,_SETPOS,Tsetpos,(current=THIS,&t,-1));
  IN.c=IN.gtc(IN.f);
  return rf_inpm(&IN, irepn, current, &t, tcond);
  }

/* Read a block into a buffer.
   The result is FAIL if less then *alen bytes have been read */
LOGICAL rf_read(
     headptr inch,  /* input channel object (STARTed) */
     seglen * alen,   /* in: maximum number of characters to input
                        out: number of characters actually input */
     uchar * buf,   /* buffer to write into */
     LOGICAL line   /* break on Line Feed */)
 {
  in IN; /* input object */
  seglen l=*alen,t;
  if((IN.gtc = (Tgetc) IFUNC(inch,_GETC))==inull) return(FALSE);
  IN.f = (FILE*) THIS;
  ALL(t,l) {
    int c = IN.gtc(IN.f);
    if(c==EOF OR (line AND (c=='\n' OR c=='\t')))
        { *alen = t; return(FAIL); };
    buf[t]=c;
    }
  return(TRUE);
  };











static int rf_getc(addr f, Tgetc gtc)
{  if (f==NULL) return(getchar());
   return (gtc((FILE*)f));
}

LOGICAL rf_insc(headptr inch, headptr outch)
{ Tgetc gtc; Tputc ptc;
  addr fi,fo;
  int c;
  gtc = (Tgetc) IFUNC(inch,_GETC); fi = THIS;
  ptc = (Tputc) IFUNC(outch,_PUTC); fo = THIS;
  while ((c=rf_getc(fi,gtc)) != EOF AND c != '\n' AND c != '\r')
	  if(NOT ptc(c,(FILE*)fo)) return(FALSE);
  return(TRUE);
  }

LOGICAL iload (in* ain, addr a)
/* Input file parameters are in the global structure IN */
{ elemptr h;
  tail t;
  static Trepn ir;
  static cvalue n;
  static LOGICAL res;
  h=PFROMFAR(a);
  EVAL(h,_SETPOS,Tsetpos,(THIS,&t,-1));
  if (isdigit(C)) {
	 if (NOT(inpnumb(ain, 1, &n))) return(FALSE);
	 if(NOT EVAL(h,_SETLEN,Tsetlen,(THIS,&t,CVAL(n)))) return(FALSE);
	 }
  if(C!='(') return(FALSE);
  ir = (Trepn) IFUNC(h,_REPN);
  C=GTC;
  res=rf_inpm(ain, ir, THIS, &t, rightbrac);
  EVAL(h,_SETLEN,Tsetlen,(THIS,&t,0));
  return(res);
  }
