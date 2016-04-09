/************************************************************************/
/*                             R E F A L - 6                            */
/*  Copyright (C) 1993 by Arkady Klimov                                 */
/*       rfarm.c     - arithmetic function                              */
/************************************************************************/

#include "refgen.h"
#include "rfalloc.h"
#include "arithm.h"
#include "rbarm.h"

 /* Взять операнд для чтения */
#define START_VAL(n,a,l,h,N) \
 if(IS_SNUMB(CTYPE(n))) \
 { h=NOHEAD; N[0]=CVAL(n); N[1]=( (short)(N[0])>=0 ? 0 : -1 );\
   a=(MPOS)&N; l=(a[0]==a[1]?1:2); } \
 else { h=CREF00(n); START(h); a=MFIRST(h); l=MLEN(h); }

#define END_VAL(h) if(h!=NOHEAD) END(h)
#define sgn(x) (x<0?-1:1)

static LOGICAL mk_lnumb(adr ad, MPOS a, seglen l, cvalue* ac)
{
     ra_norm(a,&l);
     if (l==1 OR l==2 AND ((short)(a[0]^a[1]))>=0) {
            *ac = CMKSIM(*a,TYPESNUMB);
            REALLOC(ad,0);
            }
     else { headptr h;
          REALLOC(ad,MSIZE(l));
         ECALL(rf_mk_empty(&h,HEADNUMB));
         AD(h)=ad;
         END(h);
         *ac=CMKREF(h,TYPELVAL);
         }
     return(TRUE);
}

LOGICAL rf_int_op (cvalue n1, cvalue n2, optype op, cvalue* ac, int nres)
/*
   n1,n2 - операнды виртуальные типа ShortInt или Int,
   op - операция: _ADD, _SUB, _MUL, _DIV, MOD, _DIVMOD
        _DIVMOD - деление для получения и частного (ac[0]) и остатка (ac[1]),
   ac - массив для результатов длины 1 или 2
   nres - длина массива ac (от 1 до 6); 2 для op='\\', 6 для _GCDSTEP.

   счетчик ссылок 1 для n1 и n2 означает, что их содержимые можно
   использовать, изменяя, для формирования результатов
   */
{
     int i,res = TRUE;
     headptr h1,h2;
     MPOS a1,a2,a3,a4,a5;
     adr ad3,ad4,ad5;
     seglen l1,l2,l3,l4,l5;
     ushort NS1[2],NS2[2];
     int sign=0;
     LOGICAL undefnum;

     START_VAL(n1,a1,l1,h1,NS1);
     START_VAL(n2,a2,l2,h2,NS2);
     undefnum = (l1==0 OR l2==0);

     switch(op) {
         case _SUB: sign--;
         case _ADD:
            if (undefnum) { res=FAIL; goto end; };
            l3=max(l1,l2)+1;
            ECALL(ALLOC(ad3,MSIZE(l3))!=NOADR);
            a3=(MPOS)SEGINF(ad3);
            ALL(i,l1) a3[i]=a1[i];
            ra_ads(a2,l2,sign,a3,l3,&l1); l3 = l1;
            break;
         case _MUL:
            if (undefnum) { res=FAIL; goto end; };
            l3=l1+l2;
            ECALL(ALLOC(ad3,MSIZE(l3))!=NOADR);
            a3=(MPOS)SEGINF(ad3);
            if(l1>l2)  ra_mul(a1,l1,a2,l2,a3,&l3);
            else       ra_mul(a2,l2,a1,l1,a3,&l3);
            break;
         case _DIV :
         case _MOD :
         case _DIVMOD:
         case _GCDSTEP:
            if(undefnum OR l2==1 AND *a2==0) { res=FAIL; goto end; }

            /* модуль делимого - в a3,l3, там будет остаток */
            ECALL(ALLOC(ad3,MSIZE(l1+2))!=NOADR);
            a3=(MPOS)SEGINF(ad3);
            if(a1[l1-1]!=0) ra_minus(a1,l1,a3,l1+1,&l3);
            else { ALL(i,l1) a3[i]=a1[i]; l3=l1; };

            /* модуль делителя - в a4,l4 */
            ECALL(ALLOC(ad4,MSIZE(l2+2))!=NOADR);
            a4=(MPOS)SEGINF(ad4);
            if(a2[l2-1]!=0) ra_minus(a2,l2,a4,l2+1,&l4);
            else { ALL(i,l2) a4[i]=a2[i]; l4=l2; };

            if(op==_GCDSTEP) { if(nres!=6) return(FALSE);
              /*
               Если аргументы близки, то по их мантиссам подбираем
               a,b,c,d , которые используем для получения результатов
                  A4 = a*A3+b*A4;
                  A3 = c*A3+d*A4;
               и возвращаем шестерку A4,A3,a,c,b,d
               Если аргументы отличаются сильно, то выполняем деление
               A3 на A4 с получением только модуля в A3, и возвращаем
               пару A4,A3.
              */
              if(l3==l4 OR (l3==l4+1 AND a4[l4-2]>a3[l3-2])
                        OR (l3==l4-1 AND a4[l4-2]<a3[l3-2]) ) {
                  seglen l = max(l3,l4);
                  int s1,s2;
                  ulong S;
                  ulong M0 = GetMantissa(a3,l-1), M;
                  ulong N0 = GetMantissa(a4,l-1), N;
                  long M1 = 1, M2 = 0, N1 = 0, N2 = 1;
                  /* Нормализуем пару мантисс */
                  ulong Mx=0,Nx=0;
                  if(l>3) { Mx = a3[l-4]; Nx = a4[l-4]; }
                  while((long)M0>0 AND (long)N0>0) {
                         Mx<<=1; Nx<<=1;
                         M0<<=1; N0<<=1;
                         }
                  M0+=HALF1(Mx); N0+=HALF1(Nx);
                  M=M0; N=N0;
                  do {
                     /* M = M1*M0+M2*N0, N = N1*M0+N2*N0 */
                     ulong P = M / N;
                     ulong Q = M - P * N;    /* 0<=P<=0xFFFF */
                     ulong Q1 = M1 - P * N1, Q2 = M2 - P * N2;
                     /* Q = Q1*M0+Q2*N0 */
                     S = labs(Q1) + labs(Q2);
/*
 printf("GCDSTEP: S=%lx,M=%ld,N=%ld,Q=%ld,M1=%ld,M2=%ld,N1=%ld,N2=%ld,Q1=%ld,Q2=%ld\n",
                   S,M,N,Q,M1,M2,N1,N2,Q1,Q2);
    */
#define IsShort(L) (HALF1(L)==PLUS OR HALF1(L)==MINUS)
                     if(NOT IsShort(Q1) OR NOT IsShort(Q2)) break;
                     M = N;
                     N = Q;
                     M1 = N1;
                     M2 = N2;
                     N1 = Q1;
                     N2 = Q2;
                     } while (S<M AND S<N);
                  /* A4 = M1*a3 + M2*a4 */
                  /* A3 = N1*a3 + N2*a4 */
                  a3[0]=0;l3=1; s1=(a1[l1-1]==0?1:-1);
                  a4[0]=0;l4=1; s2=(a2[l2-1]==0?1:-1);
                  ra_adm(a1,l1,(ushort)labs(M1),s1*sgn(M1),a4,l2+2,&l4);
                  ra_adm(a2,l2,(ushort)labs(M2),s2*sgn(M2),a4,l2+2,&l4);
                  ra_adm(a1,l1,(ushort)labs(N1),s1*sgn(N1),a3,l1+2,&l3);
                  ra_adm(a2,l2,(ushort)labs(N2),s2*sgn(N2),a3,l1+2,&l3);
/*                  printf("\na3= "); ALL(i,l3) printf("%04x ",a3[i]); */
                  if((res=mk_lnumb(ad4, a4, l4, ac  ))!=TRUE) return res;
                  if((res=mk_lnumb(ad3, a3, l3, ac+1))!=TRUE) return res;
                  if((res=rf_mk_cnumb(M1*s1, ac+2, TRUE))!=TRUE) return res;
                  if((res=rf_mk_cnumb(N1*s1, ac+3, TRUE))!=TRUE) return res;
                  if((res=rf_mk_cnumb(M2*s2, ac+4, TRUE))!=TRUE) return res;
                  if((res=rf_mk_cnumb(N2*s2, ac+5, TRUE))!=TRUE) return res;
                  goto end;
                  }
              } /* конец итерационной части GCDSTEP */

            /* продолжение деления и GCDSTEP */
            /* место для частного */
            if(op!=_MOD) {
                  l5=max(l4,l3)-l4+2;
               ECALL(ALLOC(ad5,MSIZE(l5))!=NOADR);
               a5=(MPOS)SEGINF(ad5);
               }
            else a5=NULL;

            ra_divmod(a3,&l3,a4,l4,a5,&l5);

            /* остатку (a3) - знак делимого */
            if(a1[l1-1]!=0) ra_minus(a3,l3,a3,l1+1,&l3);

            if(op!=_GCDSTEP) {
                /* стереть модуль делителя */
                REALLOC(ad4,0);
                }
            if(op!=_MOD) {
                /* установить знак отношения */
                LOGICAL POS= a1[l1-1]==a2[l2-1]; /* отношение положительно */
                /* частному - знак отношения или (для _GCDSTEP) наоборот */
                if((op==_GCDSTEP)==POS) ra_minus(a5,l5,a5,l5+1,&l5);
                };
            switch(op) {
              case _DIV:
                /* стереть остаток */
                REALLOC(ad3,0);
                /* частное - в ac[0] */
                if((res=mk_lnumb(ad5, a5, l5, ac))!=TRUE) return res;
                goto end;
              case _DIVMOD: if(nres!=2) return(FALSE);
                /* остаток - в ac[1] */
                if((res=mk_lnumb(ad3, a3, l3, ac+1))!=TRUE) return res;
                /* частное - в ac[0] */
                if((res=mk_lnumb(ad5, a5, l5, ac))!=TRUE) return res;
                goto end;
              case _GCDSTEP:
                /* модуль делителя (a4) - в ac[0] */
                if((res=mk_lnumb(ad4, a4, l4, ac))!=TRUE) return res;
                /* остаток - в ac[1] */
                if((res=mk_lnumb(ad3, a3, l3, ac+1))!=TRUE) return res;
                ac[2] = CMKSIM(0,TYPESNUMB);
                ac[3] = CMKSIM(1,TYPESNUMB);
                /* знак делителя - в ac[4] */
                ac[4] = CMKSIM(sgn((short)(a2[l2-1])),TYPESNUMB);
                /* отрицательное отношение (a5) - в ac[5] */
                if((res=mk_lnumb(ad5, a5, l5, ac+5))!=TRUE) return res;
                goto end;
              case _MOD: break;
              }
            break;
         case _OR :
         case _XOR:
         case _AND:
            l3=max(l1,l2);
            ECALL(ALLOC(ad3,MSIZE(l3))!=NOADR);
            a3=(MPOS)SEGINF(ad3);
            ra_bit(a1,l1,a2,l2,a3,l3,&l3,op);
            break;
         };
     if((res=mk_lnumb(ad3, a3, l3, ac))!=TRUE) return res;
end: END_VAL(h1);
     END_VAL(h2);
     return res;
     }

LOGICAL rf_cgetreal(cvalue n, real* ar)
        /* n is virtual */
        /* m is either _SREAL or _LREAL */
{
    int t=CTYPE(n);
    headptr h; float f;
           if (IS_SNUMB(t)) *ar = (short)CVAL(n);
      else if (IS_REAL(t)) { cvalue2float(n,f); *ar=(real)f; }
      else if (IS_REF(t))
        switch(MODE(TYPE(h=CREF00(n)))) {
            case MASK:
                  START(h); {
                     MPOS X = MFIRST(h);
                     seglen lX = MLEN(h);
                     *ar = ra_int2double(X,lX);
                     }
                  END(h);
                  break;
            case REAL: head2real(h,*ar); break;
            default: return FAIL;
            }
      else return FAIL;
      return TRUE;
}

static LOGICAL rf_hgetnumb(headptr h, long * an)
 { real r; MPOS a;
   switch(MODE(TYPE(h))) {
      case MASK:
          START(h);
            a = MFIRST(h);
            *an = ((long)(a[1])<<16) + a[0];
            if(MLEN(h) < 2) *an = (short) *an;
          END(h); break;
      case REAL: head2real(h,r); *an=r; break;
      default: return FAIL;
      }
   return TRUE;
   };

/* Test type of cc and get long integer to n */
LOGICAL rf_cgetnumb(cvalue cc, long * an)
{  int t = CTYPE(cc);
   float f;
   if (IS_SNUMB(t)) {
      *an = (short) CVAL(cc);
      return(TRUE);
      }
   if (IS_REAL(t)) {
      cvalue2float(cc,f);
      *an = f;
      return(TRUE);
      };
   if(IS_REF(t)) return rf_hgetnumb(CREF00(cc),an);
   return(FAIL);
   };

/* Test type of p and get long integer to n */
LOGICAL rf_getnumb ( elemptr p, long * an)
{
   if(ISSNUMB(p)) {
      *an = (short) VAL(p);
      return TRUE;
      };
   return rf_cgetnumb(DATA(p),an);
 }
