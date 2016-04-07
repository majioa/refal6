                              /* Rect.h */
#ifndef __RECT_H
#define __RECT_H

#ifndef __STDLIB_H
#include <stdlib.h>
#define __STDLIB_H
#endif


#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define min(a,b)        (((a) < (b)) ? (a) : (b))

#define  ISIN(a, l, x)                     \
  ((a) + (l) > (x)  &&  (x) >= (a))

# define INTRS(a1, l1, a2, l2)             \
  l1 += a1;                                \
  a1  = max (a1, a2);                  \
  l1  = min (l1, (a2)+(l2)) - (a1)

struct rect
         { int   x, y;      /* {left, top}     -- xlow, ylow */
           int   w, h;      /* {width, height}               */
         };
typedef struct rect   rect;

#define RECT_NEW(name, rptr)               \
  rect name = *((rect *)(rptr))

#define RECT_COPY(rptr, rptr1)             \
  *((rect *)(rptr)) = *((rect *)(rptr1))

/***********************************************************************
struct loc { int   x, y; };
typedef struct loc   loc;

#define LOC_COPY(lptr, lptr1)              \
  *((loc *)(lptr)) = *((loc *)(lptr1))

#define RECT_NEW(name, rptr)               \
  rect name; RECT_COPY (rptr, &name)

#define RECT_COPY(rptr, rptr1)             \
  LOC_COPY (rptr, rptr1);                  \
  LOC_COPY (&((rptr)->w), &((rptr1)->w))
***********************************************************************/

#define RECT_ISIN(rptr, xx, yy)            \
  (ISIN ((rptr)->x, (rptr)->w, xx)  &&     \
   ISIN ((rptr)->y, (rptr)->h, yy))

#define RECT_EMPTY(rptr)                   \
  (0 >= (rptr)->w  ||  0 >= (rptr)->h)

#define RECT_INTRS(rptr, rptr1)            \
  INTRS ((rptr)->x, (rptr)->w, (rptr1)->x, (rptr1)->w);\
  INTRS ((rptr)->y, (rptr)->h, (rptr1)->y, (rptr1)->h)

/**********************************************************************/
int     rect_intrs (rect * rptr, rect * rptr1);
int     rect_get   (rect * rptr, rect * rptr1);
int     rect_set   (rect * rptr, rect * rptr1);
/**********************************************************************/

#endif  /* __RECT_H */
                          /* End of Rect.h */
