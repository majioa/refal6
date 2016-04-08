if %1.==. tounix *.*
del c:\tmp\*.*
for %%f in (%1) do dos2unix %%f c:\tmp\%%f
rem copy c:\tmp\*.* i:\refal\source\*.*