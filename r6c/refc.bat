@echo off
if not .%1==.rssynt goto A
copy rslex.rex+rssyn.rex rssynt.rex
goto E
:A
if not .%1==.rssynt6 goto B
copy rslexn.rex+rssyn.rex rssynt6.rex
goto E
:B
if not .%1==.rccomp goto C
copy rccompn.rex+rcgen.rex+rcout.rex rccomp.rex
goto E
:C
if not .%1==.c goto D
copy rcmain.rex+rsserv.rex+rclist.rex c.rex
goto E
:D
ri i+c+*go %1 %2 %3 %4 /l -B20000 -W0
:E
