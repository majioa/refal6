/************************************************************************/
/*  R E F A L - 6           Copyright (C) 1992 by Arkady Klimov         */
/*       rfstor.h    - storage control routines                         */
/************************************************************************/

#ifndef __RFSTOR_H
#define __RFSTOR_H

#ifdef STOR_OWN
#define EXT
#else
#define EXT extern
#endif

typedef unsigned seglen;  /* ### type for segment length (see rfalloc.h) */
typedef int      iseglen; /* ### type for segment length (see rfalloc.h) */

#define NWDS 6
enum    word_number   { _T, _F, _TABLE, _SYSTABLE, _SYSARG, _SYSID };

EXT        cvalue   COMWORD [NWDS];
EXT     elemptr  Afreestor;        /* Free storage head element        */
#define Zfreestor PREV(Afreestor)
EXT     elemptr  b;                /* Right end of result formed       */
EXT     elemptr  systable;         /* Address of SYSTABLE headder      */
EXT     elemptr  sysarg;           /* Address of SYSARG headder        */
EXT     elemptr  sysid;            /* Address of SYSID headder         */
EXT     elemptr  blbr;             /* Address of term () headder       */
EXT     char     flags[MAXFLAG];   /* Flags from DOS command           */
EXT     elemptr  lbrac;            /* Last left bracket not closed     */
EXT     int      rf_error;         /* Error code                       */
EXT     int      rf_exitcd;        /* Exit code when end               */
EXT     char *   exename;          /* Arg(0)                           */
EXT     char *   ininame;          /* Arg(1) or "default.ini"          */
EXT     headptr  hashwd[HASHWD];   /* Hash-table for words             */
#define MBUF     79           /* Maximum length of output buffer  */
EXT     int      LBUF;           /* Length of output buffer          */
EXT     char*    Buf;            /* Buffer for pretty output         */

extern FILE *    stdtrc;           /* File for debug information       */

#ifdef STOR_OWN
       FILE * stdtrc = stderr;
static char* WORDS[NWDS] = {"T","F","TABLE","SYSTABLE","SYSARG","SYSID"};
static long  totstor,totword;
#endif
















LOGICAL arglong(char*, int, long *);
void    headptrfree(headptr);
void    cvalfree(cvalue);
void    elemfree(elemptr);
void    cvalact(cvalue c);

LOGICAL rf_mk_sword(char* nm, cvalue * ac);
LOGICAL rf_mk_word (uchar* n, seglen ln, cvalue * ac, int simplify);
                   /* result cvalue is actual */

LOGICAL newword(char*);
int     initstor(int, char**);
LOGICAL rf_movb();
LOGICAL rf_movbx(elemptr *);

void fromhashlist(headptr h);
void tohashlist(headptr* ah);
LOGICAL testmem (long * al);
long int memleft();
void countstor(long* tot, long* rest, long* used);
void rf_mem_stat(FILE* f);
void printallwords(FILE* f);

#endif