/************************************************************************/
/*   R E F A L - 6 :     Copyright (C) 1992 by Arkady Klimov            */
/*        video.c    - graphics videosupport object                     */
/************************************************************************/
#include "refgen.h"
#include "rfalloc.h"
#include "contain.h"
#include "channel.h"
#include "video.h"
#include <math.h>

#define CAR(E,c) { if(empty(E))return(FAIL); else CARV(E,c) }
#define CARV(E,c)  { c = DATA((E).L); butfirst(E); }
#define CONTB(c,E) { headptr h=CREF00(c);\
          if((E.L=NEXT(h))==h) E.L=E.R=NOELEM; else E.R = PREV(h);}

#define NGRW 52
typedef enum {
 _VIDEO, _get, _set, _Get, _Set, _equ,
  _List, _Instance, _MakeInstance, _ToScreen,
 _A, _B, _C, _K, _L, _P, _Q,
 _Draw, _Fill, _Move, _To, _From,
 _Out, _Text, _Front, _Area,
 _Pixel, _Line_, _Rectangle_, _Ellipse_, _Polygon_,
 _Color, _BackColor, _Background,
 _ClipArea, _Base, _Current, _Mapping, _ActivePage, _VisualPage,
 _Shift, _Extension, _Rotation,
 _Function, _Palette, _DrawStyle, _FillStyle, _Font,
 _BIOS, _Xor, _On, _Off
 } grword;

static cvalue GRW[NGRW];

static char* GRWS[NGRW] = {
 "VIDEO", ".", ":", "Get" , "Set", "=",
 "List", "Instance", "MakeInstance", "ToScreen",
 "A", "B", "C", "K", "L", "P", "Q",
 "Draw", "Fill", "Move", "To", "From",
 "Out", "Text", "Front", "Area",
 "Pixel", "Line", "Rectangle", "Ellipse", "Polygon",
 "Color", "BackColor", "Background", 
 "ClipArea", "Base", "Current", "Mapping", "ActivePage", "VisualPage",
 "Shift", "Extension", "Rotation",
 "Function", "Palette", "DrawStyle", "FillStyle", "Font",
 "BIOS", "Xor", "On", "Off"
 };

int  c_video;
static ifunc p_video(headptr h, int m, addr * aa);










static LOGICAL VideoInit=FALSE;

void inivideo()
{ 
   if(NOT INIWORDS(NGRW,GRW,GRWS)) rf_err(ERCINIT);
   c_video = iregtype (p_video, "VIDEO", 0);
};

typedef enum { _ABS, _PAG, _BAS, _BGI } modpos;  /* 4 modes for pos */

static LOGICAL PCam2Pos (tvideo* aV, cpos cP, pos* aP, modpos mP)
{  long N = aV->Norm;
   long D = cP.Z/DFOCUS + N;
   cpos C; pos P;
   if (D==0) return(FAIL);
   C.X = cP.X / D; P.X = (intpos) C.X; if((long)(P.X) != C.X) return(FAIL);
   C.Y = cP.Y / D; P.Y = (intpos) C.Y; if((long)(P.Y) != C.Y) return(FAIL);
   if (mP != _BAS) {
      P.X += aV->Base.X;
      P.Y += aV->Base.Y;
      }
   if (mP == _BGI) {
      P.X -= aV->ClipArea.P.X;
      P.Y -= aV->ClipArea.P.Y;
      }
   if (mP == _ABS) P.Y += aV->ActivePage * StandVideo.ClipArea.V.Y;
   *aP = P;
   return(TRUE);
   };

static LOGICAL VCam2Vec (tvideo* aV, cvec cP, vec* aP)
{  long N = aV->Norm;
   long D = aV->Current.Z/DFOCUS + N;
   long dZ = cP.Z/DFOCUS;
   cpos C; pos P;
   if (D==0) return(FAIL);
   C.X = ((cP.X - (dZ * aV->Current.X) / D) / D);
   C.Y = ((cP.Y - (dZ * aV->Current.Y) / D) / D);
   P.X = (intpos) C.X; if((long)(P.X) != C.X) return(FAIL);
   P.Y = (intpos) C.Y; if((long)(P.Y) != C.Y) return(FAIL);
   *aP = P;
   return(TRUE);
   };

















static LOGICAL rf_get_func(unsigned func)
{  grword nw;
        if (func==VS_SET) nw = _Set;
   else if (func==VS_XOR) nw = _Xor;
   else return(FAIL);
   COPSYMB(GRW[nw]);
   return(TRUE);
}

static LOGICAL rf_get_pos_c (int m, cpos* ap)
{  int i;
   bl;
   COPSYMB(GRW[m]);
   ALL(i,3) if(NOT rf_mknumb( ((long*)ap)[i] )) return(FALSE);
   br;
   return(TRUE);
   };

static LOGICAL rf_get_pos(int m, pos* ap)
{  int i;
   bl;
   COPSYMB(GRW[m]);
   ALL(i,2) if(NOT rf_mknumb((long) ((intpos*)ap)[i] )) return(FALSE);
   br;
   return(TRUE);
   };

static LOGICAL rf_get_map(tvideo * av)
{  int k;
   bl;
   if(NOT rf_get_pos_c (_K, &(av->Origin))) return(FALSE);
   if(NOT rf_mknumb(av->Norm)) return(FALSE);
   ALL(k,av->Dim)
      if(NOT rf_get_pos_c (_L, &(av->Basis[k]))) return(FALSE);
   br;
   return(TRUE);
   };

static LOGICAL rf_get_rect(rect Rect)
{  bl;
   if(NOT rf_get_pos(_A, &(Rect.P))) return(FALSE);
   if(NOT rf_get_pos(_C, &(Rect.V))) return(FALSE);
   br;
   return(TRUE);
   };

static LOGICAL rf_get_palette(palette * aPalette)
{  int k;
   bl;
   ALL(k,aPalette->size) 
      if(NOT rf_mknumb((long)(aPalette->colors[k]))) return(FALSE);
   br;
   return(TRUE);
   };






static LOGICAL gr_get_color(cvalue vc, unsigned* acolor)
{
  if(IS_SNUMB(CTYPE(vc)) OR IS_CHAR(CTYPE(vc)))
        *acolor = (CVAL(vc));
  else {
     long N;
     if(rf_cgetnumb(vc,&N)!=TRUE) return(FAIL);
     *acolor = N;
     }
  return(TRUE);
  };



static LOGICAL gr_get_func(cvalue vc, unsigned* afunc)
{
   if (vc==GRW[_Set]) *afunc = VS_SET;
   else
   if (vc==GRW[_Xor]) *afunc = VS_XOR;
   else return(FAIL);
   return(TRUE);
}

static LOGICAL gr_get_onoff(cvalue vc, int* amode)
{
   if (vc==GRW[_On])  *amode = TRUE;
   else
   if (vc==GRW[_Off]) *amode = FALSE;
   else return(FAIL);
   return(TRUE);
}

typedef enum { _SCREEN, _CAMERA, _WORLD } spacemode;

static void add_cpos(cpos* ap, cpos* aq, long N)
{ 
  ap->X += N*aq->X;
  ap->Y += N*aq->Y;
  ap->Z += N*aq->Z;
};

static void muldiv_cpos(cpos* ap, long Fact, long Div)
{ long d;
  if(Fact%Div==0) { Fact /= Div; Div = 1; }
  if(Div%Fact==0) { Div /= Fact; Fact = 1; }
  d=Div/2;
  ap->X = (ap->X+(ap->X>=0?d:-d)) / Div * Fact;
  ap->Y = (ap->Y+(ap->Y>=0?d:-d)) / Div * Fact;
  ap->Z = (ap->Z+(ap->Z>=0?d:-d)) / Div * Fact;
};










static LOGICAL gr_get_pos_c (tvideo * av, cvalue cc, cpos* aP, povec PoVec)
{   cpos cr;   cvalue fc;
    tail t; Tgetn igetn; addr thi;
    headptr h; int k;
    LOGICAL res = TRUE;
    spacemode  map = _SCREEN;
    if (IS_SIMPL(CTYPE(cc))) return(FAIL);
  	h = CREF00(cc); /* cc IS-REF */
	if(IFUNC(h,_CONTAIN)==inull) return(FAIL);
	START(h);
	  EVAL(h,_SETPOS,Tsetpos,(THIS,&t,0));
	  igetn = (Tgetn) IFUNC(h,_GETN); thi = THIS;
      if(NOT igetn(thi,&t,&fc)) { res=FAIL; }
      cr.X = cr.Y = cr.Z = 0; 
           if(fc==GRW[_A]) { cr.X = -(av->Base.X)*(av->Norm);
                             cr.Y = -(av->Base.Y)*(av->Norm);
                             }
      else if(fc==GRW[_B]) { }
      else if(fc==GRW[_C]) { cr = av->Current; } 
      else if(fc==GRW[_K]) {                   map = _CAMERA; }
      else if(fc==GRW[_L]) { cr = av->Current; map = _CAMERA; }
      else if(fc==GRW[_P]) { cr = av->Origin;  map = _WORLD; }
      else if(fc==GRW[_Q]) { cr = av->Current; map = _WORLD; }
      else res = FAIL;
  	  if(res==TRUE) for (k=0; igetn(thi,&t,&cc); k++ ) {
         long N;
         if((res=rf_cgetnumb(cc,&N))!=TRUE) break;
              if (map==_WORLD AND k < (av->Dim))
                                         add_cpos(&cr,&(av->Basis[k]),N);
         else if(map==_CAMERA AND k < 3) ((long*)(&cr))[k] += N; 
         else if(map==_SCREEN AND k < 2) ((long*)(&cr))[k] += (av->Norm)*N; 
         else { res=FAIL; break; }
         }
    END(h);
    if (map==_SCREEN AND k!=2) res=FAIL;
    if (map==_CAMERA AND k!=3) res=FAIL;
    if(res==TRUE) {
       if(PoVec==_VECTOR) add_cpos(&cr, &(av->Current), -1L);
       else               av->Current = cr;
       *aP = cr;
       };
    return(res);
    };

static LOGICAL gr_get_pos(tvideo * av, cvalue cc, pos* ap, modpos mP)
{  cpos cP;
   if(gr_get_pos_c(av, cc, &cP, _POINT) != TRUE) return(FAIL);
   if(PCam2Pos(av,cP,ap,mP)!=TRUE) return(FAIL);
   return(TRUE);
   };

static LOGICAL gr_get_vec(tvideo * av, cvalue cc, vec* ap)
{  cpos cP;
   if(gr_get_pos_c(av,cc, &cP, _VECTOR) != TRUE) return(FAIL);
   if(VCam2Vec(av,cP,ap) != TRUE) return(FAIL);
   return(TRUE);
   };



static void rotate(cvec* A, cvec* B, int Angle)
{ int i;
  double pi = 3.1415926536,
         alfa = Angle * pi / 180,
         s = sin(alfa),
         c = cos(alfa);
         ALL(i,3) {
            double a = ((long*)A)[i], b = ((long*)B)[i];
            ((long*)A)[i] = floor(0.5+c*a+s*b);
            ((long*)B)[i] = floor(0.5-s*a+c*b);
            };
};

static LOGICAL gr_get_map(tvideo * av, cvalue cc)
{   long n,d;
    cpos o, m[MAXDIM], SavCur;
    cvalue nc,vc;
    tail t; Tgetn igetn; addr thi;
    headptr h; int k,i;
    LOGICAL res = FAIL;
    if (IS_SIMPL(CTYPE(cc))) return(FAIL);
  	h = CREF00(cc); /* cc IS-REF */
	if(IFUNC(h,_CONTAIN)==inull) return(FAIL);
	START(h);
	  EVAL(h,_SETPOS,Tsetpos,(THIS,&t,0));
	  igetn = (Tgetn) IFUNC(h,_GETN); thi = THIS;
      /* Save old Current */
      SavCur = av->Current;
      /* Get Origin */
      if(NOT igetn(thi,&t,&vc)) goto qfail;
		   if(vc==GRW[_Shift]) {  /* (Shift t.Pos) */
               if(NOT igetn(thi,&t,&vc)) goto qfail;
               if((res=gr_get_pos_c(av,vc,&o,_POINT))!=TRUE) goto qfail;
               av->Origin    = av->Current;
               }
	  else if(vc==GRW[_Extension]) {  /* (Extention s.Num s.Denom) */
               /* Get Num */
               if(NOT igetn(thi,&t,&nc)) goto qfail;
               if(rf_cgetnumb(nc,&n)!=TRUE) goto qfail;
               /* Get Denom */
               if(NOT igetn(thi,&t,&nc)) goto qfail;
			   if(rf_cgetnumb(nc,&d)!=TRUE) goto qfail;
			   ALL(i,av->Dim) {
                  muldiv_cpos(&(av->Basis[i]),n,1);
                  muldiv_cpos(&(av->Basis[i]),1,d);
               }  }
	  else if(vc==GRW[_Rotation]) {  /* (Rotation s1 s2 s.Angle) */
			   int num[3];
               /* Get i1, i2, angle */
               ALL(i,3) {
                 if(NOT igetn(thi,&t,&nc)) goto qfail;
				 if(rf_cgetnumb(nc,&n)!=TRUE) goto qfail;
				 num[i] = (int) n;
				 }
			   ALL(i,2) if(num[i]<=0 OR num[i] > av->Dim) goto qfail;
			   rotate(&(av->Basis[num[0]-1]),
					  &(av->Basis[num[1]-1]),
                      num[2]
                      );
               }
      else {
      if((res=gr_get_pos_c(av,vc,&o,_POINT))!=TRUE) goto qfail;
      /* Get Norm */
      if(NOT igetn(thi,&t,&nc)) goto qfail;
      if(rf_cgetnumb(nc,&n)!=TRUE) goto qfail;
      /* Get Basis */
  	  for (k=0; igetn(thi,&t,&vc); k++ ) {
         if(k>MAXDIM) goto qfail;
         if((res=gr_get_pos_c(av,vc,&(m[k]),_VECTOR))!=TRUE) goto qfail;
         muldiv_cpos(&(m[k]), 1L, av->Norm);
         };
      muldiv_cpos(&(av->Current), n, av->Norm);
      av->Origin    = av->Current;
      /* Restore old Current */
        muldiv_cpos(&SavCur,n,av->Norm);
      av->Norm=n;
      av->Dim=k;
      ALL(i,k) av->Basis[i]=m[i];
      }
      av->Current = SavCur;
      res=TRUE;
qfail:
    END(h);
    return(res);
    };

static void end_font(font * aFont)
{  if(aFont->started) {
      if(aFont->cfont!=NOCVALUE)
         {  headptr h=CREF00(aFont->cfont);  END(h);  }
      aFont->started = FALSE;
}     }

static void start_font(font * aFont)
{   if(NOT aFont->started) {
          if(aFont->cfont!=NOCVALUE) {
             headptr hfnt=CREF00(aFont->cfont);
             START(hfnt);
             SetFont(SEGINF(AD(hfnt)),aFont->width,aFont->height);
             }
          else SetBiosFont();
          aFont->started = TRUE;
}         }

















static LOGICAL gr_get_font(cvalue vc, font* aFont)
 { expr E;  cvalue fc;
   headptr h;
   end_font(aFont);
   cvalfree(aFont->cfont);
   *aFont = StandVideo.Font;
   if(vc==GRW[_BIOS]) ;
   else {
      if(IS_SIMPL(CTYPE(vc))) return(FAIL);
      h = CREF00(vc);
      if(MODE(TYPE(h))!=CHAIN) return(FAIL);
      E.L = NEXT(h); E.R = PREV(h);
      if(E.L==h) return(FAIL);
      CAR(E,fc);
      if(IS_SIMPL(CTYPE(fc)) OR TYPE(h=CREF00(fc))!=HEADSTR) return(FAIL);
      ++VAL(h);
      aFont->cfont = fc;
      CAR(E,fc);
      if(NOT IS_SNUMB(CTYPE(fc))) return(FAIL);
      aFont->width = CVAL(fc);
      CAR(E,fc);
      if(NOT IS_SNUMB(CTYPE(fc))) return(FAIL);
      aFont->height = CVAL(fc);
      }
   return(TRUE);
   }

static LOGICAL gr_get_rect(tvideo * av, cvalue cc, rect * aRect)
{   cvalue vc;
    tail t; Tgetn igetn; addr thi;
	headptr h;
    LOGICAL res = FAIL;
    if (IS_SIMPL(CTYPE(cc))) return(FAIL);
  	h = CREF00(cc); /* cc IS-REF */
	if(IFUNC(h,_CONTAIN)==inull) return(FAIL);
	START(h);
	  EVAL(h,_SETPOS,Tsetpos,(THIS,&t,0));
	  igetn = (Tgetn) IFUNC(h,_GETN); thi = THIS;
      /* Get Point */
      if(NOT igetn(thi,&t,&vc)) goto qfail;
      if((res=gr_get_pos(av,vc,&(aRect->P),_PAG))!=TRUE) goto qfail;
      /* Get Vector */
      if(NOT igetn(thi,&t,&vc)) goto qfail;
      if((res=gr_get_vec(av,vc,&(aRect->V)))!=TRUE) goto qfail;
      res=TRUE;
qfail:
    END(h);
    return(res);
    };

static void intersegment(intpos* ap1, intpos* av1, intpos p2, intpos v2)
{   intpos q1, q2;
	if (*av1<0) { *ap1 += *av1; *av1 = -*av1; };
    q1 = *ap1 + *av1;           q2 = p2 + v2;
    if(*ap1<p2) *ap1=p2;        if(q1>q2) q1=q2;
    *av1 = q1-*ap1;             if(*av1<0) *av1=0;
    };



static LOGICAL gr_get_palette(cvalue cc, palette * aPalette)
{   cvalue vc; unsigned Col;
    tail t; Tgetn igetn; addr thi;
    headptr h; int k;
    LOGICAL res = FAIL;
    if (IS_SIMPL(CTYPE(cc))) return(FAIL);
  	h = CREF00(cc); /* cc IS-REF */
	if(IFUNC(h,_CONTAIN)==inull) return(FAIL);
	START(h);
	  EVAL(h,_SETPOS,Tsetpos,(THIS,&t,0));
	  igetn = (Tgetn) IFUNC(h,_GETN); thi = THIS;
      /* Get Palette */
  	  for (k=0; k<aPalette->size AND igetn(thi,&t,&vc); k++ ) {
         if((res=gr_get_color(vc,&Col))!=TRUE) goto qfail;
         aPalette->colors[k] = Col;
         };
      res=TRUE;
qfail:
    END(h);
    return(res);
    };

static LOGICAL video_reset(tvideo * av, int w)
{ if (VideoInit) 
   switch(w) {       case 0:
     case _Color:      SETCOLOR(av);
     case _BackColor:  SETTEXTCOLOR(av); if(w) break;
     case _Background: if(av->Background) BackgroundOn ();
                        else               BackgroundOff();
                        if(w) break;
     case _ClipArea:   setviewport(av->ClipArea.P.X,  av->ClipArea.P.Y,
                                    av->ClipArea.P.X+av->ClipArea.V.X-1,
                                    av->ClipArea.P.Y+av->ClipArea.V.Y-1,
                                    1);
                        if(w) break;
     case _Function:   SETDRAWFUNC(av);  if(w) break;
     case _Palette:    if(w) { setallpalette(&(av->Palette)); break; }
     case _ActivePage: setactivepage(av->ActivePage); if(w) break;
     case _VisualPage: setvisualpage(av->VisualPage); if(w) break;
     }
  return(TRUE);
}

static LOGICAL video_set(tvideo * av, cvalue nc, cvalue vc)
{
    if(IS_BRAC(CTYPE(nc))) {
       if(NOT IS_BRAC(CTYPE(vc))) return(FAIL);
       else {
          LOGICAL res=TRUE;
          headptr h1 = CREF(nc);
          elemptr p1 = NEXT(h1);
          headptr h2 = CREF(vc);
          elemptr p2 = NEXT(h2);
          while(p1!=h1) {
            if(p2==h2) return(FAIL);
            if(res==TRUE) res = video_set(av,DATA(p1),DATA(p2));
            p1 = NEXT(p1); p2 = NEXT(p2);
            }
          return(res);
       }  }
    else if(nc==GRW[_Current])
      { cpos p; if(gr_get_pos_c(av,vc,&p,_POINT)!=TRUE) return(FAIL); }
    else if(nc==GRW[_ClipArea]) { rect Rect;
        if(gr_get_rect(av,vc,&Rect)!=TRUE) return(FAIL);
        intersegment(&(Rect.P.X),&(Rect.V.X),
                      av->ClipArea.P.X,av->ClipArea.V.X);
        intersegment(&(Rect.P.Y),&(Rect.V.Y),
                      av->ClipArea.P.Y,av->ClipArea.V.Y);
        av->ClipArea = Rect;
        video_reset(av,_ClipArea);
        }
    else if(nc==GRW[_Base]) {
        if(gr_get_pos(av,vc,&(av->Base),_PAG)!=TRUE) return(FAIL);
        }
    else if(nc==GRW[_Mapping])
      { if(gr_get_map(av,vc)!=TRUE) return(FAIL); }
    else if(nc==GRW[_Pixel]) {
        pos cur;
        if(gr_get_color(vc,&(av->Color))!=TRUE) return(FAIL);
        if(PCam2Pos(av,av->Current, &cur, _BGI)!=TRUE) return(FAIL);
        if(VideoInit) PutPixel(av, cur.X, cur.Y);
        }
    else if(nc==GRW[_Color]) {
        if(gr_get_color(vc,&(av->Color))!=TRUE) return(FAIL);
        video_reset(av,_Color);
        }
    else if(nc==GRW[_Palette]) {
        if(gr_get_palette(vc,&(av->Palette))!=TRUE) return(FAIL);
        video_reset(av,_Palette);
        }
    else if(nc==GRW[_BackColor]) {
        if(gr_get_color(vc,&(av->BackColor))!=TRUE) return(FAIL);
        video_reset(av,_BackColor);
        }
    else if(nc==GRW[_Background]) {        /* #TXT */
        if(gr_get_onoff(vc,&(av->Background))!=TRUE) return(FAIL);
        video_reset(av,_Background);
        }
    else if(nc==GRW[_Font]) {
        if(gr_get_font(vc, &(av->Font))!=TRUE) return(FAIL);
        }
    else if(nc==GRW[_DrawStyle]);
    else if(nc==GRW[_FillStyle]);
    else if(nc==GRW[_Function]) {
        if(gr_get_func(vc,&(av->Function))!=TRUE) return(FAIL);
        }
    else if(nc==GRW[_ActivePage]) {
        if(IS_SNUMB(CTYPE(vc))) av->ActivePage = CVAL(vc); else return(FAIL);
        video_reset(av,_ActivePage);
        }
    else if(nc==GRW[_VisualPage]) {
        if(IS_SNUMB(CTYPE(vc))) av->VisualPage = CVAL(vc); else return(FAIL);
        video_reset(av,_VisualPage);
        }
    else return(FAIL);
    return(TRUE);
}



static LOGICAL video_get_list(tvideo * av, expr eM) /* -> free store */;

static LOGICAL video_get(tvideo * av, cvalue nc) /* -> free store */
{ pos cur;
  long v=0L;
     if(IS_BRAC(CTYPE(nc))) { expr eM;
          CONTB(nc,eM);
          return video_get_list(av,eM);
          }
     if(nc==GRW[_Current]) { return rf_get_pos_c(_K, &(av->Current)); }
else if(nc==GRW[_ClipArea]) { return rf_get_rect(av->ClipArea); }
else if(nc==GRW[_Base]) { return rf_get_pos(_A, &(av->Base)); }
else if(nc==GRW[_Mapping]) { return rf_get_map(av); }
else if(nc==GRW[_Pixel]) {
             if(PCam2Pos(av,av->Current, &cur, _BGI)!=TRUE) return(FAIL);
             if(VideoInit) v=GetPixel(cur.X,cur.Y); else v=0;
             }
else if(nc==GRW[_Color])    v=av->Color;
else if(nc==GRW[_BackColor]) v=(av->BackColor);
else if(nc==GRW[_Background]) {
     COPSYMB(GRW[(av->Background?_On:_Off)]);
     return(TRUE);
     }
else if(nc==GRW[_Palette]) { return rf_get_palette(&(av->Palette)); }
else if(nc==GRW[_Font]) {
    if (av->Font.cfont==NOCVALUE) { COPSYMB(GRW[_BIOS]) }
    else {
         bl;
         COPSYMB(av->Font.cfont);
         if(NOT rf_mknumb((long)av->Font.width )) return(FALSE);
         if(NOT rf_mknumb((long)av->Font.height)) return(FALSE);
         br;
         }
    return(TRUE);
    }
else if(nc==GRW[_DrawStyle]);
else if(nc==GRW[_FillStyle]);
else if(nc==GRW[_Function]) return rf_get_func(av->Function);
else if(nc==GRW[_ActivePage]) v=av->ActivePage;
else if(nc==GRW[_VisualPage]) v=av->VisualPage;
else return(FAIL);
     return rf_mknumb(v);
     }

static LOGICAL video_get_list(tvideo * av, expr eM) /* -> free store */
{ LOGICAL res;
  cvalue nc;
  while(NOT empty(eM)) {
     CARV(eM,nc);
     if((res=video_get(av,nc))!=TRUE) return(res);
     }
  return(TRUE);
}







#define square(p,q) ((long)(p.X)*(long)(q.Y)-(long)(p.Y)*(long)(q.X))

static LOGICAL draw(tvideo * av, grword mode, expr eM)
{ cvalue fc,vc; int res; pos p,q,v,w; int a,b;
  LOGICAL Front = FALSE;
while(TRUE) {
  CAR(eM,fc);
  if(fc==GRW[_Line_]) {
    CAR(eM,vc);
    if((res=gr_get_pos(av,vc,&p,_BGI))!=TRUE) return res;
    for( ; NOT empty(eM) ;) {
       CARV(eM,vc)
       if((res=gr_get_pos(av,vc,&q,_BGI))!=TRUE) return res;
       if(VideoInit) Line(av, p.X, p.Y, q.X, q.Y);
       p = q;
    }  }
  else if(fc==GRW[_Rectangle_]) {
    CAR(eM,vc);
    if((res=gr_get_pos(av,vc,&p,_BGI))!=TRUE) return(res);
    CAR(eM,vc);
    if(NOT empty(eM)) return(FAIL);
    if((res=gr_get_vec(av,vc,&v))!=TRUE) return(res);
    if (v.X!=0 AND v.Y!=0) {
	  q.X = p.X + v.X; /* if (v.X>0) q.X--; else p.X--; */
	  q.Y = p.Y + v.Y; /* if (v.Y>0) q.Y--; else p.Y--; */
      if(p.X<0) p.X=0; if(p.Y<0) p.Y=0; /* patch for Turbo C 2.0 */
      if(VideoInit) {
        if(mode==_Draw) Rectangle(av, p.X, p.Y, q.X, q.Y);
        else           FRectangle(av, p.X, p.Y, q.X, q.Y);
    } } }
  else if(fc==GRW[_Pixel])
    while(NOT empty(eM)) {
       CARV(eM,vc);
       if((res=gr_get_pos(av,vc,&p,_BGI))!=TRUE) return res;
       if(VideoInit) PutPixel(av, p.X, p.Y);
       }
  else if(fc==GRW[_Ellipse_]) {
    CAR(eM,vc);
    if((res=gr_get_pos(av,vc,&p,_BGI))!=TRUE) return(res);
    CARV(eM,vc)
    if((res=gr_get_vec(av,vc,&v))!=TRUE) return res;
    CARV(eM,vc)
    if((res=gr_get_vec(av,vc,&w))!=TRUE) return res;
    if(NOT empty(eM)) return(FAIL);
    a = v.X; if(a<0) a = -a;
    b = w.Y; if(b<0) b = -b;
    if(VideoInit) if(NOT Front OR square(v,w)>0)
       if(mode==_Draw)   Ellipse(av, p.X, p.Y, a,b);
       else             FEllipse(av, p.X, p.Y, a,b);
    }










  else if(fc==GRW[_Polygon_]) {
#define MAXPOINTS 6
        int k,l,l1; adr a=anull; pos * ap; expr E; long S;
        pos P[MAXPOINTS];
        cpos cp;
        if(empty(eM)) return(TRUE);
        /* Compute the number of points trying to fit MAXPOINTS */
        cp = av->Current;
        for(l=0,E=eM; NOT empty(E); l++) {
           CARV(E,vc);
           if((res=gr_get_pos(av,vc,&(P[l%MAXPOINTS]),_BGI))!=TRUE) return res;
           }
        if(l>=MAXPOINTS) {
		   if(REALLOC(a,(l+1)*sizeof(pos))==NOADR) return(FALSE);
           ap = (pos*) SEGINF(a);
           av->Current = cp; /* restore Current */
           ALL(k,l) {
               CAR(eM,vc);
               if((res=gr_get_pos(av,vc,&q,_BGI))!=TRUE) return(FALSE);
               ap[k].X = q.X;
               ap[k].Y = q.Y;
               }
           }
        else ap = P;
        ap[l] = ap[0];
        l1 = l+1;
        S=0;
        if(Front) ALL(k,l) S+=square(ap[k],ap[k+1]);
        if(VideoInit) if(S>=0) {
           LinkPoints(l1,ap);
           if(mode==_Draw)   Polygon(av, l1, ap);
           else             FPolygon(av, l1, ap);
           }
        REALLOC(a,0);
        }
  else if(fc==GRW[_Area]) { unsigned BdrClr;
    CAR(eM,vc);
    if((res=gr_get_pos(av,vc,&p,_BGI))!=TRUE) return(res);
    CAR(eM,vc);
    if(gr_get_color(vc,&BdrClr)!=TRUE) return(FAIL);
    if(VideoInit) floodfill(p.X,p.Y,BdrClr);
    }
  else if(fc==GRW[_Front]) { Front = TRUE; continue; }
  else return(FAIL);
  return(TRUE);
} }














static int iputc(int c, tvideo* av)
{
 LOGICAL Put = av->LineVisible; /* Condition for clipping */
 Put &= av->TextCur.X >= av->ClipArea.P.X;
    av->TextCur.X+=(av->Font.width);
 Put &= av->TextCur.X <= av->ClipArea.P.X+av->ClipArea.V.X;
    if(Put) OutChar((char)c);
    else SetPos(av->TextCur.X,av->TextCur.Y);
    return(c);
    }

static LOGICAL settextpos(tvideo * av)
{ int TextY;
 if(PCam2Pos(av,av->Current,&(av->TextCur),_ABS)!=TRUE) return(FAIL);
 TextY = av->TextCur.Y - av->ActivePage * StandVideo.ClipArea.V.Y;
 av->LineVisible = VideoInit AND (TextY >= av->ClipArea.P.Y) AND
         (TextY+(av->Font.height) <= av->ClipArea.P.Y+av->ClipArea.V.Y);
 SetPos(av->TextCur.X, av->TextCur.Y);
 av->TextSet=TRUE;
 if(av->LineVisible) {
   SetManualRegsProgramming();
   ProgramControllerRegisters();
   start_font(&(av->Font));
   }
 return(TRUE);
}

static LOGICAL unsettextpos(tvideo * av)
{long N = av->Norm;
 long D = av->Current.Z/DFOCUS + N;
 pos cur;
 if(PCam2Pos(av,av->Current,&cur,_ABS)!=TRUE) return(FAIL);
 av->Current.X+=(av->TextCur.X-cur.X)*D;
 av->Current.Y+=(av->TextCur.Y-cur.Y)*D;
 av->TextSet=FALSE;
 if(av->LineVisible) RestoreControllerRegisters();
 return(TRUE);
}

static LOGICAL out(tvideo* av, expr eM)
{ cvalue mc, tc;
  elemptr savb = b;
  LOGICAL res = TRUE;
  if(settextpos(av)!=TRUE) return(FAIL);
  CAR(eM,mc);
  if(mc==GRW[_Text]) {
    while (NOT empty(eM)) {
      CAR(eM,tc);
      if((res=c_getvr(tc,FALSE))!=TRUE) return(res);
      if(b!=savb) rf_outm((FILE*)av,iputc,NEXT(savb),b,FALSE,sysid);
      b=savb;
      };
    }
  else return(FAIL);
  if(unsettextpos(av)!=TRUE) return(FAIL);
  return(res);
}



static LOGICAL move(tvideo* av, expr eM)
{ cvalue vc,dc;
  headptr h;
  elemptr r = eM.R;
  LOGICAL res,string;
  pos p,q; vec v;
    CAR(eM,vc);
    if(vc!=GRW[_Rectangle_]) return(FAIL);
    CAR(eM,vc);
    /* Check, whether last term is a string */
    string = (IS_REF(TYPE(r)) AND TYPE(h=REF00(r))==HEADSTR);
    if((res=gr_get_pos(av,vc,&p,(string? _BGI:_ABS)))!=TRUE) return(res);
    CAR(eM,vc);
    if((res=gr_get_vec(av,vc,&v))!=TRUE) return(res);
    if (v.X!=0 AND v.Y!=0) {
      q.X = p.X + v.X; if (v.X>0) q.X--; else p.X--;
      q.Y = p.Y + v.Y; if (v.Y>0) q.Y--; else p.Y--;
      CAR(eM,dc);
      CAR(eM,vc);
      if(NOT empty(eM)) return(FAIL);
      /* Get image to or from the buffer */
      if (string) {
         START(h);
              if(dc==GRW[_To]) {
                 if(p.X<0 OR p.Y<0 OR q.X<0 OR q.Y<0) res=FAIL;
                 else if(VideoInit) {
                    unsigned size = imagesize(p.X, p.Y, q.X, q.Y);
				    if(REALLOC(AD(h),size)==NOADR) res=FAIL;
                    else getimage(p.X, p.Y, q.X, q.Y, SEGINF(AD(h)));
                 }  }
         else if(dc==GRW[_From]) {
            if(VideoInit)
               putimage(p.X, p.Y, SEGINF(AD(h)), PutImageOp[av->Function]);
            }
         else res=FAIL;
         END(h);
         return res;
         }
      /* Move rectangle along the screen */
      if((res=gr_get_vec(av,vc,&v))!=TRUE) return(res);
           if(dc==GRW[_To])
              { if(VideoInit) MoveRectangle(p.X, p.Y, q.X, q.Y, v.X, v.Y);}
      else if(dc==GRW[_From])
              { if(VideoInit)  MoveRectangle(
                          p.X+v.X, p.Y+v.Y, q.X+v.X, q.Y+v.Y, -v.X, -v.Y);}
      else return FAIL;
      }
  return TRUE;
}











static LOGICAL toscreen(tvideo* av, expr eM)
{
  cvalue mc,vc;
  LOGICAL res;
  pos p;
    CAR(eM,mc);
    for ( ; NOT empty(eM) ; ) {
       CARV(eM,vc);
       if (mc==GRW[_A]) {
            if((res=gr_get_pos(av,vc,&p,_ABS))!=TRUE) return(res);
            if(NOT rf_get_pos(_A, &p)) return(FALSE);
	    }
       else if (mc==GRW[_B]) {
            if((res=gr_get_pos(av,vc,&p,_BAS))!=TRUE) return(res);
            if(NOT rf_get_pos(_B, &p)) return(FALSE);
	    }
       else if (mc==GRW[_C]) {
            if((res=gr_get_vec(av,vc,&p))!=TRUE) return(res);
            if(NOT rf_get_pos(_C, &p)) return(FALSE);
	    }
       else return FAIL;
       }
    return TRUE;
  };

static LOGICAL comnd(tvideo* av, expr eM);

static LOGICAL comnd_list(tvideo* av, expr eM)
{   elemptr h;
    cvalue tc;
    expr eC;
    LOGICAL res=TRUE;
 while (NOT empty(eM)) {
    CAR(eM,tc);
    if (IS_SIMPL(CTYPE(tc))) return(FAIL);
  	h = CREF00(tc); /* cc IS-REF */
	if(MODE(TYPE(h)) != CHAIN) return(FAIL);
    eC.L = NEXT(h);
    if(eC.L == h) return(FAIL);
    eC.R = PREV(h);
    res = comnd(av,eC);
    if(res!=TRUE) return(res);
    };
  return(res);
};

static LOGICAL comnd_inst(tvideo LocalVideo, expr eM)
{ LOGICAL res;
   cvalact(LocalVideo.Font.cfont);
   res = comnd_list(&LocalVideo, eM);
   end_font(&(LocalVideo.Font));
   cvalfree(LocalVideo.Font.cfont);
   return(res);
   }






static LOGICAL comnd(tvideo* av, expr eM)
{ cvalue mc, nc, vc;
  LOGICAL res = TRUE;
  CAR(eM,mc);
  if(mc==GRW[_get] OR mc==GRW[_Get]) return video_get_list(av,eM);
  else if(mc==GRW[_set] OR mc==GRW[_Set]) {
       CAR(eM,nc);
       CAR(eM,vc);
       if(vc==GRW[_equ]) CAR(eM,vc) else return(FAIL);
       if(NOT empty(eM)) return(FAIL);
       return video_set(av,nc,vc);
       }
  else if(mc==GRW[_Draw]) res = draw(av, _Draw, eM);
  else if(mc==GRW[_Fill]) res = draw(av, _Fill, eM) ;
  else if(mc==GRW[_Move]) res = move(av, eM);
  else if(mc==GRW[_Out])  res = out(av, eM);
  else if(mc==GRW[_List]) res = comnd_list(av,eM);
  else if(mc==GRW[_ToScreen]) res = toscreen(av,eM);
  else if(mc==GRW[_Instance]) {
       end_font(&(av->Font));
       res=comnd_inst(*av,eM);
       video_reset(av,0);
       }
  else if(mc==GRW[_MakeInstance]) {
	 headptr rh; cvalue rc;
     end_font(&(av->Font));
     CAR(eM,rc)
     if(NOT empty(eM)) return(FAIL);
     if (IS_SIMPL(CTYPE(rc))) return(FAIL);
     rh = CREF00(rc);
	 if(NOT rf_get_empty(rh,HD(c_video,OBJ))) return(FAIL);
     START(rh);
     * (tvideo*) SEGINF(AD(rh)) = *av;
     cvalact(av->Font.cfont);
     END(rh);
/*   COPSYMB(rc); */
	 }
  else return(FAIL);
  return(res);
}

LOGICAL isend (addr thi, expr eM)
{ headptr h=PFROMFAR(thi);
  tvideo * av;
  if(SEGLEN(AD(h))!=sizeof(tvideo)) return(FALSE);
  av = (tvideo*) SEGINF(AD(h));
  video_reset(av,0);
  return comnd(av,eM);
  }

static LOGICAL icopy(addr thi, headptr from)
{  tvideo * av;
   headptr h = PFROMFAR(thi);
   if(NOT icopy_alloc(thi,from)) return(FALSE);
   cvalact( (av=(tvideo*)SEGINF(AD(h))) -> Font.cfont );
   av->Started = FALSE;
   return(TRUE);
   }


ifunc p_video(headptr h, int m, addr * aa)
{  tvideo * av;
   *aa = PTOFAR(h);
   switch(m) {
     case _SEND:  return((ifunc)isend);
     case _PUTC: {
       if(SEGLEN(AD(h))!=sizeof(tvideo)) return(inull);
       av = (tvideo*) SEGINF(AD(h));
       video_reset(av,0);
       if(settextpos(av)!=TRUE) return(inull);
       *aa = (addr) av;
       return((ifunc)iputc);
       }
	 case _VIDEO:
     case _CHANNEL: return(itrue);
     case _NEW:
       AD(h)=anull;
       if (REALLOC(AD(h),sizeof(tvideo))==NOADR) return(inull);
       av = (tvideo*) SEGINF(AD(h));
       *av = StandVideo;
       END(h);
       return(itrue);
     case _COPY:   return((ifunc)icopy);
     case _DONE:
       LOCK(h);
       if(SEGLEN(AD(h))==sizeof(tvideo))
          cvalfree(((tvideo*) SEGINF(AD(h)))->Font.cfont);
       UNLOCK(h);
       break;
     case _END:
       if(SEGLEN(AD(h))!=sizeof(tvideo)) return(inull);
       av = (tvideo*) SEGINF(AD(h));
       end_font(&(av->Font));
       if(av->TextSet) if(unsettextpos(av)!=TRUE) return(inull);
       if(VideoInit) if(av->Started) ED_DisplayPointer();
       av->Started=FALSE;
       break;
     case _START:
       if(p_alloc(h,m,aa)==inull) return(inull);
       if(SEGLEN(AD(h))!=sizeof(tvideo)) return(inull);
       av = (tvideo*) SEGINF(AD(h));
       av->Started=TRUE;
       if(VideoInit) ED_HidePointer();
       return(itrue);
     }
   return(p_alloc(h,m,aa));
}













static LOGICAL rb_graph(LOGICAL initx)
{  int res=0;
   elemptr EargL, EargR;
   elemptr *AeresL, *AeresR;
   popae(AeresL,AeresR);
   pope(EargL,EargR);
   if (EargL != NOELEM AND EargR != NOELEM) return FAIL;
   if(initx) {
      InitGraph("",res);
      VideoInit = TRUE;
      if(res!=0) rf_err(ERCINIT);
      StandVideo.ClipArea.V.X = getmaxx()+1;
      StandVideo.ClipArea.V.Y = getmaxy()+1;
      StandVideo.Font.cfont = NOCVALUE;
      getpalette(&StandVideo.Palette);
      ED_Init();
      }
   else if(VideoInit) { CloseGraph; ED_Done(); };
   *AeresL=*AeresR=NOELEM;
   return (TRUE);
}

LOGICAL rb_initgraph()  { return rb_graph(TRUE ); }
LOGICAL rb_closegraph() { return rb_graph(FALSE); }
