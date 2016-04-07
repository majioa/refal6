/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1993 by Arkady Klimov            */
/*       rbar.c    - arithmetic built-in functions                      */
/************************************************************************/
/*                   List of functions                                  */
/*E    <COMPARE a b> == '<' | '=' | '>'   ,     rb_compare             */
/*E    <ADD a b> == .sum                  ,     rb_add                 */
/*E    <SUB a b> == .dif                  ,     rb_sub                 */
/*E    <MUL a b> == .prod                 ,     rb_mul                 */
/*E    <DIV a b> == .quot                 ,     rb_div                 */
/*E    <MOD a b> == .rem                  ,     rb_mod                 */
/*E    <DIVMOD s1 s2> == s.quot s.rem     ,     rb_divmod              */
/*E    <GSDSTEP a b> == c d i j k l       ,     rb_gcdstep             */
/*        where c=i*a+k*b; d=j*a+l*b                                   */
/*E    <TIME> == 'DD MMM YYYY HH.MM.SS'   ,     rb_time                */
/*E    <TIME_ELAPSED> == .ticks           ,     rb_time_elapsed        */
/*E    <SHORT_INT  n> == i                ,     rb_short_int           */
/*E    <INT        n> == i                ,     rb_int                 */
/*E    <SHORT_REAL n> == r                ,     rb_short_real          */
/*E    <REAL       n> == r                ,     rb_real                */
/*E    <ABS  n> == n1                     ,     rb_abs                 */

#include <time.h>
#include "refgen.h"
#include "rfalloc.h"
#include "arithm.h"
#include "rbarm.h"
/* #include "rfarm.c" */

/* make object of type REAL */
LOGICAL rf_mk_hreal(real r, headptr *ah)
/* result *ah is actual (and STARTED) */
{       headptr h;
        movbx(b,h);
        weld(b,NEXT(h));
        VAL(h)=1;
        TYPE(h)=HD(REAL,CON);
        real2head(r,h);
        *ah=h;
        return TRUE;
        }

/* make cvalue for real number r */
LOGICAL rf_mk_creal(double x, cvalue* ac, LOGICAL simple)
{
   if(simple) {
        float f = (float)x;
        float2cvalue(f,*ac);
        }
   else {
        headptr h;
		real r = x;
        ECALL( rf_mk_hreal(r,&h) );
        *ac = CMKREF(h,TYPELVAL);
        }
   return TRUE;
   };

/* cvalue n -> numbermode m, long l, real r */
#define NormNumb(n,m,l,r) {                                       \
      int t = CTYPE(n); headptr h; float f;                       \
           if (IS_SNUMB(t)) { cvalue2int(n,l); m=_SINT; }         \
      else if (IS_REAL(t)) { cvalue2float(n,f); r=f; m=_SREAL; }  \
      else if (IS_REF(t))                                         \
        switch(MODE(TYPE(h=CREF00(n)))) {                         \
            case MASK: m=_LINT; break;                            \
			case REAL: head2real(h,r); m=_LREAL; break;           \
            default: m=_OTHER_NUMBER;                             \
            }                                                     \
      else m=_OTHER_NUMBER;                                       \
      }

LOGICAL rf_arop(cvalue n1, cvalue n2, optype op, cvalue* ac, int nres)
/*
   n1,n2 - операнды виртуальные,
   op - операция: _ADD, _SUB, _MUL, _REL, _DIV, _MOD, _DIVMOD,
                 _AND, _XOR, _OR;
   _DIV, _DIVMOD -
        для вещ чисел делит нацело и выдает частное и остаток
   _REL - "отношение" - для целых определена только когда делится нацело
   ac - массив для результатов актуальных длины 1 или 2
   nres - длина массива ac (1 или 2); nres=2 только для op='\\'.
   счетчик ссылок 1 для n1 и n2 означает, что их содержимые можно
   использовать, изменяя, для формирования результатов
*/
{
  int res = TRUE, i;
  long L[6];
  long l, l1, l2;
  numbermode m1, m2, m;
  real r1, r2;

  NormNumb(n1,m1,l1,r1);
  NormNumb(n2,m2,l2,r2);

  switch(m=max(m1,m2)) {
     case _SINT: {
          /* sint x sint x op -> lint */
          switch(op) {
              case _ADD    : l = l1+l2; break;
              case _SUB    : l = l1-l2; break;
			  case _MUL    : l = l1*l2; break;
			  case _MOD: case _DIV: case _DIVMOD: case _GCDSTEP:
				 if(l2==0) return(FAIL);
				 switch(op) {
					case     _MOD : l    = l1%l2; break;
					case  _DIVMOD : L[1] = l1%l2;
					case     _DIV : l    = l1/l2; break;
					case _GCDSTEP : { long p1=1,p2=0,q1=0,q2=1,p,q,k;
						while (l2!=0) { k=l1/l2;
                            l=l1-k*l2; l1=l2; l2=l;
                            p=p1-k*p2; p1=p2; p2=p;
                            q=q1-k*q2; q1=q2; q2=q;
                            };
                        l=l1; L[1]=l2; L[2]=p1; L[3]=p2; L[4]=q1; L[5]=q2;
                        }
					} break;
              case _AND : l = l1&l2; break;
              case _XOR : l = l1^l2; break;
              case _OR  : l = l1|l2; break;
              default: return FAIL;
			  };
          L[0]=l;
		  ALL(i,nres) if((res=rf_mk_cnumb(L[i], ac+i, TRUE))!=TRUE) return res;
          break;
          };
     case _LINT: res=rf_int_op(n1,n2,op,ac,nres); break;
     case _SREAL:
     case _LREAL: { real r;
          if(m1<_SREAL) rf_cgetreal(n1,&r1);
          if(m2<_SREAL) rf_cgetreal(n2,&r2);
          /* real x real x op -> real */
          switch(op) {
			  case _ADD: r = r1+r2; break;
			  case _SUB: r = r1-r2; break;
			  case _MUL: r = r1*r2; break;
			  case _DIV: r = r1/r2; break;
			  case _MOD: r = fmod((double)r1,(double)r2); break;
			  case _DIVMOD: { double d;
                 r = fmod((double)r1,(double)r2);
                 if((res=rf_mk_creal(r,ac+1,m==_SREAL))!=TRUE) return res;
			     modf(r1/r2,&d); r = d; break;
                 }
              default: return FAIL;
              };
		  res=rf_mk_creal(r,ac,m==_SREAL);
		  break;
          };
     case _OTHER_NUMBER: return FAIL;
     };
  return res;
  };

/******************************************************************/
/*     Real math functions                                        */

#define NAF 14
typedef enum {
 _Abs, _Sign,
 _Sqrt, _Exp, _Log,
 _Sin, _Asin, _Sinh,
 _Cos, _Acos, _Cosh,
 _Tan, _Atan, _Tanh
 } aftype;

static cvalue AF[NAF];

static char* AFS[NAF] = {
 "Abs", "Sign",
 "Sqrt", "Exp", "Log",
 "Sin", "Asin", "Sinh",
 "Cos", "Acos", "Cosh",
 "Tan", "Atan", "Tanh"
 };

static int res;

LOGICAL rf_func(aftype fun, cvalue arg, cvalue* ares)
/*
   Вычисление числовой функции fun одного числового аргумента:
   Sign: Int -> Sint
         Real -> Sint
   Abs:  Int -> Int
         Sreal -> Sreal
         Lreal -> Lreal
   arg - аргумент виртуальный
   ares - результат актуальный
*/
{
  real r;
  long l;
  int s;
  int t = CTYPE(arg);
  headptr h;
  float f;
  res = TRUE;
       if (IS_SNUMB(t)) { cvalue2int(arg,l); s=(l<0?-1:(l>0?1:0)); }
  else if (IS_REAL(t)) { cvalue2float(arg,f); r=f; s=(r<0?-1:(r>0?1:0)); }
  else if (IS_REF(t))
    switch(MODE(TYPE(h=CREF00(arg)))) {
        case MASK: s=(ra_sign(h)==0 ? 1 : -1); break;
        case REAL: head2real(h,r); r=f; s=(r<0?-1:(r>0?1:0)); break;
        default: return FAIL;
        }
  else return FAIL;

  switch (fun) {
     case _Sign: *ares = CMKSIM(s,TYPESNUMB); break;
	 case _Abs:  res = rf_arop(CMKSIM(s,TYPESNUMB),arg,_MUL,ares,1); break;
     default: res = FAIL;
     };
  return res;
}

LOGICAL rf_rfunc(aftype fun, cvalue arg, cvalue* ares)
/*
   Вычисление вещественной функции fun одного вещественного аргумента
   fun =
         Sin  | Cos  | Tg  |
         Asin | Acos | Atg |
         Sinh | Cosh | Tgh |
         Sqrt | Exp  | Log
   arg - аргумент виртуальный
   ares - результат актуальный
*/
{
  real a; double aa,rr;
  res = TRUE;

  RCALL(res,rf_cgetreal(arg,&a));
  aa=a;
  switch(fun) {
      case _Sqrt : rr = sqrt(aa); break;
      case _Exp  : rr = exp (aa); break;
      case _Log  : rr = log (aa); break;
      case _Sin  : rr = sin (aa); break;
      case _Asin : rr = asin(aa); break;
      case _Sinh : rr = sinh(aa); break;
      case _Cos  : rr = cos (aa); break;
      case _Acos : rr = acos(aa); break;
      case _Cosh : rr = cosh(aa); break;
      case _Tan  : rr = tan (aa); break;
      case _Atan : rr = atan(aa); break;
	  case _Tanh : rr = tanh(aa); break;
      default: return FAIL;
	  };
  if(res!=TRUE) return res;
  return rf_mk_creal(rr, ares, IS_SIMPL(CTYPE(arg)) );
  };

int matherr(struct _exception *a)
{
   res = FAIL;
   return 1;
}

LOGICAL rb_func ()
{
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr savb;
   cvalue c,cfun;
   LOGICAL res;
   int fun;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb = b;
   if (EargL == NULL OR EargL == EargR OR NEXT(EargL) != EargR )
	  return(FAIL);
   cfun = DATA(EargL);
   ALL(fun,NAF) if(AF[fun]==cfun) break;
   switch (fun) {
     case _Abs:
     case _Sign: RCALL( res, rf_func (fun, DATA(EargR), &c) ); break;
        default: RCALL( res, rf_rfunc(fun, DATA(EargR), &c) );
     }
     movb;
     SETDATA(b,c);
   *AeresL = NEXT(savb); *AeresR = b;
   BETWEEN(savb,NEXT(b),EargL,EargR);
   b = savb;
   return (TRUE);
   }

LOGICAL rb_fun (int fun)
{
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr savb;
   cvalue c;
   LOGICAL res;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb = b;
   if (EargL == NULL OR EargL != EargR ) return(FAIL);
   switch (fun) {
     case _Abs:
     case _Sign: RCALL( res, rf_func (fun, DATA(EargR), &c) ); break;
        default: RCALL( res, rf_rfunc(fun, DATA(EargR), &c) );
     }
     movb;
     SETDATA(b,c);
   *AeresL = NEXT(savb); *AeresR = b;
   BETWEEN(savb,NEXT(b),EargL,EargR);
   b = savb;
   return (TRUE);
   }

rb_abs () { return rb_fun(_Abs ); };
rb_sign() { return rb_fun(_Sign); };
rb_sqrt() { return rb_fun(_Sqrt); };
rb_exp () { return rb_fun(_Exp ); };
rb_log () { return rb_fun(_Log ); };
rb_sin () { return rb_fun(_Sin ); };
rb_asin() { return rb_fun(_Asin); };
rb_sinh() { return rb_fun(_Sinh); };
rb_cos () { return rb_fun(_Cos ); };
rb_acos() { return rb_fun(_Acos); };
rb_cosh() { return rb_fun(_Cosh); };
rb_tan () { return rb_fun(_Tan ); };
rb_atan() { return rb_fun(_Atan); };
rb_tanh() { return rb_fun(_Tanh); };

void initar() { if(NOT INIWORDS(NAF,AF,AFS)) rf_err(ERCINIT); }


/*****************************************************************/
/* Number converting
   There are four number modes: ShortInt, Int, ShortReal, Real.
   Mode Int includes ShortInt, whereas modes Real and ShortReal are distinct.
   Whether element of Int is ShortInt depends only on its value.
   Converting to ShortInt implies converting to Int and then taking
   modulo MaxShortInt-MinShortInt+1 in the range of ShortInt.
   The sign can be lost.
   Converting to Int does not change Int or ShortInt, and takes the whole
   part of [Short]Real.
   Converting to [Short]Real implies converting to Real and then
   optional converting to ShortReal, which implies loosing accuracy.
   */

static
LOGICAL rf_ar_int(cvalue n, cvalue* ac, numbermode m)
        /* n is virtual, *ac is actual */
        /* m is either _SINT or _LINT  */
{
    LOGICAL res;
    int t=CTYPE(n);
    real r; double x; MPOS X; seglen lX; adr adX;
    headptr h; float f;
           if (IS_SNUMB(t)) { *ac = n; return TRUE; }
      else if (IS_REAL(t)) { cvalue2float(n,f); r=(real)f; }
      else if (IS_REF(t))
        switch(MODE(TYPE(h=CREF00(n)))) {
            case MASK: if(m==_SINT) {
                         START(h);
                         *ac= CMKSIM(*MFIRST(h),TYPESNUMB);
                         END(h);
                         }
                       else { *ac=n; VAL(h)++; }
                       return TRUE;
            case REAL: head2real(h,r); break;
            default: return FAIL;
            }
      else return FAIL;
      if(m==_SINT) {
         ushort C;
		 ra_double2int((double)r,&C,1);
         *ac= CMKSIM(C,TYPESNUMB);
         return TRUE;
         };
      /* Evaluate lX */
      for(x=r,lX=1; x<-1.0 OR x>=1.0 ; lX++) x/=base;
	  ECALL(ALLOC(adX,MSIZE(lX))!=NOADR)
      X = (MPOS)SEGINF(adX);
	  ra_double2int((double)r,X,lX);
      res = rf_mk_numb(X,lX,ac,TRUE);
      REALLOC(adX,0);
      return(res);
}

LOGICAL rb_ar_conv(numbermode m)
{
  cvalue c;
  LOGICAL res;
  elemptr EargL, EargR;
  elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL != EargR) return FAIL;
   c = DATA(EargL);          /* c is virtual */
   switch(m) {
      case _SINT:
      case _LINT:  RCALL(res,rf_ar_int(c,&c,m)); break;
      case _SREAL:
      case _LREAL: {
           real r;
           RCALL(res,rf_cgetreal(c,&r));
           RCALL(res,rf_mk_creal(r,&c,m==_SREAL));
           break;
           }
      }
   elemfree(EargL);
   SETDATA(EargL,c);
   *AeresL=*AeresR=EargL;
   return TRUE;
 };
LOGICAL rb_short_int  () { return rb_ar_conv(_SINT ); };
LOGICAL rb_int        () { return rb_ar_conv(_LINT ); };
LOGICAL rb_short_real () { return rb_ar_conv(_SREAL); };
LOGICAL rb_real       () { return rb_ar_conv(_LREAL); };

LOGICAL rb_arop (char op, int nres)
{
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr savb;
   cvalue c[6];
   LOGICAL res;
   int i;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb = b;
   if (EargL == NULL OR EargL == EargR OR NEXT(EargL) != EargR )
	  return(FAIL);
   RCALL( res, rf_arop(DATA(EargL), DATA(EargR), op, c, nres) );
   ALL(i,nres) {
     movb;
     SETDATA(b,c[i]);
     };
   *AeresL = NEXT(savb); *AeresR = b;
   BETWEEN(savb,NEXT(b),EargL,EargR);
   b = savb;
   return (TRUE);
   }

LOGICAL rb_add () { return rb_arop(_ADD,1); };
LOGICAL rb_sub () { return rb_arop(_SUB,1); };
LOGICAL rb_mul () { return rb_arop(_MUL,1); };
LOGICAL rb_div () { return rb_arop(_DIV,1); };
LOGICAL rb_mod () { return rb_arop(_MOD,1); };
LOGICAL rb_divmod  () { return rb_arop(_DIVMOD,2); };
LOGICAL rb_gcdstep () { return rb_arop(_GCDSTEP,6); };
LOGICAL rb_and () { return rb_arop(_AND,1); };
LOGICAL rb_xor () { return rb_arop(_XOR,1); };
LOGICAL rb_or  () { return rb_arop(_OR ,1); };


LOGICAL rb_compare()
{
   int c;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   elemptr savb;
   savb = b;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL OR EargL == EargR OR NEXT(EargL) != EargR)
		 return(FAIL);
   c = rf_cmp(EargL,EargL,EargR,EargR);
   movb;
	 if (c >0) SETSIM(b,'>',TYPECHAR);
	 if (c <0) SETSIM(b,'<',TYPECHAR);
	 if (c==0) SETSIM(b,'=',TYPECHAR);
	 *AeresL = *AeresR = b;
   BETWEEN(savb,NEXT(b),EargL,EargR);
   b = savb;
   return (TRUE);
}

LOGICAL rb_time()
{
   time_t t;
   uchar * stim;
   elemptr savb;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb = b;
   if (NOT(EargL == NULL)) return(FAIL); EargR++;
   time(&t);
   stim = (uchar*)ctime(&t);
   if (NOT rf_mkstr(&stim[ 8], 3)) return(FALSE);  /* DD_  */
   if (NOT rf_mkstr(&stim[ 4], 4)) return(FALSE);  /* MMM_ */
   if (NOT rf_mkstr(&stim[20], 4)) return(FALSE);  /* YYYY */
   if (NOT rf_mkstr(&stim[10], 9)) return(FALSE);  /* _HH:MM:SS */
   *AeresL = NEXT(savb);
   *AeresR = b;
   weld(savb,NEXT(b));
   b = savb;
   return (TRUE);
}

#ifdef EXIST_SYS_TIMEB_H

#include <sys\timeb.h>
static long time_ticks()
{  struct timeb ltime;
   ftime(&ltime);
   return (ltime.time*100 + ltime.millitm/10);
}

#else

static long time_ticks()
{  time_t ltime;
   time(&ltime);
   return (ltime*100);
}

#endif

static long savtime;
void initime() { savtime = time_ticks(); };

LOGICAL rb_time_elapsed ()
{
   elemptr savb;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   savb = b;
   if (NOT(EargL == NULL)) return(FAIL); EargR++;
   rf_mknumb(time_ticks()-savtime);
   *AeresL = NEXT(savb);
   *AeresR = b;
   weld(savb,NEXT(b));
   b = savb;
   return (TRUE);
}
