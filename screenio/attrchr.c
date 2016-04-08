                            /* AttrChr.c */

#ifndef __ATTRCHR_H
#include "attrchr.h"
#endif

            void attr_fill (attrptr aptr, int len, int attr)
/**********************************************************************/
{ do
      (aptr++)->attr = attr;
  while (--len > 0);
}

             void attr_or (attrptr aptr, int len, int attr)
/**********************************************************************/
{ do
    { aptr->attr |= attr;
      aptr++;
    }
  while (--len > 0);
}

             void attr_and (attrptr aptr, int len, int attr)
/**********************************************************************/
{ do
    { aptr->attr &= attr;
      aptr++;
    }
  while (--len > 0);
}

             void attr_xor (attrptr aptr, int len, int attr)
/**********************************************************************/
{ do
    { aptr->attr ^= attr;
      aptr++;
    }
  while (--len > 0);
}

                  void attr_ngt (attrptr aptr, int len)
/**********************************************************************/
{ int attr;
  do
    { attr = aptr->attr;
      (aptr++)->attr = ((attr & 112) >> 4) | ((attr & 7) << 4) | (attr & 136);
    }
  while (--len > 0);
}
                         /* End of AttrChr.c */
