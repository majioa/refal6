START "cvd.rex"
<BlockBegin CVD LENCONT COMPARE MUL SUB "*" SYMB DIVMOD GETS>
*CVD=F15(K P k *Cvd1 k *LENCONT dD lB i 224 dD i 100000000000000000000000000000 lB p)
*Cvd1=F44(K K K L O k *COMPARE eE lA oA u 9 C '<' P k *Cvd2 gG lB p G P k *Cvd1 dD k *SUB k *MUL i 2 dE lB i 32 lB gG k *"*" dG dG lB lB p)
*Cvd2=F44(K u 8 K P k *SYMB dD lB p L O k *DIVMOD dD dE lA oA u 11 C 0 K P k *Cvd2 dM gG lB p K K P k *Cvd2 dL gG lB k *Cvd3 dM eG lB p)
*Cvd3=F68(K u 44 P k *GETS k '0' "j[" '000000000000000000000000000' k *SYMB dD lB b jB -30 29 lB p L O k *DIVMOD dD dE lA oA K K P k *Cvd3 dL gG lB k *Cvd3 dM eG lB p)
<BlockEnd>
