/******************************************************************
   R E F A L - 6      Copyright (C) 1992 by Arkady Klimov
   Class CHANNEL (generic)
*******************************************************************/

#ifndef __CHANNEL_H
#define __CHANNEL_H

#ifndef __REFCOM_H
#include "refcom.h"
#endif

#include <stdio.h>

/* Methods */
#define _CHANNEL        20
#define _PUTC   _CHANNEL+1
#define _GETC   _CHANNEL+2
#define _UNGETC _CHANNEL+3
#define _FEOF   _CHANNEL+4

typedef int (*Tputc) (int c, FILE* f);
typedef int (*Tgetc) (FILE* f);
typedef int (*Tungetc) (int c, FILE* f);
typedef int (*Tfeof) (FILE* f);

#define GETC(O)     EVAL(O,_GETC,Tgetc,((FILE*)wrkptr))
#define PUTC(C,O)   EVAL(O,_PUTC,Tputc,(C,(FILE*)wrkptr))
#define UNGETC(C,O) EVAL(O,_UNGETC,Tungetc,(C,(FILE*)wrkptr))
#define FEOF(O)     EVAL(O,_FEOF,Tfeof,((FILE*)wrkptr))

ifunc p_channel(headptr, int, addr*);

#endif