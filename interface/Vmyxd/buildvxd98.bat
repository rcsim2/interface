F:\98DDK\bin\win98\ml -coff -DBLD_COFF -DIS_32 -W2 -c -Cx -Zm -DMASM6 -DDEBLEVEL=0 /IF:\98DDK\inc\win98 Vmyxd.asm
F:\98DDK\bin\link /VXD /NOD /IGNORE:4078 /IGNORE:4039 /OUT:Vmyxd.vxd /MAP:Vmyxd.map /DEF:Vmyxd.def Vmyxd.obj
copy Vmyxd.vxd C:\Windows\System
