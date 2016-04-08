                              /* Slide.h */
#ifndef __SLIDE_H
#define __SLIDE_H

#ifndef __RECT_H
#include "rect.h"
#endif

#ifndef __ATTRCHR_H
#include "attrchr.h"
#endif

typedef struct slide * sldptr;
struct  slide
          {     int   x, y;       /* RECT:      left, top;            */
                int   w, h;       /*            width, height.        */
            attrptr   aptr;       /* BUFFER of attribute char;        */
                int   xsize;      /*             X-size into buffer.  */
                int   attr;       /* FOR OUTPUT: current attribute;   */
                int   posx, posy; /*             x, y-position;       */
                int   len;        /*             maximum length;      */
            attrptr   cptr;       /*             address current char.*/
                int   curx, cury; /* CURSOR:     x, y-coordinate;     */
                int   curt;       /*             type.                */
             sldptr   sptr;       /* Owner slide.                     */
                int   page;       /* Screen page (< 0 - into memory). */
          };
typedef struct slide  slide;

#define SLIDE_ISIN(sptr)         \
  (sptr->len > 0  &&  sptr->posx >= sptr->x)

#define SLIDE_APTR(sptr, xx, yy) \
  ((sptr)->xsize * ((yy) - (sptr)->y) + ((xx) - (sptr)->x) + (sptr)->aptr)

#define SLIDE_REDRAW(sptr, rptr) \
  slide_redraw (sptr,SLIDE_APTR(sptr,(rptr)->x,(rptr)->y),(rptr)->w,(rptr)->h)

#define SLIDECURSOLD  -1
#define SLIDECURSOFF  32*256
#define SLIDECURSINS   6*256 + 7
#define SLIDECURSOVR   5*256 + 8

/**********************************************************************/
slide * slide_init     (slide * sptr, rect * rptr);
slide * subslide_init  (slide * sptr, slide * sptrs, rect * rptr);
int     slide_done     (slide * sptr);
int     slide_fill     (slide * sptr, int chr, int attr, rect * rptr);
int     slide_corr     (slide * sptr, int bool, int attr, rect * rptr);
int     slide_copy     (slide * sptr, slide * sptr1, rect * rptr);
int     slide_swap     (slide * sptr, slide * sptr1, rect * rptr);
int     slide_xy_set   (slide * sptr, int x, int y);
int     slide_pos_set  (slide * sptr, int posx, int posy);
int     slide_attr_set (slide * sptr, int attr);
int     slide_curs_set (slide * sptr, int curx, int cury, int curt);
int     slide_getc     (slide * sptr);
int     slide_ungetc   (int chr, slide * sptr);
int     slide_feof     (slide * sptr);
int     slide_putc     (int chr, slide * sptr);
int     slide_puts     (slide * sptr, char *  cptr, int len);
int     slide_put      (slide * sptr, attrptr aptr, int len);
int     slide_redraw   (slide * sptr, attrptr aptr, int w, int h);
int     slide_axy      (slide * sptr, attrptr aptr, int * x, int * y);
/**********************************************************************/

#define slide_copy0(sptr, sptr1)  slide_copy (sptr, sptr1, NULL)
#define slide_swap0(sptr, sptr1)  slide_swap (sptr, sptr1, NULL)

#endif  /* __SLIDE_H */
                          /* End of Slide.h */
