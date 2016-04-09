                              /* rfKey.c */
#ifndef __RFKEY_H
#include "rfkey.h"
#endif

#ifndef __STDIO_H
#include <stdio.h>
#define __STDIO_H
#endif

#ifndef __CONIO_H
#include <conio.h>
#define __CONIO_H
#endif

headptr stdkeyptr  = NOHEAD;

int     c_key      = 0;

int     KeyUngetch = EOF;

                            int rfkey_init ()
/************************************************************************/
{ c_key = iregtype (p_key, n_key, 0);
  if (iniextbox (n_stdkey, &stdkeyptr)  AND
      mk_key (stdkeyptr))
    { END (stdkeyptr);
      return (1);
    }
  return (0);
}

                            int rfkey_getc ()
/************************************************************************/
{ int ires;
  if ((ires = KeyUngetch) < EOF)
    { KeyUngetch = - ires;
      ires = 0;
    }
    else
    { if (ires == EOF)
        { for (; NOT kbhit (););
          ires = getch ();
        }
        else
          KeyUngetch = EOF;
    }
  return (ires);
}

                        int rfkey_ungetc (int c)
/************************************************************************/
{ return (KeyUngetch = c); }

              ifunc p_key (headptr h, int code, addr * aa)
/************************************************************************/
{ switch (code)
  { case _KEY:
      break;
    case _GETC:
      return ((ifunc)rfkey_getc);
    case _UNGETC:
      return ((ifunc)rfkey_ungetc);
    case _FEOF:
      return (ifalse);
    default:
      return (p_channel (h, code, aa));
  }
  return (itrue);
}

                         int mk_key (headptr h)
/************************************************************************/
{ TYPE (h)   = HEADKEY;
  KeyUngetch = EOF;
  return (1);
}
                           /* End of rfKey.c */
