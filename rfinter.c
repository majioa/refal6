/************************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
       rfinter.c     - RASL interpreter
************************************************************************/
#include "refint.h"

static int breakcounter = 0;
#define brktest if(NOT breakcounter--) { breakcounter = 50; BRKTEST;}

void handint(int n)                                  /* handint */
{  while (BRKTEST) getch();
   printlv("INTERRUPT!!!\n");
   rf_error = ERRBRK;
}

#define  fail  goto tofail;

#define ret(erc) { rf_error = erc; goto end; }
#define retf(erc) { endf; ret(erc); }

#define assert(cond,erc) if(NOT(cond)) ret(erc);
#define verify(cond,erc) if(NOT(cond)) ret(erc);

#define FREEXN(n,x,y) FREEX(n,x,y)

extern LOGICAL       (*rf_subr[])();

































/* LOGICAL rf_inter(newact, long * steps, int* anel)
    newact is an element, which contains in REF(newact) the
    pointer to the element AT, which represents an active term.
    REF(AT) is the head of active term (ATH) which contains in
    REF(ATH) either NOELEM or the pointer to the subsequent AT, etc.
    When the interpreter starts a step REF(newact) becomes
    a pointer to the subsequent AT. During the step each time
    a new active term NewAT is activated, REF(newact) is copied to
    REF(REF(NewAT)) and then REF(newact) becomes NewAT. Thus a list of
    active terms is supported as being sorted in their activation order.
    Usually newact is also an element which bounds the view field,
    i.e. the head of the view field (vf is a list between newact and newact).
    if the process will be broken, it can be continued.
    Parameter steps is the pointer to the counter of steps to execute.
    If it is not zero initially, then when it exhausts the process breaks.
    If the process ends with fail
    (rf_error=ERRIMP or ERRSTOR or ERRFUN or ERRSTKO or ERRELTO),
    then newact contains the next AT,
    and the current AT can be found in tbel(0) and tbel(1).
    More exactly, tbel(0) is the AT of the main function, the following
    elements until tbel(1) are the terms of auxiliary view fields,
    which were evaluated on the path from the beginning of the main AT
    to the point of fail.
    In the case of ERRSTOR the pointer sp is wrong. */

LOGICAL rf_inter(elemptr newact, long * steps)
{
static int ins, n, m;
static LOGICAL  res;
static stelptr savsp;
static  int onel;
static  elemptr j,oexa;
static  elemptr p,p1,p2,s,h;
static LOGICAL notpl;

elemptr lastact;
elemptr bstart;  /* the beginning of the result  */
stelptr oldsp;   /* to keep old sp after PUSHVF  */
pctype  pc;      /* RASL program counter         */
elemptr b1, b2;  /* Bounds of the current hole   */
int        nel;     /* Element number (TE position) */
int        pst;     /* TPL stack position           */
headptr cfh;     /* current function headder (actual) */
adr     fbas;    /* current function body adress (only for VECTOR) */

    bstart = b;
    pst = 0;
    notpl = FALSE;
    lastact = newact; /* to avoid warnings */
    oldsp = sp;
    cfh = NOHEAD;  /* to eliminate warning "use of cfh before definition" */
    goto nextstep;








tofail:
    while(TRUE) {
        sp--;
tofail1:
        popst(b1,b2,nel,pc);
        if (pc != NOPC) break;
        switch ((sysnels)nel) {
            case _nofail: break;
            case _vfld: FREEXN(5,b1,b1); break;
            case _error: retf(ERRIMP);
            case _fail: /* just after the aux view field */
                endf;
                sp--;
                oldsp=(sp->ssp);
                tel-=(nel=sp->snel);
                unsavecfh;  /* restore function */
                sp--; /* pop line with _vfld */
                popst(b1,b2,nel,pc);
                if(pc!=0 OR nel!=_vfld)
                    { printlv("Fail: Stack error"); rf_exit(30); }
                REF(newact) = REF(REF(b1)); /* Justify newact */
                FREEXN(6,b1,b1); /* Throw out aux view field */
                fail;
            case _failinit: retf(ERRFAIL);
            case _alt: sp--; break; /* skip alt line */
            case _stake: fail;
            case _cut: {
                int failforce=1;
                for(;;) {
                    sp--;
                    popst(b1,b2,nel,pc);
                    if(pc==NOPC) switch(nel) {
                        case _vfld: FREEXN(7,b1,b1); break;
                        case _alt:  break;
                        case _cut:  failforce++; break;
                        case _stake: if(--failforce==0) fail; break;
                        default: goto tofail1;
                }    }    }
            default: printlv("IMP: Stack error"); rf_exit(30);
        }    }
restart:

#ifdef DEBUG
   if (testfl AND account != NULL) account[0]++;
#endif

while (TRUE) { EXINS;













#ifdef DEBUG
if (testfl) {
    if (account != NULL) account[ins]++;
    if (dbgfl) {
        if (lbrac != NOELEM) {
            if (NOT rf_chck(NEXT(Afreestor), PREV(Afreestor))) {
                printlv("\n*** FREE STORAGE ***\n");
                rf_exit(3);
            }    }
        else {
        if (NOT rf_chckf(NEXT(bstart), PREV(Afreestor))) {
                printlv("\n*** FREE STORAGE ***\n");
                rf_exit(3);
            }    }
        if (NOT rf_chckf(NEXT(Afreestor), bstart)) {
            printlv("\n*** VIEW FIELD   ***\n");
            rf_exit(3);
            }
        rf_dbg(b1,b2,ins);
    }    }
#endif







































switch (ins) {
    case NOP:
        goto impcode;
    case PS:
        movb1;
        if (NOT ISBRAC(b1)) fail;
        tbel(nel++) = b1;
        b2 = b1 = REF(b1);
        tbel(nel++) = b1;
        break;
    case PSR:
        movb2;
        if (NOT ISBRAC(b2)) fail;
        tbel(nel++) = b2;
        tbel(nel++) = REF(b2);
        break;
    case CSYM:
        movb1;
        if (NOT EQSA(b1)) fail;
        tbel(nel++) = b1;
        NEXTOP;
        break;
    case CSYMR:
        movb2;
        if (NOT EQSA(b2)) fail;
        tbel(nel++) = b2;
        NEXTOP;
        break;
    case SYMS:
        ARG1(n);
        while (n-- > 0) {
            movb1;
            if (NOT EQSA(b1)) fail;
            tbel(nel++) = b1;
            NEXTOP;
            }
        break;
    case SYMSR:
        ARG1(n);
        while (n-- > 0) {
            movb2;
            if (NOT EQSA(b2)) fail;
            tbel(nel++) = b2;
            NEXTOP;
            }
        break;














    case SYM:
        movb1;
        if (NOT ISSYMB(b1)) fail;
        tbel(nel++) = b1;
        break;
    case SYMR:
        movb2;
        if (NOT ISSYMB(b2)) fail;
        tbel(nel++) = b2;
        break;
    case OSYM:
        movb1;
        ARG1(n);  s = tbel(n);
        if (NOT EQSYM(b1,s)) fail;
        tbel(nel++) = b1;
        break;
    case OSYMR:
        movb2;
        ARG1(n);  s = tbel(n);
        if (NOT EQSYM(b2,s)) fail;
        tbel(nel++) = b2;
        break;
    case TERM:
        movb1;
        tbel(nel++) = b1;
        break;
    case TERMR:
        movb2;
        tbel(nel++) = b2;
        break;
    case OTERM:
        savsp = sp;
        ARG1(n);
        oexa = tbel(n);
        j = PREV(oexa);
        goto oldexp;
    case OTERMR:
        savsp = sp;
        ARG1(n);
        onel = nel++;
        oexa = tbel(n);
        j = NEXT(oexa);
        goto oldexpr;
    case EXP:
        tbel(nel++) = NEXT(b1);
        tbel(nel++) = PREV(b2);
        break;
    case EMP:
        if (NEXT(b1)!=b2) fail;
        break;










    case OEXP:
        savsp = sp;
        ARG1(n);
        tbel(nel++) = NEXT(b1);
        j = PREV(tbel(n-1)); oexa = tbel(n);
oldexp:    for(;;) {
            while (j != oexa) {
                b1 = NEXT(b1);
                if (b1 == b2) { sp = savsp; fail; }
                j = NEXT(j);
                if (ISBRAC(b1)) {
                  if (ISBRAC(j)) {
                    pushe(b1,b2); pushe(j,oexa);
                    b1 = b2 = REF(b1);
                    j = REF(j);    oexa = PREV(j);
                    continue;
                  }    }
                else if (EQSYM(b1,j)) continue;
                sp = savsp;
                fail;
                }
            if (sp == savsp) break;
            if (NEXT(b1) != b2) { sp = savsp; fail; }
            pope(j,oexa); pope(b1,b2);
            }
        tbel(nel++) = b1;
        break;
    case OEXPR:
        savsp = sp;
        ARG1(n);
        onel = nel++;
        tbel(nel++) = PREV(b2);
        j = NEXT(tbel(n)); oexa = tbel(n-1);
oldexpr: for(;;) {
            while (j != oexa) {
                b2 = PREV(b2);
                if (b2 == b1) { sp = savsp;  fail; }
                j = PREV(j);
                if (ISBRAC(b2)) {
                  if (ISBRAC(j)) {
                    pushe(b1,b2); pushe(j,oexa);
                    b1 = b2 = REF(b2);
                    j = REF(j);    oexa = NEXT(j);
                    continue;
                  }    }
                else if (EQSYM(b2,j)) continue;
                sp = savsp;
                fail;
                }
            if (sp == savsp) break;
            if (NEXT(b1) != b2) { sp = savsp; fail; }
            pope(j,oexa); pope(b1,b2);
            }
        tbel(onel) = b2;
        break;





    case SETB:
        ARG1(n);
        b1 = tbel(n);
        ARG2(m);
        b2 = tbel(m);
        break;
    case CHKB:
        ARG1(n);
        if(VAL(REF00(tbel(n)))!=1) notpl = TRUE;
        break;
    case PLEN:
        pushst(b1,b2,nel,pc);
        sp++;
        tbel(nel++) = NEXT(b1);
        tbel(nel++) = b1;
        NEXTOP;
        break;
    case PLENS:
        pushst(b1,b2,nel,pc);
        tbel(nel) = NEXT(b1);
        tbel(nel+2) = b1;
        break;
    case LEN:
        b1 = tbel(++nel);
        movb1;
        tbel(nel) = b1;
        sp++;
        nel++;
        break;
    case LENS:
        b1 = tbel(nel+2);
        do { movb1; } while (NOT EQSA(b1));
        tbel(++nel) = PREV(b1);
        tbel(++nel) = b1;
        nel++;
        sp++;
        NEXTOP;
        break;
    case LENP:
        b1 = tbel(nel+2);
        do { movb1; } while (NOT ISBRAC(b1) );
        tbel(++nel) = PREV(b1);
        tbel(++nel) = b1;
        b1 = b2 = tbel(++nel) = REF(b1);
        nel++;
        sp++;
        break;
    case LENOS:
        ARG1(n);
        s = tbel(n);
        b1 = tbel(nel+2);
        do { movb1; } while (NOT EQSYM(s,b1));
        tbel(++nel) = PREV(b1);
        tbel(++nel) = b1;
        nel++;
        sp++;
        break;



    case BL:
        bl;
        break;
    case BR:
        br;
        break;
    case BLR:
        blr;
        break;
    case MULT:
        ARG1(n);
        mult(n);
        break;
    case MULE:
        ARG1(n);
        m = n-1;
        goto mulmn;
    case MUL:
        ARG1(m);
        ARG2(n);
mulmn:
        p = PREV(tbel(m));
        p1 = tbel(n);
        while (p != p1) {
            movb;
            p = NEXT(p);
            MKINF(b,p);
            if(NOT ISSIMPL(b)) ++VAL(REF00(b));
            }
        break;
    case TPLE:
        ARG1(n);
        m=n-1;
        goto tplmn;
    case TPL:
        ARG1(m);
        ARG2(n);
tplmn:
        if(notpl) { notpl=FALSE; goto mulmn; }
        tpl(m,n);
        break;
    case NS:    tons:
        movb;
        MKTA;
        NEXTOP;
        if (NOT ISSIMPL(b))
              ++VAL(REF00(b));
        break;
    case TEXT:
        ARG1(n);
        while (n--) {
            movb;
            MKTA;
            if (NOT ISSIMPL(b)) ++VAL(REF00(b));
            NEXTOP;
            }
        break;



    case BLNS:
        bl;
        goto tons;
    case BRACT:
        br;
        ARG1(n); /* 0 or 1 :=: <strong call> or [soft call] */
        h = REF(b);
        SETREF(h,REF00(lastact),n & modacts);
        SETREF00(lastact,b);
        lastact = h;
        break;
    case BUILTIN:
        ARG1(n);
        if (NEXT(b1) == b2) pushe(NOELEM,NOELEM)
        else pushe(NEXT(b1),PREV(b2));
        pushae(EresL,EresR);
        tel+=nel;
        level++;
        res = (*rf_subr[n])();
        level--;
        tel-=nel;
        switch(res) {
           case FAIL: fail;
           case FALSE: retf(rf_error==0? ERRIMP : rf_error);
           }
        weld(b1,b2);
        if (EresL == NOELEM) break;
        AFTER(b,EresL,EresR);
        b = EresR;
        break;
    case RETURN:  retf(ERREXIT);





























    case EVAL:          /* EVAL(n); n is the number of ":" */
        ARG1(n);
        extpl;
        if (tracefl) rf_trace2(bstart,b);
    /* insert RESULT in <n f ...> after tbel(0) */
        movb; p = b;
        movb; SETSIM(b,n,TYPESNUMB);
        { headptr hf=REF00(tbel(3));
          if(TYPE(hf)!=HEADFUNC) hf=applyobject;
          COPREF(hf);
          }  /* 11.8.93 */
        CROSS(bstart,b); b = bstart;
        movbx(b,s);
        SETBRAC(s,p);
        AFTERX(PREV(tbel(1)),s,s);
    /* make line to throw out view field in case of IMP */
        pushst(s,NOELEM,_vfld,NOPC);
        sp++;
    /* Activate s [ p = REF(s) ]*/
        SETACT(p, REF00(lastact));
        if(lastact==newact) {  /* Activate immediately */
            tbel(nel)=s;
            tbel(nel+1)=NEXT(s);
            tbel(nel+2)=b2=p;
/*            tbel(nel+3)=b1=NEXT(b2);                 11.8.93 */
            tbel(nel+3)=b1=NEXT(NEXT(b2));        /* 11.8.93 */
            if (tracefl) rf_trace1(tbel(nel));
            nel=nel+4;
            break;
            }
        SETREF00(lastact,s);
        lastact = p;
        savecfh;        /* save current function headder */
        (sp->snel) = nel;    /* save(oldsp,nel,pc) */
        (sp -> ssp) = oldsp;
        (sp -> spc) = pc;
        tel += nel;
        sp++;
        oldsp = sp;
        assert (tel < telmax, ERRELTO);
        assert (spe < spemax, ERRSTKO);
        goto testbrk;
    case EST:
        extpl;
        if (tracefl) rf_trace2(bstart,b);
        endf;
        p1 = tbel(0);
        p = PREV(p1);
        /* Throw out Active term with aux view fields */
        /* Move nonempty list of AT's and VF's to free storage */
        /* Make brackets of all AT's and VF's */
        p2=tbel(1);
        FREEXN(4,p1,PREV(p2));
        /* insert RESULT */
        if (b != bstart) { AFTERX(p, NEXT(bstart), b); b = bstart; }
        sp = oldsp;



testbrk:
        brktest;
/*DEBUG    if(         )rf_error=ERRDATA;  */
        if(*steps>0) if(--*steps==0) rf_error=ERRSTEP;
        if(rf_error!=0) goto end;
nextstep:    /* bstart==b, sp == oldsp */
        verify( (tbel(0)=REF(newact))!=NOELEM , ERREND);
        tbel(1) = NEXT(tbel(0));
        tbel(2) = b2 = REF(tbel(0));
        nel = 4;
        REF(newact) = REF00(b2);
           step++;
        lastact = newact;
        if (tracefl)
        rf_trace1(tbel(0));
        tbel(3) = b1 = NEXT(b2);
        verify( b1!=b2 , ERRFUN );
        if ( (TYPE(b1)&TYPESIMPL)==0 ) { /* ISBRAC OR ISREF */
            if(TYPE(cfh=REF00(b1)) != HEADFUNC) {
                b1=PREV(b1);
                cfh = applyobject;
                if(TYPE(cfh)!=HEADFUNC) ret(ERRFUN);
                }
            sp->spc=NOPC;
            switch (STYPE(b2)) {
              case HEADACT:     sp->snel=_error;    break;
              case HEADVACT:    sp->snel=_fail;     break;
/*              case HEADINITACT: sp->snel=_failinit; break; */
              default:     sp->snel=_error;
              };
            sp++;
            startf;
            expc;
            if (chkpc) break; /* start execution */
            endf;
            }
        else
        if(ISSIMPL(b1)) {
            /* accept the result of EVALVF and continue */
            verify(ISSNUMB(b1), ERRFUN);
            tbel(3)=b1=NEXT(b1);           /* 11.8.93 */
            sp--;
            oldsp = (sp -> ssp);      /* unsave(oldsp,nel,pc) */
            tel -= (nel = sp->snel);
            pc = (sp -> spc);
            unsavecfh;   /* unsave current function headder */
            nel = nel+4;
            break;
            }
        ret(ERRFUN);










    case STAKE:
        sp->spc = NOPC;
        sp->snel = _stake;
        sp++;
        break;
    case CUT:
        sp->spc = NOPC;
        sp->snel = _cut;
        sp++;
        break;
    case DO:
todo:
        sp->spc = NOPC;
        sp->snel = _error;  /* pushst(X,X,imp,NOPC);  */
        sp++;
        break;
    case UNDO:
        while (TRUE) {
            sp--;
            if(sp->spc==NOPC) {
                if (sp->snel==_error) break;
                if (sp->snel==_vfld) {
                    sp--;
                    if(sp->spc==NOPC AND sp->snel==_error) {
                        sp->snel=_nofail;
                        sp=sp+2;
                        break;
                    }    }
                printlv("UNDO: Stack error"); rf_exit(30);
            }    }
        break;
    case BRANCH:
        if(NOT TEST_BRANCH) goto impcode;
        pushst(b1,b2,nel,BRANCH2);
        sp++;
        pc = BRANCH1;
        break;
    case BRANCHR:
        if(NOT TEST_BRANCH) goto impcode;
        pushst(b1,b2,nel,BRANCH1);
        sp++;
        pc = BRANCH2;
        break;

















    case ALT:
        if(NOT TEST_BRANCH) goto impcode;
        pushst(b1,b2,nel,BRANCH2);  /* A continuation */
        sp++;
        sp->spc = NOPC;
        sp->snel = _alt;   /* pushst(X,X,alt,NOPC) */
        sp++;
        pc = BRANCH1;
        break;
    case END:
        extpl;
        while (TRUE) {
            sp--;
            popst(b1,b2,nel,pc);
            if(pc==NOPC) {
                if (nel==_vfld) FREEXN(8,b1,b1); else
                if (nel==_alt) break;
                }
            }
        fail;  /* To the continuation */
    case FAILS: fail;
    case BLOCK: if(modacts==0) goto todo;
                else break;

    default:
        printlv("INT: ");
        fprintf(stdtrc,"*** Invalid RASL op-code: %d\n",ins);
        retf(ERRIMP);
impcode:
        printlv("INT: ");
        fprintf(stdtrc,"*** Improper RASL code: %d\n",ins);
        retf(ERRIMP);
    }    }

end:
    sp = oldsp;
    b = bstart;
    switch (rf_error) {
      case ERREND:
      case ERREXIT: return(TRUE);
      case ERRFAIL: return(FAIL);
      default: return(FALSE);
      }
    }
