/******************************************************************
   R E F A L - 6      Copyright (C) IPS AN 1991
   rftable.c - headder table functions
*******************************************************************/
#include "rftable.h"

/*   Get the next link from the table    */
/*   The result name and value are ghost */
static LOGICAL tbox_next(headptr box, elemptr pos, elemptr * pos1,
               cvalue * aname, cvalue * avalue)
{  elemptr n,v;
   if(pos==NULL) pos = TABLEBEGIN(box);
   n = NEXT(pos);
   if (n == box) return(FALSE);
   v = NEXT(n);
   if( v == box ) return(FALSE);
   * pos1 = v;
   *aname = DATA(n);
   *avalue = DATA(v);
   return(TRUE);
   };

/*    Get the value from the table given a name   */
static LOGICAL tbox_value(headptr box, int mode,
              cvalue name, cvalue * avalue)
{  elemptr p,p0,p1,h;
   LOGICAL found;
   if(NOT ISTABLE(box)) return(FALSE);
 /* Search name */
   p0 = TABLEBEGIN(box);
   for (p = NEXT(p0) ; p!=box ; p = NEXT(p1) ) {
      p1 = NEXT(p);
      if(p1==box) return(FALSE);
      if(EQTERMC(p,name)) break;
      };
   found = (p!=box);
 /* Delete */
   if(found AND (mode=='N' OR mode=='D')) {
       AFTERX(b,p,p1);
       found = FALSE;
       };
 /* Insert */
   if ((NOT found) AND (mode=='N' OR mode=='U')) {
      /* Make empty box head */
    if(NOT rf_mk_hboxn(name,&h)) return(FALSE);
      /* Make name symbol */
    oldb=b;
    COPSYMB(name);
      /* Make box symbol */
    movb; SETREF(b,h,TYPEREF);
    p1 = b;








      /* Insert into the table */
    AFTERX(PREV(box),NEXT(oldb),b);
    b=oldb;
    found = TRUE;
    }
 /* Make actual result */
   *avalue = ( found ? ADATA(p1) : NOCVALUE );
   return ( TRUE );
   };

/* Get the name from the table given a value */
static LOGICAL tbox_name(headptr box, int mode,
                         cvalue value, cvalue * aname)
{  elemptr p,p0,p1;
   LOGICAL found;
   char sname[20];
   if(NOT ISTABLE(box)) return(FALSE);
 /* Search name */
   p0 = TABLEBEGIN(box);
   for (p = NEXT(p0) ; p!=box ; p = NEXT(p1) ) {
      p1 = NEXT(p);
      if(p1==box) return(FALSE);
      if(EQTERMC(p1,value)) break;
      };
   found = (p!=box);
 /* Delete */
   if(found)
     if(mode=='N' OR mode=='D') {
       AFTERX(b,p,p1);
       found = FALSE;
       };
 /* Insert */
   if (NOT found)
     if(mode=='N' OR mode=='U') {
    oldb=b;
      /* Make name */
    sprintf(sname,"$X_%lx",*(long*)&value);
      /* Make name symbol */
    if(NOT newword(sname)) return(FALSE);
    p = b;
      /* Make value symbol */
    COPSYMB(value);
      /* Insert into the table */
    AFTERX(PREV(box),p,b);
    b=oldb;
    found = TRUE;
    }
 /* Make actual result */
   *aname = ( found ? ADATA(p) : NOCVALUE );
   return ( TRUE );
   };









/*  Add the link <name,value> to the table   */
static LOGICAL tbox_link(headptr table, cvalue name, cvalue value)
{
  oldb=b;
  COPSYMB(name);
  COPSYMB(value);
  AFTERX(table,NEXT(oldb),b);
  b=oldb;
  return(TRUE);
  };

LOGICAL rf_tab_val (headptr table, int mode, cvalue name, cvalue * avalue)
{
   if(ISTABLE(table))
      return tbox_value(table,mode,name,avalue);
   return(FALSE);
   };

LOGICAL rf_tab_nam (headptr table, int mode, cvalue value, cvalue * aname)
{
  if(ISTABLE(table))
     return tbox_name(table,mode,value,aname);
  return(FALSE);
  };

LOGICAL rf_tab_link (headptr table, cvalue name, cvalue value)
{
  if(ISTABLE(table))
     return tbox_link(table,name,value);
  return(FALSE);
  };

LOGICAL rf_tab_next(headptr table, long pos, long * pos1,
                    cvalue * aname, cvalue * avalue)
{
  if(ISTABLE(table))
     return tbox_next(table,(elemptr)pos,(elemptr*)pos1,aname,avalue);
  return(FALSE);
  };

/*         Empty to table                          */
LOGICAL tbox_init(headptr box)
{
  if(TYPE(box) == HEADEMPTY) rf_get_empty(box,HEADTABLE);
  return(ISTABLE(box));
  };

