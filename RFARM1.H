/************************************************************************/
/*                             R E F A L - 6                            */
/*  Copyright (C) 1996 by Arkady Klimov                                 */
/*       rfarm.h     - arithmetic function                              */
/************************************************************************/

#include "refgen.h"
#include "rfalloc.h"
#include "arithm.h"
#include "rbarm.h"

LOGICAL rf_int_op (cvalue n1, cvalue n2, optype op, cvalue* ac, int nres);
/*
   n1,n2 - операнды виртуальные типа ShortInt или Int,
   op - операция: _ADD, _SUB, _MUL, _DIV, MOD, _DIVMOD
        _DIVMOD - деление для получения и частного (ac[0]) и остатка (ac[1]),
   ac - массив для результатов длины 1 или 2
   nres - длина массива ac (от 1 до 6); 2 для op='\\', 6 для _GCDSTEP.

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
