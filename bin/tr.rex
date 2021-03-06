START "tr.rex"
<BlockBegin FILTER BRK TRC TRCA REST CLEAR Map Foldr "<" APPEND BREAK STEP STDOUT PRINT! WRITE! WRITELN! PRINTLN! TYPE SYSID TABLE_VALUE MODE REFCOUNT MAKE TABLE_NAME LENCONT COMPARE MOVE GETV SETV PRINTLN TESTMEM SYSTABLE TABLE_LIST MK_TABLE ADD TABLE_LINK>
*Pbrk=F28(O u 18 k *FILTER eE lA oA r k *Pbrk eE k *BREAK lA lA oB O gO p k *Pbrk eE lA oC O gK p)
*FILTER=F2(O p)
*BRK=F8(O k *Map i (*Setenvlp "Break point" *Pbrk) gE lB p)
*Ptrc=F113(O u 103 k *FILTER eE lA oA r k *STEP lA oB G P u 54 k *Ptrc eE lA oC O k *STEP lB oD s G P oE k *PRINT! i *STDOUT dN i '-' dY jE ': <' *Ptrc ' ' lB k *WRITE! i *STDOUT gE lB k *PRINT! jF *STDOUT '> -> ' lB k *WRITELN! i *STDOUT gT lB eT p k *STEP lA oF G P k *PRINTLN! i *STDOUT dN i '-' dS jQ ': <' *Ptrc '...> -> $FAIL' lA oG y k *Ptrc eE lA oH O gK p)
*TRC=F8(O k *Map i (*Setenvlp "Tracing point" *Ptrc) gE lB p)
*Ptrca=F46(O u 36 k *FILTER eE lA oA r k *PRINT! jD *STDOUT '<' *Ptrca ' ' lA k *WRITE! i *STDOUT eE lA k *PRINTLN! jB *STDOUT '>' lA oB k *Ptrca eE lA oC O gS p k *Ptrca eE lA oD O gK p)
*TRCA=F8(O k *Map i (*Setenvlp "Tracing point" *Ptrca) gE lB p)
*Setenvlp=F196(G G u 11 A O SFB P k *Setenvlp gE eI lB p G O k *TYPE dF lA oA u 22 C 'F' G P k *TABLE_VALUE jB *SYSID 'O' dF lA oB G P k *Setenvlp gE dS gH lB p u 141 C 'R' G P k *MODE dF lA oC C FUNC P u 117 k *"<" k *REFCOUNT dF lA i 256 lA oD k *MAKE dS lB oE s G P k *Foldr i *Rept a dE "d\\" b eH dE lA oF K P k *APPEND i *SYSID k *TABLE_NAME jB *SYSID 'O' dF lA "d\\" lA oG w 42 u 28 k *COMPARE k *LENCONT dF lA i 3 lA oH C '>' P k *MOVE dF "d\\" lA k *MAKE dS dF lA oI O gp x k *SETV "d\\" k *GETV dF lA lA oJ O gk x oK k *SETV dF k *GETV da lA a fDF "d\\" b lA oL k *PR dD i " at " dF i " is set" lA oM O gs p oN k *PR i "Function " dF i " looks active" lB p G G P k *PR dF i " is not a function" lB p)
*Rept=F31(u 19 A G K P SDB K P a k *Rep eG k *GETV dH lB lB b p K K P k *PR dD i " is not a replacement: (s t) is expected" lB dE p)
*Rep=F19(G K u 13 U W ID O gG dE k *Rep gE gJ lB p O gG p)
*PPP=F6(O k *PPP gE lB p)
*REST=F8(O k *Map i *Rest gE lB p)
*Rest=F120(G P k *TYPE dD lA oA u 20 C 'F' G P k *TABLE_VALUE jB *SYSID 'O' dD lA oB G P k *Rest dO lB p u 79 C 'R' G P k *MODE dD lA oC C FUNC P k *GETV dD lA oD B O SUU G G ID G P k *MOVE "d[" dD lB oE k *Rept k *PPP dD b i *PPP lB oF s K P k *SETV k *MAKE dO "d[" lA k *GETV dd lA lA oG k *TABLE_NAME jB *SYSID 'D' "d[" lA oH k *PR dX i " at " dD i " is removed" lA oI O gr p G G P k *PR i "Function " dD i " was not modified" lB p)
*PR=F6(O k *PRINTLN gE lB p)
*CLEAR=F20(P k *TESTMEM lA oA G P k *Clrtbl jB *SYSTABLE 2 lB k *Clrtbl jB *SYSID 1 lB p)
*Clrtbl=F28(G G P k *TABLE_LIST dD lA oA O k *TABLE_LINK k *MK_TABLE dD lA k *Clr k *ADD dE i 2 lA eK lA lA oB p)
*Clr=F31(G u 3 P p G G O k *REFCOUNT dF lA oA u 9 ID P k *Clr dD gH lB p G P gF k *Clr hDH lB p)
<BlockEnd>
