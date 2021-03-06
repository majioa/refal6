#if !defined(__EVENTDRV_H)
#define __EVENTDRV_H

#include "comdef.h"
/*=====  Header file  ==========================  (c)  mkSoft  1992   =====*/
/*                                                                         */
/*              Драйвер событий (Event Driver)  -  ver 0.0                 */
/*              ==========================================                 */
/*                                                                         */
/*                                                                         */
/*       Этот драйвер работает ПОКА ТОЛЬКО с картой VGA  в ее стандартном  */
/*  графическом режиме (640*480*16); для работы драйвера требуется модуль  */
/*  GrTxtV.                                                                */
/*                                                                         */
/*       Драйвер  формирует  поток событий (нажатия клавиш на клавиатуре;  */
/*  нажатие кнопок или перемещение мыши)  и передает его рабочей програм-  */
/*  ме. Мышь может имитироваться клавиатурой.                              */
/*                                                                         */
/*       Если мышь присутствует, она должна поддерживаться Microsoft Mou-  */
/*  se Driver'ом версия 7.0 или выше.                                      */
/*                                                                         */
/*       Указатель мыши рисуется драйвером мыши (если мышь присутствует),  */
/*  или настоящим драйвером (если мышь отсутствует).                       */
/*                                                                         */
/*       Режим имитации мыши клавиатурой может быть включен либо програм-  */
/*  мой, либо человеком с клавиатуры. Режим имитации устанавливается, ес-  */
/*  ли выполнено хотя бы одно из этих включений. Включение/выключение ре-  */
/*  жима имитации с клавиатуры производится нажатием Alt+Space.  В режиме  */
/*  имитации:  нажатие стрелок приводит к перемещению указателя  мыши  по  */
/*  экрану (при прижатой клавише Shift - ускоренное перемещение); нажатие  */
/*  пробела или возврата каретки эквивалентно нажатию левой кнопки  мыши;  */
/*  нажатие Escape - нажатию правой кнопки мыши; нажатие пробела, возвра-  */
/*  та каретки или Escape при прижатой клавише Shift эквивалентно двойно-  */
/*  му нажатию соответствующей кнопки мыши;  то же при  прижатой  клавише  */
/*  CapsLock - нажатию соответствующей клавиши без отпускания  ("отпуска-  */
/*  ние" произойдет при повторном нажатии этой клавиши - не обязательно с  */
/*  модификаторами).                                                       */
/*                                                                         */
/*       Режимы  "Drag On / Drag Off"  переключается только программно. В  */
/*  режиме "Drag On" нажатия и отпускания кнопок мыши являются отдельными  */
/*  событиями; событием также является изменение координат указателя (пе-  */
/*  ремещение) мыши. В режиме "Drag Off" перемещение мыши вообще не явля-  */
/*  ется событием, а отпускания кнопок игнорируются.                       */
/*                                                                         */
/*=========================================================================*/


/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      Структура, в которой возвращается информация о событии             */

typedef  struct  {
    WORD  ev_code;              /* код события                             */
    WORD  sc_code;              /* скан-код основной клавиши               */
    WORD  mod_state;            /* состояние клавиш-модификаторов          */
    WORD  ptrX, ptrY;           /* x- и y-координаты указателя мыши        */
  }                  EVENT;



/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      Видимые процедуры и функции                                        */

void  ED_Init  ( void );    /*    Инициализировать драйвер.  Эта процедура */
                            /* должна быть вызвана после инициализации мо- */
                            /* дуля GrTxtV  (который, в свою очередь, дол- */
                            /* жен быть инициализирован после переключения */
                            /* экрана в графический режим).                */

void  ED_Done  ( void );    /*    Закончить работу драйвера. Эта процедура */
                            /* пока ничего не делает,  но на всякий случай */
                            /* лучше ее вызвать.                           */

void  ED_GetEvent  ( EVENT  *event );      /*   Дожидается какого-либо со- */
                                  /* бытия и возвращает в переменной event */
                                  /* информацию о нем.  Событием является: */
                                  /* нажатие клавиши, нажатие кнопки мыши, */
                                  /* перемещение мыши.                     */

void  ED_CheckEvent  ( EVENT  *event );       /*   Возвращает в переменной */
                         /* event информацию о событии, произошедшем после */
                         /* последнего вызова  процедуры  ED_GetEvent  или */
                         /* ED_CheckEvent.  Если никакого события не прои- */
                         /* зошло,  в полях ev_code и sc_code возвращается */
                         /* 0; поля  mod_state, ptrX и ptrY  соответствуют */
                         /* состоянию на момент вызова.                    */

void  ED_DragOn  ( void );    /*   После вызова этой процедуры перемещение */
                              /* мыши является событием.                   */

void  ED_DragOff  ( void );   /*   После вызова этой процедуры перемещение */
                              /* мыши перестает быть событием.             */

BOOLEAN  ED_DragState  ( void );      /*   Сообщает текущей режим Drag:    */
                                      /* true - Drag On, false - Drag Off. */

void  ED_KbdMouseOn  ( void );        /*   Включает режим имитации мыши на */
                                      /* клавиатуре.                       */

void  ED_KbdMouseOff  ( void );      /*   Выключает режим имитации мыши на */
                                     /* клавиатуре.                        */

BOOLEAN  ED_KbdMouseState  ( void );  /*   Сообщает текущий режим имитации */
                                    /* мыши на клавиатуре (включенный про- */
                                    /* граммно): true - On, false - Off.   */

int   ED_DisplayPointer ( void ); /*  Увеличивает на 1 внутренний счетчик, */
                      /* и если после этого он становится неотрицательным, */
                      /* выводит на экран указатель мыши.  В качестве  ре- */
                      /* зультата возвращается значение внутреннего  счет- */
                      /* чика  (как правило,  вызывающей процедурой не ис- */
                      /* пользуется).                                     */








int   ED_HidePointer  ( void );  /*   Уменьшает на 1 внутренний счетчик, и */
                       /* если после этого  он  становится  отрицательным, */
                       /* убирает с экрана указатель мыши.  В качестве ре- */
                       /* зультата возвращается значение внутреннего счет- */
                       /* чика  (как правило, вызывающей процедурой не ис- */
                       /* пользуется).                                     */

void  ED_MovePointer  ( INT x, INT y );   /*   Перемещает указатель мыши в */
                                          /* заданную параметрами позицию. */


/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      Определения констант - кодов событий                               */
/*                                                                         */
/*       Любая клавиша, вырабатывающая печатную литеру, возвращает в поле  */
/*  ev_code: в старшем байте - 0, в младшем байте - ASCII-код.             */
/*       Для всех остальных событий старший байт ev_code отличен от 0.     */
/*       Для событий, связанных с мышью, старший бит старшего байта в по-  */
/*  ле ev_code равен 1; для событий, связанных с клавиатурой - 0.          */
/*       Для событий,  связанных с клавиатурой и не являющихся  печатными  */
/*  литерами, код события формируется следующим способом:                  */
/*    - если возвращенный  BIOS'ом  ASCII-код  отличен от 0 и меньше 0x20  */
/*      (это происходит при нажатии клавиш Esc, Bs, Tab, Cr и при нажатии  */
/*      буквенно-цифровых клавиш с Ctrl),  код события содержит в старшем  */
/*      байте 1 и в младшем байте ASCII-код. Исключение: ^(2/@) возвраща-  */
/*      ет ASCII-код 0 и скан-код 3, но код события будет здесь 0x0100.    */
/*    - если возвращенный BIOS'ом  ASCII-код  есть 0  (это происходит при  */
/*      нажатии F1-F10, стрелок, PgUp, PgDn, Home, End, Ins, Del, буквен-  */
/*      но-цифровых клавиш при прижатой клавише Alt),  то код события со-  */
/*      держит в старшем байте 2 и в младшем байте  возвращенный  BIOS'ом  */
/*      скан-код (не всегда совпадает с истинным скан-кодом клавиши!).     */


#define  evcd_MouDrag   0x8000
#define  evcd_MouLBPre  0x8011
#define  evcd_MouLBRel  0x8012
#define  evcd_MouLBDbl  0x8013
#define  evcd_MouRBPre  0x8021
#define  evcd_MouRBRel  0x8022
#define  evcd_MouRBDbl  0x8023




















/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      Определения констант - значений поля sc_code                       */
/*                                                                         */
/*       Любая клавиша возвращает в поле sc_code истинный скан-код клави-  */
/*  ши в младшем байте и 0 в старшем байте. Истинные скан-коды:            */

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

/*       Мышь возвращает следующие скан-коды:                              */

#define  sccd_MouDrag    0x8000       /* событие вызвано перемешением мыши */
#define  sccd_MouLeft    0x8001       /* событие вызвано левой кнопкой     */
#define  sccd_MouRight   0x8002       /* событие вызвано правой кнопкой    */

/*"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""*/
/*      Определения констант для выделения битов клавиш-модификаторов      */
/*                              (в поле mod_state записи EVENT).           */
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
/*                             Рефал-интерфейс                             */
/*                             ===============                             */
/*                                                                         */
/*       Имеется едиственный объект EventHandler, который умеет обрабаты-  */
/*  вать следующие сообщения:                                              */
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
/*  Здесь: s.OnOff принимает одно из двух возможных значений: On или Off;  */
/*  s.X и s.Y - символы-числа; e.Event - выражение, содержащее информацию  */
/*  о событии; оно имеет следующий формат:                                 */
/*                                                                         */
/*          t.Event  t.Modifiers  s.X  s.Y                                 */
/*                                                                         */
/*  где:  s.X и s.Y  - символы-числа - координаты указателя мыши в момент  */
/*  события;  t.Modifiers  - скобочный терм, содержимое которого является  */
/*  последовательностью символов-слов из следующего списка:                */
/*                                                                         */
/*          LeftShift  RightShift  LeftCtrl  RightCtrl                     */
/*          LeftAlt  RightAlt  CapsLock  NumLock                           */
/*                                                                         */
/*  (при этом присутствующие слова идут в приведенном порядке);   t.Event  */
/*  - скобочный терм, возможное содержимое которого описывается следующей  */
/*  диаграммой (движение по диаграмме - слева направо):                    */
/*                                                                         */
/*                      ┌── ASCII ──── s.Char ──┐                          */
/*                      │                       │                          */
/*            ┌── Kbd ──┼── Ctrl ──┐            ├── s.Scan ──┐             */
/*            │         │          ├── s.Code ──┘            │             */
/*            │         └── Func ──┘                         │             */
/*            │                                              │             */
/*          ──┤                                              ├──           */
/*            │                                              │             */
/*            │           ┌── Drag ──────────────────────┐   │             */
/*            │           │             ┌── Press ────┐  │   │             */
/*            └── Mouse ──┼── Left  ──┐ │             │  │   │             */
/*                        │           ├─┼── Release ──┼──┴───┘             */
/*                        └── Right ──┘ │             │                    */
/*                                      └── Double ───┘                    */
/*                                                                         */
/*  где:  s.Char - символ-литера; s.Code - символ-число, содержащий млад-  */
/*  ший байт поля ev_code; s.Scan - символ-число, содержащий младший байт  */
/*  поля sc_code.                                                          */
/*=========================================================================*/
