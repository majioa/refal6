append ;
if exist reflib.lib del reflib.lib
d:\tc\tlib reflib +RFSTOR +RFINTER +RFARM +RBARM +ARITHM +RBSYS +RFSERV
d:\tc\tlib reflib +RFELST +RFINIT +RBEVAL +RFGO +RFOUTP +RFINP +REFGEN +REFINI
d:\tc\tlib reflib +CLASS +CHANNEL +FILEIO +RBTABLE +RFTABLE +RBIO +RFALLOC
d:\tc\tlib reflib +CONTAIN +CONCHA +CONSTR +CONVECT +CONSTOR +CONMASK +RBCONT
d:\tc\tlib reflib +RBCLASS +VIDEO +RBMOUSE +EVENTDRV +GRTXTV +EGAVGA , lib.lst
