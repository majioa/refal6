/******************************************************************
   R E F A L - 6      Copyright (C) 1992 by Arkady Klimov
     rfio.h - functions from rfoutp.c and rfinp.c
*******************************************************************/

#include <stdint.h>
#include <unistd.h>
#include "refelem.h"
#include "channel.h"

/******************************* Entries from rfoutp.c */
void rf_errmsgs(char * s);
void rf_errmsg(int n);
void rf_err(int n);
headptr mkh_fileio(FILE* f);
void initio();
void rf_close(int n);
void endio();
void outwdh (FILE* f, Tputc ptc, headptr h, LOGICAL isexp);
void outref(FILE* f, Tputc ptc,
          headptr hx,  /* object headder */
          LOGICAL isexp, headptr table);
FILE * rf_open(char * fn, char* mod);
void rf_outm (FILE * f, Tputc ptc,
          elemptr p1, elemptr p2,
          LOGICAL isexp, headptr table);
void rf_outc (headptr cp,
          elemptr p1, elemptr p2,
          LOGICAL isexp, headptr table);
typedef int (*Tsave) ();

/******************************* Entries from rfinp.c */
typedef enum { termfeed, linefeed, filefeed, rightbrac, anglebrac } termcond;

/* Input object */
typedef struct {
    FILE* f;
    Tgetc gtc;
    Tungetc ungtc;
    headptr table;
    int c;
    } in;

/* Function rf_inpm is recursive over the input line bracket structure */

LOGICAL rf_inpc(
    headptr inch, /* input channel object (STARTed) */
    headptr outcont, /* output container object (STARTed) */
    termcond tcond, headptr table);

LOGICAL rf_insc(headptr inch, headptr outch);

/* Read a block into a buffer.
   The result is FAIL if less then *alen bytes have been read */
LOGICAL rf_read(
     headptr inch,  /* input channel object (STARTed) */
     seglen * alen,   /* in: maximum number of characters to input
                        out: number of characters actually input */
     uchar * buf,   /* buffer to write into */
     LOGICAL line   /* break on Line Feed */);

typedef int (*Tload) (in* ain, addr a);
extern LOGICAL iload (in* ain, addr a);
