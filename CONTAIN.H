/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1921 by Arkady Klimov            */
/*       rfcont.h    - container functions interface                    */
/************************************************************************/
#ifndef __CONTAIN_H
#define __CONTAIN_H

#ifndef __REFCOM_H
#include "refcom.h"
#endif

#ifndef NODEFTAIL
typedef void* tail;
#endif

enum contain_ops {  _CONTAIN = 100,
	_SETPOS , /* int isetpos (addr THIS, tail * at, int n)  n<=>0 */
	_GETN ,   /* LOGICAL igetn   (addr THIS, tail * at, cvalue * ac)
			 *ac is virtual!!!; if FALSE, *ac = ' '.          */
	_REPN ,   /* LOGICAL irepn   (addr THIS, tail * at, cvalue c)
						 c is actual!!!           */
	_GETLEN , /* int     igetlen (addr THIS, tail t)                  */
	_SETLEN , /* LOGICAL isetlen (addr THIS, tail * at, int l) l>=0   */
	_SHIFT    /* LOGICAL ishift  (addr THIS, tail * at, int n) n<=>0  */
    };
/*  SETPOS: n1 = n - (result position in range 0..lencont) */
/*  GETN: FALSE, if position is out of range */
/*  REPN, SETLEN, SHIFT: FALSE, if out of memory     */

typedef int (*Tsetpos) (addr THIS, tail * at, int n);
typedef int (*Tgetn)   (addr THIS, tail * at, cvalue * ac);
typedef int (*Trepn)   (addr THIS, tail * at, cvalue c);
typedef int (*Tgetlen) (addr THIS, tail t);
typedef int (*Tsetlen) (addr THIS, tail * at, seglen l);
typedef int (*Tshift)  (addr THIS, tail * at, int n);

ifunc p_contain(headptr h, int m, addr * aa);
ifunc p_allocont(headptr h, int m, addr * aa);

/* irepn for free storage list */
extern LOGICAL irepnfree(void*,tail*,cvalue);

/* #define ISCONT(h) (rf_defobj(h,_CONTAIN,HEADBOX)) */
#define CHARVIR ' '
#endif