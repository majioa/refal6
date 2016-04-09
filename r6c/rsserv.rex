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
