/******************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
   rbeval.c - EVAL builtin function
*******************************************************************/

/*E   <STEP [s.New]> == [s.Old]              ,     rb_step        */
/*E   <APPLY sF eA>  == RFINT('<sF eA>')     ,     rb_apply       */
/*E   <BREAK> ==                             ,     rb_break       */

#include "refint.h"
#include "rfio.h"
extern long int memleft();

LOGICAL rb_step()
{  elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   oldb = b;
   if (EargL == NOELEM) {   /* Res = Step */
     if (NOT rf_mknumb(step)) return(FALSE);
     *AeresL = NEXT(oldb);
     *AeresR = b;
     weld(oldb,NEXT(b));
     b = oldb;
     }
   else if (EargL==EargR) {  /* Step = Arg */
     if(NOT rf_getnumb (EargL, &step)) return(FAIL);
     AFTER(b,EargL,EargR);
         *AeresL=*AeresR=NOELEM;
     }
   else return(FAIL);
     return (TRUE);
}

LOGICAL rb_break()
{  elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR); EargL++; EargR++;
   rf_error = ERRBRK;
   *AeresL = *AeresR = NOELEM;
   return (TRUE);
}

static int get_comnd(char * comlist)
{ int c,C;
  for (; (c = GETCH, C = toupper(c), strchr(comlist,C)==NULL OR c==0) ;)
    { if (c!='\n') PUTCH(7); if(c==0) GETCH; }
  ECHO(c);
  return(C);
  };







/* Evaluate active term with the headder hat
    The result is added to new expression (after b)
    The result is either passive expression or the View Field.
    the parameter hat is a headder of Active Term,
    NEXT(hat) is a function symbol followed by the argument,
    PREV(hat) is the end of the argument.
    Active terms are linked in a list which starts in VF headder
    If Active Term fails, thew result depends on "exitonfail":
       TRUE - exit immediately to calling function with res = FAIL;
       FALSE - break and ask user as usual;
    */
LOGICAL rf_eval (headptr hat, LOGICAL exitonfail)
{ headptr hvf;
  elemptr savb, *savtel, bstart, pb, pe;
  stelptr savsp;
  headptr nextact;  /* Stack of evaluation result markers */
  long steps;
  int saverr;
  LOGICAL aux;      /* If current step is of auxiliary $-function */
  static int c;
  static long count;
  static elemptr at,vf,p,q,ha;
  static LOGICAL res;
  static elemptr ta,fs, *oldtel;
  /* Make view field with one active term */
  savb = b;
  movbx(b,hvf);
  movbx(hvf,at);
  CROSS(b,at);
  movb; vf = b;
  SETREF(vf,hvf,TYPEBRAC);
  SETACT(hvf,at);  /* type is HEADACT, not HEADVF */
  SETREF(at,hat,TYPEBRAC);
  SETREF(hat,NOELEM,HEADACT);
  nextact=NOELEM;
  steps=0;
  savsp = sp;
   /* Make rf_inter to yield FAIL properly */
  sp->spc=NOPC;
  sp->snel=_failinit;
  INCSP;

  savtel = tel;
run: BRKINIT;
  res = (rf_error ? FALSE : rf_inter(hvf,&steps));
  switch(rf_error) {
    case ERRBRK: case ERRSTEP: rf_error=0; break;
    case ERREXIT: goto end;
    case ERREND: rf_error=0;
        if(nextact==NULL) goto end;
        /* Process the results of E commands */
        while(REF(hvf)==NULL AND nextact!=NULL) {
         /* restore REF(hvf) and print the result */
        p=nextact; q = PREV(p);
        REF(hvf)=REF(p);
        pb = NEXT(q); pe = PREV(q);
        nextact=NEXT(nextact);
        AFTER1(b,p); SETSIM(p,0,TYPESIMPL);
        AFTER1(b,q); SETSIM(q,0,TYPESIMPL);
            printlv("E-RES: ");
            if (NEXT(pb) != pe) rf_oute( NEXT(pb), PREV(pe));
            fprintf(stdtrc,"\n");
        } break;
    case ERRFAIL: if(exitonfail) { res=FAIL; rf_error=0; goto end; }
    default: printlv("EVAL: "); rf_errmsg(rf_error);
    }
ask: printlv("EVAL: Act Vf Fre Run [n]Step [n]Trc Evl Chg Kil New Inf M W D H cLr Quit>");
   for(count = 0; isdigit(c=get_comnd("0123456789RTSACKEVWLHDFNMQI")); )
              count=count*10+(c-'0');
  if (count==0) count = 1;
  SKIPLINE; /*  fprintf(stdtrc,"\n"); */ /* fflush(stdin); */
    switch(c) {
       case 'R' : goto run;
       case 'T' : tracefl = TRUE;  steps=count; goto run;
       case 'S' : tracefl = FALSE; steps=count; goto run;
       case 'A' :
       case 'C' :
       case 'K' :
       case 'E' :       /* get  ha - where the newact is,
               PB, PE - bounds for the active term  */
          if(rf_error==0) {
            ta=REF(hvf);
            if(ta==NOELEM) { printlv("No Active Term\n"); goto ask; }
            ha=REF(ta);
            fs = NEXT(ha);
            aux = ISSIMPL(fs);
            if(aux) {
               oldtel = tel - ((sp-1)->snel);
               pb = PREV (oldtel[0]);
               pe = oldtel[1];
               }
            else { pb = PREV(ta); pe = NEXT(ta); }
            }
          else {
            aux = FALSE;
            pb=PREV(tbel(0));
            pe=tbel(1);
            }
       if (c=='E') {
            if(rf_error!=0) goto run;
            movbx(b,p); movbx(p,q); weld(b,NEXT(q));
            NEXT(p)=nextact; nextact=p;
            PREV(p)=q; NEXT(q)=pb; PREV(q)=pe;
            REF(p)=REF(ha);
            REF(ha)=NOELEM;
            goto run;
            }
       else if(c=='C' OR c=='K') {
            bstart = b;
            if(rf_error==0) /* justify next active term address */
               REF(hvf) = REF(ha);
            rf_error = 0;
            if(c=='C') {
               printlv("Enter expression> ");
               fflush(stdin); /* clear the buffer of stdin or chan[0] */
               tel=tel+4;
               res = rf_inpc(chan[0], Afreestor, linefeed, systable);
               tel=tel-4;
               }
            if(rf_error!=0) {
                b = bstart;
                printlv("EVAL: ");
                rf_errmsg(rf_error);
                rf_error = 0;
                goto ask;
                }
            AFTERX(b,NEXT(pb),PREV(pe));   /* throw out this active term */
            if (c=='C') {
               printlv("Active term changed for: ");
               if(b!=bstart) {
                  rf_oute(NEXT(bstart), b);
                  AFTERX(pb, NEXT(bstart), b);  /* insert new expr into vf */
                  b=bstart;
                  }
               fprintf(stdtrc,"\n");
               }
            else  printlv("Active term killed\n");
            if (aux) { /* Justify sp and tel */
                DECSP;
                tel-=(sp->snel);      /* unsave(sp) */
                STORSP(sp->ssp);
                }
            if (c=='C') goto ask; else goto run;
            }
       else /* if(c=='A') */ {
            printlv("AT: "); rf_oute(NEXT(pb),PREV(pe)); fprintf(stdtrc,"\n");
            goto ask;
            }
       case 'V' :
            printlv("VF: ");
            if (NEXT(hvf) != hvf) rf_oute(NEXT(hvf),PREV(hvf));
            fprintf(stdtrc,"\n");
            goto ask;
       case 'W': printallwords(stdtrc); goto ask;
       case 'H': rf_prholes(stdtrc); goto ask;
       case 'L': CLRSCR; goto ask;
       case 'D': if (stdtrc==stderr) {
            char name[40];
            fprintf(stdtrc,"Enter the filename to reDirect tracing to> ");
            fgets(name, sizeof(name), stdin);
            fprintf(stdtrc," Press D to reDirect tracing back to console\n");
            stdtrc=fopen(name,"a");
            if(stdtrc==NULL) stdtrc = stderr;
            }
            else { fclose(stdtrc); stdtrc = stderr; }
            goto ask;
       case 'F' :
            printlv("FS: ");
            if(NEXT(b)!=Afreestor) rf_oute(NEXT(b),PREV(Afreestor));
            fprintf(stdtrc,"\n");
            goto ask;
       case 'N':
            printlv("Enter expression> ");
            bstart = b;
            saverr = rf_error;
            rf_error = 0;
            fflush(stdin); /* clear the buffer of stdin or chan[0] */
            tel=tel+4;
            res = rf_inpc(chan[0], Afreestor, linefeed, systable);
            tel=tel-4;
            if(rf_error!=0) { printlv("EVAL: "); rf_errmsg(rf_error); }
            rf_error = saverr;
            printlv("The result is: ");
            if(b!=bstart) {
                rf_oute(NEXT(bstart), b);
                b=bstart;
                }
            fprintf(stdtrc,"\n");
            goto ask;
       case 'M':
            rf_chckf(NEXT(Afreestor),PREV(Afreestor));
            rf_out_stat (stdtrc);
            goto ask;
       case 'Q':
            printlv(" Exit? (Y/N/P)>");
            c=get_comnd("YNP");
            fprintf(stdtrc,"\n");
            if(c=='N') goto ask;
            if(c=='Y') rf_error=ERREXIT;
            break;
       case 'I':
            fprintf(stdtrc,"sp=%p,tel=%p,b=%p,tel[0-3]=(%p,%p,%p,%p)\n",
                    sp,tel,b,tel[0],tel[1],tel[2],tel[3]);
            fprintf(stdtrc,"step=%ld, snel=%d, ssp=%p\n",
                    step, (sp-1)->snel,(sp-1)->ssp);
            goto ask;
       default: goto ask;
       }
end:
    b = savb;
    SETSIM(hvf,1,HEADBRAC);
    STORSP(savsp);
    tel = savtel;
    /* Clear the stack of result markers */
    while(nextact!=NULL) {
        p=nextact; q = PREV(p);
        nextact=NEXT(nextact);
        AFTER1(b,p); SETSIM(p,0,TYPESIMPL);
        AFTER1(b,q); SETSIM(q,0,TYPESIMPL);
        }
    if( NEXT(hvf)!=hvf) { b = PREV(hvf); AFTERX(savb,NEXT(hvf),b); }
    return(res);
  }


















LOGICAL rb_apply()
{  elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr hat,bstart;
   LOGICAL res;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   bstart=b;
   if (EargL == NULL) return(FAIL);
   movb; hat=b;
   if(NOT copvg(EargL, EargR)) return(FALSE);
   CROSS(bstart,b);
   b=bstart;
   SETSIM(hat,1,HEADBRAC);
   if((res=rf_eval(hat,TRUE)) != TRUE) { b=bstart; return(res); }
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
