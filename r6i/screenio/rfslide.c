                             /* rfSlide.c */
#ifndef __RFSLIDE_H
#include "rfslide.h"
#endif

#ifndef __REFGEN_H
#include "refgen.h"
#endif

headptr  stdscrptr = NOHEAD;

int      c_slide   = 0;
int      c_screen  = 0;
int      c_snap    = 0;

                           int rfslide_init ()
/************************************************************************/
{ c_slide  = iregtype (p_slide,  n_slide,  0);
  c_screen = iregtype (p_screen, n_screen, 0);
  c_snap   = iregtype (p_snap,   n_snap,   0);
  if (iniextbox (n_stdscr, &stdscrptr)  AND
      mk_screen (stdscrptr, 0))
    { END (stdscrptr);
      return (1);
    }
  return (0);
}

             ifunc p_slide (headptr h, int code, addr * aa)
/************************************************************************/
{ adr       adh;
  rfslide * rptr;
  headptr   hs;
  if ((adh = AD (h)) == ANULL) goto fail;
  rptr = (rfslide *) SEGINF (adh);
  hs   = rptr->optr;
  *aa  = rptr->sptr;
  switch (code)
    { case _START:
        { slide * sptr;
          if (p_alloc (h, code, aa) == inull) goto fail;
          rptr = (rfslide *) SEGINF (AD(h));
          sptr = rptr->sptr = (slide *) ((char *) rptr + sizeof (rfslide));
          if ((hs = rptr->optr) != h)
            { slide * sptrs;
              if (START (hs) == inull                            OR
                 (sptrs = (slide *) HADDR (hs, _SLIDE)) == NULL  OR
                  sptrs->xsize != sptr->xsize) goto fail;
              if (NOT RECT_EMPTY (sptr)  AND
                 (rptr->odx < 0  OR  rptr->ody < 0  OR
                  rptr->odx + sptr->w > sptrs->w    OR
                  rptr->ody + sptr->h > sptrs->h)) goto fail;
              sptr->aptr = sptrs->aptr + rptr->odx + rptr->ody * sptrs->xsize;
              sptr->sptr = sptrs;
            }
            else
            { if (rptr->odx | rptr->ody) goto fail;
              sptr->aptr = (attrptr) ((char *) sptr + sizeof (slide));
              sptr->sptr = sptr;
            }
          slide_pos_set (sptr, sptr->posx, sptr->posy);
          break;
        }
      case _END:
        { if (h != hs)
            { END (hs); }
          return (p_alloc (h, code, aa));
        }
      case _DONE:
        { LOCK (h);
          if (h != hs)
              headptrfree (hs);
          UNLOCK (h);
          return (p_alloc (h, code, aa));
        }
      case _SLIDE:
        break;
      case _MAKE_SUBSLIDE:
        return ((ifunc)mk_subslide);
      case _POS_SET:
        return ((ifunc)slide_pos_set);
      case _ATTR_SET:
        return ((ifunc)slide_attr_set);
      case _CURS_SET:
        return ((ifunc)slide_curs_set);
      case _XY_SET:
          return ((ifunc)slide_xy_set);
      case _PUTC:
          return ((ifunc)slide_putc);
      case _GETC:
          return ((ifunc)slide_getc);
      case _UNGETC:
          return ((ifunc)slide_ungetc);
      case _FEOF:
          return ((ifunc)slide_feof);
      case _RECT_GET:
          return ((ifunc)rect_get);
      default:
          if ((wrkfun = p_channel (h, code, aa)) != inull)
              return (wrkfun);
          if ((wrkfun = p_alloc (h, code, aa)) != inull)
              return (wrkfun);
          return (p_rect (h, code, aa));
    }
  return (itrue);
fail:
  return (inull);
}

                  int mk_slide (headptr h, rect * rptr)
/************************************************************************/
{ adr adh;
  int size = RECT_EMPTY (rptr)? 0: sizeof (attrchr) * rptr->w * rptr->h;
  if (rf_get_empty (h, HEADEMPTY)  AND
     (adh = rf_realloc (ANULL, sizeof(rfslide)+sizeof(slide)+size)) != NULL)
    { slide   * fsptr;
      rfslide * frptr = (rfslide*) SEGINF (adh);
      TYPE (h) = HEADSLIDE;
      HDSETA (h, adh);
      frptr->optr = h;
      frptr->sptr = fsptr = (slide *) ((char *) frptr + sizeof (rfslide));
      frptr->odx  = frptr->ody = 0;
      fsptr->aptr = (attrptr) ((char*) fsptr + sizeof (slide));
      fsptr->page = -1;
      return (slide_init (fsptr, rptr) != NULL);
    }
  return (0);
}

          int mk_subslide (headptr h, headptr hs, rect * rptr)
/************************************************************************/
{ adr adh;
  slide * hsptr;
  if (rf_get_empty (h, HEADEMPTY)                         AND
      ISOBJ (hs)  AND  h != hs  AND  START (hs) != inull  AND
      (hsptr = (slide *) HADDR (hs, _SLIDE)) != NULL      AND
      (adh   = rf_realloc (ANULL, sizeof(rfslide) + sizeof(slide))) != NULL)
    { headptr   h0;
      slide   * fsptr;
      rfslide * frptr = (rfslide*) SEGINF (adh);
      RECT_NEW (r, hsptr);
      rect_intrs (&r, rptr);
      TYPE (h) = HEADSLIDE;
      HDSETA (h, adh);
      if ((h0 = ((rfslide *) SEGINF (AD(hs)))->optr) == h) goto fail;
      ++ VAL (frptr->optr = h0);
      frptr->sptr = fsptr = (slide *) ((char *) frptr + sizeof (rfslide));
      frptr->odx  = r.x - hsptr->x;
      frptr->ody  = r.y - hsptr->y;
      if (subslide_init (fsptr, hsptr, &r) == NULL) goto fail;
      END (hs);
      return (1);
    }
fail:
  return (0);
}

             ifunc p_screen (headptr h, int code, addr * aa)
/************************************************************************/
{ adr adh;
  if ((adh = AD (h)) == ANULL) return (inull);
  *aa = ((rfslide *) SEGINF (adh))->sptr;
  switch (code)
    { case _START:
        { screen  * sptr;
          rfslide * rptr;
          if (p_alloc (h, code, aa) == inull) goto fail;
          rptr = (rfslide *) SEGINF (AD(h));
          sptr = rptr->sptr = (screen *) ((char *) rptr + sizeof (rfslide));
          if (rptr->optr != h  OR  rptr->odx | rptr->ody != 0) goto fail;
          sptr->sptr = sptr;
          slide_pos_set (sptr, sptr->posx, sptr->posy);
        }
          break;
      case _SCREEN:
          break;
      case _MAKE_SUBSLIDE:
          return ((ifunc)mk_snap);
      case _CURS_SET:
          return ((ifunc)screen_curs_set);
      default:
          return (p_slide (h, code, aa));
    }
  return (itrue);
fail:
  return (inull);
}

                   int mk_screen (headptr h, int page)
/************************************************************************/
{ adr adh;
  if (rf_get_empty (h, HEADEMPTY)  AND
     (adh = rf_realloc (ANULL, sizeof (rfslide) + sizeof (screen))) != NULL)
    { rfslide * rptr = (rfslide*) SEGINF (adh);
      TYPE (h) = HEADSCREEN;
      HDSETA (h, adh);
      rptr->optr = h;
      rptr->sptr = (slide *) ((char *) rptr + sizeof (rfslide));
      rptr->odx  = rptr->ody = 0;
      return (screen_init ((screen *) (rptr->sptr), page) != NULL);
    }
  return (0);
}

              ifunc p_snap (headptr h, int code, addr * aa)
/************************************************************************/
{ adr adh;
  if ((adh = AD (h)) == ANULL) return (inull);
  *aa = ((rfslide *) SEGINF (adh))->sptr;
  switch (code)
    { case _SNAP:
          break;
/*    case _MAKE_SUBSLIDE:    */
/*        return (mk_snap);   */
      case _CURS_SET:
          return ((ifunc) snap_curs_set);
      default:
          return (p_screen (h, code, aa));
    }
  return (itrue);
}

            int mk_snap (headptr h, headptr hs, rect * rptr)
/************************************************************************/
{ adr adh;
  screen * hsptr;
  if (rf_get_empty (h, HEADEMPTY)                         AND
      ISOBJ (hs)  AND  h != hs  AND  START (hs) != inull  AND
      (hsptr = (screen *) HADDR (hs, _SCREEN)) != NULL    AND
      (adh   = rf_realloc (ANULL, sizeof(rfslide) + sizeof(snap))) != NULL)
    { headptr   h0;
      slide   * fsptr;
      rfslide * frptr = (rfslide*) SEGINF (adh);
      RECT_NEW (r, hsptr);
      rect_intrs (&r, rptr);
      TYPE (h) = HEADSNAP;
      HDSETA (h, adh);
      if ((h0 = ((rfslide *) SEGINF (AD(hs)))->optr) == h) goto fail;
      ++ VAL (frptr->optr = h0);
      frptr->sptr = fsptr = (slide *) ((char *) frptr + sizeof (rfslide));
      frptr->odx  = r.x - hsptr->x;
      frptr->ody  = r.y - hsptr->y;
      if (snap_init ((snap *) fsptr, hsptr, &r) == NULL) goto fail;
      END (hs);
      return (1);
    }
fail:
  return (0);
}
                         /* End of rfSlide.c */
