                            /* rfSlide.h */
#ifndef __RFSLIDE_H
#define __RFSLIDE_H

#ifndef __RFZCOM_H
#include "rfzcom.h"
#define __RFZCOM_H
#endif

#ifndef __CHANNEL_H
#include "channel.h"
#define __CHANNEL_H
#endif

#ifndef __RFRECT_H
#include "rfrect.h"
#endif

#ifndef __RECT_H
#include "rect.h"
#endif

#ifndef __SLIDE_H
#include "slide.h"
#endif

#ifndef __SCREEN_H
#include "screen.h"
#endif

enum slide_methods
          { _SLIDE = 110,
            _MAKE_SUBSLIDE,
            _POS_SET,
            _ATTR_SET,
            _CURS_SET
          };

typedef int (*Tmake_subslide) (headptr h, headptr hs, rect* rptr);
typedef int (*Tpos_set)  (addr THIS, int posx, int posy);
typedef int (*Tattr_set) (addr THIS, int attr);
typedef int (*Tcurs_set) (addr THIS, int curx, int cury, int curt);
typedef int (*Txy_set)   (addr THIS, int x, int y);

enum screen_methods
          { _SCREEN = 120 };

enum snap_methods
          { _SNAP   = 130 };

struct  rfslide
          {   slide * sptr;     /* slide                  */
            headptr   optr;     /* owner slide            */
                int   odx, ody; /* shift into owher slide */
          };
typedef struct rfslide  rfslide;

#define n_stdscr "STDSCR"
extern  headptr  stdscrptr;

#define n_slide  "SLIDE"
#define n_screen "SCREEN"
#define n_snap   "SNAP"

extern  int c_slide;
extern  int c_screen;
extern  int c_snap;

#define HEADSLIDE  HD (c_slide,  OBJ)
#define HEADSCREEN HD (c_screen, OBJ)
#define HEADSNAP   HD (c_snap,   OBJ)

/**********************************************************************/
int     rfslide_init     ();
ifunc   p_slide          (headptr h, int code, addr * aa);
int     mk_slide         (headptr h, rect * rptr);
int     mk_subslide      (headptr h, headptr hs, rect * rptr);
/**********************************************************************/
ifunc   p_screen         (headptr h, int code, addr * aa);
int     mk_screen        (headptr h, int page);
/**********************************************************************/
ifunc   p_snap           (headptr h, int code, addr * aa);
int     mk_snap          (headptr h, headptr hs, rect * rptr);
/**********************************************************************/

#endif  /* __RFSLIDE_H */
                         /* End of rfSlide.h */
