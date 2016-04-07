/************************************************************************/
/*     R E F A L - 6   Copyright (C) 1992 by Alex Zibrov                */
/*             rbRect.c  -  rect-area built-in functions                */
/************************************************************************/
/*                                                                      */
/*                          List of functions:                          */
/*                                                                      */
/*  t.Rect is either s.Slide or (sX sY sW sH)                           */
/*E    <RECT_GET t.Rect> == (sX sY sW sH)           ,   rb_rect_get     */
/*E    <RECT_SET s.Slide t.Rect> ==                 ,   rb_rect_set     */
/*E    <RECT_INTRS e.Rects> == (sX sY sW sH)        ,   rb_rect_intrs   */
/*E    <RECT_UNION e.Rects> == (sX sY sW sH)        ,   rb_rect_union   */
/*E    <RECT_DIFF t.Rect1 [t.Rect2]> == sT sL sR sB ,   rb_rect_diff    */
/*E    <RECT_EMPTY t.Rect> == T | F                 ,   rb_rect_empty   */
/*                                                                      */
/************************************************************************/

#include "refgen.h"
#include "rfrect.h"

                         LOGICAL rb_rect_get ()
/************************************************************************
<RECT_GET t.srect> -> (e.rect)  --  get rect-area of t.srect
*************************************************************************/
{ rect      r;
  elemptr   savb = b;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
      bl;
  if (EargL != NULL  AND  EargL == EargR  AND
      rfrect_get (EargL, &r)  AND
      RFRECT_PUT (&r))
    { br;
      *AeresL = NEXT (savb);
      *AeresR = b;
      BETWEEN (savb, NEXT(b), EargL, EargR);
      b = savb;
      return (TRUE);
    }
  return (FALSE);
}

                         LOGICAL rb_rect_set ()
/************************************************************************
<RECT_SET s.srect t.rect> -> EMPTY  --  set rect-area to t.rect
*************************************************************************/
{ headptr   h;
  rect      r;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NULL  AND  EargL != EargR  AND  NEXT (EargL) == EargR  AND
      rfrect_get (EargR, &r)  AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))  AND
      START (h) != inull      AND
      HCALL (h, _RECT_SET, Trect_set, (THIS, &r)))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
  return (FALSE);
}

                        LOGICAL rb_rect_intrs ()
/************************************************************************
                        Intersections rect-areas.
<RECT_INTRS t.rect1 [t.rect2 ...]> -> (e.rect)
*************************************************************************/
{ rect      r1;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NOELEM  OR  NOT
      rfrect_get (EargR, &r1)) goto fail;
  for (p = EargL; p != EargR; p = NEXT (p))
    { rect r2;
      if (NOT rfrect_get (p, &r2)) goto fail;
      RECT_INTRS (&r1, &r2);
    }
  p = b;
  bl;
  if (RFRECT_PUT (&r1))
    { br;
      *AeresL = NEXT (p);
      *AeresR = b;
      BETWEEN (p, NEXT(b), EargL, EargR);
      b = p;
      return (TRUE);
    }
fail:
  return (FALSE);
}

                        LOGICAL rb_rect_union ()
/************************************************************************
                           Unions rect-areas.
<RECT_UNION t.rect1 [t.rect2 ...]> -> (e.rect)
*************************************************************************/
{ rect      r1;
  int       empty;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NOELEM  OR  NOT
      rfrect_get (EargR, &r1)) goto fail;
  empty = RECT_EMPTY (&r1);
  for (p = EargL; p != EargR; p = NEXT (p))
    { rect r2;
      if (NOT rfrect_get (p, &r2)) goto fail;
      if (NOT RECT_EMPTY (&r2))
        { if (empty)
            { RECT_COPY (&r1, &r2);
              empty = 0;
            }
            else
            { r1.w += r1.x;
              r1.h += r1.y;
              r2.w += r2.x;
              r2.h += r2.y;
              r1.x  = min (r1.x, r2.x);
              r1.y  = min (r1.y, r2.y);
              r1.w  = max (r1.w, r2.w) - r1.x;
              r1.h  = max (r1.h, r2.h) - r1.y;
            }
        }
    }
  p = b;
  bl;
  if (RFRECT_PUT (&r1))
    { br;
      *AeresL = NEXT (p);
      *AeresR = b;
      BETWEEN (p, NEXT(b), EargL, EargR);
      b = p;
      return (TRUE);
    }
fail:
  return (FALSE);
}

                        LOGICAL rb_rect_diff ()
/************************************************************************
                        Subtract rect-areas.
<RECT_DIFF t.rect1 [t.rect2]> -> (e.up) (e.left) (e.right) (e.bottom)
*************************************************************************/
{ rect      r1;
  int       zzzz;
  elemptr   savb = b;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NOELEM  OR  NOT
      rfrect_get (EargL, &r1)) goto fail;
  if (EargL != EargR)
    { rect r2;
      if (NEXT (EargL) != EargR  OR  NOT
          rfrect_get (EargR, &r2)) goto fail;
      if (RECT_EMPTY (&r1)  OR  RECT_EMPTY (&r2)) goto one;
      zzzz = r1.y + r1.h;
/* top: */
      r1.h = r2.y - r1.y;
      bl;
      if (NOT RFRECT_PUT (&r1)) goto fail;
      br;
      r1.h = zzzz;
      zzzz = r2.x + r2.w;
/* left: */
      r2.w = r2.x - r1.x;
      r2.x = r1.x;
      r2.h = r1.h - r2.y;
      bl;
      if (NOT RFRECT_PUT (&r2)) goto fail;
      br;
/* right: */
      r2.x = zzzz;
      r2.w = r1.x + r1.w - r2.x;
      bl;
      if (NOT RFRECT_PUT (&r2)) goto fail;
      br;
/* buttom: */
      r1.y  = r2.y + r2.h;
      r1.h -= r1.y;
      bl;
      if (NOT RFRECT_PUT (&r1)) goto fail;
      br;
    }
    else
    { headptr h;
one:  bl;
      if (NOT RFRECT_PUT (&r1)) goto fail;
      br;
      r1.x = r1.y = r1.w = r1.h = 0;
      bl;
      if (NOT RFRECT_PUT (&r1)) goto fail;
      br;
      h = REF (b);
      VAL (h) += 2;
      for (zzzz = 2; zzzz-- > 0;)
        { movb;
          SETBRAC (b, h);
        }
    }
  *AeresL = NEXT (savb);
  *AeresR = b;
  BETWEEN (savb, NEXT(b), EargL, EargR);
  b = savb;
  return (TRUE);
fail:
  return (FALSE);
}

                        LOGICAL rb_rect_empty ()
/************************************************************************
                            Empty rect-area?
<RECT_EMPTY t.rect> -> T | F
*************************************************************************/
{ rect      r;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL == EargR  AND
      rfrect_get (EargL, &r))
    { elemptr savb = b;
      cvalue  res  = COMWORD[(RECT_EMPTY (&r) ? _T : _F )] ;
      movb;
      SETDATA (b, res);
      *AeresL = *AeresR = b;
      BETWEEN (savb, NEXT(b), EargL, EargR);
      b = savb;
      return (TRUE);
    }
  return (FALSE);
}
#include "rfrect.c"
#include "rect.c"
                          /* End of rbRect.c */
