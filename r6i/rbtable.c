/******************************************************************
   R E F A L - 6      Copyright (C) IPS AN 1991

   rbtable.c - table builtin functions
*******************************************************************/

/*E    <MAKE TABLE [s.Loc]> -> s.Loc       ,  rb_mk_table         */
/*E    <TABLE_VALUE s.Tab sM sN> -> sV |   ,  rb_tbl_value        */
/*E    <TABLE_NAME  s.Tab sM sV> -> sN |   ,  rb_tbl_name         */
/*E    <TABLE_LIST s.Tab> -> e.List        ,  rb_tbl_list         */
/*E    <TABLE_LINK s.Tab e.List> ->        ,  rb_tbl_link         */

#include "rftable.h"

LOGICAL rb_mk_table()
{
   elemptr p;
   headptr h;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL == NULL) {
     if(NOT rf_mk_empty(&h,HEADTABLE)) return(FALSE);
     movbx(b,p);
     SETREF(p,h,TYPEREF);
     WELD(b,NEXT(p));
     }
   else if (EargL == EargR AND ISREF(EargL) ) {
     p = EargL;
     h = REF00(p);
     if(NOT rf_get_empty(h,HEADTABLE)) return(FALSE);
     }
   else return(FALSE);
   /* p is an actual box symbol , h is its headder, box is empty  */
   *AeresL = *AeresR = p;
   return(TRUE);
   };




















/*   <TABLE_VALUE s.Tbl s.Mode s.Name> == s.Value | Empty  */
LOGICAL rb_tbl_value()
{
   elemptr t,m,n,v;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   headptr ht;
   cvalue nc,vc;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if((t=EargL)==NULL OR
       NOT ISREF(t) OR NOT ISTABLE(ht=REF00(t))) return(FAIL);
   if(t==EargR OR NOT ISCHAR(m=NEXT(t))) return(FAIL);
   if(m==EargR OR (n=NEXT(m))!=EargR) return(FAIL);
   nc=DATA(n);
   if(NOT rf_tab_val(ht,toupper(VAL(m)),nc,&vc)) return(FALSE);
   if(vc!=NOCVALUE) {
     movbx(b,v);
     SETDATA(v,vc);
     }
   else v=b;
   BETWEEN(b,NEXT(v),EargL,EargR);
   *AeresL = *AeresR = ( v==b ? NOELEM : v );
   return(TRUE);
   };

/*   <TABLE_NAME s.Tbl s.Mode s.Value> == s.Name | Empty  */
LOGICAL rb_tbl_name()
{
   elemptr t,m,v,n;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   cvalue nc,vc;
   headptr ht;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if((t=EargL)==NOELEM OR
       NOT ISREF(t) OR NOT ISTABLE(ht=REF00(t))) return(FAIL);
   if(t==EargR OR NOT ISCHAR(m=NEXT(t))) return(FAIL);
   if(m==EargR OR (v=NEXT(m))!=EargR) return(FAIL);
   vc = DATA(v);
   if(NOT rf_tab_nam(ht,toupper(VAL(m)),vc,&nc)) return(FALSE);
   if(nc!=NOCVALUE) {
     movbx(b,n);
     SETDATA(n,nc);
     }
   else n=b;
   BETWEEN(b,NEXT(n),EargL,EargR);
   *AeresL = *AeresR = ( n==b ? NOELEM : n );
   return(TRUE);
   };









LOGICAL rb_tbl_list()
{
   elemptr savb,t;
   long pos;
   cvalue nc,vc;
   headptr ht;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if((t=EargL)==NOELEM OR t!=EargR OR
       NOT ISREF(t) OR NOT ISTABLE(ht=REF00(t))) return(FAIL);
   savb = b;
   for(pos=(long)NOELEM; rf_tab_next(ht,pos,&pos, &nc, &vc);) {
      COPSYMB(nc);
      COPSYMB(vc);
      }
   if(b==savb) *AeresL=*AeresR=NOELEM;
   else {
     *AeresL = NEXT(savb);
     *AeresR = b;
     }
   BETWEEN(savb,NEXT(b),EargL,EargR);
   b = savb;
   return(TRUE);
   };

LOGICAL
rb_tbl_link()
{
   elemptr t,n,v,p;
   headptr ht;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if((t=EargL)==NOELEM OR
       NOT ISREF(t) OR NOT ISTABLE(ht=REF00(t))) return(FAIL);
   for(p=t; p!=EargR; p = v) {
      n = NEXT(p);
      if(n==EargR) return(FAIL);
      v = NEXT(n);
      if(NOT rf_tab_link(ht,DATA(n),DATA(v))) return(FALSE);
      };
   *AeresL = *AeresR = NOELEM;
   AFTER(b,EargL,EargR);
   return(TRUE);
   };