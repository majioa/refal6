                             /* rfRect.h */
#ifndef __RFRECT_H
#define __RFRECT_H

#ifndef __RFZCOM_H
#include "rfzcom.h"
#define __RFZCOM_H
#endif

#ifndef __RECT_H
#include "rect.h"
#endif

enum rect_methods
          { _RECT = 100,
            _RECT_GET,
            _RECT_SET,
            _XY_SET
          };

typedef int (*Trect_get) (addr THIS, rect * rptr);
typedef int (*Trect_set) (addr THIS, rect * rptr);

#define RFRECT_PUT(rptr) \
  (rf_mknumb ((long) (rptr)->x)  AND  rf_mknumb ((long) (rptr)->y)  AND \
   rf_mknumb ((long) (rptr)->w)  AND  rf_mknumb ((long) (rptr)->h))

/**********************************************************************/
int     rfrect_get  (elemptr p, rect * rptr);
ifunc   p_rect      (headptr h, int code, addr * aa);
/**********************************************************************/

#endif  /* __RFRECT_H */
                          /* End of rfRect.h */
