/******************************************************************
   R E F A L - 6      Copyright (C) 1992 Arkady Klimov
   rfalloc.c - Heap Memory management
*******************************************************************/
#include "config.h"
/* #include <dos.h> */

#ifdef EXIST_ALLOC_H
#include <alloc.h>
#define CORELEFT (farcoreleft()-20000L) /* 20000 bytes left for C */
#else
#include <malloc.h>
#define CORELEFT 3600000L
#define farmalloc(L) malloc(L)
#endif
#include "rfalloc.h"

/* This module implements functions to allocate dynamic memory segments
It's main features are:
    The memory segment address (type adr) can be stored in a headder
    (field NEXT).
    Each (unlocked) memory segment is connected with it's own headder.
    The application accesses the segment via the headder.
    The segment when accessed is locked to prevent moving.
    When there is no propper hole to allocate new segment a squeeze
    operation is performed, during which all unlocked segments move downmost.

The implementation features:
    All segments have 16-byte boundary alignment.
    Each segment has a title which is a pair:
    <headptr Head, slen Size>.
    There ara four kinds of segments:
    <NOHEAD,0>    - fixed null segment (this is out of heap),
    <FREESEG, hlen HSize> - free segment of size HSize (in paragraphs).
    <NOHEAD, seglen Length> - used LOCKED segment,
    <Head, seglen Length> - used movable segment (Length is in bytes).
    Parameter Length refers to user information which does not include
    title information.
    Number of paragraphs, occupied by a segment can be
    calculated from its Length as HLEN(seglen Size) */

/*  There is a fixed global null segment called anull: */
adr anull;

/* Unused memory is arranged in several holes with following title */




static int prm=0; /* prm = 1 enables much printing */






/* Holes are addressed and sized in terms of paragraphs */

#ifdef HUGE32  /* defined in config.h */

/* Boundary alignment is 8 paragraphs each of 1 byte */
typedef char*  hadr; /* private segment (hole) address */
#define NOHOLE NULL
typedef long   hlen; /* private segment (hole) size */
#define SIZEOFPAR 1
#define ANULLSIZE 8  /* hlen */
#define HLEN(Size)   ((Size+7+HEADSIZE)&(-8L))
#define HEAPBASE(a)  a
#define HOLE(s)      ((hole*)(s))
#define MOVSEG(s,d,l) memmove(d,s,(size_t)l)   /* (hadr,hadr,size_t) */
#define CHKP(p)
#define HADRP(p) ((char*)p-HEADSIZE)  /* p - far ptr to user memory */
#ifdef AT286
#define HADR_PLUS_LEN(a,l) hadr_plus_len(a,l)
static hadr hadr_plus_len (hadr a, hlen l)
{
 l += FP_OFF(a);
 return (hadr) MK_FP( FP_SEG(a)+(unsigned)(l>>4),((unsigned)l)&15);
};
#else
#define HADR_PLUS_LEN(a,l)  ((a)+(l))
#endif
#else

/* Boundary alignment is 1 paragraph of 16 byte */

typedef unsigned hadr; /* private segment (hole) address (in paragraphs) */
#define NOHOLE 0
typedef unsigned hlen; /* private segment (hole) size in paragraphs */
#define SIZEOFPAR 16
#define ANULLSIZE 1    /* hlen */
#define HLEN(Size)   (((Size-1+HEADSIZE)>>4)+1)
#define HEAPBASE(a)  FP_SEG(a)+((FP_OFF(a)+15)>>4);
#define HOLE(s) ((hole*) MK_FP(s,0))
#define MOVSEG(s,d,l) movedata(s,0,d,0,l*SIZEOFPAR);
#define CHKP(p) if(FP_OFF(p)!=HEADSIZE)\
                       fprintf(stdtrc,"Rfree: illegal address %p\n",p);
#define HADRP(p)  FP_SEG(p)
#define HADR_PLUS_LEN(a,l) ((a)+(l))

#endif















#ifdef  SPLIT
#define ADR(h)   h
#define HADR(s)  s
#define FREESEG -1          /* const to denote free segment (a hole) */
#endif

#ifdef  LARGE
#ifndef HUGE32
#define ADR(h)   MK_FP(h,0)
#define HADR(s)  FP_SEG(s)
#define FREESEG  (headptr) MK_FP(-1,-1)
#endif
#endif

#ifdef  HUGE32
#define ADR(h)   (adr) h
#define HADR(s)  (hadr) s
#define FREESEG  (headptr) (((char*) NULL)-1)
#endif

typedef    struct {
    hadr next;   /* next hole or NULL */
    hlen size;   /* size of the hole in paragraphs */
    } hole;

/* Macro functions of inner segment , which is the same as hole */
#define HSHEAD(s) SEGHEAD(ADR(s))
#define HSLEN(s)  HLEN(SEGLEN(ADR(s)))
#define FSLEN(s)  SEGLEN(ADR(s))
#define HLOCKED(s) LOCKED(ADR(s))
#define HNEXT(s)  (HOLE(s)->next)
#define HSIZE(s)  (HOLE(s)->size)
#define ISFREE(s) (HSHEAD(s) == (headptr)FREESEG)
#define MOVSEGH(s,d,l) { MOVSEG(s,d,l); AD(HSHEAD(d))=ADR(d); }

/* Private global data */
static hlen heapleft, heapfreed, heapused, heapmaxused;
static hadr heapstart, heapend, heapfree;

/* There is a global list of holes starting in heapfree */
/* If hadr h is a hole, then its pointer address is HOLE(h),
   and hence, its size is in HOLE(h)->size,
   and   its successor is in HOLE(h)->next */
/* So far, if hadr s is a hole occupied by a segment, then
   its segment address is ADR(s), and hence
   its headder is SEGHEAD(ADR(s)) or HSHEAD(s) and
   its size is HLEN(SEGLEN(ADR(s))) or HSLEN(s).
   However if hadr s is a free segment, i.e. HSHEAD(s)==SEGFREE,
   then its size is (SEGLEN(ADR(s)) or FSLEN(s).
   The value of type hlen can be added to the address of type hadr
   resulting in the address of type hadr,
   which points behind the given hole */








/* List of functions which do garbage collection */
#define MAXPF 10
typedef LOGICAL (*t_purfun)();
static t_purfun purfun[MAXPF];
static int m_purfun=0;
LOGICAL heapgrown; /* It is set when heap grows up */

void rf_prholes(FILE* f) { hadr cur; rf_heap_stat(f);
    for (cur=heapfree; cur != NULL ; cur = HNEXT(cur)) {
        fprintf(f,"   HOLE: %p ", HOLE(cur));
        fprintf(f," LENGTH %ld\n", (long) (HSIZE(cur)));
}       }

void prfree(int k)
 {  hadr *afree;
      fprintf(stdtrc,"\n   free(adr,len): ");
    for (afree = &heapfree; k-- AND *afree != NULL; afree = &(HNEXT(*afree)))
        fprintf(stdtrc,"(%p,%u) ",*afree, (unsigned int)*afree);
  }

void setpurge(t_purfun f)
{ if(m_purfun<MAXPF) purfun[m_purfun++]=f;
  else fprintf(stdtrc,"RFALLOC: purge funcs table overflow\n");
  };

LOGICAL allpurfun(hlen s)
{ LOGICAL res; int i;
 /* union REGS regs; */
  res = FALSE;
#define  CURSNORM 0x706
#define  CURSHIGH 0x800
#define SETCURS(l)
 /* { regs.h.ah=1; regs.x.cx=l; int86(0x10,&regs,&regs); } */
  SETCURS(CURSHIGH);
  if(strchr(flags,'s')) {
    fprintf(stdtrc,"PURGE MEMORY WHILE ALLOCATING %u ELEMENTS\n",(unsigned int)s);
    rf_prholes(stdtrc); /* print out list of holes */
    }
  ALL(i,m_purfun) if((res = res | (*(purfun[i]))())!=0) break;
  SETCURS(CURSNORM);
  return(res);
  };

static LOGICAL squeese(); /* Lowest purge function */
static LOGICAL freemem() {
    elemptr savb;
    long N;
    LOGICAL res;
    savb=b;    testmem(&N); b=savb;
    if(strchr(flags,'s')) {          /* print out list of holes */
       fprintf(stdtrc,"FREEMEM:\n");
       rf_prholes(stdtrc);
       fprintf(stdtrc,"END FREEMEM:\n");
       }
    res=(heapfreed!=0);
    return(res);
    };



static LOGICAL squeese() /* Lowest purge function */
{  hlen len;
   hadr cur,firsthole,*anexthole;
   LOGICAL res;
   hadr a, *afree;

res = FALSE;
anexthole= &firsthole;
firsthole = NOHOLE;

for (cur = heapstart ; cur!=heapend; cur = HADR_PLUS_LEN(cur,len)) {
    if (*anexthole == NULL) {
        /* There is no hole below the current segment */
        if (cur == heapfree) {
            /* make new hole from the old one */
            len = HSIZE(cur);
            heapfree = HNEXT(cur);
            }
        else if(ISFREE(cur)) {
            /* make new current hole from the free segment */
            len = FSLEN(cur);
            res = TRUE;
            }
        else {
            len = HSLEN(cur);
            if (HLOCKED(cur)) {
                continue;
                }
            /* find a hole to move segment cur into */
            afree = &firsthole;
            while (afree != anexthole) {
                if (HSIZE(*afree) >= len) break;
                afree = &HNEXT(*afree);
                }
            if(afree == anexthole) /* no hole */ continue;
            if(HSIZE(a=*afree) == len) {
                if (anexthole == &HNEXT(a)) anexthole = afree;
                else *afree = HNEXT(a);
                }
            else {
                *afree = HADR_PLUS_LEN(*afree,len);
                if (anexthole == &HNEXT(a)) anexthole = &HNEXT(*afree);
                else HNEXT(*afree)=HNEXT(a);
                HSIZE(*afree)=HSIZE(a)-len;
                }
            MOVSEGH(cur,a,len);
            res = TRUE;
            }
        /* make new hole from cur */
        *anexthole = cur;
        HNEXT(cur) = NOHOLE;
        HSIZE(cur) = len;
        }







    else { /* *anexthole != NULL, HNEXT(*anexthole) = NULL */
         /* There is a hole (*anexthole) below the current segment */
        if (cur == heapfree) {
            len = HSIZE(cur);
            /* append the old hole to the current one */
            HSIZE(*anexthole) += len;
            heapfree = HNEXT(heapfree);
            }
        else if(ISFREE(cur)) {
            /* append free segment to the current hole */
            len = FSLEN(cur);
            HSIZE(*anexthole) += len;
            }
        else {
            len = HSLEN(cur);
            if (HLOCKED(cur)) { anexthole = &HNEXT(*anexthole); continue; }
            /* if(NOT HLOCKED(cur)) */
            HSIZE(*anexthole) += len;
            /* find a hole to move segment cur into */
            afree = &firsthole;
            while(HSIZE(*afree) < len) {
                if (afree==anexthole) { fprintf(stdtrc,"SQUEESE: Error, no hole\n"); rf_exit(30); }
                afree = &HNEXT(*afree);
                }
            if(HSIZE(a = *afree) == len) { /* hence a is not *anexthole */
                if (anexthole == &HNEXT(a)) anexthole = afree;
                *afree = HNEXT(a);
                MOVSEGH(cur,a,len);
                }
            else { /* a may be the *anexthole */
                hadr savnext;
                hlen savlen;
                *afree = HADR_PLUS_LEN(*afree,len);
                if (anexthole == &HNEXT(a)) anexthole = &HNEXT(*afree);
                savnext = HNEXT(a);
                savlen = HSIZE(a) - len;
                MOVSEGH(cur,a,len);
                HNEXT(*afree)=savnext;
                HSIZE(*afree)=savlen;
            }    }
        res = TRUE;
    }    }
heapfree = firsthole;
heapleft = 0;
for (cur=heapfree; cur != NULL ; cur = HNEXT(cur)) {
    heapleft += HSIZE(cur);
    heapfreed=0;
    }
if(strchr(flags,'s')) {          /* print out list of holes */
     fprintf(stdtrc,"SQUEESE:\n");
     rf_prholes(stdtrc);
     fprintf(stdtrc,"END SQUEESE:\n");
     }
if(strchr(flags,'z')) prm=1;
return(res);
}




static hadr rf_halloc(hlen s)
{  hadr a, *afree;
S: for (afree = &heapfree; *afree != NULL; afree = &(HNEXT(*afree)) )
        if (HSIZE(*afree) >= s) break;
   if (*afree==NULL) {
                  if (NOT allpurfun(s)) { rf_error = ERRSTOR; return(0); }
                goto S;
                }
   a = *afree;
   if(HSIZE(a) == s) { *afree = HNEXT(a); }
   else {
      *afree=HADR_PLUS_LEN(*afree,s);
      HNEXT(*afree)=HNEXT(a);
      HSIZE(*afree)=HSIZE(a)-s;
      }
   HSHEAD(a) = FREESEG; /*  HNEXT(a) = FREESEG; */
   FSLEN(a) = s;        /*  HSIZE(a) = s; */
   heapleft-=s;
   heapused+=s;
   if(heapused>heapmaxused) heapmaxused = heapused;
   if(prm) fprintf(stdtrc,"rf_halloc: %u pars in %p\n",(unsigned int)s,a);
   return(a);
   };

static void rf_hfree(hadr s, hlen n)
{ HSHEAD(s) = FREESEG;  /*  HNEXT(s) = FREESEG; */
  FSLEN(s) =  n;        /*  HSIZE(s) = n; */
  heapused-=n;
  heapfreed+=n;
  if(prm) fprintf(stdtrc,"rf_hfree: %u paragraphs in %p\n",(unsigned int)n,s);
  }

void* rmalloc(seglen size)
{ hadr a;
  a = rf_halloc(HLEN(size));
  if(a==0) return(NULL);
  SEGHEAD(ADR(a))=NOHEAD;
  SEGLEN(ADR(a))=size;
    if(strchr(flags,'s'))
    fprintf(stdtrc,"Rmalloc: %u bytes in %p\n",size,SEGINF(ADR(a)));
  return(SEGINF(ADR(a)));
  };

void rfree(void* p)
{ hadr a;
  CHKP(p);
  a = HADRP(p);
  if(HSHEAD(a)!=NULL) fprintf(stdtrc,"Rfree: unlocked segment %p\n",p);
  rf_hfree(a,HSLEN(a));
  };










void rf_mem_init()
{ hlen size;
  long argh,tot;
  hadr start; /* in paragraphs */
  void far* a;
  tot = (CORELEFT)&(-1024L);
  if (arglong(flags,'H',&argh)) tot = argh*1024L;
  a = farmalloc(tot+16L);
  if(strchr(flags,'s')) fprintf(stdtrc,"\nAllocate heap at %p\n",a);
  start = (hadr) HEAPBASE(a);
  size = (hlen) (tot/SIZEOFPAR);
  if(strchr(flags,'s')) {
    fprintf(stdtrc,"Heap size is %ld, ",((long)size)*SIZEOFPAR);
    fprintf(stdtrc,"heap start is %p\n", HOLE(start));
    }
  anull = ADR(start);
  start+=ANULLSIZE;
  heapfree = heapstart = start;
  heapend = HADR_PLUS_LEN(heapstart,size);
  heapleft = size;
  HNEXT(heapfree) = NOHOLE;
  HSIZE(heapfree) = size;
  heapmaxused = heapused = heapfreed = 0;
  SEGHEAD(anull) = NOHEAD;
  SEGLEN(anull) = 0;
  setpurge(squeese);
  setpurge(freemem);
  };

adr rf_realloc(adr a, seglen size)
{ hlen s,l;
  hadr aa;
  if (size==0)
    { if (a != anull) rf_hfree(HADR(a),HLEN(SEGLEN(a))); return(anull); }
  s = HLEN(size);
  if (a==anull)  aa = rf_halloc(s);
  else {
    if (SEGHEAD(a)!=NULL)
      fprintf(stdtrc,"Rf_realloc: unlocked segment %p\n",HOLE(a));
    l = HLEN(SEGLEN(a));
    aa=HADR(a);
    if      (s<l) rf_hfree(HADR_PLUS_LEN(aa,s),l-s);
    else if (s>l) {
      aa = rf_halloc(s); /* segment a must be LOCKED */
      if (aa!=0) { MOVSEG(HADR(a),aa,l); rf_hfree(HADR(a),l); }
    } }
  if(aa==0) return(NOADR);
  SEGHEAD(ADR(aa)) = NOHEAD;  /* segment is locked */
  SEGLEN(ADR(aa))=size;
  return(ADR(aa));
};

void rf_heap_stat(FILE * f)
{ fprintf(f,"HEAP bytes: used %lu, freed %lu, left %lu, maxused - %lu\n",
                   (long)heapused*SIZEOFPAR,
                   (long)heapfreed*SIZEOFPAR,
                   (long)heapleft*SIZEOFPAR,
                   (long)heapmaxused*SIZEOFPAR);
};
