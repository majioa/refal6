START "rccompl.rex"
<BlockBegin Compile L-op SETV GETV POS SETN GETN FIRST LAST LENGTH COMPARE ADD>
*Compile=F21(O k *SETV jB *Funno 1 lB oA s k *Opt i (BOUNDS 3 2) k *Group i ((A A 2 3 4) () () A W) gE lB lB p)
*Group=F10(K O k *Merge k *Paths hDF lB lB p)
*Paths=F22(K u 18 A O SEB O a k *Path dD eH lB b k *Paths dD gJ lB p P p)
*Struct=F14(u 7 C Attr P i (N V M U K) p C Tbel P i (A E B F N) p)
*Path=F341(u 9 K H O k *Path hDG lB p u 158 A K K K G u 134 G P SDB A u 24 C L O SKB O k *Mk-do dJ lB k *Path-l dF dI k *Exp-l fFH eO lB a gQ b lB p u 36 C R O SKB O w 13 u 6 eQ oA P dI x oB P i V x oC G P k *Exp-r dV dJ eH eO lB k *Path-r dD gQ lB p C G O SKB u 20 P oD k *Block k *Mk-do dJ lB lB k *Group a fFI i W b eO lB p O oE k *Block k *Mk-do dJ lB lB k ALT k *Group a k *SETA dF jC Tbel E V lB eH jB V W b eO lB b k *Path-r a fFI k *After-do dJ lB b gQ lB p C S P SDB O i DO k *Path a fFI i SS b gL lB p u 125 K u 29 C NOT K O i STAKE k *Group k *SETA dD jC Attr K W lB a dF jB CUT (FAIL) b k CUT gH b lB p u 30 C BIND A G O SFB u 10 P i DO k *Path dD dF lB p O i DO k *Path dD dF i UNDO gL lB p u 11 C UNDO O dE k *Path dD gG lB p u 17 C DO O k *Path k *SETA dD jC Attr K S lB gG lB p u 11 C STAKE O dE k *Path dD gG lB p u 11 C CUT O dE k *Path dD gG lB p A C FAIL P SEB P dG p A K K K G C W P SDB P k *GETA dF jC Tbel E N lA oF G G P k *MK-b i SETB dF lB i EXP a k *Movop dO lB dP k *P1 dP lB b k *Endop dI lB p)
*Path-l=F38(K G B L L H O SGG O k *MK-b i SETB dD lB gL k *Path a k *SETA dD jB Tbel (A N) k V dJ b lB hIH dE i W b eN lB p)
*Path-r=F61(A K K K G u 11 C S P SDB P k *Endop dI lB p u 11 C SS P SDB P k *Endop dI lB p G P SDB O k *Mk-bnel dF lA oA K P k EVAL k *Gennum lB b k *MK-b i BOUNDS dQ lB k *Path a dQ fGJ b gL lB p)
*Mk-bnel=F37(K P k *GETA dD jB Tbel N lA oA G P k *SETA dD jB Tbel (A E B F N) k A i A k *P2 dI lB k *P3 dI lB k *P4 dI lB b lB p)
*MK-b=F15(G K P a dD k *GETA dE jC Tbel F B lB b p)
*Endop=F14(u 7 C A P i EST p C V P i END p)
*Mk-do=F11(u 7 C S P i DO p G P p)
*After-do=F12(u 7 C S P i SS p G P dD p)
*Block=F11(u 6 C DO P dD p P i BLOCK p)
*L-op=F23(u 10 G P k *Oneof dD i (EXP EMP PS PSR TERM TERMR SYM SYMR) lB p A G O SDB P k *Oneof dF i (CSYM CSYMR OEXP OEXPR OSYM OSYMR SYMS SYMSR SETB OTERM OTERMR) lB p)
*Merge=F30(u 25 A O SDB u 17 A O SHB O k *Merge a k *Merge1 dD eK lB b gM lB p P eG p P i FAIL p)
*Merge1=F59(A u 18 A C BRANCH O SFE O SDB O dF k *Merge1 a eL b gN lB p u 23 K O SDB MF O k *L-op dF lA oA C T P dF k *Merge1 a eH b gK lB p u 5 P SDB P p O SDB O k BRANCH eG b gI p)
*Gennum=F23(P k *GETV i *Funno lB oA s G P k *SETV i *Funno k *P1 dH lB lB oB s dH p)
*Mk-map=F43(A A G P SFE G O SDB A H B O SPP IH P SLB P dH a k *Pair eK eR lA b oA u 7 C V K P gY p C A K P gY i V dY p)
*Pair=F15(u 12 K K O a gE b k *Pair gG lB p P p)
*SETA=F57(K K u 18 G G P k *SETN dD k *POS k *Struct dE lB dF lB dG lB p A u 26 K O SFB A K O SKB P k *SETA k *SETA gE dH dM lB dE a eJ b a eO b lB p P SFB A P SHB P dD p)
*GETA=F26(K K u 21 G O k *GETN dD k *POS k *Struct dE lB dF lB lB k *GETA gE gH lB p P p)
*Testvar=F32(A u 19 SDB A G K P SFB P SEE V X IH MI P SLE G O dP p O SDB A G K P SHB P i () p)
*Varop=F109(G K A G K P SFB P dD dH k *Testvar eF lA oA u 23 C L u 10 C S G P k OSYM dP b p C T G P k OTERM dP b p u 23 C R u 10 C S G P k OSYMR dP b p C T G P k OTERMR dP b p u 23 C L u 10 C S A P SPL P i SYM p C T A P SPL P i TERM p C R u 10 C S A P SPL P i SYMR p C T A P SPL P i TERMR p)
*Expl-next=F68(G K K A A u 8 P SIH P SGB P hDF p u 28 O SIH A O SMH O SGB P k SETB k *Getbs dM lB b k *Expl i L hDF eP a dI eR b lB p A O SKJ O SIH P SGB P k SETB k *Getbs dK lB b k *Exple hDF eN k () eP b lB p)
*Getbs=F26(A A L L O SFE B O SMM K K O SDB P k *Ends dQ dH lA oA G G P dY dX p)
*Exp-l=F34(A G G G G G P SDB K K O k *Expl i L dJ gL a a dF b dI b gN a a dF b dH b i (()) lB p)
*Expl=F515(u 232 C L G K K A O SHB u 22 A O SLB K P i EMP k *Expl-next gF k *Apt dG k *Mk-map hHL lB lB dP lB p u 27 G L B O SOO O k CSYM dL b k *Expl dD k *P1 dE lB gG a eK dL dE b hPM lB p A u 55 C P O SLB B B O STT O SRR A O SYR O k *P1 dE lA oA G P i PS k *Expl dD k *P2 dE lB gG k (V) dc b eP k (V) dc b a dY a a eK dL dE b hUS b "e^" b lB p u 48 G K P SLB L B O SRR O k *Oneof dN i (S T) lA oB C T P k *Varop dD dF dL lB k *Expl dD k *P1 dE lB k *Apt dF dL dE lB dG a eK dL dE b hSP lB p C E K P SLB L B O SRR O k *Testvar dF dL lA oC u 38 G P k OEXP "d[" b k *Expl dD k *P2 dE lB k *Apt dF dL k *P1 dE lB lB dG a eK dL a dE k *P1 dE lB b b hSP lB p A P "S[Y" P k *Expl i R hEP lB p C R G K K A O SHB u 51 A C E K P SLB A O SPB K P i EXP k *Expl-next k *P2 dE lB k *Apt dF dL k *P1 dE lB lB k *Apt dG k *Mk-map dH a dL a dE k *P1 dE lB b eS b lB lB dT lB p u 27 L B H O SNN O k CSYMR dO b k *Expl dD k *P1 dE lB hFQ a dO dE eS b dL lB p u 55 B B B O SQQ C P O SOO O SMM A O SYM O k *P1 dE lA oD G P i PSR k *Expl dD k *P2 dE lB hFS a dP dE eX b a dY a k (V) dc b eV k (V) dc b b "e^" b lB p L B B O SPP u 44 G K P SNN O k *Oneof dS i (S T) lA oE C T P k *Varop dD dF dO lB k *Expl dD k *P1 dE lB k *Apt dF dO dE lB hGR a dO dE eV b dL lB p C E K P SNN O k *Testvar dF dO lA oF u 38 G P k OEXPR "d[" b k *Expl dD k *P2 dE lB k *Apt dF dO k *P1 dE lB lB hGR a dO a dE k *P1 dE lB b eV b dL lB p A P "S[Y" P dL oG A A O Scb O Sa_ P k BOUNDS k *Getbs a dH dM b lB b k *Expl-next hEG a a ef a hHM b b eh b lB p)
*Exple=F52(G K K A O SGB A C E K P SKB L B O SQQ O jB PLEN LEN k *Expl i L k *P2 dD lB k *Apt dE dK k *P1 dD lB lB dF a eJ dK a dD k *P1 dD lB b b hRO lB p)
*Exp-r=F14(G G K K O k *Expr fDI lA oA L O gP p)
*Expr=F146(G G K K u 35 A C K O SHB O k *Expr fDG k *Narg eL lA lA oA L O i BL gU k BRACT k *Actmod dE lB b k *Expr hDF dS gN lB p u 4 P dG p u 49 O dE oB w 13 u 7 C W P i V x G P dD x oC G P k *Findsub dR fGI lA oD G K L O dX oE A K O "S`^" P k *Genmov gX lB k *Expr hDF dY "g[" lB p u 24 K O k *Isconst dH lA oF C T P k NS k *Demeta dH lB b k *Expr hDG gJ lB p A C P O SHB O k *Expr fDG eL lA oG L O i BL gU i BR k *Expr hDF dS gN lB p)
*Narg=F3(O gE p)
*Actmod=F13(u 7 C W P i 1 p G P i 2 p)
*Genmov=F16(G A O SEB P a k *Movop dD lB k *Ends eH lB b p)
*Movop=F14(u 7 C A P i TPL p C V P i MUL p)
*Ends=F37(u 6 G P dD dD p u 11 A O SDB O k *Ends eG gI lB p u 11 B O SEE O k *Ends gG eI lB p G H O dD dE p)
*Isconst=F35(u 21 A u 12 C P O SDB O k *Isconst eH gJ lB p O SDB O i F p u 8 G O k *Isconst gF lB p P i T p)
*Demeta=F31(u 9 G O dD k *Demeta gF lB p u 18 A C P O SDB O a k *Demeta eH lB b k *Demeta gJ lB p P p)
*Findsub=F126(G A O SEB O dD k *Search dD i (0 0 0) eH a eJ b lA oA u 29 C A u 13 EC 0 0 0 P k *Findsub i V hEJ lB p G G G G P k *Findsub i V hEJ lB p ID G G G O k *FIRST dP eJ lA oB A O SYW O dD oC u 11 C V P dc a gT b "g^" dE p C A P k *LAST dR eH lA oD A B O Skk O Shf O k *LAST dQ eo lA oE A O Svt O k *LAST dP ey lA oF A O "S�~" O dc a gT b "g^" a em a "e�" "g{" b gq b p)
*Search=F106(G A u 95 G O SEB B SKK K O SEJ U W ID A SRJ O SSS V X ML K P SXS O k *Maxcom a "e]" b a eN b lA oA B B O See O Scc O k *LENGTH i X eg lA oB G P k *COMPARE dG dp lA oC u 27 C '<' P k *Search dD a dp k *LENGTH ei lB k *LENGTH gU lB "d[" gg b dD a "e]" b fTJ lB p G P k *Search gE dD a "e]" b hTJ lB p O SEB B O SJJ O eH p)
*Maxcom=F34(A u 24 A K K P SFE O SDB A MH O SLB P dI k *Maxcom a eK b a eP b lB p O SDB A O SHB P hDH p)
*Opt=F395(u 307 A u 18 C BRANCH O SDB O a dF k *Opt eH lB b k *Opt gJ lB p u 20 C ALT O SDB O k *Alt k *Opt eH lB a k *Opt gJ lB b lB p u 19 C BOUNDS G G P SDB O k *Bounds a eH b k *Opt gJ lB lB p u 19 C SETB G G P SDB O k *Setb a eH b k *Opt gJ lB lB p u 24 C TPL G G P SDB O k *P1 dG lA oA IH P k TPLE dH b k *Opt gJ lB p u 39 C MUL G u 14 IG P SDB O k MULT dG b k *Opt gJ lB p G P SDB O k *P1 dG lA oB IH P k MULE dH b k *Opt gJ lB p u 25 C CSYM G P SDB A C CSYM G P SHB O k *Opt k SYMS i 2 dG dK b gM lB p u 27 C SYMS G O SDB A C CSYM G P SJB O k *Opt a dF k *P1 dG lB eI dM b gO lB p u 25 C SYMR G P SDB A C SYMR G P SHB O k *Opt k SYMSR i 2 dG dK b gM lB p u 27 C SYMSR G O SDB A C SYMR G P SJB O k *Opt a dF k *P1 dG lB eI dM b gO lB p u 25 C NS K P SDB A C NS K P SHB O k *Opt k TEXT i 2 dG dK b gM lB p C TEXT G O SDB A C NS K P SJB O k *Opt a dF k *P1 dG lB eI dM b gO lB p u 19 C BL A C NS K P SEB O k BLNS dH b k *Opt gJ lB p u 54 EB PLEN LEN u 12 C PS O jB PLENS LENP k *Opt gH lB p A u 18 C SYM G P SFB O i PLENS k LENS dI b k *Opt gK lB p C OVSYM G P SFB O i PLENS k LENOS dI b k *Opt gK lB p u 9 K O dD k *Opt gF lB p P p)
*Alt=F17(B O SEE u 6 C EST P gG p O k ALT gG b eI p)
*Bounds=F112(u 16 A G G P SDB A C SETB IF IG P SHB O gN p K u 42 A u 20 C BRANCH O SEB O a dG k *Bounds dD eI lB b k *Bounds dD gK lB p C ALT O SEB O a dG k *Bounds dD eI lB b k *Bounds dD gK lB p u 11 C STAKE O dE k *Bounds dD gG lB p u 11 C CUT O dE k *Bounds dD gG lB p u 11 C BLOCK O dE k *Bounds dD gG lB p u 11 C DO O dE k *Bounds dD gG lB p O gF p)
*Setb=F24(A G G P SDB u 11 A C SETB G G P SHB O hHN p O k SETB eG b gI p)
*Oneof=F20(G A u 11 SEB P SFF U W ID O i T p O SEB P i F p)
*Apt=F9(A O SDB O a eG gI b p)
*P1=F9(G P k *ADD dD i 1 lB p)
*P2=F9(G P k *ADD dD i 2 lB p)
*P3=F9(G P k *ADD dD i 3 lB p)
*P4=F9(G P k *ADD dD i 4 lB p)
<BlockEnd>
