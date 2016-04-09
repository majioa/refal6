/************************************************************************
/*   R E F A L - 6      Copyright (C) 1992 Arkady Klimov                */
/*       rfmain.c    - main module of Refal-6 system                    */
/************************************************************************/

#define DEFINE_EXTCOM 1

#include <stdbool.h>
#include "refgen.h"
extern LOGICAL rg_GO();

bool main (int argc, char** argv)
{
   if(NOT initstor(argc,argv)) rf_err(ERCSTOR);
   if(NOT rf_inist()) rf_err(ERCINIT);
   pushe(NOELEM,NOELEM);
   pushae(EresL,EresR);

   rg_GO();

   rf_exit(rf_error);  /* rf_termst is herein */
   return(TRUE);
}
