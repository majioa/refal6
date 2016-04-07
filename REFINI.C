/************************************************************************/
/*   R E F A L - 6      Copyright (C) IPS AN 1991                       */
/*   refini.c  - constant initialization for generated C-functions      */
/************************************************************************/
#include "refgen.h"
#include "rftable.h"

LOGICAL inibox(char * string, headptr* ah)
{ cvalue c;
  if(NOT rf_mk_sword(string, &c)) return(FALSE);
  if(NOT rf_mk_hboxn(c, ah)) return(FALSE);
  cvalfree(c);
  return(TRUE);
};

LOGICAL iniextbox(char * string, headptr * ah)
{ cvalue nc, vc;
	if(NOT rf_mk_sword(string, &nc)) return(FALSE);
	if(NOT rf_tab_val(systable, 'U', nc, &vc)) return(FALSE);
	cvalfree(nc);
	if(NOT IS_REF(CTYPE(vc))) return(FALSE);
	*ah = CREF00(vc);
  return(TRUE);
};

LOGICAL   INIWORDS(int NWORDS, cvalue* WORD, char** WORDS)
{ int i;
  ALL(i,NWORDS) if(NOT rf_mk_sword(WORDS[i],&(WORD[i]))) return(FALSE);
  return(TRUE);
  };

LOGICAL   INILONGS(int NLONGS, cvalue* LONG, long* LONGS)
{ int i;
  ALL(i,NLONGS)
	if( NOT rf_mk_cnumb(LONGS[i],&(LONG[i]),TRUE )) return(FALSE);
  return(TRUE);
  };

LOGICAL    INIBOXS(int NBOXS, cvalue* BOX, char** BOXS)
{ int i;
  headptr h;
  ALL(i,NBOXS)
	if( inibox(BOXS[i],&h) ) BOX[i] = CMKREF(h,TYPEREF);
	else return(FALSE);
  return(TRUE);
  };

LOGICAL     INIEXTBOXS(int NEXTBOXS, cvalue* EXTBOX, char** EXTBOXS)
{ int i;
  headptr h;
  ALL(i,NEXTBOXS)
	if( iniextbox(EXTBOXS[i],&h) ) EXTBOX[i] = CMKREF(h,TYPEREF);
	else return(FALSE);
  return(TRUE);
  };