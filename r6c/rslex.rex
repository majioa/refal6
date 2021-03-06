START "rslex.rex"
<BlockBegin ERR WNG Flag Inp Inpa Skipc SkipLine InsSC Word String Isdelim Index Oneof Getelem TYPE UPPER IMPLODE MUL ADD CHR NUMB>
*Skipc=F96(u 9 C ' ' O k *Skipc gF lB p u 9 C '	' O k *Skipc gF lB p u 9 C '' O k *Skipc gF lB p u 14 C '
' O k *Skipc k *Inpa a gF b lB lB p u 23 C '/' u 8 C '/' O k *Skipc2 lB p C '*' O k *Skipc k *Skipcom gG lB lB p u 9 EB '--' O k *Skipc2 lB p u 11 G u 5 G O hDG p P dD p P k *Skipc2 lB p)
*Skipcom=F37(u 11 C 0 O k *ERR i "Unpaired comment /*" lB dD p u 7 EB '*/' O gG p u 8 G O k *Skipcom gF lB p P k *Skipcom k *Inp lB lB p)
*Skipc2=F27(P k *Inp lA oA u 8 C '*' O k *Skipc2 lB p u 6 P k *Skipc2 lB p O k *Skipc eI lB p)
*SkipLine=F16(u 13 U W C '
' O k *Inpa a gE b lB p O p)
*InsSC=F53(u 9 C ' ' O k *InsSC gF lB p u 9 C '	' O k *InsSC gF lB p u 15 P k *Flag i '5' lA oA A O SHF P i ';' p u 13 EB '/*' O k *InsSC k *Skipcom gG lB lB p O gE p)
*Word=F12(O k *Worda k *TYPE k *Skipc gE lB lB lB p)
*Worda=F58(u 10 C 'L' G O k *Word1 hEG lB p u 16 C 'l' O k *ERR a gF b i "Word is reserved as variable" lB i '*' eF p u 21 C 'O' u 9 C '!' O k *Word1 hEG lB p C '?' O k *Word1 hEG lB p G G O k *Words hEG lB p)
*Word1=F14(G O k *Index2 k '.' dD b k *TYPE gF lB lB p)
*Case=F15(u 5 G O gF p A O SDB O k *UPPER gI lB p)
*Words=F40(u 15 C '\"' O k *StrWord k *String dD i () gF lB lB p u 14 G O k *ERREXP i word hDF lB i '*' dD eF p P k *ERREXP i word lB i '*' p)
*StrWord=F20(A u 8 P SDB O i '*' gG p O SDB O k *IMPLODE TD gG lB gI p)
*String=F96(G A O SEB u 9 ID O k *String1 gE gK lB p u 35 C '\\' u 22 G O k *Cvspec eJ eL lA oA G O k *String dD a TE gH dQ b eS lB p P k *String gE k *Inp lB lB p u 11 C 0 O k *ERR dE i "Unexpected end of file" lB p u 25 G u 14 G O k *String dD a TE gH dI b hJL lB p P k *ERRUNP dD lB i () p P k *ERRUNP dD lB i () p)
*String1=F32(G A u 9 P SEB O a dD b gH p O SEB u 14 ID O k *String dD a TE gH dD b gK lB p O hEJ p)
*Cvspec=F113(G u 14 O k *Flag i '5' lA oA A O SKI P hDF p u 5 ID O hEG p u 6 C '\'' O hEG p u 6 C '\"' O hEG p u 8 C 'n' O i '
' gG p u 8 C 't' O i '	' gG p u 8 C 'v' O i '' gG p u 8 C 'b' O i '' gG p u 8 C 'r' O i '\r' gG p u 8 C 'f' O i '' gG p u 9 C 'x' O k *Hexchar gG lB p G O k *Digits jB 0 () k *TYPE hEG lB lB p)
*Hexchar=F66(u 53 K K O k *Hexdigit dD lA k *Hexdigit dE lA oA u 23 C T G C T G P k *CHR k *ADD k *MUL dM i 16 lB dO lB lB gG p G G G G P i ' ' k *ERR a gG b i "Illegal hex code" lB p O i ' ' k *ERR a gE b i "Illegal hex code" lB p)
*Hexdigit=F86(G P k *TYPE dD lA oA u 11 C 'D' O i T k *NUMB dD lB p G O k *UPPER dD lA oB u 8 C 'A' P jB T 10 p u 8 C 'B' P jB T 11 p u 8 C 'C' P jB T 12 p u 8 C 'D' P jB T 13 p u 8 C 'E' P jB T 14 p u 8 C 'F' P jB T 15 p G P jB F 0 p)
*Number=F30(A O SDB u 10 C 'D' O k *Number1 dD gJ lB p G O k *ERR a gJ b i "No number after " TD gG lB i 0 eJ p)
*Number1=F18(A O SDB G O k *Number1a a TD gG dH b k *TYPE gJ lB lB p)
*Number1a=F50(A O SDB u 10 C 'D' O k *Number1 dD gJ lB p G u 26 C 'x' O SEE u 11 C '0' P k *Hexnumb i 0 gK lB p H O k *NUMB TD gG lB hIK p O k *NUMB TD gG lB gJ p)
*Hexnumb=F32(G u 27 G O k *Hexdigit dE lA oA C T G P k *Hexnumb k *ADD dM k *MUL dD i 16 lB lB gG lB p O hDF p)
*Digits=F82(u 18 C 3 A O SEB G O k *CHR k *NUMB TE gH lB lB gK p G A u 26 O SEB C 'D' G O k *Digits k *ADD dD i 1 lB a TE gH dJ b k *TYPE gL lB lB p u 18 P SEB G G O i '\\' hHJ k *WNG a eJ b i "Illegal character after \\ " lB p O SEB G G O k *CHR k *NUMB TE gH lB lB hJL p)
*Isdelim=F9(G P k *Oneof dD i ('=,:{};' 0) lB p)
*Index=F26(u 14 C '.' O k *Index2 i ('.') k *TYPE gF lB lB p O k *Index2 i ('*') k *TYPE gE lB lB p)
*Index1=F19(A G O SDB G O k *Index2 a TD hFH dI b k *TYPE gK lB lB p)
*Index2=F100(A O SDB u 10 C 'L' O k *Index1 dD gJ lB p u 10 C 'l' O k *Index1 dD gJ lB p u 10 C 'D' O k *Index1 dD gJ lB p u 55 C 'O' u 10 C '-' O k *Index1 dD hIK lB p u 10 C '_' O k *Index1 dD hIK lB p u 10 C '!' O k *Index1 dD hIK lB p u 10 C '?' O k *Index1 dD hIK lB p G O k *Index3 dD hIK lB p G O k *Index3 dD gJ lB p)
*Index3=F73(A u 21 G u 8 P SDB O i '*' gH p G P SDB O k *Compind dG lB gI p u 14 C '.' G O SDB O k *Compind TD hGI lB gK p C '*' G O SDB O k *Flag i '5' lA oA u 13 A O SPN P k *Compind dG lB TD gI gK p G P k *Compind TD hGI lB gK p)
*Compind=F14(O k *IMPLODE k *Case k *Flag i 'U' lB gE lB lB p)
*Oneof=F20(G A u 11 SEB P SFF U W ID O i T p O SEB P i F p)
*ERREXP=F12(G O k *ERR a gF b i "Expected " dD lB p)
*ERRUNP=F12(G O k *ERR a gF b i "Unpaired " dD lB p)
*Getelem=F388(u 289 C 'O' u 8 C ')' O i Brac hEG p u 8 C '(' O i Brac hEG p u 8 C '<' O i Brac hEG p u 8 C '>' O i Brac hEG p u 14 C '\'' O i String k *String dE i () gG lB p u 11 C '\"' O i Symbol k *Words hEG lB p u 84 G u 44 O w 36 SDF u 5 C '=' P x u 5 C ',' P x u 5 C ':' P x u 5 C '{' P x u 5 C '}' P x C ';' P x oA i Delim hEG p C ' ' O w 24 SDF u 5 C '+' P x u 5 C '-' P x u 5 C '*' P x C '&' P x oB i Symbol k *IMPLODE dE lB gH p u 11 C '*' O i Box k *Word gG lB p u 11 C '&' O i Box k *Word gG lB p u 11 C '!' O i Symbol k *Word1 hEG lB p u 13 C '?' O i Symbol k *Word1 i '!' gG lB p u 16 C '+' O i Symbol k *Number i ('+') k *TYPE gG lB lB p u 16 C '-' O i Symbol k *Number i ('-') k *TYPE gG lB lB p G O u 47 w 36 SDF u 5 C '.' P x u 5 C '/' P x u 5 C '%' P x u 5 C '^' P x u 5 C '|' P x C '~' P x oC i Symbol k *IMPLODE dE lB gG p i Other hEG p u 12 C 'L' G O i Symbol k *Word1 hEG lB p u 61 C 'l' u 12 C 'e' O i Var dE k *Index gG lB p u 12 C 's' O i Var dE k *Index gG lB p u 12 C 't' O i Var dE k *Index gG lB p u 12 C 'v' O i Var dE k *Index gG lB p G O i Other hEG p u 14 C 'D' G O i Symbol k *Number1 i () hEG lB p EB 'N' 0 O i Delim hEG p)
<BlockEnd>
