                             /* rfRect.c */
#ifndef __RFRECT_H
#include "rfrect.h"
#endif

                 int rfrect_get (elemptr p, rect * rptr)
/************************************************************************/
{ if (ISBRAC (p))
    { elemptr r;
      p = REF (p);
      if ((r = PREV (p)) == p) goto fail;
      if ((p = NEXT (p)) == r  OR  NOT ISSNUMB (p)) goto fail;
      rptr->x = VAL (p);
      if ((p = NEXT (p)) == r  OR  NOT ISSNUMB (p)) goto fail;
      rptr->y = VAL (p);
      if ((p = NEXT (p)) == r  OR  NOT ISSNUMB (p)) goto fail;
      rptr->w = VAL (p);
      if ((p = NEXT (p)) != r  OR  NOT ISSNUMB (p)) goto fail;
      rptr->h = VAL (p);
      return (1);
    }
    else
    { headptr h;
      if (ISREF (p)  AND  ISOBJ (h=REF00(p))  AND
          START (h) != inull                  AND
          HCALL (h, _RECT_GET, Trect_get, (THIS, rptr)))
        { END   (h);
          return (1);
        }
    }
fail:
  return (0);
}

              ifunc p_rect (headptr h, int code, addr * aa)
/************************************************************************/
{ switch (code)
    { case _RECT:
          break;
      default:
          return (p_object (h, code, aa));
    }
  return (itrue);
}
                          /* End of rfRect.c */
