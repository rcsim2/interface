F:\MASM611\BIN\ml -coff -DBLD_COFF -DIS_32 -W2 -c -Cx -Zm -DMASM6 -DDEBLEVEL=0 /IG:\WIN95~25\95DDK\INC32 Vmyxd.asm > output.txt
C:\PROGRA~1\DEVSTU~1\VC\BIN\link /VXD /NOD /OUT:Vmyxd.vxd /MAP:Vmyxd.map /DEF:Vmyxd.def Vmyxd.obj >> output.txt
copy Vmyxd.vxd C:\Windows\System
