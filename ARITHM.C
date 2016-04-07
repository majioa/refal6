/************************************************************************/
/*                             R E F A L - 6                            */
/*  Copyright (C) 1993 by Arkady Klimov                                 */
/*       arithm.c     - arithmetic routines                             */
/************************************************************************/
#include <math.h>
#include <stdlib.h>

/* typedef unsigned seglen; */
#include "arithm.h"

/* #define ALL(i,m) for(i=0;i<m;i++) */
#define AND &&
#define OR  ||

/*
  ������� �᫮ ���� ���: A,lA, ��� A - ���ᨢ �������, lA - ��� �����.
  A[0] -������ �������, A[lA-1] - ���� �᫠: 0 ��� -1.
  ������� ����� ⨯ unsigned short int.
  ������� �᫮ ��⠢���� �� ������� � ��⥬� ��᫥��� �� �᭮�����
  2**BitsShort. ����⥫�� �᫠ �।�⠢���� � �������⥫쭮� ����.
  ����� �������� ��� ����� ���� �ਯ�ᠭ� ᫥�� �������⭮.
  �� ��।�� ��ࠬ��� ��� १���� ��।����� 㪠��⥫� �� �����
  ���饣� १����, �� �⮬ ��� ���� १���� ������ ���� �⢥����
  �����筮 ����.
  �������� ���᫨⥫��� ��楤�� ���筮 ����ଠ��������.
  ��楤�� ra_norm ��ଠ����� �᫮ ����뢠���� ��譨�
  �������� ���. �� �⮬ ��⠭���� ������ ���� ���.
  */

/* ����� ��砨 �᭮���� ࠡ�祩 �㭪樨 ra_addm: */
/* ��������� �� ����:  */
void ra_mul1( ushort B[], seglen lB, ushort k, ushort C[], seglen* alC)
/* ��������:   C = B*k
 ���
    B - 1-� ᮬ����⥫�,
    k - 2-� ᮬ����⥫� (���� �������)
    C - ���� १����, 䠪��᪠� ����� �㤥� � *alC
 C ����� ᮢ������ � �. ����� � �㤥� �� 1 ����� ����� B
*/
{   int i;
    ulong v=0;                   /* ����祥 ���� */
    C[lB] = B[lB-1];  /* ��⠭���� ����� १���� */
    ALL(i,lB) {
    /* ���᫥��� ��।��� �������� �ந�������� B*k */
      v = HALF1(v) + (ulong) B[i] * (ulong) k;
      C[i] = HALF0(v);
      };
    *alC = lB+1;
}









/* ����� �����:  */
void ra_minus( ushort B[], seglen lB,
               ushort C[], seglen mlC, seglen* alC)
/* ��������:
                C = - B
 ��� B - ��室��� �᫮,
     C - ���� १���� (������ mlC), 䠪��᪠� ����� �㤥� � *alC
     C ����� ᮢ������ � �, ����� ���祭�� ����� 㢥������� �� 1 ����
     ������� ��ଠ�������, �᫨ ��㬥�� �� ��ଠ�������.
*/
{   int i;
    ulong u=0;                        /* ����祥 ���� */
    ALL(i,lB) {
    /* ���⠥� �� ��७�� ��।��� ���� B */
      u = (long)(short) HALF1(u) - (ulong) B[i];
    /* �����뢠�� ��।��� ���� � १���� C */
      C[i] = HALF0(u);
      };
    if(C[lB-1]==1 AND lB<mlC) { C[lB]=0 ; lB++; }
    *alC = lB;
}

#ifndef ra_ads
/* �������� � ���⠭��:  */
void ra_ads(  ushort B[], seglen lB, int sign,
              ushort C[], seglen mlC, seglen* alC)
/* ��������: C = C + B*sign
 ��� C - 1-e ᫠������, B - 2-� ᫠������,
    sign - ���� � �ଥ int: >=0 ��� <0
    C - ���� १���� (������ mlC), 䠪��᪠� ����� �㤥� � *alC
*/
{   int i; ushort alt;
    seglen lA = *alC;                   /* ����� 1-�� ᫠������� */
    ushort signA = C[lA-1],  signB = B[lB-1];    /* ����� � � B */
    seglen lC = max(lB,lA)+1;
    if(lC > mlC) lC = mlC;   /* lC = min(lC,mlC); */
    if(lC < lB)  lB = lC;    /* lB = min(lB,lC)  */
    lC = min(lC,mlC);
    for (i=lA;i<lC;i++) C[i] = signA;   /* ������� A �� � */
    *alC = lC;
  { ulong u=0;                          /* �㬬��� */
    ALL(i,lB) {
    /* �ਡ���塞 � �㬬� ��७�� � ��।��� ���� B */
            C[i] = u = (ulong) C[i]
                  + (long)(short)HALF1(u)
                  + (sign>=0? (ulong) B[i] : - (ulong) B[i]) ;
             }
    /* ���㬬��㥬 ��७�� (����) */
    for(; i<lC; i++) {
      u = (long)(short)HALF1(u) + (sign>=0? (ulong) signB : - (ulong) signB);
      if(HALF0(u)==0) break;
      C[i] = u += (ulong) C[i];
    } }
}
#endif





/* �᭮���� ࠡ��� �㭪��:  */
void ra_adm( ushort B[], seglen lB, ushort k, int sign,
              ushort C[], seglen mlC, seglen* alC)
/* ��������:
                C = C + B*k*sign
 ��� C - 1-e ᫠������ � ���� १���� (������ mlC),
    *alC - ��室��� ����� � � 䠪��᪠� ����� १����,
    B - 2-� ᫠������,
    k - �����⥫� ��� 2-�� ᫠������� (���� �������)
    sign - ���� � �ଥ int: >=0 ��� <0
    C - ���� १���� (������ mlC), 䠪��᪠� ����� �㤥� � *alC
*/
{   int i;
    seglen lA = *alC;                   /* ����� 1-�� ᫠������� */
    ushort signA = C[lA-1], signB = B[lB-1];    /* ����� � � B */
    seglen lC = max(lB,lA)+1;
    if(lC > mlC) lC = mlC;   /* lC = min(lC,mlC); */
    if(lC < lB)  lB = lC;    /* lB = min(lB,lC)  */
    for (i=lA;i<lC;i++) C[i] = signA;   /* ������� A �� � */
    *alC = lC;
    if(k==0) return;
#ifndef asm
  { ulong u=0, v=0;                        /* ����稥 ���� */
    ALL(i,lB) {
    /* ���᫥��� ��।��� �������� �ந�������� B*k */
      v = (ulong) B[i] * (ulong) k + HALF1(v);
    /* �ਡ���塞 � �㬬� ��७�� � ��।��� ���� B*k */
      C[i] = u = (ulong) C[i]
                  + (long)(short)HALF1(u)
                  + (sign>=0? (ulong) HALF0(v) : - (ulong) HALF0(v)) ;
      };
    /* ���㬬��㥬 ��७�� (����) */
    for(; i<lC; i++) {
      u = (long)(short)HALF1(u) + (sign>=0? (ulong) signB : - (ulong) signB);
      if(HALF0(u)==0) break;
      C[i] = u += (ulong) C[i];
    } }
#else
	asm push ds
	asm	lds si,B      ; /*   ds:si = B */
	asm	les di,C      ; /*   es:di = C */
	asm	xor bx,bx     ; /* bx = 0 */
	asm	pushf         ; /* push(carry=0) */
	asm	cld           ; /* lodsw increments */
	asm	mov cx, lB    ; /* cx = lB */
	asm	cmp word ptr sign,0        ; /* sign? */
	asm	jl short SUB      ;
ADD:
    asm lodsw         ; /* ax = [ds:si] = *Bi, i++; */
    asm mul word ptr k         ; /* dx:ax = ax * k */
    asm add ax,bx     ;
    asm adc dx,0      ; /* dx:ax += bx */
    asm mov bx,dx     ; /* bx = dx */
    asm popf
    asm adc word ptr [es:di], ax  ; /* *Ci+= ax + CARRY ; i++; */
    asm inc di
    asm inc di
    asm pushf
    asm loop ADD

    asm mov cx, lC
    asm sub cx, lB     ; /* cx = lC-lB */
    asm je XXX
    asm popf
ADD1:
    asm mov ax, signB  ; /* ax = signB */
    asm adc ax,0
    asm je  ADD2
    asm add word ptr [es:di], ax  ; /* *Ci+= ax + CARRY ; i++; */
    asm inc di
    asm inc di
    asm loop ADD1
ADD2:
    asm jmp short EEE
SUB:
    asm lodsw         ; /* ax = [ds:si] = *Bi, i++; */
    asm mul word ptr k         ; /* dx:ax = ax * k */
    asm add ax,bx     ;
    asm adc dx,0      ; /* dx:ax += bx */
    asm mov bx,dx     ; /* bx = dx */
    asm popf
    asm sbb word ptr [es:di], ax  ; /* *Ci-= ax + CARRY ; i++; */
    asm inc di
    asm inc di
    asm pushf
    asm loop SUB
    asm mov cx, lC
    asm sub cx, lB     ; /* cx = lC-lB */
    asm je XXX
    asm popf
SUB1:
    asm mov ax, signB  ; /* ax = signB */
    asm adc ax,0
    asm je  SUB2
    asm sub word ptr [es:di], ax  ; /* *Ci-= ax + CARRY ; i++; */
    asm inc di
    asm inc di
    asm loop SUB1
SUB2:
    asm jmp short EEE
XXX:
	asm popf
EEE:
	asm pop ds
#endif
}
/* ��������� ������� �ᥫ */
void ra_mul( ushort A[], seglen lA,
             ushort B[], seglen lB,
             ushort C[], seglen* alC)
/* ���ᨢ C ������ ����� �� ����� lA+lB ����⮢ */
/* ������� ����� ������� ����ଠ��������� (����� A=0) */
{   int i; seglen lC, mlC = lA+lB;
    seglen lB1 = lB-1;
    C[0] = 0; lC=1;
    ALL(i,lB1) { ra_adm(A,lA,*B,1,C,mlC,&lC); lC--; C++; B++; mlC--;}
    /* ������� ࠧ�� ��ࠡ��뢠���� �ᮡ� */
	if(B[0]==MINUS) ra_adm(A,lA,1,-1,C,mlC,&lC);
    *alC=lC+lB1;
};

/* Long division:  C = A / D, A = A % D */
/* Divisor is positive and normalized */
/* The remainder will take place of the divident */
/* The divident (if negative) may lengthen by 1 (or up to lD+1) */
/* The quotient is not produced if C = NULL
        (but its length *alC is produced) */

void ra_divmod( ushort A[], seglen* alA /* IN-OUT */,
             ushort D[], seglen lD,
             ushort C[], seglen* alC /* only OUT */)

/* The array C mast contain no less then max(2,lA-(lB-2)) elements */

{   int i,m,p,r,sign;
    ulong k;
    short signA;
	seglen lA = *alA, lA0=0, mlA;
    //seglen lC=max(1,lA-(lD-2)); ERROR because lA and lD are unsigned
    seglen lC=max(lA+1,lD) - lD + 1;  // 21 July 2000
    seglen lX;
    ulong MD = GetMantissa(D,lD-1) /* mantissa of the divisor */;
    ulong NMD = MD /* normalized mantissa of the divisor */;
    ulong MA                       /* mantissa of the divident */;
    int shift=BitsShort;

  /* throw minor zeroes of divisor out */
	while(D[0]==0 AND lA>2) { D++; lD--; A++; lA--; lA0++; }
	mlA = lA+1;

    /* Normalizing mantissa of divizor */
    while(HALF1(NMD)!=0) { NMD=NMD>>1; shift--; };

    /* NMD++; /* Round mantissa up */

    if(C!=NULL) ALL(m,lC) C[m]=0; /* Initializing C=0; */

       /* For all m, 0 <= m < lC, in descending order, where m is
          the index of macrodigit to be evaluated
          the following loop for evaluating m-th macrodigit
          of the divident is executed
          The sign digit of the quotient is evaluated first
          (due to the case of nagative divident) */

    p=lC-1;

   /* p - a position to decrease quotient by 1 in case of negative divisor */

   /* printf("\n D= "); ALL(i,lD) printf("%04x ",D[lD-i-1]); */

    for(m=lC-1; m>=0; m--) {

       /* Normalize divident, if zero -> exit
          Adjust divizor shifted m position left to divident
          If divident is positive, then
              If divisor sign goes out of the divident sign (to the left)
                  -> NEXT;
              If signs of divident and divisor are adjusted
              compare divident without m minor macrodigits with
              the divisor: Estimate ratio of divident and divisor
              in the form of two-digit number: k.l , where k is a whole
              part and .l is a fraction part.
      If divident is less then divisor then
          if m=0 then exit;
          if m>0 then instead of k which is 0 use l
            taking into account the shift by 1 position to the right.
          Subtract the divisor multiplied by k from divident at position m;
      add k to quotient at position p=m (or p=m-1, see above);
      keep position p.
             -> NEXT;
          If divident is negative (this is possible at the beginning
          as well as due to the overestimated digit of the quotient)
      add divisor to divident at position p, and subtract 1 from quotient
            -> repeat;
       */

repeat: /* lD+m >= lA */
        signA=A[lA-1];
        while(A[lA-2]==signA AND lA>1) lA--; /* Normalizing A */
/*  printf("\n m= %d",m);
  printf(" A= "); ALL(i,lA) printf("%04x ",A[lA-i-1]); */
        if(lA==1 AND A[0]==0) break;
  if (signA==0) { /* Positive divident */
	    /* lD+m >= lA */
            if(lD+m>lA) continue;
            /* lD+m==lA AND lA>=2 */
            MA = GetMantissa(A,lA-1);    /* mantissa of the divident */
            /* Evaliate r = (A>=D? 0 : 1) */
            r=0;
            for (i=1;i<=lD;i++)
               if (A[lA-i]!=D[lD-i]) {
                  if (A[lA-i]<D[lD-i]) r=1;
                  break;
                  };
            if(r /* the divident is less than shifted divisor */) {
                if(m==0) break;
                else k=(MA<<shift) / NMD;
        		if(HALF1(k)) {
                /* printf("\n Macrodigit overflow"); */
		            k=(ulong)(ushort)(-1);
		            }
                }
            else /* r==0 */ k =  MA / MD;
            p = m-r;
            sign=1;
            }
        else { /* negative divident */
            k=1;
            sign=-1;
            }
/* printf("\nA=A%sD*%04x*e**%d %c",(sign<0?" + ":" - "),
        (ushort)k,p,(k==1?'*':' ')); */
    if(C!=NULL) {  /* Cp = Cp+k*sign */
        ushort kk[2]; kk[0] = k; kk[1] = k>>BitsShort; /* cast k as ushort* */
        lX=lC-p;
        ra_ads(kk,2, sign, C+p, lC-p, &lX);
        }
              /* Ap = Ap - D*k*sign */
    lX=lA-p;  ra_adm(D,lD, (ushort)k,-sign, A+p,mlA-p,&lX);  lA=lX+p;
        goto repeat;
	}
	*alA=lA+lA0;
    *alC=lC;
    };

double base = MBASE; /* 65536.0 */

/* �८�ࠧ����� ������饥 � ������� 楫��, ࠢ��� 楫�� ��� ��室���� */
void ra_double2int(double x, ushort X[], seglen lX)
{   int i;
    double y;
    ALL(i,lX) {
         x/=base;
         y = modf(x,&x); /* split x between and y (x+y = old-x) */
         if(y<0) { y += 1.0; x -= 1.0; };
         X[i] = y*base;
         };
/*  if(lX) if(X[lX-1]!=(ushort)-1) X[lX-1]=0; */
};

/* �८�ࠧ����� ������� 楫�� � ������饥 � ���㣫����� */
double ra_int2double(ushort X[], seglen lX)
{ double x = (short) X[lX-1];
  int i;
  for(i=lX-2; i>=0; i--)
	 x = x*base+X[i];
  return x;
};

/* ��ଠ������ �������� �᫠ */
void ra_norm( ushort A[], seglen* alA)
        { while (*alA>1 AND A[*alA-1]==A[*alA-2]) (*alA)--; }

/* �८�ࠧ������ � 10^9 - ���� ��⥬� ��᫥��� */
void ra_cvd( ushort A[], seglen lA /* �室 , A>=0 , ����� ������ */,
             ushort W[], /* ࠡ�稩 ���ᨢ ⮩ �� �����, �� � A */
    ulong  D[], seglen mlD, seglen* alD /* ��室��� ���ᨢ ��� 0..10^9-1 */
             )
/* ��ࠡ��뢠���� �� ����� mlD ������ ��� १���� */
{
	ushort B[3]={ (ushort)(1000000000L%MBASE),
                  (ushort)(1000000000L/MBASE), 0 }; /* ����⥫� */
	seglen lB=3, lD, lW;
	int i;
    ALL(lD,mlD) {
        ra_norm(A,&lA);
        if (lA==1 AND A[0]==0) break;
        ra_divmod(A,&lA, (MPOS)B,lB, W, &lW);
        D[lD] = (((ulong)(A[1]))<<BitsShort) + A[0];
        /* ���᫠�� W � A */
        ALL(i,lW) A[i]=W[i]; lA=lW;
        }
    *alD = lD;
    };

/* �८�ࠧ������ � 10^4 - ���� ��⥬� ��᫥��� */
void ra_cvds( ushort A[], seglen lA /* �室 , A>=0 , ����� ������ */,
              ushort W[], /* ࠡ�稩 ���ᨢ ⮩ �� �����, �� � A */
    ushort D[], seglen mlD, seglen* alD /* ��室��� ���ᨢ ��� 0..10^4-1 */
             )
/* ��ࠡ��뢠���� �� ����� mlD ������ ��� १���� */
{
	ushort B[2]={ 10000,0 }; /* ����⥫� */
	seglen lB=2, lD, lW;
	int i;
    ALL(lD,mlD) {
        ra_norm(A,&lA);
        if (lA==1 AND A[0]==0) break;
        ra_divmod(A,&lA, (MPOS)B,lB, W, &lW);
        D[lD] = *A;
        /* ���᫠�� W � A */
        ALL(i,lW) A[i]=W[i]; lA=lW;
        }
    *alD = lD;
    };

/* �����᪠� ����⮢�� ������ */
void ra_bit( ushort A[], seglen lA,
             ushort B[], seglen lB,
             ushort C[], seglen mlC, seglen* alC,
             optype op )
/* ��� ����樨 : _OR, _XOR, _AND */
{
	ushort a,b,c;
	int i;
    ALL(i,mlC) {
       a = (i<lA? A[i] : A[lA-1]);
       b = (i<lB? B[i] : B[lB-1]);
       switch(op) {
          case _OR : c = a | b; break;
          case _XOR: c = a ^ b; break;
          case _AND: c = a & b; break;
          }
       C[i] = c;
       };
     *alC = mlC;
     };
