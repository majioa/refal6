/******************************************************************
   R E F A L - 6      Copyright (C) IPS AN 1991
   channel.c - Class CHANNEL (generic)
*******************************************************************/

/* #define EXT_CHANNEL */
#include "channel.h"
#include <stdio.h>

int ieof() { return(EOF); };

ifunc p_channel(headptr h, int code, addr * aa)
{
  *aa = PTOFAR(h);
  switch(code) {
    case _GETC :
    case _PUTC :
    case _FEOF :
	case _UNGETC : return(ieof);
	case _CHANNEL : return(itrue);
	default: return(p_object(h,code,aa));
	}
};
