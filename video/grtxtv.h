/*=========================================  (c)  mkSoft  1991, 1992  =====*/
/*                                                                         */
/*      Header-файл для GrTxtV                                             */
/*                                                                         */
/*=========================================================================*/


typedef  struct  {
    unsigned   int   modes;
    void       far * font_ptr;
    unsigned  char   char_wdth, char_hight;
    unsigned  char   char_wdth_bytes;
    unsigned  char   char_wdth_bits;
    unsigned  char   char_wdth_mask;
    unsigned   int   char_size;
    unsigned   int   cur_pos;
    unsigned  char   cur_shift;
    unsigned  char   foreground, background;
  }                                           grtxtv_config;


extern  void far pascal  InitGrTxtV  ( void );

extern  void far pascal  GetTextOutputConfig  ( grtxtv_config far *  conf );
extern  void far pascal  SetTextOutputConfig  ( grtxtv_config far *  conf );

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

extern  void far pascal  SetPos  (  int  x,
                                    int  y  );

extern  void far pascal  OutChar  (  char  c  );

extern  void far pascal  OutString  (  unsigned int  l,
                                         char far *  s  );

extern  void far pascal  DrawRectangle  (  int  x1,
                                       int  y1,
                                       int  x2,
                                       int  y2  );

extern  void far pascal  DrawRectangleW  (  int  x1,
                                        int  y1,
                                        int  x2,
                                        int  y2,
                               unsigned int  wdth  );

extern  void far pascal  FillRectangle  (  int  x1,
                                           int  y1,
                                           int  x2,
                                           int  y2  );

extern  void far pascal  MoveRectangle  (  int  x1,
                                           int  y1,
                                           int  x2,
                                           int  y2,
                                           int  vx,
                                           int  vy  );

/*                                                                         */
/*.........................................................................*/
