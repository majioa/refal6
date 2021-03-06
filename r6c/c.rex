START "rcmain.rex"
<BlockBegin SYSINFO MODTABLE Lineno Ftable Rest Modname Typename Extern Errfl Inifls Getdef Testdef Extdef Use Def ERR ERR0 Flag Setflags CompFile CompMem SWAP GO COMP comp CL cl Swap SYSARG GET SHIFT GETV EXIT UPPER LOWER SETV MK_TABLE GETENV GETS SUB SYMB WRITELN! TABLE_VALUE TIME_ELAPSED MODE LOAD CLOSE_CHANNEL ERASE_FILE OPEN_FILE STDOUT PRINTLN! CONST>
*GO=F45(P k *PROUT i "Refal-6 Compiler. Copyright (C) 1993 by Ark. Klimov" lB k *COMP k *GETV k *SHIFT k *GET i *SYSARG lB jB 0 -2 lB lB lB oA s k *Flag i 'M' lB oB s u 6 A O SLJ P p K P k *EXIT lB p)
*COMP=F9(O k *Init k *Regs gE lB lB p)
*comp=F6(O k *COMP gE lB p)
*CL=F8(O k *COMP gE i "/M" lB p)
*cl=F6(O k *CL gE lB p)
*Regs=F102(u 64 P i () k *PROUT lB k *PROUT i "Command line syntax:" lB k *PROUT i "   rf c [switches] file1 file2 ..." lB k *PROUT i "switch /L - make a listfile FILEi.RFL" lB k *PROUT i "switch /M - compile to memory" lB k *PROUT i "switch /U - uppercase words in source text" lB k *PROUT i "switch /5 - no special meaning for BackSlash in strings" lB k *PROUT jB "switch /W or /N or /S - " "make code in word or number or symbol format" lB k *PROUT i "switch /V or /C - make code for vector or chain format" lB k *PROUT i "switch /Xxxx - output file with extension .xxx" lB k *PROUT i " SET RESTYPE = XXX redefines extension of result: FILEi.XXX" lB k *PROUT i "switch /Yyy  - use compiler module RSSYNTyy.RSW for parsing" lB p u 31 U W K O k *GETV dF lA oA C '/' G O k *UPPER dN lA oB G P a dU eP b k *Regs gE gH lB p O a gE b p)
*Init=F14(B O SEE O k *Setflags gG lB k *Trfiles TD gI lB p)
*Trfiles=F18(u 15 K O k *Trfile k *GETV dD lB lB k *Trfiles gF lB p P p)
*Trfile=F34(O k *Opinp k *Mkext k *LOWER eE lB i () lB lB k *Inifls lB oA s u 7 U W C F O p O k *Compmod k *Parse lB lB p)
*Mkext=F46(B u 9 D '.' O SEE O gH dD p u 25 H O SEE O k *Oneof dF i ('/\\') lA oA C F P k *Mkext gH a dF TD gJ b lB p O SEE O gG TD gI i ('ref') p)
*Opinp=F73(B O SEE O k *OPEN jB 'r' 12 eG i '.' eI lA oA u 42 C T P k *SETV i *Modname eG lA k *SETV i *Typename eI lA k *PROUT eG i '.' eI i ':' lA oB O k *Oplst k *Flag i 'L' lB gG jD '.rfl' lB p C F P dN k *ERR0 i "Unable to open " gG i '.' TD gI lB p)
*Oplst=F41(u 33 A P SDB O k *OPEN jB 'w' 5 eG lA oA u 8 C T P a gG b p C F P dL k *ERR0 i "Unable to open " gG lB p K O k *ERASE gF lB p)
*Parse=F18(P k *CLOAD i *Getdef k *SyntModName lB lB oA s k *Ptime jB "Parsing time = " *Syntax lB p)
*SyntModName=F33(P k *Flag i 'Y' lB oA s u 7 C F P i rssynt p A O SHF P k *SETV jG X 'rssynt' eK lB p)
*Syntax=F37(P k *SETV jB *Lineno 0 lA k *MK_TABLE i *Ftable lA k *SETV i *Extern lA k *SETV i *Errfl lA k *SETV i *Rest lA oA k *GetMod i () lB k *Testdef lB p)
*GetMod=F158(A O SDB O k *Getdef lA oA u 40 C 0 P SEE u 27 P k *Extdef lB oB s u 4 P gI p O k *Testdef lB k Module k *Swaps jB () () lB gI b p O k *ERR i "Unexpected end of file. $END is expected" lB p u 101 A u 23 C Module O SNL P k *GetMod a TD gG a k *Swaps jB () () lB gI b b lB p C End O SNL P SEE u 14 P k *ERR i "Unbalanced $END" lB oC s k *GetMod hDI lB p B O STT K K O k *Testdef lB oD s k *Extdef lB oE s O k *GETV i *Extern lB oF s O k *GetMod a TD gV b TD TS gZ k Module k *Swaps TD TS gX lB k *Use ej lB k *Def ed lB gI b lB p O k *GetMod hDI eO lB p)
*Swap=F14(G A O SEB P a k *SWAP dD TE gH lB b p)
*Swaps=F16(K K P k *Swap i *Extern dD lB k *Swap i *Ftable dE lB p)
*Modext=F71(P u 15 k *Flag i 'X' lA oA A O SHF P i '.' eK p u 14 k *GETENV i RESTYPE lA oB K O i '.' dH eJ p k *GETS jC *SYSINFO 1 2 lA oC u 23 C VECTOR u 10 C NUMBER P jD '.rnx' p C WORD P jD '.rex' p O jD '.rsx' p)
*Ptime=F38(G K P k *CSeconds lB a dE lB k *CSeconds lB oA s G H O k *PROUT dD k *Tosec k *SYMB k *SUB dK dJ lB lB lB i " seconds" lB oB s eM p)
*Tosec=F19(u 10 H H H O hGF i '.' hED p O k *Tosec i '0' gE lB p)
*Compmod=F147(O u 144 k *GETV i *Errfl lA oA P k *PROUT i "No syntax errors" lB k *CLOAD jB *CompFile rccomp lB oB s k *Flag i 'M' lB oC s u 68 C F P k *GETV i *Modname lB k *Modext lB oD s O k *OPEN jB 'w' 4 eX lB oE s u 33 C T P k *WRITELN! jB 4 START k *SETV i X eX lB lB k *Ptime i "Compilation time = " k *CompFile eE b lB k *CLOSE i 4 lB oF s p C F P k *ERR i "Unable to open " eX lB p A O SRP P k *Ptime i "Compilation time = " k *CompMem eE b lB oG s O k *TABLE_VALUE jB *MODTABLE 'U' k *SETV i X k *GETV i *Modname lA i '.' lA lA oH G P k *SETV "d`" k *GETV "d`" lB "e[" lB p p)
*CSeconds=F5(P k *TIME_ELAPSED lB p)
*CLOAD=F21(G K P k *MODE dD lA oA u 9 C EMPTY P k *LOAD dE lB p G P p)
*CLOSE=F6(O k *CLOSE_CHANNEL gE lB p)
*ERASE=F6(O k *ERASE_FILE gE lB p)
*OPEN=F9(G G O k *OPEN_FILE hEG dD lB p)
*PROUT=F8(O k *PRINTLN! i *STDOUT gE lB p)
*WORD?=F22(G P k *CONST dD lA oA C T P k *MODE dD lA oB C STRING P SML O gP p)
*Oneof=F20(G A u 11 SEB P SFF U W ID O i T p O SEB P i F p)
<BlockEnd>
START "rsserv.rex"
<BlockBegin Lineno Errfl Modname Typename Len Addl ERR ERR0 WNG Flag Inp Inpa Setflags GTR WTR EXITCD PRINTLN! CPR LENCONT SUB ADD EOF READ_LINE!>
*Addl=F12(G O k *WTR dD k *GTR dD lB gF lB p)
*ERR=F19(O k *EXITCD i 10 lB k *WTR jB *Errfl T lB k *ERR1 i ERROR gE lB p)
*ERR0=F19(O k *EXITCD i 10 lB k *WTR jB *Errfl T lB k *PRINTLN! i 2 gE lB p)
*WNG=F8(O k *ERR1 i WARNING gE lB p)
*ERR1=F38(G u 25 A O SEB O k *ERR2 a dD k *SUB k *CPR i *Len lB k *LENCONT dE lB lB b gJ lB p O k *ERR2 a dD i 0 b gF lB p)
*ERR2=F45(A G G P SDB O k *ERR3 k *Flag i 'L' lB i "*** " k *CPR i *Modname lB i "." k *CPR i *Typename lB i "(" k *CPR i *Lineno lB i ", " dG i "): " dF i ": " gI lB p)
*ERR3=F22(u 14 A O SDB O k *PROUT gI lB k *Plist eI lB p K O k *PROUT gF lB p)
*Flag=F12(G P k *Flag1 dD k *CPR i *Flags lB lB p)
*Flag1=F29(G u 23 A u 10 ID O SEB O a TE gI b p G O SEB O k *Flag1 dD gK lB p P i F p)
*Inp=F10(P k *Inpa k *GetLst i 12 lB lB p)
*Inpa=F54(K P k *WTR i *Lineno k *ADD k *GTR i *Lineno lA i 1 lA lA oA SCB u 6 C 0 P dI p A u 19 G O SIB P k *WTR i *Len k *LENCONT dD lB lB k *CPR dD lB p P SIB P k *Inp lB p)
*OutLst=F23(O k *Flag i 'L' lA oA u 5 G P gE p A O SJH P gE k *Plist eE lB p)
*Setflags=F12(u 3 P p O k *WTR i *Flags gE lB p)
*Plist=F8(O k *PRINTLN! i 5 gE lB p)
*PROUT=F8(O k *PRINTLN! i 1 gE lB p)
*GetLst=F45(G P k *EOF dD lA oA u 31 C F P k *READ_LINE! dD lA oB u 13 P k *EOF dD lA oC C T P i 0 p O a k *OutLst eO lB b p C T P i 0 p)
<BlockEnd>
START "rclist.rex"
<BlockBegin ERR Flag Inifls Usefls Builtcd GETV OPEN_FILE CLOSE_CHANNEL SUB TABLE_VALUE MK_TABLE INPEXP SETV>
*Reads=F23(u 20 K O k *Readfls i 10 k *St-ext k *GETV dD lB lB lB k *Reads gF lB p P p)
*St-ext=F17(u 8 U W C '.' O hDH p O gE jD '.rfi' p)
*Readfls=F53(u 11 C 0 O k *ERR i "No file " gF lB p G O k *OPEN_FILE i *Ci eF i R lA oA u 15 C T P k *Inpall i *Ci lB k *CLOSE_CHANNEL i *Ci lB p C F P k *Readfls k *SUB dD i 1 lB gF lB p)
*Valfls=F10(G P k *TABLE_VALUE jB *Funlist 'U' dD lB p)
*Inifls=F18(P k *MK_TABLE i *Funlist lB oA s k *Loadfls k *Reads k *Flsname lB lB lB p)
*Usefls=F9(O k *Loadfls k *Reads gE lB lB p)
*Flsname=F27(P k *Flag i 'B' lB oA s u 6 G P i ('builtin.fls') p A u 5 P SHF P p O SHF P dH p)
*Inpall=F18(G P k *INPEXP dD lA oA u 3 P p O eJ k *Inpall dD lB p)
*Builtcd=F17(G P k *Valfls dD lA oA G P k *Lookup k *GETV dI lB lB p)
*Lookup=F30(u 16 A G K P SDB O SFE A G G O SJE P dM p u 8 K O k *Lookup gF lB p P i '*' p)
*Loadfls=F36(u 33 A G O SDB O k *Valfls dF lB oA s G P k *SETV dO a dF eH b k *GETV dO lB lB oB s k *Loadfls gJ lB p P p)
<BlockEnd>
