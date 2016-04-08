/************************************************************************/
/*                                                                      */
/*                             R E F A L - 6                            */
/*                                                                      */
/*  Copyright (C) 1988 by Refal Systems Inc.                            */
/*  Copyright (C) 1988 by Program Systems Institute AS USSR             */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/*                rbKey.c - keyboard built-in functions                 */
/*                                                                      */
/************************************************************************/

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

/************************************************************************/
/*                                                                      */
/*                          List of functions:                          */
/*                                                                      */
/*E          <KEY_READ> == sK          ,    rb_key_read                 */
/*E          <KEY_UNREAD sK> ==        ,    rb_key_unread               */
/*E          <KEY_PRESSED> == T | F    ,    rb_key_pressed              */
/*                                                                      */
/************************************************************************/

                         LOGICAL rb_key_read ()
/**********************************************************************
            Read s.key from the keyboard STDKEY (with wait).
<KEY_READ> -> s.key  --  >= 0: ascii code; < 0: -(scan_keyboard_code).
***********************************************************************/
{ elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  elemptr   savb = b;
  popae (AeresL, AeresR);
  pope  (EargL, EargR);
  if (EargL != NULL) return (FALSE);
  movb;
  TYPE(b) = TYPESNUMB;
  if ((VAL(b) = KeyUngetch) == EOF)
    { for (; NOT kbhit (););
      if ((VAL(b) = getch ()) == 0)
          VAL(b) = - getch ();
    }
  KeyUngetch = EOF;
  *AeresL = NEXT (savb);
  *AeresR = b;
  weld (savb, NEXT (b));
  b = savb;
  return (TRUE);
}

                        LOGICAL rb_key_unread ()
/**********************************************************************
<KEY_UNREAD s.char> -> EMPTY  --  unget s.char to the keyboard STDKEY
***********************************************************************/
{ elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL, EargR);
  *AeresL = *AeresR = NOELEM;
  if (EargL != NOELEM  AND  EargL == EargR  AND  ISSIMPL(EargL))
    { KeyUngetch = VAL (EargL);
      AFTER1 (b, EargL);
      return (TRUE);
    }
  return (FALSE);
}

                        LOGICAL rb_key_pressed ()
/************************************************************************
<KEY_PRESSED> -> T | F  --  STDKEY is empty?
*************************************************************************/
{ elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NOELEM)
    { elemptr savb = b;
      cvalue  res  = COMWORD[( KeyUngetch != EOF  OR  kbhit () ? _T : _F )] ;
      movb;
      SETDATA (b, res);
      *AeresL = *AeresR = b;
      weld (savb, NEXT(b));
      b = savb;
      return (TRUE);
    }
  return (FALSE);
}
#include "rfkey.c"
                           /* End of rbKey.c */
