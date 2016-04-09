                              /* Screen.c */
#ifndef __SCREEN_H
#include "screen.h"
#endif

#ifndef __STDLIB_H
#include <stdlib.h>
#define __STDLIB_H
#endif

#ifndef __CONIO_H
#include <conio.h>
#define __CONIO_H
#endif

#ifndef __DOS_H
#include <dos.h>
#define __DOS_H
#endif

screen    ScreenStd =
            {  0,  0,           /* RECT:      left, top;            */
              80, 25,           /*            width, height.        */
   (attrchr*) MK_FP (0xB800,0), /* BUFFER of attribute char;        */
              80,               /*             X-size into buffer.  */
              LIGHTGRAY,        /* FOR OUTPUT: current attribute;   */
               0,  0,           /*             x, y-position;       */
              80,               /*             maximum length;      */
   (attrchr*) MK_FP (0xB800,0), /*             address current char.*/
               0,  0,           /* CURSOR:     x, y-coordinate;     */
              SLIDECURSOLD,     /*             type.                */
              &ScreenStd,       /* Owner slide.                     */
               0                /* Screen page.                     */
            };

int       ScreenMode   = 3;
int       ScreenPage   = 0;
int       ScreenCursor = SLIDECURSOLD;
int       ScreenMaxPage[SCREENMAXMODE] = {7, 7, 3, 3,-1,-1,-1, 0};

             screen * screen_init (screen * sptr, int page)
/**********************************************************************/
{ union REGS regs;
  regs.h.ah  = 0x0F;
  int86 (0x10, &regs, &regs);
  ScreenPage = regs.h.bh;
  ScreenMode = regs.h.al;
  if (ScreenMaxPage[ScreenMode] < page  ||  page < 0) goto fail;
#if 0
  if (sptr == NULL)
      if ((sptr = (screen *) malloc (sizeof(screen))) == NULL) goto fail;
#endif /* 0 */
  sptr->x    = sptr->y = 0;
  sptr->w    = sptr->xsize = regs.h.ah;
  sptr->h    = 25;
  sptr->attr = LIGHTGRAY;
  sptr->sptr = (slide *) sptr;
  sptr->page = page;
  sptr->aptr = (attrchr*)
          (ScreenMode == MONO? MK_FP (0xB000, 0): MK_FP (0xB800, 0));
  sptr->aptr+= page * sptr->w * sptr->h * sizeof (attrchr);
  slide_pos_set ((slide *) sptr, sptr->x, sptr->y);
  screen_curs_get (sptr);
  return (sptr);
fail:
  return (NULL);
}

                   int screen_curs_get (screen * sptr)
/**********************************************************************/
{ union REGS regs;
  regs.h.ah = 3;
  regs.h.bh = sptr->page;
  int86 (0x10, &regs, &regs);
  sptr->curx   = regs.h.dl + sptr->x;
  sptr->cury   = regs.h.dh + sptr->y;
  ScreenCursor = regs.x.cx;
  sptr->curt   = sptr->page == ScreenPage? ScreenCursor: SLIDECURSOLD;
  return (1);
}

    int screen_curs_set (screen * sptr, int curx, int cury, int curt)
/**********************************************************************/
{ union REGS regs;
  slide_curs_set ((slide *) sptr, curx, cury, curt);
  if (RECT_ISIN (sptr, curx, cury))
    { regs.h.ah = 2;
      regs.h.bh = sptr->page;
      regs.h.dl = curx - sptr->x;
      regs.h.dh = cury - sptr->y;
      int86 (0x10, &regs, &regs);
    }
  curt = sptr->curt;
  if (curt != SLIDECURSOLD  &&  curt != ScreenCursor  &&
      sptr->page == ScreenPage)
    { regs.h.ah = 1;
      regs.x.cx = curt;
      int86 (0x10, &regs, &regs);
      regs.h.ah = 3;
      regs.h.bh = sptr->page;
      int86 (0x10, &regs, &regs);
      return (curt == (ScreenCursor = regs.x.cx));
    }
  return (1);
}

                     int screen_show (screen * sptr)
/**********************************************************************/
{ int page = sptr->page;
  if (ScreenMaxPage[ScreenMode] >= page  &&  page >= 0)
    { union REGS regs;
      regs.h.ah = 5;
      regs.h.al = ScreenPage = page;
      int86 (0x10, &regs, &regs);
      return (screen_curs_set (sptr, sptr->curx, sptr->cury, sptr->curt));
    }
  return (0);
}

                     int screen_mode_set (int mode)
/**********************************************************************/
{ union REGS regs;
  regs.h.ah = 0;
  regs.h.al = mode;
  int86 (0x10, &regs, &regs);
  return (screen_init (&ScreenStd, ScreenStd.page) != NULL  &&
          ScreenMode == mode);
}

        snap * snap_init (snap * sptr, slide * sptrs, rect * rptr)
/**********************************************************************/
{ int x, y;
#if 0
  if (sptr == NULL)
      if ((sptr = (snap *) malloc (sizeof(snap))) == NULL) return (NULL);
#endif /* 0 */
  *((slide *) sptr) = *sptrs;
  rect_intrs ((rect *) sptr, rptr);
  x = sptr->x;
  y = sptr->y;
  sptr->aptr = SLIDE_APTR (sptrs, x, y);
  slide_pos_set ((slide *) sptr, x, y);
  sptr->curt = SLIDECURSOLD;
  snap_curs_set (sptr, x, y, SLIDECURSOLD);
  return (sptr);
}

      int snap_curs_set (snap * sptr, int curx, int cury, int curt)
/**********************************************************************/
{ attrptr aptr;
  sptr->curx = curx;
  sptr->cury = cury;
  if (RECT_ISIN (sptr, curx, cury))
      aptr = SLIDE_APTR (sptr, curx, cury);
    else
    { aptr = sptr->aptr;
      if (curt != SLIDECURSOLD)
          curt =  SLIDECURSOFF;
    }
  if (curt != SLIDECURSOLD)
      sptr->curt = curt;
  curt = sptr->curt;
  sptr = (snap *) sptr->sptr;
  slide_axy ((slide *) sptr, aptr, &curx, &cury);
  return (screen_curs_set ((screen *) sptr, curx, cury, curt));
}
                          /* End of Screen.c */
