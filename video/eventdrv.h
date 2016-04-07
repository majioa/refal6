#if !defined(__EVENTDRV_H)						
#define __EVENTDRV_H

#include "comdef.h"
/*=====  Header file  ==========================  (c)  mkSoft  1992   =====*/
/*                                                                         */
/*              �ࠩ��� ᮡ�⨩ (Event Driver)  -  ver 0.0                 */
/*              ==========================================                 */
/*                                                                         */
/*                                                                         */
/*       ��� �ࠩ��� ࠡ�⠥� ���� ������ � ���⮩ VGA  � �� �⠭���⭮�  */
/*  ����᪮� ०��� (640*480*16); ��� ࠡ��� �ࠩ��� �ॡ���� �����  */
/*  GrTxtV.                                                                */
/*                                                                         */
/*       �ࠩ���  �ନ���  ��⮪ ᮡ�⨩ (������ ������ �� ���������;  */
/*  ����⨥ ������ ��� ��६�饭�� ���)  � ��।��� ��� ࠡ�祩 �ணࠬ-  */
/*  ��. ���� ����� ����஢����� ��������ன.                              */
/*                                                                         */
/*       �᫨ ���� ���������, ��� ������ �����ন������ Microsoft Mou-  */
/*  se Driver'�� ����� 7.0 ��� ���.                                      */
/*                                                                         */
/*       �����⥫� ��� ������ �ࠩ��஬ ��� (�᫨ ���� ���������),  */
/*  ��� �����騬 �ࠩ��஬ (�᫨ ���� ���������).                       */
/*                                                                         */
/*       ����� ����樨 ��� ��������ன ����� ���� ����祭 ���� �ணࠬ-  */
/*  ���, ���� 祫������ � ����������. ����� ����樨 ��⠭����������, ��-  */
/*  �� �믮����� ��� �� ���� �� ��� ����祭��. ����祭��/�몫�祭�� �-  */
/*  ���� ����樨 � ���������� �ந�������� ����⨥� Alt+Space.  � ०���  */
/*  ����樨:  ����⨥ ��५�� �ਢ���� � ��६�饭�� 㪠��⥫�  ���  ��  */
/*  �࠭� (�� �ਦ�⮩ ������ Shift - �᪮७��� ��६�饭��); ����⨥  */
/*  �஡��� ��� ������ ���⪨ �������⭮ ������ ����� ������  ���;  */
/*  ����⨥ Escape - ������ �ࠢ�� ������ ���; ����⨥ �஡���, �����-  */
/*  � ���⪨ ��� Escape �� �ਦ�⮩ ������ Shift �������⭮ ������-  */
/*  �� ������ ᮮ⢥�����饩 ������ ���;  � �� ��  �ਦ�⮩  ������  */
/*  CapsLock - ������ ᮮ⢥�����饩 ������ ��� ���᪠���  ("���᪠-  */
/*  ���" �ந������ �� ����୮� ����⨨ �⮩ ������ - �� ��易⥫쭮 �  */
/*  ����䨪��ࠬ�).                                                       */
/*                                                                         */
/*       ������  "Drag On / Drag Off"  ��४��砥��� ⮫쪮 �ணࠬ���. �  */
/*  ०��� "Drag On" ������ � ���᪠��� ������ ��� ����� �⤥��묨  */
/*  ᮡ��ﬨ; ᮡ�⨥� ⠪�� ���� ��������� ���न��� 㪠��⥫� (��-  */
/*  ६�饭��) ���. � ०��� "Drag Off" ��६�饭�� ��� ����� �� �-  */
/*  ���� ᮡ�⨥�, � ���᪠��� ������ �����������.                       */
/*                                                                         */
/*=========================================================================*/


/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      �������, � ���ன �����頥��� ���ଠ�� � ᮡ�⨨             */

typedef  struct  {
    WORD  ev_code;              /* ��� ᮡ���                             */
    WORD  sc_code;              /* ᪠�-��� �᭮���� ������               */
    WORD  mod_state;            /* ���ﭨ� ������-����䨪��஢          */
    WORD  ptrX, ptrY;           /* x- � y-���न���� 㪠��⥫� ���        */
  }                  EVENT;



/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      ������ ��楤��� � �㭪樨                                        */

void  ED_Init  ( void );    /*    ���樠����஢��� �ࠩ���.  �� ��楤�� */
                            /* ������ ���� �맢��� ��᫥ ���樠����樨 ��- */
                            /* ��� GrTxtV  (�����, � ᢮� ��।�, ���- */
                            /* ��� ���� ���樠����஢�� ��᫥ ��४��祭�� */
                            /* �࠭� � ����᪨� ०��).                */

void  ED_Done  ( void );    /*    �������� ࠡ��� �ࠩ���. �� ��楤�� */
                            /* ���� ��祣� �� ������,  �� �� ��直� ��砩 */
                            /* ���� �� �맢���.                           */

void  ED_GetEvent  ( EVENT  *event );      /*   ���������� ������-���� �- */
                                  /* ���� � �����頥� � ��६����� event */
                                  /* ���ଠ�� � ���.  ����⨥� ����: */
                                  /* ����⨥ ������, ����⨥ ������ ���, */
                                  /* ��६�饭�� ���.                     */

void  ED_CheckEvent  ( EVENT  *event );       /*   �����頥� � ��६����� */
                         /* event ���ଠ�� � ᮡ�⨨, �ந��襤襬 ��᫥ */
                         /* ��᫥����� �맮��  ��楤���  ED_GetEvent  ��� */
                         /* ED_CheckEvent.  �᫨ �������� ᮡ��� �� �ந- */
                         /* ��諮,  � ����� ev_code � sc_code �����頥��� */
                         /* 0; ����  mod_state, ptrX � ptrY  ᮮ⢥������ */
                         /* ���ﭨ� �� ������ �맮��.                    */

void  ED_DragOn  ( void );    /*   ��᫥ �맮�� �⮩ ��楤��� ��६�饭�� */
                              /* ��� ���� ᮡ�⨥�.                   */

void  ED_DragOff  ( void );   /*   ��᫥ �맮�� �⮩ ��楤��� ��६�饭�� */
                              /* ��� ����⠥� ���� ᮡ�⨥�.             */

BOOLEAN  ED_DragState  ( void );      /*   ����頥� ⥪�饩 ०�� Drag:    */
                                      /* true - Drag On, false - Drag Off. */

void  ED_KbdMouseOn  ( void );        /*   ����砥� ०�� ����樨 ��� �� */
                                      /* ���������.                       */

void  ED_KbdMouseOff  ( void );      /*   �몫�砥� ०�� ����樨 ��� �� */
                                     /* ���������.                        */

BOOLEAN  ED_KbdMouseState  ( void );  /*   ����頥� ⥪�騩 ०�� ����樨 */
                                    /* ��� �� ��������� (����祭�� ��- */
                                    /* �ࠬ���): true - On, false - Off.   */

int   ED_DisplayPointer ( void ); /*  �����稢��� �� 1 ����७��� ���稪, */
                      /* � �᫨ ��᫥ �⮣� �� �⠭������ ������⥫��, */
                      /* �뢮��� �� �࠭ 㪠��⥫� ���.  � ����⢥  �- */
                      /* ����� �����頥��� ���祭�� ����७����  ���- */
                      /* 稪�  (��� �ࠢ���,  ��뢠�饩 ��楤�ன �� ��- */
                      /* ��������).                                     */








int   ED_HidePointer  ( void );  /*   �����蠥� �� 1 ����७��� ���稪, � */
                       /* �᫨ ��᫥ �⮣�  ��  �⠭������  ����⥫��, */
                       /* 㡨ࠥ� � �࠭� 㪠��⥫� ���.  � ����⢥ �- */
                       /* ����� �����頥��� ���祭�� ����७���� ���- */
                       /* 稪�  (��� �ࠢ���, ��뢠�饩 ��楤�ன �� ��- */
                       /* ��������).                                     */

void  ED_MovePointer  ( INT x, INT y );   /*   ��६�頥� 㪠��⥫� ��� � */
                                          /* �������� ��ࠬ��ࠬ� ������. */


/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      ��।������ ����⠭� - ����� ᮡ�⨩                               */
/*                                                                         */
/*       �� ������, ��ࠡ��뢠��� ������ �����, �����頥� � ����  */
/*  ev_code: � ���襬 ���� - 0, � ����襬 ���� - ASCII-���.             */
/*       ��� ��� ��⠫��� ᮡ�⨩ ���訩 ���� ev_code �⫨祭 �� 0.     */
/*       ��� ᮡ�⨩, �易���� � �����, ���訩 ��� ���襣� ���� � ��-  */
/*  �� ev_code ࠢ�� 1; ��� ᮡ�⨩, �易���� � ��������ன - 0.          */
/*       ��� ᮡ�⨩,  �易���� � ��������ன � �� ������  ����묨  */
/*  ���ࠬ�, ��� ᮡ��� �ନ����� ᫥���騬 ᯮᮡ��:                  */
/*    - �᫨ �����饭��  BIOS'��  ASCII-���  �⫨祭 �� 0 � ����� 0x20  */
/*      (�� �ந�室�� �� ����⨨ ������ Esc, Bs, Tab, Cr � �� ����⨨  */
/*      �㪢����-��஢�� ������ � Ctrl),  ��� ᮡ��� ᮤ�ন� � ���襬  */
/*      ���� 1 � � ����襬 ���� ASCII-���. �᪫�祭��: ^(2/@) ������-  */
/*      �� ASCII-��� 0 � ᪠�-��� 3, �� ��� ᮡ��� �㤥� ����� 0x0100.    */
/*    - �᫨ �����饭�� BIOS'��  ASCII-���  ���� 0  (�� �ந�室�� ��  */
/*      ����⨨ F1-F10, ��५��, PgUp, PgDn, Home, End, Ins, Del, �㪢��-  */
/*      ��-��஢�� ������ �� �ਦ�⮩ ������ Alt),  � ��� ᮡ��� �-  */
/*      ��ন� � ���襬 ���� 2 � � ����襬 ����  �����饭��  BIOS'��  */
/*      ᪠�-��� (�� �ᥣ�� ᮢ������ � ��⨭�� ᪠�-����� ������!).     */


#define  evcd_MouDrag   0x8000
#define  evcd_MouLBPre  0x8011
#define  evcd_MouLBRel  0x8012
#define  evcd_MouLBDbl  0x8013
#define  evcd_MouRBPre  0x8021
#define  evcd_MouRBRel  0x8022
#define  evcd_MouRBDbl  0x8023




















/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      ��।������ ����⠭� - ���祭�� ���� sc_code                       */
/*                                                                         */
/*       �� ������ �����頥� � ���� sc_code ��⨭�� ᪠�-��� �����-  */
/*  � � ����襬 ���� � 0 � ���襬 ����. ��⨭�� ᪠�-����:            */

#define  sccd_Esc       0x0001              /*     Esc                     */
#define  sccd_1         0x0002              /*     1  !                    */
#define  sccd_2         0x0003              /*     2  @                    */
#define  sccd_3         0x0004              /*     3  #                    */
#define  sccd_4         0x0005              /*     4  $                    */
#define  sccd_5         0x0006              /*     5  %                    */
#define  sccd_6         0x0007              /*     6  ^                    */
#define  sccd_7         0x0008              /*     7  &                    */
#define  sccd_8         0x0009              /*     8  *                    */
#define  sccd_9         0x000A              /*     9  (                    */
#define  sccd_0         0x000B              /*     0  )                    */
#define  sccd_MinUnd    0x000C              /*     -  _                    */
#define  sccd_EqPlus    0x000D              /*     =  +                    */
#define  sccd_Bs        0x000E              /*     Bs                      */
#define  sccd_Tab       0x000F              /*     Tab                     */
#define  sccd_Q         0x0010              /*     Q                       */
#define  sccd_W         0x0011              /*     W                       */
#define  sccd_E         0x0012              /*     E                       */
#define  sccd_R         0x0013              /*     R                       */
#define  sccd_T         0x0014              /*     T                       */
#define  sccd_Y         0x0015              /*     Y                       */
#define  sccd_U         0x0016              /*     U                       */
#define  sccd_I         0x0017              /*     I                       */
#define  sccd_O         0x0018              /*     O                       */
#define  sccd_P         0x0019              /*     P                       */
#define  sccd_LBB       0x001A              /*     [  {                    */
#define  sccd_RBB       0x001B              /*     ]  }                    */
#define  sccd_Cr        0x001C              /*     Cr                      */
#define  sccd_Ctrl      0x001D              /*     Ctrl                    */
#define  sccd_A         0x001E              /*     A                       */
#define  sccd_S         0x001F              /*     S                       */
#define  sccd_D         0x0020              /*     D                       */
#define  sccd_F         0x0021              /*     F                       */
#define  sccd_G         0x0022              /*     G                       */
#define  sccd_H         0x0023              /*     H                       */
#define  sccd_J         0x0024              /*     J                       */
#define  sccd_K         0x0025              /*     K                       */
#define  sccd_L         0x0026              /*     L                       */
#define  sccd_ScC       0x0027              /*     ;  :                    */
#define  sccd_QA        0x0028              /*     '  "                    */
#define  sccd_BaT       0x0029              /*     `  ~                    */
#define  sccd_LShift    0x002A              /*     LeftShift               */
#define  sccd_BsV       0x002B              /*     \  |                    */
#define  sccd_Z         0x002C              /*     Z                       */
#define  sccd_X         0x002D              /*     X                       */
#define  sccd_C         0x002E              /*     C                       */
#define  sccd_V         0x002F              /*     V                       */
#define  sccd_B         0x0030              /*     B                       */
#define  sccd_N         0x0031              /*     N                       */





#define  sccd_M         0x0032              /*     M                       */
#define  sccd_CL        0x0033              /*     ,  <                    */
#define  sccd_PG        0x0034              /*     .  >                    */
#define  sccd_SQ        0x0035              /*     /  ?                    */
#define  sccd_RShift    0x0036              /*     RightShift              */
#define  sccd_PScrStar  0x0037              /*     PrtScr  GreyStar        */
#define  sccd_Alt       0x0038              /*     Alt                     */
#define  sccd_Space     0x0039              /*     Space                   */
#define  sccd_CapsLock  0x003A              /*     CapsLock                */
#define  sccd_F1        0x003B              /*     F1                      */
#define  sccd_F2        0x003C              /*     F2                      */
#define  sccd_F3        0x003D              /*     F3                      */
#define  sccd_F4        0x003E              /*     F4                      */
#define  sccd_F5        0x003F              /*     F5                      */
#define  sccd_F6        0x0040              /*     F6                      */
#define  sccd_F7        0x0041              /*     F7                      */
#define  sccd_F8        0x0042              /*     F8                      */
#define  sccd_F9        0x0043              /*     F9                      */
#define  sccd_F10       0x0044              /*     F10                     */
#define  sccd_NumLock   0x0045              /*     NumLock                 */
#define  sccd_ScrollLock  0x0046            /*     ScrollLock              */
#define  sccd_Home      0x0047              /*     Home   7   (keypad)     */
#define  sccd_Up        0x0048              /*     Up     8   (keypad)     */
#define  sccd_PgUp      0x0049              /*     PgUp   9   (keypad)     */
#define  sccd_GMinus    0x004A              /*     GreyMinus  (keypad)     */
#define  sccd_Left      0x004B              /*     Left   4   (keypad)     */
#define  sccd_555       0x004C              /*            5   (keypad)     */
#define  sccd_Right     0x004D              /*     Right  6   (keypad)     */
#define  sccd_GPlus     0x004E              /*     GreyPlus   (keypad)     */
#define  sccd_End       0x004F              /*     End    1   (keypad)     */
#define  sccd_Down      0x0050              /*     Down   2   (keypad)     */
#define  sccd_PgDn      0x0051              /*     PgDn   3   (keypad)     */
#define  sccd_Ins       0x0052              /*     Ins    0   (keypad)     */
#define  sccd_Del       0x0053              /*     Del    .   (keypad)     */

/*       ���� �����頥� ᫥���騥 ᪠�-����:                              */

#define  sccd_MouDrag    0x8000       /* ᮡ�⨥ �맢��� ��६�襭��� ��� */
#define  sccd_MouLeft    0x8001       /* ᮡ�⨥ �맢��� ����� �������     */
#define  sccd_MouRight   0x8002       /* ᮡ�⨥ �맢��� �ࠢ�� �������    */

/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      ��।������ ����⠭� ��� �뤥����� ��⮢ ������-����䨪��஢      */
/*                              (� ���� mod_state ����� EVENT).           */
#define  bit_LShift     0x0001
#define  bit_RShift     0x0002
#define  bit_Shift      0x0003
#define  bit_LCtrl      0x0004
#define  bit_RCtrl      0x0008
#define  bit_Ctrl       0x000C
#define  bit_LAlt       0x0010
#define  bit_RAlt       0x0020
#define  bit_Alt        0x0030
#define  bit_CapsLock   0x0040
#define  bit_NumLock    0x0080

#endif



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                             ��䠫-����䥩�                             */
/*                             ===============                             */
/*                                                                         */
/*       ������� ����⢥��� ��ꥪ� EventHandler, ����� 㬥�� ��ࠡ���-  */
/*  ���� ᫥���騥 ᮮ�饭��:                                              */
/*                                                                         */
/*          <EventHandler  Init>                    ==                     */
/*          <EventHandler  Done>                    ==                     */
/*          <EventHandler  GetEvent>                ==  e.Event            */
/*          <EventHandler  CheckEvent>              ==  e.Event            */
/*          <EventHandler  Drag>                    ==  s.OnOff            */
/*          <EventHandler  Drag  s.OnOff>           ==                     */
/*          <EventHandler  KbdMouse>                ==  s.OnOff            */
/*          <EventHandler  KbdMouse  s.OnOff>       ==                     */
/*          <EventHandler  Pointer>                 ==  s.OnOff            */
/*          <EventHandler  Pointer  s.OnOff>        ==                     */
/*          <EventHandler  MovePointer  s.X  s.Y>   ==                     */
/*                                                                         */
/*  �����: s.OnOff �ਭ����� ���� �� ���� ��������� ���祭��: On ��� Off;  */
/*  s.X � s.Y - ᨬ����-�᫠; e.Event - ��ࠦ����, ᮤ�ঠ饥 ���ଠ��  */
/*  � ᮡ�⨨; ��� ����� ᫥���騩 �ଠ�:                                 */
/*                                                                         */
/*          t.Event  t.Modifiers  s.X  s.Y                                 */
/*                                                                         */
/*  ���:  s.X � s.Y  - ᨬ����-�᫠ - ���न���� 㪠��⥫� ��� � ������  */
/*  ᮡ���;  t.Modifiers  - ᪮���� ��, ᮤ�ন��� ���ண� ����  */
/*  ��᫥����⥫쭮���� ᨬ�����-᫮� �� ᫥���饣� ᯨ᪠:                */
/*                                                                         */
/*          LeftShift  RightShift  LeftCtrl  RightCtrl                     */
/*          LeftAlt  RightAlt  CapsLock  NumLock                           */
/*                                                                         */
/*  (�� �⮬ ���������騥 ᫮�� ���� � �ਢ������� ���浪�);   t.Event  */
/*  - ᪮���� ��, ��������� ᮤ�ন��� ���ண� ����뢠���� ᫥���饩  */
/*  ����ࠬ��� (�������� �� ����ࠬ�� - ᫥�� ���ࠢ�):                    */
/*                                                                         */
/*                      ��� ASCII ���� s.Char �Ŀ                          */
/*                      �                       �                          */
/*            ��� Kbd ����� Ctrl �Ŀ            ��� s.Scan �Ŀ             */
/*            �         �          ��� s.Code ���            �             */
/*            �         ��� Func ���                         �             */
/*            �                                              �             */
/*          �Ĵ                                              ���           */
/*            �                                              �             */
/*            �           ��� Drag ���������������������Ŀ   �             */
/*            �           �             ��� Press ���Ŀ  �   �             */
/*            ��� Mouse ����� Left  �Ŀ �             �  �   �             */
/*                        �           ����� Release ����������             */
/*                        ��� Right ��� �             �                    */
/*                                      ��� Double ����                    */
/*                                                                         */
/*  ���:  s.Char - ᨬ���-����; s.Code - ᨬ���-�᫮, ᮤ�ঠ騩 ����-  */
/*  訩 ���� ���� ev_code; s.Scan - ᨬ���-�᫮, ᮤ�ঠ騩 ����訩 ����  */
/*  ���� sc_code.                                                          */
/*=========================================================================*/
