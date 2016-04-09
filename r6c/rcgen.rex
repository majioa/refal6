START "rcgen.rex"
<BlockBegin Flag SYSINFO ERR Codgen Vmode SETV GETN LENGTH ADD CHR COMPARE GETV POS>
*Codgen=F26(G O k *SETV i *Funname dD lB oA s k *Cgen a k *Vmode lB k *Wmode lB k *Mmode lB b gF lB p)
*Cgen=F84(K u 13 G O k *Op gE lB k *Cgen dD gG lB p u 66 A u 23 C BRANCH O SEB O k *Opb dD dG k *Cgen dD TE gI lB lB k *Cgen dD gK lB p u 23 C ALT O SEB O k *Opb dD dG k *Cgen dD TE gI lB lB k *Cgen dD gK lB p G O SEB O k *Op dD TE hGI lB k *Cgen dD gK lB p P p)
*Vmode=F39(P u 14 k *Flag i 'V' lA oA A O SHF P i VECTOR p u 14 k *Flag i 'C' lA oB A O SHF P i CHAIN p oC P k *Getinf i 1 lB p)
*Wmode=F54(P u 14 k *Flag i 'W' lA oA A O SHF P i WORD p u 14 k *Flag i 'N' lA oB A O SHF P i NUMBER p u 14 k *Flag i 'S' lA oC A O SHF P i SYMBOL p oD P k *Getinf i 2 lB p)
*Mmode=F21(P k *Flag i 'M' lA oA u 8 A O SHF P i T p G P i F p)
*Getinf=F9(G P k *GETN i *SYSINFO dD lB p)
*Opb=F13(K G O k *Op gE lB k *Ob dD gG lB p)
*Ob=F28(A u 10 C CHAIN O SDB O a gJ b p C VECTOR O SDB O k *ADD k *LENGTH gJ lB i 1 lB eJ p)
*Op=F70(A G u 10 G C F P SDB C BLOCK P p u 14 C NUMBER G P SDB G O k *O dI lB gK p u 32 C WORD G P SDB G u 10 P k *Word k *O dI lB lB p O k *Opw k *Optype dI lB k *O dI lB gK lB p C SYMBOL G P SDB G O hIK p)
*Opw=F49(u 11 C 1 G G P k *Word gF lB p u 12 C 2 G G G P k *Word hEG lB p u 12 C T G K P k *Word dE lB dF p C E G G O k *Word gF lB gH p)
*Word=F14(O k *SETV i W k *CHR k *Ups gE lB lB lB p)
*Ups=F44(u 3 P p G O k *COMPARE dD i 196 lA oA u 15 C '<' P k *ADD dD i 64 lB k *Ups gF lB p G P k *ERR i "Too many variables in function " k *GETV i *Funname lB lB i 0 p)
*O=F9(G P k *POS i (NOP PS PSR CSYM CSYMR SYMS SYMSR SYM SYMR OSYM OSYMR TERM TERMR OTERM OTERMR EXP EMP OEXP OEXPR SETB CHKB PLEN PLENS LEN LENP LENS LENOS NOP NOP NOP NOP NOP NOP BL BR BLR MULT MULE MUL TPLE TPL NS TEXT BLNS BRACT BUILTIN RETURN EVAL EST STAKE CUT DO UNDO BRANCH BRANCHR ALT END FAIL BLOCK) dD lB p)
*Optype=F16(G P i (PS 0 PSR 0 CSYM T CSYMR T SYMS E SYMSR E SYM 0 SYMR 0 OSYM 1 OSYMR 1 TERM 0 TERMR 0 OTERM 1 OTERMR 1 EXP 0 EMP 0 OEXP 1 OEXPR 1 SETB 2 CHKB 1 PLEN 0 PLENS 0 LEN 0 LENP 0 LENS T LENOS 1 BL 0 BR 0 BLR 0 MULT 1 MULE 1 MUL 2 TPLE 1 TPL 2 NS T TEXT E BLNS T BRACT 1 BUILTIN 0 RETURN 0 EVAL 1 EST 0 STAKE 0 CUT 0 DO 0 UNDO 0 BRANCH B BRANCHR B ALT B END 0 FAIL 0 BLOCK 0) oA A SIG P SJJ U W ID G O dN p)
<BlockEnd>
