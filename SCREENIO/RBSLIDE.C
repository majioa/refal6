/************************************************************************/
/*     R E F A L - 6   Copyright (C) 1992 by Alex Zibrov                */
/*                rbSlide.c  - slide built-in functions                 */
/************************************************************************/
/*E  <MAKE_SLIDE s.Loc t.Rect> == s.Loc             , rb_make_slide     */
/*E  <MAKE_SUBSLIDE s.Loc s.Sld [t.Rect]> == s.Loc  , rb_make_subslide  */
/*E  <SLIDE_FILL s.Sld s.Char s.Attr [t.Rect] ==    , rb_slide_fill     */
/*E  <SLIDE_CORR s.Sld s.Fun s.Attr [t.Rect] ==     , rb_slide_corr     */
/*E  <SLIDE_COPY s.Sld1 s.Sld2 [t.Rect] ==          , rb_slide_copy     */
/*E  <SLIDE_SWAP s.Sld1 s.Sld2 [t.Rect] ==          , rb_slide_swap     */
/*E  <SLIDE_POS_GET s.Sld> == s.PosX s.PosY         , rb_slide_pos_get  */
/*E  <SLIDE_POS_SET s.Sld s.PosX s.PosY> ==         , rb_slide_pos_set  */
/*E  <SLIDE_ATTR_GET s.Sld> == s.Attr               , rb_slide_attr_get */
/*E  <SLIDE_ATTR_SET s.Sld s.Attr> ==               , rb_slide_attr_set */
/*E  <SLIDE_CURS_GET s.Sld> == s.CurX s.CurY s.Type , rb_slide_curs_get */
/*E  <SLIDE_CURS_SET s.Sld s.CurX s.CurY s.Type> == , rb_slide_curs_set */
/*E  <SLIDE_OWNER_GET s.Sld> == s.Osld sX sY        , rb_slide_owner_get*/
/*E  <SLIDE_XY_SET s.Sld sX sY> ==                  , rb_slide_xy_set   */
/*E  <MAKE_SCREEN s.Loc s.Page> == s.Loc            , rb_make_screen    */
/*E  <SCREEN_SHOW s.Scr> ==                         , rb_screen_show    */
/************************************************************************/

#ifndef __RFSLIDE_H
#include "rfslide.h"
#endif

#ifndef __REFGEN_H
#include "refgen.h"
#endif

                        LOGICAL rb_make_slide ()
/************************************************************************
<MAKE_SLIDE s.object t.rect> -> s.object  --  make new slide-object
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
      ISREF (EargL)           AND  mk_slide (h=REF00(EargL), &r))
    { END (h);
      *AeresL = *AeresR = EargL;
      AFTER1 (b, EargR);
      return (TRUE);
    }
  return (FALSE);
}

                       LOGICAL rb_make_subslide ()
/************************************************************************
                        Make new subslide-object.
<MAKE_SUBSLIDE s.object s.slide [t.rect]> -> s.object
*************************************************************************/
{ headptr   h, hs;
  rect    * rptr = NULL;
  rect      r;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NULL  OR  EargL == EargR) goto fail;
  if ((p = NEXT (EargL)) != EargR)
      if (NEXT (p) != EargR  OR NOT
          rfrect_get (EargR, rptr=&r)) goto fail;
  if (ISREF (EargL)  AND  ISREF (p)  AND  ISOBJ (hs=REF00(p))  AND
      HCALL (hs, _MAKE_SUBSLIDE, Tmake_subslide, (h=REF00(EargL), hs, rptr)))
    { END   (h);
      *AeresL = *AeresR = EargL;
      AFTER (b, p, EargR);
      return (TRUE);
    }
fail:
  return (FALSE);
}

                         LOGICAL rb_slide_fill ()
/************************************************************************
                      Filling rect-area into slide.
<SLIDE_FILL s.slide  -- slide;
            s.char   -- filling char;
            s.attr   -- filling attribute;
           [t.rect]> -- rect-area.
                     -> EMPTY
*************************************************************************/
{ headptr   h;
  slide   * sptr;
  int       chr;
  rect    * rptr = NULL;
  rect      r;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NULL  OR  EargL == EargR  OR
     (p = NEXT (EargL)) == EargR         OR  NOT
      ISREF (EargL)  OR  NOT ISOBJ (h=REF00(EargL))  OR NOT
      ISCHAR (p)) goto fail;
  chr = VAL  (p);
  p   = NEXT (p);
  if (NOT ISSNUMB (p)) goto fail;
  if (p != EargR)
      if (NEXT (p) != EargR  OR  NOT
          rfrect_get (EargR, rptr=&r)) goto fail;
  if (START (h) != inull  AND
     (sptr = (slide *) HADDR (h, _SLIDE)) != NULL  AND
      slide_fill (sptr, chr, VAL (p), rptr))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
fail:
  return (FALSE);
}

                         LOGICAL rb_slide_corr ()
/************************************************************************
                     Correction rect-area into slide.
<SLIDE_CORR s.slide  -- slide;
            s.bool   -- number of corrective boolean function;
            s.attr   -- corrective attribute;
           [t.rect]> -- rect-area.
                     -> EMPTY
*************************************************************************/
{ headptr   h;
  slide   * sptr;
  int       bool;
  rect    * rptr = NOELEM;
  rect      r;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NOELEM  OR  EargL == EargR  OR
     (p = NEXT (EargL)) == EargR         OR  NOT
      ISREF (EargL)  OR  NOT ISOBJ (h=REF00(EargL))  OR NOT
      ISSNUMB (p)) goto fail;
  bool = VAL  (p);
  p    = NEXT (p);
  if (NOT ISSNUMB (p)) goto fail;
  if (p != EargR)
      if (NEXT (p) != EargR  OR  NOT
          rfrect_get (EargR, rptr=&r)) goto fail;
  if (START (h) != inull  AND
     (sptr = (slide *) HADDR (h, _SLIDE)) != NOELEM  AND
      slide_corr (sptr, bool, VAL (p), rptr))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
fail:
  return (FALSE);
}

                         LOGICAL rb_slide_copy ()
/************************************************************************
                    Copy slide2 to slide1 on to rect.
<SLIDE_COPY s.slide1 s.slide2 [t.rect]> -> EMPTY
*************************************************************************/
{ headptr   h1, h2;
  slide   * sptr1;
  slide   * sptr2;
  rect    * rptr = NULL;
  rect      r;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NOELEM  OR  EargL == EargR) goto fail;
  if ((p = NEXT (EargL)) != EargR)
      if (NEXT (p) != EargR  OR NOT
          rfrect_get (EargR, rptr=&r)) goto fail;
  if (ISREF (EargL)  AND  ISOBJ (h1=REF00(EargL))  AND
      ISREF (p)      AND  ISOBJ (h2=REF00(p))      AND
      START (h1) != inull  AND
      START (h2) != inull  AND
     (sptr1 = (slide *) HADDR (h1, _SLIDE)) != NULL  AND
     (sptr2 = (slide *) HADDR (h2, _SLIDE)) != NULL  AND
      slide_copy (sptr1, sptr2, rptr))
    { END   (h2);
      END   (h1);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
fail:
  return (FALSE);
}

                         LOGICAL rb_slide_swap ()
/************************************************************************
              Swaping between slide1 and slide2 on to rect.
<SLIDE_SWAP s.slide1 s.slide2 [t.rect]> -> EMPTY
*************************************************************************/
{ headptr   h1, h2;
  slide   * sptr1;
  slide   * sptr2;
  rect    * rptr = NULL;
  rect      r;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL == NOELEM  OR  EargL == EargR) goto fail;
  if ((p = NEXT (EargL)) != EargR)
      if (NEXT (p) != EargR  OR NOT
          rfrect_get (EargR, rptr=&r)) goto fail;
  if (ISREF (EargL)  AND  ISOBJ (h1=REF00(EargL))  AND
      ISREF (p)      AND  ISOBJ (h2=REF00(p))      AND
      START (h1) != inull  AND
      START (h2) != inull  AND
     (sptr1 = (slide *) HADDR (h1, _SLIDE)) != NULL  AND
     (sptr2 = (slide *) HADDR (h2, _SLIDE)) != NULL  AND
      slide_swap (sptr1, sptr2, rptr))
    { END   (h2);
      END   (h1);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
fail:
  return (FALSE);
}

                       LOGICAL rb_slide_pos_get ()
/************************************************************************
<SLIDE_POS_GET s.slide> -> s.posX s.posY  --  get XY-position
*************************************************************************/
{ headptr   h;
  slide   * sptr;
  elemptr   savb = b;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL == EargR  AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))   AND
      START (h) != inull                  AND
     (sptr = (slide *) HADDR (h, _SLIDE)) != NULL  AND
      rf_mknumb ((long) sptr->posx)       AND
      rf_mknumb ((long) sptr->posy))
    { END (h);
      *AeresL = NEXT (savb);
      *AeresR = b;
      BETWEEN (savb, NEXT(b), EargL, EargR);
      b = savb;
      return (TRUE);
    }
  return (FALSE);
}

                       LOGICAL rb_slide_pos_set ()
/************************************************************************
<SLIDE_POS_SET s.slide s.posX s.posY> -> EMPTY  --  set XY-position
*************************************************************************/
{ headptr   h;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL != EargR    AND
     (p = NEXT (EargL)) == PREV (EargR)     AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))  AND
      START (h) != inull  AND  ISSNUMB (p)  AND  ISSNUMB (EargR)  AND
      HCALL (h, _POS_SET, Tpos_set, (wrkptr, VAL (p), VAL (EargR))))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
  return (FALSE);
}

                       LOGICAL rb_slide_attr_get ()
/************************************************************************
<SLIDE_ATTR_GET s.slide> -> s.attr -- get current attribute
*************************************************************************/
{ headptr   h;
  slide   * sptr;
  elemptr   savb = b;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL == EargR           AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))   AND
      START (h) != inull                           AND
     (sptr = (slide *) HADDR (h, _SLIDE)) != NULL  AND
      rf_mknumb ((long) sptr->attr))
    { END (h);
      *AeresL = *AeresR = b;
      BETWEEN (savb, NEXT(b), EargL, EargR);
      b = savb;
      return (TRUE);
    }
  return (FALSE);
}
                       LOGICAL rb_slide_attr_set ()
/************************************************************************
<SLIDE_ATTR_SET s.slide s.attr> -> EMPTY  --  set current attribute
*************************************************************************/
{ headptr   h;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL != EargR  AND  NEXT (EargL) == EargR  AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))  AND
      START (h) != inull  AND  ISSNUMB (EargR)    AND
      HCALL (h, _ATTR_SET, Tattr_set, (wrkptr, VAL(EargR))))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
  return (FALSE);
}

                       LOGICAL rb_slide_curs_get ()
/************************************************************************
                    Get coordinate and type of cursor.
<SLIDE_CURS_GET s.slide> -> s.curX s.curY s.type
*************************************************************************/
{ headptr   h;
  slide   * sptr;
  elemptr   savb = b;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL == EargR  AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))   AND
      START (h) != inull                           AND
     (sptr = (slide *) HADDR (h, _SLIDE)) != NULL  AND
      rf_mknumb ((long) sptr->curx)       AND
      rf_mknumb ((long) sptr->cury)       AND
      rf_mknumb ((long) sptr->curt))
    { END (h);
      *AeresL = NEXT (savb);
      *AeresR = b;
      BETWEEN (savb, NEXT(b), EargL, EargR);
      b = savb;
      return (TRUE);
    }
  return (FALSE);
}

                       LOGICAL rb_slide_curs_set ()
/************************************************************************
                    Set coordinate and type of cursor.
<SLIDE_CURS_SET s.slide s.curX s.curY s.type> -> EMPTY
*************************************************************************/
{ headptr   h;
  elemptr   p1, p2;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL != EargR  AND
     (p1 = NEXT (EargL)) != EargR         AND
     (p2 = NEXT (p1)) == PREV (EargR)     AND
      ISSNUMB (p1)   AND  ISSNUMB (p2)  AND  ISSNUMB (EargR)  AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))              AND
      START (h) != inull                                      AND
      HCALL (h,_CURS_SET,Tcurs_set,(wrkptr,VAL(p1),VAL(p2),VAL(EargR))))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
  return (FALSE);
}

                      LOGICAL rb_slide_owner_get ()
/************************************************************************
<SLIDE_OWNER_GET s.slide> -> s.o_slide s.ownX s.ownY  --  get owner slide
*************************************************************************/
{ headptr   h;
  elemptr   savb = b;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL == EargR          AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))  AND
      START (h) != inull  AND  HADDR (h, _SLIDE) != NULL)
    { rfslide * rptr = (rfslide *) SEGINF (AD(h));
      slide   * sptr = rptr->sptr;
      attrptr   aptr = sptr->aptr;
      int       x, y;
      movb;
      SETREF (b, rptr->optr, TYPEREF);
      ++ VAL (rptr->optr);
/*    do sptr = sptr->sptr;         */
/*    while (sptr != sptr->sptr);   */
      slide_axy (sptr->sptr, aptr, &x, &y);
      if (rf_mknumb ((long) x)  AND
          rf_mknumb ((long) y))
        { END (h);
          *AeresL = NEXT (savb);
          *AeresR = b;
          BETWEEN (savb, NEXT(b), EargL, EargR);
          b = savb;
          return (TRUE);
        }
    }
  return (FALSE);
}

                        LOGICAL rb_slide_xy_set ()
/************************************************************************
<SLIDE_XY_SET s.slide sX sY> -> EMPTY  --  set XY-position {left, top}
*************************************************************************/
{ headptr   h;
  elemptr   p;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL != EargR    AND
     (p = NEXT (EargL)) == PREV (EargR)     AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))  AND
      START (h) != inull  AND  ISSNUMB (p)  AND  ISSNUMB (EargR)  AND
      HCALL (h, _XY_SET, Txy_set, (wrkptr, VAL (p), VAL (EargR))))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER (b, EargL, EargR);
      return (TRUE);
    }
  return (FALSE);
}

/************************************************************************/
/*                                                                      */
/*                            S C R E E N S                             */
/*                                                                      */
/************************************************************************/

                        LOGICAL rb_make_screen ()
/************************************************************************
<MAKE_SCREEN s.object s.page> -> s.object  --  make new screen-object
*************************************************************************/
{ headptr   h;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL != EargR   AND
      NEXT (EargL) == EargR                AND
      ISREF (EargL)  AND  ISSNUMB (EargR)  AND
      mk_screen (h=REF00(EargL), VAL (EargR)))
    { END (h);
      *AeresL = *AeresR = EargL;
      AFTER1 (b, EargR);
      return (TRUE);
    }
  return (FALSE);
}

                        LOGICAL rb_screen_show ()
/************************************************************************
<SCREEN_SHOW s.screen> -> EMPTY  --  show screen (set page)
*************************************************************************/
{ headptr   h;
  screen  * sptr;
  elemptr   EargL, EargR;
  elemptr * AeresL;
  elemptr * AeresR;
  popae (AeresL, AeresR);
  pope  (EargL,  EargR);
  if (EargL != NOELEM  AND  EargL == EargR             AND
      ISREF (EargL)  AND  ISOBJ (h=REF00(EargL))     AND
      START (h) != inull                             AND
     (sptr = (screen *) HADDR (h, _SCREEN)) != NULL  AND
      screen_show (sptr))
    { END   (h);
      *AeresL = *AeresR = NOELEM;
      AFTER1 (b, EargL);
      return (TRUE);
    }
  return (FALSE);
}
#include "rfslide.c"
#include "slide.c"
#include "screen.c"
#include "attrchr.c"
                          /* End of rbSlide.c */
