/******************************************************************
   R E F A L - 6      Copyright (C) IPS AN 1991
   fileio.c - Class FILEIO (instance of CHANNEL)
*******************************************************************/

#include "refcom.h"
#include "fileio.h"
#include "rfalloc.h"

static int funeof(FILE* f)
{ return(feof(f)); };

ifunc p_fileio (headptr h, int code, addr * aa)
{
   HDGETFAR(h,*aa);
   if(*aa==NULL) return(p_object(h,code,aa));
   switch (code) {
	 case _PUTC: return((ifunc)fputc);
	 case _GETC: return((ifunc)fgetc);
	 case _UNGETC: return((ifunc)ungetc);
	 case _FEOF: return((ifunc)funeof);
	 case _DONE: fclose((FILE*)(*aa)); return(itrue);
	 case _NEW: HDSETFAR(h,NULL); return(itrue);
	 default: return(p_channel(h, code, aa));
   }
};

void mk_fileio(headptr h, FILE* f)
{
  TYPE(h) = (f==NULL? HEADEMPTY : HD(FILEIO,OBJ));
  HDSETFAR(h,f);
}
