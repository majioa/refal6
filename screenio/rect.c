                              /* Rect.c */
#ifndef __RECT_H
#include "rect.h"
#endif

               int rect_intrs (rect * rptr, rect * rptr1)
/**********************************************************************/
{ if (rptr1 != NULL)
    { RECT_INTRS (rptr, rptr1); }
  return (! RECT_EMPTY (rptr));
}

                int rect_get (rect * rptr, rect * rptr1)
/**********************************************************************/
{ RECT_COPY (rptr1, rptr);
  return (1);
}

                int rect_set (rect * rptr, rect * rptr1)
/**********************************************************************/
{ RECT_COPY (rptr, rptr1);
  return (1);
}
                          /* End of Rect.c */
