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
