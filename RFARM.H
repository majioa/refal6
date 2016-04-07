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
   n1,n2 - ���࠭�� ����㠫�� ⨯� ShortInt ��� Int,
   op - ������: _ADD, _SUB, _MUL, _DIV, MOD, _DIVMOD
        _DIVMOD - ������� ��� ����祭�� � ��⭮�� (ac[0]) � ���⪠ (ac[1]),
   ac - ���ᨢ ��� १���⮢ ����� 1 ��� 2
   nres - ����� ���ᨢ� ac (�� 1 �� 6); 2 ��� op='\\', 6 ��� _GCDSTEP.

   ���稪 ��뫮� 1 ��� n1 � n2 ����砥�, �� �� ᮤ�ন�� �����
   �ᯮ�짮����, �������, ��� �ନ஢���� १���⮢
   */

LOGICAL rf_cgetreal(cvalue n, real* ar);
        /* n is virtual */
        /* m is either _SREAL or _LREAL */

/* Test type of cc and get long integer to n */
LOGICAL rf_cgetnumb(cvalue cc, long * an);

/* Test type of p and get long integer to n */
LOGICAL rf_getnumb ( elemptr p, long * an);
