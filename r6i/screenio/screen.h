                             /* Screen.h */
#ifndef __SCREEN_H
#define __SCREEN_H

#ifndef __SLIDE_H
#include "slide.h"
#endif

typedef struct slide  screen;

typedef struct slide  snap;

extern  screen ScreenStd;
extern  int    ScreenMode;
extern  int    ScreenPage;
extern  int    ScreenCursor;
extern  int    ScreenMaxPage[];

#define SCREENMAXMODE  8

/**********************************************************************/
screen * screen_init     (screen * sptr, int page);
int      screen_curs_get (screen * sptr);
int      screen_curs_set (screen * sptr, int curx, int cury, int curt);
int      screen_show     (screen * sptr);
int      screen_mode_set (int mode);
/**********************************************************************/
snap   * snap_init       (snap * sptr, slide * sptrs, rect * rptr);
int      snap_curs_set   (snap * sptr, int curx, int cury, int curt);
/**********************************************************************/

#define screen_back(sptr, bptr)   slide_copy (sptr, &ScreenStd, bptr)
#define screen_rest(sptr, bptr)   slide_copy (&ScreenStd, sptr, bptr)
#define screen_swap(sptr, bptr)   slide_swap (&ScreenStd, sptr, bptr)

#endif  /* __SCREEN_H */
                          /* End of Screen.h */
