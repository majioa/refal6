// Copyright (C) 2005  by Piotr He³ka (piotr.helka@nd.e-wro.pl)
// Linux C++ (not full) implementation of Borland's conio.h 
// v 1.01
// It uses Ncurses lib, so accept also its terms.



// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

// ----------------------------- krotki opis ------------------

// Biblioteka obs³uguje mniej lub bardziej zgodnie:
//
// cgets()
// cputs()
// clreol()
// clrscr()
// cprintf()
// cscanf()
// getch() (chyba nie wszystkie kody tak jak w conio.h)
// getche()  
// gotoxy()
// kbhit()
// putch()
// textbackground()
// textcolor()
// wherex()
// wherey()
// window()
//
// kompatbyilno¶æ w kierunku Linux CONIO.H -> DOS CONIO.H
// bedzie zachowana

// Aby skompilowac
// $g++ nazwa_progsa.cpp -lncurses -o nazwa_progsa.o

// ------------------------------- define ---------------------

#ifndef __NCURSES_H
#include <ncurses.h>
#endif

#ifndef __CONIO_H
#define __CONIO_H

#define MAX_OKIEN 256

#define BLACK       0
#define RED         1
#define GREEN       2
#define BROWN       3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define LIGHTGRAY   7
#define DARKGRAY    0
#define LIGHTRED    1
#define LIGHTGREEN  2
#define YELLOW      3
#define LIGHTBLUE   4
#define PINK        5
#define LIGHTCYAN   6
#define WHITE       7

// -------------------------------- globalne ------------------

//int (* wsk_f)(void) = getch;

#undef getch
#define getch CURSgetch

#undef getche
#define getche CURSgetche


typedef struct
{
	int 	xup;
	int 	yup;
	int 	xdown;
	int 	ydown;
	WINDOW*	okno;
} Okno;

#ifndef CONIO_SELF
extern bool	zainicjowane; // = FALSE; //czy juz po initscr() ?
extern int	znakSpecjalny; // = -1; //potrzebne do getch'a
extern int	n; // = 0; //liczba uzytych okienek

extern short	kolorTekstu; // = COLOR_WHITE;
extern short	kolorTla; // = COLOR_BLACK;
extern short	biezacaPara;

extern Okno	okienka[MAX_OKIEN];	//tablica struktur aktywnych okienek
extern WINDOW*	aktywneOkno; // = NULL;	//wsk na aktywne okno

extern void inicjuj();
extern int simple_strlen(char* str);
extern void cputs(char* str);
extern char* cgets(char* str);
extern void clreol();
extern void clrscr();
extern int cprintf(char *fmt, ...);
extern int cscanf(char *fmt, ...);
extern int CURSgetch();
extern int CURSgetche();
extern int gotoxy(int x, int y);
extern int kbhit();
extern int putch(int znak);
extern void textbackground(short kolor);
extern void textcolor(short kolor);
extern int wherex(void);
extern int wherey(void);
extern void window(int xup, int yup, int xdown, int ydown);

#endif
#endif
