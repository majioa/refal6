#include "rfalloc.h"
#define HCALL(H,M,T,P)  ((wrkfun=IFUNC(H,M))!=inull AND (*(T)(wrkfun)) P)
#define HADDR(H,M)    (IFUNC(H,M)==inull? NULL : wrkptr)
