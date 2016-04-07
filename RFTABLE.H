/******************************************************************
   R E F A L - 6      Copyright (C) IPS AN 1991
   rftable.h - headder table functions
*******************************************************************/

#ifndef __REFGEN_H
#include "refgen.h"
#endif

#define TABLEBEGIN(h) h

LOGICAL rf_tab_val (headptr table, int mode, cvalue name, cvalue * avalue);

LOGICAL rf_tab_nam (headptr table, int mode, cvalue value, cvalue * aname);

LOGICAL rf_tab_link (headptr table, cvalue name, cvalue value);

/* The resulting name and value are virtual */
LOGICAL rf_tab_next(headptr table, long pos, long * pos1,
					cvalue * aname, cvalue * avalue);

/*     test if headder h is a table    */
LOGICAL rf_istable(headptr h);

/*         Empty box to table          */
LOGICAL tbox_init(headptr box);

