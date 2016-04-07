                              /* Slide.c */
#ifndef __SLIDE_H
#include "slide.h"
#endif

#ifndef __STDLIB_H
#include <stdlib.h>
#define __STDLIB_H
#endif

#ifndef __CONIO_H
#include <conio.h>
#define __CONIO_H
#endif

#ifndef __STDIO_H
#include <stdio.h>
#define __STDIO_H
#endif

#ifndef __MEM_H
#include <mem.h>
#define __MEM_H
#endif

           void memswap (char * dest1, char * dest2, size_t n)
/**********************************************************************/
{ char   buff[80];
  size_t lbuff;
  do
    { lbuff = min (n, sizeof (buff));
      memcpy ((void*) buff,  (void*) dest1, lbuff);
      memcpy ((void*) dest1, (void*) dest2, lbuff);
      memcpy ((void*) dest2, (void*) buff,  lbuff);
      dest1 += lbuff;
      dest2 += lbuff;
    }
  while ((n -= lbuff) != 0);
}

             slide * slide_init (slide * sptr, rect * rptr)
/**********************************************************************/
{ int x, y;
#if 0
  x = RECT_EMPTY (rptr) ? 0 : rptr->w * rptr->h * sizeof(attrchr);
  if (sptr == NULL)
    { if ((sptr = (slide *) malloc (sizeof(slide)+x)) == NULL) goto fail;
      sptr->aptr = (attrptr) ((char *) sptr + sizeof(slide));
      sptr->page = -2;
    }
    else
    { if (sptr->aptr == NULL  &&  x)
        { if ((sptr->aptr = (attrptr) malloc (x)) == NULL) goto fail;
          sptr->page = -3;
        }
    }
#endif /* 0 */
  RECT_COPY (sptr, rptr);
  sptr->sptr  = sptr;
  sptr->xsize = sptr->w;
  slide_pos_set  (sptr, x = sptr->x, y = sptr->y);
  slide_attr_set (sptr, LIGHTGRAY);
  sptr->curt = SLIDECURSOLD;
  slide_curs_set (sptr, x, y, SLIDECURSOLD);
  return (sptr);
fail:
  return (NULL);
}

    slide * subslide_init (slide * sptr, slide * sptrs, rect * rptr)
/**********************************************************************/
{ int x, y;
#if 0
  if (sptr == NULL)
      if ((sptr = (slide *) malloc (sizeof(slide))) == NULL) goto fail;
#endif /* 0 */
  *sptr = *sptrs;
  rect_intrs ((rect *) sptr, rptr);
  x = sptr->x;
  y = sptr->y;
  sptr->aptr = SLIDE_APTR (sptrs, x, y);
  slide_pos_set ((slide *) sptr, x, y);
  sptr->curt = SLIDECURSOLD;
  slide_curs_set (sptr, x, y, SLIDECURSOLD);
  return (sptr);
#if 0
fail:
  return (NULL);
#endif /* 0 */
}

#if 0
                      int slide_done (slide * sptr)
/**********************************************************************/
{ if (sptr != NULL  &&  sptr->page < -1)
    { if (sptr->page == -3)
          sptr = (slide *) sptr->aptr;
      if (sptr != NULL  &&  sptr->page >= -3)
          free (sptr);
      return (1);
    }
  return (0);
}
#endif /* 0 */

      int slide_fill (slide * sptr, int chr, int attr, rect * rptr)
/**********************************************************************/
{ RECT_NEW (r, sptr);
  if (rect_intrs (&r, rptr))
    { int     h = 0;
      attrchr achr;
      achr.chr  = chr;
      achr.attr = attr;
      do
        { attrptr aptr = SLIDE_APTR (sptr, r.x, r.y + h);
          int w = r.w;
          do  *aptr++ = achr;
          while (--w > 0);
        }
      while (++h < r.h);
/*    SLIDE_REDRAW (sptr, &r);   */
    }
  return (1);
}

     int slide_corr (slide * sptr, int bool, int attr, rect * rptr)
/**********************************************************************/
{ RECT_NEW (r, sptr);
  if (rect_intrs (&r, rptr))
    { attrptr aptr = SLIDE_APTR (sptr, r.x, r.y);
      attr_func func;
      int  inot, ingt;
      inot = (bool & NOT_ATTR) != 0;
      ingt = (bool & NGT_ATTR) != 0;
      if ((bool &= 15) > 7)
        { bool = ~bool & 7;
          inot = !inot;
        }
      switch (bool)
        { case 0:
            attr = 0;
          case 3:
            if (inot)
                attr = ~attr;
            if (ingt)
                attr = ((attr & 112) >> 4) | ((attr & 7) << 4) | (attr & 136);
            inot = ingt = 0;
            func = attr_fill;
            break;
          case 4:
            attr = ~attr;
          case 1:
            func = attr_and;
            break;
          case 2:
            inot = !inot;
            attr = ~attr;
          case 7:
            func = attr_or;
            break;
          case 6:
            func = attr_xor;
            break;
          case 5:
            func = NULL;
        }
      do
        { if (func != NULL)
              (*func) (aptr, r.w, attr);
          if (inot)
              attr_xor (aptr, r.w, ~0);
          if (ingt)
              attr_ngt (aptr, r.w);
/*        slide_redraw (sptr, aptr, r.w, 1);    */
          aptr += sptr->xsize;
        }
      while (--(r.h) > 0);
    }
  return (1);
}

        int slide_copy (slide * sptr, slide * sptr1, rect * rptr)
/**********************************************************************/
{ RECT_NEW   (r,  sptr);
  RECT_INTRS (&r, sptr1);
  if (rect_intrs (&r, rptr))
    { int w = sizeof (attrchr) * r.w;
      int h = 0;
      do  memcpy ((void*) SLIDE_APTR (sptr,  r.x, r.y + h),
                  (void*) SLIDE_APTR (sptr1, r.x, r.y + h), w);
#if 0
        { attrptr aptr  = SLIDE_APTR (sptr,  r.x, r.y + h);
          attrptr aptr1 = SLIDE_APTR (sptr1, r.x, r.y + h);
          int w = r.w;
          do  *aptr++ = *aptr1++;
          while (--w > 0);
        }
#endif /* 0 */
      while (++h < r.h);
/*    SLIDE_REDRAW (sptr, &r);   */
    }
  return (1);
}

        int slide_swap (slide * sptr, slide * sptr1, rect * rptr)
/**********************************************************************/
{ RECT_NEW   (r,  sptr);
  RECT_INTRS (&r, sptr1);
  if (rect_intrs (&r, rptr))
    { int w = sizeof (attrchr) * r.w;
      int h = 0;
      do  memswap ((char*) SLIDE_APTR (sptr,  r.x, r.y + h),
                   (char*) SLIDE_APTR (sptr1, r.x, r.y + h), w);
#if 0
        { attrptr aptr  =  SLIDE_APTR (sptr,  r.x, r.y + h);
          attrptr aptr1 =  SLIDE_APTR (sptr1, r.x, r.y + h);
          attrchr achr;
          int w = r.w;
          do
            { achr     = *aptr;
              *aptr++  = *aptr1;
              *aptr1++ = achr;
            }
          while (--w > 0);
        }
#endif /* 0 */
      while (++h < r.h);
/*    SLIDE_REDRAW (sptr,  &r);   */
/*    SLIDE_REDRAW (sptr1, &r);   */
    }
  return (1);
}

              int slide_xy_set (slide * sptr, int x, int y)
/************************************************************************/
{ x -= sptr->x;
  y -= sptr->y;
  sptr->x    += x;
  sptr->y    += y;
  sptr->posx += x;
  sptr->posy += y;
  sptr->curx += x;
  sptr->cury += y;
  return (1);
}

           int slide_pos_set (slide * sptr, int posx, int posy)
/************************************************************************/
{ sptr->posx = posx;
  sptr->posy = posy;
  sptr->cptr = NULL;
  sptr->len  = 0;
  if ((posy -= sptr->y) >= 0  &&  posy < sptr->h)
    { posx  -= sptr->x;
      sptr->cptr = posx + sptr->xsize * posy + sptr->aptr;
      sptr->len  = sptr->w - posx;
    }
  return (1);
}

               int slide_attr_set (slide * sptr, int attr)
/************************************************************************/
{ sptr->attr = attr;
  return (1);
}

     int slide_curs_set (slide * sptr, int curx, int cury, int curt)
/************************************************************************/
{ sptr->curx = curx;
  sptr->cury = cury;
  if (curt != SLIDECURSOLD)
      sptr->curt = RECT_ISIN (sptr, curx, cury)? curt: SLIDECURSOFF;
  if (sptr->sptr != sptr)
    { attrptr aptr = RECT_ISIN (sptr, curx, cury)?
                          SLIDE_APTR (sptr, curx, cury): sptr->aptr;
      curt = sptr->curt;
      sptr = sptr->sptr;
      slide_axy (sptr, aptr, &curx, &cury);
      return (slide_curs_set (sptr, curx, cury, curt));
    }
  return (1);
}

                      int slide_getc (slide * sptr)
/************************************************************************/
{ sptr->posx++;
  if (sptr->cptr != NULL)
    { attrchr achr = *(sptr->cptr)++;
      if (--(sptr->len) >= 0  &&  sptr->posx > sptr->x)
        { sptr->attr = achr.attr;
          return (achr.chr);
        }
    }
  return (EOF);
}

              int slide_ungetc (int chr, slide * sptr)
/************************************************************************/
{  sptr->posx--;
  if (sptr->cptr != NULL)
    { sptr->cptr--;
      if (++(sptr->len) > 0  &&  sptr->posx >= sptr->x)
        { attrchr achr;
          achr.chr  = chr;
          achr.attr = sptr->attr;
          *(sptr->cptr) = achr;
/*        slide_redraw (sptr, sptr->cptr, 1, 1)      */
        }
    }
  return (chr);
}

                      int slide_feof (slide * sptr)
/************************************************************************/
{ return (sptr->len <= 0); }

                 int slide_putc (int chr, slide * sptr)
/************************************************************************/
{  sptr->posx++;
  if (sptr->cptr != NULL)
    { if (--(sptr->len) >= 0  &&  sptr->posx > sptr->x)
        { attrchr achr;
          achr.chr  = chr;
          achr.attr = sptr->attr;
          *(sptr->cptr) = achr;
/*        slide_redraw (sptr, sptr->cptr, 1, 1)      */
          sptr->cptr++;
        }
    }
  return (chr);
}

#if 0
           int slide_puts (slide * sptr, char * cptr, int len)
/**********************************************************************/
{ attrchr achr;
  attrptr aptr = sptr->cptr;
  int x = sptr->x - sptr->posx;
  sptr->posx += len;
  x   = max (x, 0);
  len = max (len, 0);
  cptr += x;
  len  -= x;
  achr.attr = sptr->attr;
  if ((x = len = min (len, sptr->len)) > 0)
    {
/*    attrptr aptr = sptr->cptr;    */
      for (; x-- > 0;)
        { achr.chr = *cptr++;
          *(sptr->cptr)++ = achr;
        }
/*    slide_redraw (sptr, aptr, len, 1);    */
      sptr->len -= len;
      return (len);
    }
  return (0);
}

          int slide_put (slide * sptr, attrptr aptr, int len)
/**********************************************************************/
{ int x = sptr->x - sptr->posx;
  x = max (x, 0);
  sptr->posx += max (len, 0);
  aptr += x;
  len  -= x;
  if ((len = min (len, sptr->len)) > 0)
    { memcpy ((void*) sptr->cptr, (void*) aptr, sizeof (attrchr) * len);
/*    slide_redraw (sptr, sptr->cptr, len, 1);    */
      sptr->len  -= len;
      sptr->cptr += len;
      return (len);
    }
  return (0);
}

       int slide_redraw (slide * sptr, attrptr aptr, int w, int h)
/************************************************************************/
{ return (1); }
#endif /* 0 */

       int slide_axy (slide * sptr, attrptr aptr, int * x, int * y)
/************************************************************************/
{ div_t xy = div ((int) (aptr - sptr->aptr), sptr->xsize);
  *x = sptr->x + xy.rem;
  *y = sptr->y + xy.quot;
  return (1);
}
                          /* End of Slide.c */
