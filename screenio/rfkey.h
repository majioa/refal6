                              /* rfKey.h */
#ifndef __RFKEY_H
#define __RFKEY_H

#ifndef __REFGEN_H
#include "refgen.h"
#endif

#ifndef __CHANNEL_H
#include "channel.h"
#endif

enum key_methods
          { _KEY = 80 };

#define n_stdkey  "STDKEY"
extern  headptr   stdkeyptr;

#define n_key     "KEY"

extern  int       c_key;

#define HEADKEY   HD (c_key, OBJ)

extern  int       KeyUngetch;

/**********************************************************************/
int     rfkey_init   ();
int     rfkey_getc   ();
int     rfkey_ungetc (int c);
ifunc   p_key        (headptr h, int code, addr * aa);
int     mk_key       (headptr h);
/**********************************************************************/

#endif  /* __RFKEY_H */
                           /* End of rfKey.h */
