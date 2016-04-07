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

/* ��������� �� ����:  C = B*k  */
void ra_mul1( ushort B[], seglen lB, ushort k, ushort C[], seglen* alC);

/* ����� �����:  C = - B */
void ra_minus( ushort B[], seglen lB,
               ushort C[], seglen mlC, seglen* alC);

/* �������� � ���⠭��:  C = C + B*sign */
#define ra_ads(B,lB,sign,C,mlC,alC) \
		ra_adm(B,lB,1,sign,C,mlC,alC)

#ifndef ra_ads
void ra_ads(  ushort B[], seglen lB, int sign,
              ushort C[], seglen mlC, seglen* alC);
#endif

/* �᭮���� ࠡ��� �㭪��:  C = C + B*k*sign */
void ra_adm(  ushort B[], seglen lB, ushort k, int sign,
              ushort C[], seglen mlC, seglen* alC);

/* ��������� ������� �ᥫ: C = A*B */
void ra_mul( ushort A[], seglen lA,
             ushort B[], seglen lB,
             ushort C[], seglen* alC);
/* ���ᨢ C ������ ����� �� ����� lA+lB ����⮢ */
/* ������� ����� ������� ����ଠ��������� (����� A=0) */

/* ������� ������� �ᥫ:  C = A / D, A = A % D */
/* ����⥫� D ����� ���, ��ଠ������� */
/* ���⮪ �㤥� �� ���� �������� */
/* ������� (�᫨ ����⥫쭮�) ����� 㤫������� �� 1 (��� �� lD+1) */
void ra_divmod( ushort A[], seglen* alA /* �室-��室 */,
             ushort D[], seglen lD,
             ushort C[], seglen* alC /* ⮫쪮 ��室 */);
/* ���ᨢ C ������ ����� �� ����� max(2,lA-(lB-2)) ����⮢ */

/* ��ଠ������ �������� �᫠ */
void ra_norm( ushort A[], seglen* alA);

/* �����᪠� ����⮢�� ������ */
void ra_bit( ushort A[], seglen lA,
             ushort B[], seglen lB,
             ushort C[], seglen mlC, seglen* alC,
             optype op );
/* ��� ����樨 : _OR, _XOR, _AND */

/* �८�ࠧ������ � 10^9 - ���� ��⥬� ��᫥��� */
void ra_cvd( ushort A[], seglen lA /* �室 , A>=0 , ����� ������ */,
             ushort W[], /* ࠡ�稩 ���ᨢ ⮩ �� �����, �� � A */
    ulong  D[], seglen mlD, seglen* alD /* ��室��� ���ᨢ ��� 0..10^9-1 */
             );

extern double base;

/* �८�ࠧ����� ������饥 � ������� 楫��, ࠢ��� 楫�� ��� ��室���� */
void ra_double2int(double x, ushort X[], seglen lX);

/* �८�ࠧ����� ������� 楫�� � ������饥 � ���㣫����� */
double ra_int2double(ushort X[], seglen lX);

#endif