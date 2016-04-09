#include "arithm.h"
#include <stdio.h>

#define ALL(i,n) for(i=0;i<n;i++)

main() {
int i;
ushort k;
ushort A[7]={0x1111,0x2222,0x9999,0x0000,0x0002,0x0000,0x0000};   seglen lA=4;
ushort B[6]={0x5555,0x6666,0x7777,0};             seglen lB=4;
ushort C[6]={0x0001,0};                 seglen lC=2;
ushort S[20];                seglen lS, mlS=8;

printf("\n");
printf("\nA=      "); ALL(i,lA) printf("%04x ",A[i]);
printf("\nB=      "); ALL(i,lB) printf("%04x ",B[i]);
/* printf("\nC= "); ALL(i,lC) printf("%04x ",C[i]); */

ALL(i,lA) S[i]=A[i]; lS=lA;  /* S = A */
k = 1;
ra_adm(B,lB, k, 1, S, mlS, &lS); /* ra_norm(S,&lS); */
printf("\nS=A+B*%u ",k); ALL(i,lS) printf("%04x ",S[i]);
getch();
return;

ra_mul(A,lA,B,lB,S,&lS);  ra_norm(S,&lS);
printf("\nA*B=     "); ALL(i,lS) printf("%04x ",S[i]);
ra_adm(C,lC, 1,1, S, mlS, &lS); /* ra_norm(S,&lS); */
printf("\nS=A*B+C= "); ALL(i,lS) printf("%04x ",S[i]);
ra_divmod(S,&lS,B,lB,A,&lA); /* ra_norm(A,&lA); */
printf("\nA=S/B "); ALL(i,lA) printf("%04x ",A[i]);
printf("\nC=S%B "); ALL(i,lS) printf("%04x ",S[i]);
getch();
return;
}