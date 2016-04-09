/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1993 by Arkady Klimov            */
/*       rbarm.h       - macros and headders for rbarm.c                */
/************************************************************************/
#ifndef __RBARM_H
#define __RBARM_H

#include "arithm.h"

#define cvalue2int(c,i) i=(int)(short)CVAL(c)
#define cvalue2float(c,f) \
         *(long*)(&(f))=((*(long*)(&(c)))&(ALLONES-REALSEL))
#define float2cvalue(f,c) \
         *(long*)(&(c))=((*(long*)(&(f)))&(ALLONES-REALSEL)) | TYPEREAL
#define head2real(h,r) { ((elemptr*)(&(r)))[0]=PREV(h);\
                         ((elemptr*)(&(r)))[1]=NEXT(h); }
#define real2head(r,h) { PREV(h)=((elemptr*)(&(r)))[0];\
                         NEXT(h)=((elemptr*)(&(r)))[1]; }

typedef enum { _SINT, _LINT, _SREAL, _LREAL, _OTHER_NUMBER } numbermode;

/* make object of type REAL */
LOGICAL rf_mk_hreal(real r, headptr *ah);
/* result *ah is actual (and STARTED) */

/* make cvalue for real number r */
LOGICAL rf_mk_creal(double x, cvalue* ac, LOGICAL simple);

LOGICAL rf_arop(cvalue n1, cvalue n2, optype op, cvalue* ac, int nres);
/*
   n1,n2 - операнды виртуальные,
   op - операция: _ADD, _SUB, _MUL, _DIV, _MOD, _DIVMOD, _AND, _XOR, _OR;
   _DIVMOD -
        для вещ чисел делит нацело и выдает частное и остаток
   ac - массив для результатов актуальных длины 1 или 2
   nres - длина массива ac (1 или 2); nres=2 только для op='\\'.
   счетчик ссылок 1 для n1 и n2 означает, что их содержимые можно
   использовать, изменяя, для формирования результатов
*/
/* static LOGICAL rf_ar_int(cvalue n, cvalue* ac, numbermode m); */
        /* n is virtual, *ac is actual */
        /* m is either _SINT or _LINT  */

LOGICAL rf_int_op (cvalue n1, cvalue n2, optype op, cvalue* ac, int nres);
/*
   n1,n2 - операнды виртуальные типа ShortInt или Int,
   op - операция: _ADD, _SUB, _MUL, _DIV, MOD, _DIVMOD
        _DIVMOD - деление для получения и частного (ac[0]) и остатка (ac[1]),
   ac - массив для результатов длины 1 или 2
   nres - длина массива ac (1 или 2); nres=2 только для op='\\'.

   счетчик ссылок 1 для n1 и n2 означает, что их содержимые можно
   использовать, изменяя, для формирования результатов
   */

LOGICAL rf_cgetreal(cvalue n, real* ar);
        /* n is virtual */
        /* m is either _SREAL or _LREAL */

/* Test type of cc and get long integer to n */
LOGICAL rf_cgetnumb(cvalue cc, long * an);

/* Test type of p and get long integer to n */
LOGICAL rf_getnumb ( elemptr p, long * an);

#endif