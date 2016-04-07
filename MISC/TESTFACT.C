#include "rfarm.c"

#include <sys\timeb.h>
long times(long* at)
{  struct timeb t;
   ftime(&t);
   return *at = t.time*100+t.millitm/10 ;
}

void fact(int n, int m, ushort A[], seglen mlA, seglen* alA)
{int i,j;  seglen lA;
 long Z=0L; seglen lZ=1;
 A[0]=1; A[1]=0; *alA=2;
 for(j=n;j<=m;j++) {
    lA=*alA;
/*  ra_addm((ushort*)&Z,lZ,A,*alA,j,1,A,mlA, alA);  */
    ra_mul1(A,*alA,j,A,alA);
    ra_norm(A,alA);
    }
 };

main() {
int i,k;
ushort A[1000],B[2000],C[2000];
seglen lA, mlA=1000;
seglen lB, mlB=2000;
seglen lC, mlC=2000;
long t1,t2; int dt;
int a=100, b=101;

printf("\n\n Start");

times(&t1); fact(1,a,A,mlA,&lA); times(&t2);
printf("\nA=%d! ready",a);
dt=t2-t1; printf(" time=%d.%02d", dt/100, dt%100);

times(&t1); fact(1,b,B,mlB,&lB); times(&t2);
printf("\nB=%d! ready",b);
dt=t2-t1; printf(" time=%d.%02d", dt/100, dt%100);

k=4096; C[0]=0; lC=1; /* Прибавить A к C k раз */
times(&t1); ALL(i,k) {
      ra_adds(A,lA,C,lC,1,C,mlC,&lC);
/*    ra_addm(A,lA,C,lC,1,1,C,mlC,&lC); */
      ra_norm(C,&lC);
      }
times(&t2);
printf("\nC=%d! * %d ready",a,k);
dt=t2-t1; printf(" time=%d.%02d", dt/100, dt%100);
printf("\nC= %d! * %d = ",a,k); ALL(i,lC) printf("%04x ",C[lC-i-1]);

times(&t1); ra_mul(A,lA,B,lB,C,&lC); times(&t2);
printf("\nC=%d!*%d! ready",a,b);
dt=t2-t1; printf(" time=%d.%02d", dt/100, dt%100);

times(&t1); fact(a+1,b,C,mlC,&lC); times(&t2);
printf("\nC=%d-%d! ready",a,b);
dt=t2-t1; printf(" time=%d.%02d", dt/100, dt%100);

printf("\nA = %d! = ",a); ALL(i,lA) printf("%04x ",A[lA-i-1]);
printf("\nB = %d! = ",b); ALL(i,lB) printf("%04x ",B[lB-i-1]);
printf("\nC= %d! / %d! = ",b,a); ALL(i,lC) printf("%04x ",C[lC-i-1]);

times(&t1); ra_divmod(B,&lB,A,lA,C,&lC); times(&t2);
 printf("\nC= %d! / %d! = ",b,a); ALL(i,lC) printf("%04x ",C[lC-i-1]);
printf("\nC=%d!/%d! ready",b,a);
dt=t2-t1; printf(" time=%d.%02d", dt/100, dt%100);

getch();
return;
}
