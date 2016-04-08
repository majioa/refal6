/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arkady Klimov            */
/*        video.h    - graphics object headder                          */
/************************************************************************/
#if !defined(__VIDEO_H)
#define __VIDEO_H

#ifndef __REFCOM_H
#include "refcom.h"
#endif
#ifndef __EVENTDRV_H
#include "eventdrv.h"
#endif
#include <graphics.h>

#define MAXDIM 5  /* The maximum dimension of virtual space */

typedef enum { VS_SET , VS_XOR } VideoFunctions;

typedef enum { _POINT, _VECTOR } povec;
typedef int   intpos;
typedef struct { intpos X; intpos Y; } pos;
typedef pos vec;
typedef struct { long X; long Y; long Z; } cpos;
typedef cpos cvec;
typedef struct { pos   P; vec   V; } rect;
typedef long   point[MAXDIM];
typedef struct palettetype palette;
typedef struct {
         cvalue   cfont;
         LOGICAL  started;
         unsigned width;
         unsigned height;
         } font;

typedef struct {
    LOGICAL  Started;
    unsigned Color;
    unsigned BackColor;
    int      Background;
    rect     ClipArea;
    pos      Base;
    cpos     Current;
    pos      TextCur;
    LOGICAL  TextSet;
    cpos     Origin;
    long     Norm;
    cpos     Basis[MAXDIM];
    int      Dim;
    unsigned Function;
    palette  Palette;
/*  style1   DrawStyle;
    style2   FillStyle; */
    font     Font;
    int      ActivePage;
    int      VisualPage;
    LOGICAL  LineVisible;
    } tvideo;

static enum putimage_ops PutImageOp[] = { COPY_PUT , XOR_PUT };

static tvideo StandVideo = { FALSE,   /* Started            */
  WHITE, BLACK, TRUE,                 /* Colors, Background */
  { {0,0}, {640,480} }, {0,0},        /* ClipArea, Base     */
  {0,0}, {0,0}, FALSE,                /* Current, TextCur, TextSet */
  {0,0,0}, 1000,                      /* Origin, Norm       */
  { { 1000,    0,    0 } ,
    {    0, 1000,    0 } ,
    {    0,    0, 1000 } }, 3,        /* Basis, Dim         */
  VS_SET,                             /* Function           */
  { 16 , { 0 } },                     /* Palette            */
  { NULL, FALSE, 8, 16 },             /* Font               */
  0, 0,                               /* Active and Visual Pages */
  TRUE                                /* LineVisible        */
  };

#define DFOCUS 1000  /* distance from eye to the screen in pixels */

extern  void far pascal  InitGrTxtV  ( void );

extern  void far pascal  SetAutoRegsProgramming      ( void );
extern  void far pascal  SetManualRegsProgramming    ( void );
extern  void far pascal  ProgramControllerRegisters  ( void );
extern  void far pascal  RestoreControllerRegisters  ( void );

extern  void far pascal  DisableVideoController  ( void );
extern  void far pascal  EnableVideoController   ( void );

extern  void far pascal  SetBiosFont  ( void );
extern  void far pascal  SetFont  (   void far *  font_ptr,
                                    unsigned int  wdth,
                                    unsigned int  hight    );

extern  void far pascal  SetColor  (  unsigned int  foreground,
                                      unsigned int  background  );

extern  void far pascal  BackgroundOn   ( void );
extern  void far pascal  BackgroundOff  ( void );

extern  void far pascal  SetPos  (  int  x, int  y  );
extern  void far pascal  OutChar  (  char  c  );
extern  void far pascal  OutString  (  unsigned int  l,
                                         char far *  s  );

extern  void far pascal  MoveRectangle  (  int  x1, int  y1,
                                           int  x2, int  y2,
                                           int  vx, int  vy  );

#define GetPixel(x,y)              getpixel(x,y)
#define PutPixel(aV,x,y)           putpixel(x,y,(aV)->Color)
#define Line(aV,x,y,xx,yy)         line(x,y,xx,yy)
#define  Rectangle(aV,x,y,xx,yy)   rectangle(x,y,xx,yy)
#define FRectangle(aV,x,y,xx,yy)   bar(x,y,xx,yy)
#define  Ellipse(aV,x,y,xx,yy)     ellipse(x,y,0,360,xx,yy)
#define FEllipse(aV,x,y,xx,yy)     fillellipse(x,y,xx,yy)
#define  Polygon(aV,n,points)      drawpoly(n,(int*)points)
#define FPolygon(aV,n,points)      fillpoly(n,(int*)points)
#define LinkPoints(n,points)
#define Fill(aV,x,y,border)           floodfill(x,y,border)

/************************************************************************/
#define InitGraph(path,rcode) { int gdriver = DETECT, gmode = VGAHI; \
                                registerbgidriver(EGAVGA_driver); \
                                initgraph(&gdriver, &gmode, path); \
                                rcode = graphresult(); \
                                if(rcode==0)InitGrTxtV(); \
                                }

#define CloseGraph closegraph()

#define SETCOLOR(aV) { setfillstyle(SOLID_FILL,(aV)->Color);\
                       setcolor((aV)->Color); }

#define SETTEXTCOLOR(aV) { SetColor((aV)->Color,(aV)->BackColor); }

#define SETDRAWFUNC(aV)  setwritemode((aV)->Function);

#define SETDRAWSTYLE(aV) setlinestyle(USERBIT_LINE,(aV)->DrawStyle,1);

#endif
