/************************************************************************/
/*                             R E F A L - 6                            */
/*  Copyright (C) 1993 by Arkady Klimov                                 */
/*       arithm.h     - arithmetic routines (function headders)         */
/************************************************************************/
#ifndef __ARITHM_H
#define __ARITHM_H
#include "refcom.h"
#include <math.h>
#define MINUS ((ushort)(-1))
#define PLUS  0
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define MAXLONGINT ((-1L)>>1)
#define MINLONGINT ((-1L)-MAXLONGINT)
#define HALF(L,n)  (((ushort*)&L)[n])
#define HALF0(L) ((ushort)(L))
#define HALF1(L) ((ushort)((L)>>16))
#define BitsShort 16
#define MBASE  (long)(1L<<BitsShort)

#define GetMantissa(X,lX) ( ((ulong) ((X)[(lX)-1])) << BitsShort ) \
            + (ushort) ( (lX)<2 ? 0 : (X)[(lX)-2] )

typedef enum { _ADD, _SUB, _MUL, _DIV, _MOD,
               _DIVMOD, _GCDSTEP, _AND, _XOR, _OR } optype;

/* Умножение на цифру:  C = B*k  */
void ra_mul1( ushort B[], seglen lB, ushort k, ushort C[], seglen* alC);

/* Смена знака:  C = - B */
void ra_minus( ushort B[], seglen lB,
               ushort C[], seglen mlC, seglen* alC);

/* Сложение и вычитание:  C = C + B*sign */
#define ra_ads(B,lB,sign,C,mlC,alC) \
        ra_adm(B,lB,1,sign,C,mlC,alC)

#ifndef ra_ads
void ra_ads(  ushort B[], seglen lB, int sign,
              ushort C[], seglen mlC, seglen* alC);
#endif

/* Основная рабочая функция:  C = C + B*k*sign */
void ra_adm(  ushort B[], seglen lB, ushort k, int sign,
              ushort C[], seglen mlC, seglen* alC);

/* Умножение длинных чисел: C = A*B */
void ra_mul( ushort A[], seglen lA,
             ushort B[], seglen lB,
             ushort C[], seglen* alC);
/* Массив C должен иметь не менее lA+lB элементов */
/* Результат может остаться ненормализованным (когда A=0) */

/* Деление длинных чисел:  C = A / D, A = A % D */
/* Делитель D больше нуля, нормализован */
/* Остаток будет на месте делимого */
/* Делимое (если отрицательное) может удлиниться на 1 (или до lD+1) */
void ra_divmod( ushort A[], seglen* alA /* вход-выход */,
             ushort D[], seglen lD,
             ushort C[], seglen* alC /* только выход */);
/* Массив C должен иметь не менее max(2,lA-(lB-2)) элементов */

/* Нормализация длинного числа */
void ra_norm( ushort A[], seglen* alA);

/* Логическая побитовая операция */
void ra_bit( ushort A[], seglen lA,
             ushort B[], seglen lB,
             ushort C[], seglen mlC, seglen* alC,
             optype op );
/* Код операции : _OR, _XOR, _AND */

/* Преобразование в 10^9 - ичную систему счисления */
void ra_cvd( ushort A[], seglen lA /* вход , A>=0 , можно портить */,
             ushort W[], /* рабочий массив той же длины, что и A */
    ulong  D[], seglen mlD, seglen* alD /* выходной массив цифр 0..10^9-1 */
             );

extern double base;

/* Преобразовать плавающее в длинное целое, равное целой части исходного */
void ra_double2int(double x, ushort X[], seglen lX);

/* Преобразовать длинное целое в плавающее с округлением */
double ra_int2double(ushort X[], seglen lX);

#endif