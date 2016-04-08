/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arkady Klimov            */
/*        rbmouse.c    - mouse builtin function                         */
/************************************************************************/

/*E        <MOUSE e.Msg> -> e.Ans          ,  rb_mouse                  */

/*     <MOUSE Init>  ==                                                 */
/*     <MOUSE Done>  ==                                                 */
/*     <MOUSE Get>   == e.Event                                         */
/*     <MOUSE Check> == e.Event                                         */
/*     <MOUSE Drag>            == s.OnOff                               */
/*     <MOUSE Drag s.OnOff>     ==                                      */
/*     <MOUSE KbdMouse>         == s.OnOff                              */
/*     <MOUSE KbdMouse s.OnOff> ==                                      */
/*     <MOUSE Pointer>          == s.OnOff                              */
/*     <MOUSE Pointer s.OnOff>  ==                                      */
/*     <MOUSE MovePointer s.X s.Y> ==                                   */

#include "refgen.h"
#include "eventdrv.h"

#define NEVW 22

typedef enum {
 _MOUSE, _Init, _Done,
 _Get, _Check, _None,
 _Drag, _KbdMouse, _Pointer,
 _MovePointer, _On, _Off,
 _Kbd, _ASCII, _Ctrl, _Func,
 _Mouse, _Left, _Right,
 _Press, _Release, _Double,
  } evword;

static cvalue EVW[NEVW];

static char* EVWS[NEVW] = {
 "MOUSE", "Init", "Done",
 "Get", "Check", "None",
 "Drag", "KbdMouse", "Pointer",
 "MovePointer", "On", "Off",
 "Kbd", "ASCII", "Ctrl", "Func",
 "Mouse", "Left", "Right",
 "Press", "Release", "Double",
 };

void inimouse()
{
  if(NOT INIWORDS(NEVW,EVW,EVWS)) rf_err(ERCINIT);
  }

#define SETNUM(V) { if(NOT IS_SNUMB(CTYPE(vc))) return(FAIL); V CVAL(vc);}

#define CAR(E,c) { if(empty(E))return(FAIL);\
                   c = DATA((E).L); butfirst(E); }




typedef enum { OFF=0 ,ON=1, EMP=2, ERR } ONOFF;

static ONOFF ev_get_onoff(expr* ae)
{  cvalue vc;
   CAR(*ae,vc) /* if empty return(EMPTY); */
   if (vc==EVW[_On ]) return(ON) ;
   else
   if (vc==EVW[_Off]) return(OFF);
   else return(ERR);
}

static LOGICAL rf_mk_onoff(ONOFF onoff)
{  COPSYMB(EVW[(onoff?_On:_Off)]);
   return(TRUE);
}

#define ev_Mouse  0x8000
#define ev_MouButton(ev)   ((ev).sc_code & 0x0003 )
#define ev_MouAction(ev)   ((ev).ev_code & 0x0003 )
#define ev_KbdMode(ev)     (((ev).ev_code & 0x0300) >>8)

/* Generating event as Refal expression */
static LOGICAL rf_get_event(LOGICAL wait)
 { EVENT event;
   int button,action;
   if(wait) ED_GetEvent  (&event);
   else     ED_CheckEvent(&event);
/*========  Generating t.Event =========  */
   bl;
   if(event.ev_code==0) { COPSYMB(EVW[_None]); }
   else
   if(event.ev_code & ev_Mouse) {
        COPSYMB(EVW[_Mouse]);
        button = ev_MouButton(event);
        action = ev_MouAction(event);
        if(button==0) { COPSYMB(EVW[_Drag]) }
        else {
           COPSYMB(EVW[(button==1? _Left : _Right)]);
           if(action)
           COPSYMB(EVW[_Press-1+action]); /* 1-Press, 2-Release, 3-Double */
        }  }
   else {
        int mode;
        COPSYMB(EVW[_Kbd]);
        mode = ev_KbdMode(event);
        COPSYMB(EVW[_ASCII+mode%3]);      /* 0-ASCII, 1-Ctrl, 2-Func */
        movb; SETSIM(b,(BYTE)(event.ev_code),(mode? TYPESNUMB:TYPECHAR));
        movb; SETSIM(b,(BYTE)(event.sc_code),TYPESNUMB);
        }
    br;
/*========  Generating t.Modifiers =========  */
    rf_mknumb((long)(event.mod_state));
/*========  Generating s.X s.Y     =========  */
    rf_mknumb((long)(event.ptrX));
    rf_mknumb((long)(event.ptrY));
    return(TRUE);
    }



static int pointercounter = -1;

LOGICAL rb_mouse()
{
   int res; expr eM; cvalue nc;
   elemptr bstart, EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL, EargR);
   eM.L = EargL; eM.R = EargR;
   bstart=b;
   res = TRUE;
   CAR(eM,nc);
        if(nc==EVW[_Get])   res = rf_get_event(TRUE);
   else if(nc==EVW[_Check]) res = rf_get_event(FALSE);
   else if(nc==EVW[_Drag]) switch(ev_get_onoff(&eM)) {
          case OFF:   ED_DragOff(); break;
          case ON:    ED_DragOn (); break;
          case EMP:   rf_mk_onoff(ED_DragState()); break;
          case ERR:   res=FAIL;
          }
   else if(nc==EVW[_KbdMouse]) switch(ev_get_onoff(&eM)) {
          case OFF:   ED_KbdMouseOff(); break;
          case ON:    ED_KbdMouseOn (); break;
          case EMP:   rf_mk_onoff(ED_KbdMouseState()); break;
          case ERR:   res=FAIL;
          }
   else if(nc==EVW[_Pointer]) switch(ev_get_onoff(&eM)) {
          case OFF:   pointercounter = ED_HidePointer(); break;
          case ON:    pointercounter = ED_DisplayPointer (); break;
          case EMP:   rf_mk_onoff(pointercounter >= 0); break;
          case ERR:   res=FAIL;
          }
   else if(nc==EVW[_MovePointer]) {
           cvalue xc,yc;
           CAR(eM,xc);
           if(NOT IS_SNUMB(CTYPE(xc))) return(FAIL);
           CAR(eM,yc);
           if(NOT IS_SNUMB(CTYPE(yc))) return(FAIL);
           ED_MovePointer(CVAL(xc), CVAL(yc));
           }
   else if(nc==EVW[_Init]) ED_Init();
   else if(nc==EVW[_Done]) ED_Done();
   else return(FAIL);
   if(res != TRUE) { b=bstart; return(res); }
   else if(b==bstart) *AeresL = *AeresR = NOELEM;
   else {
        *AeresL = NEXT(bstart);
        *AeresR= b;
        weld(bstart,NEXT(b));
        b = bstart;
        }
   AFTER(b,EargL,EargR);
   return(TRUE);
   };
