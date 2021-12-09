ml -coff -DBLD_COFF -DIS_32 -W2 -c -Cx -Zm -DMASM6 -DDEBLEVEL=0 /IG:\WIN95~25\95DDK\INC32 vfintd.asm
link /VXD /NOD /OUT:vfintd.vxd /MAP:vfintd.map /DEF:vfintd.def vfintd.obj

rem ml -W2 -c -Cx -Zm -DMASM6 -DDEBLEVEL=0 tstfint.asm
rem need 16-bits segmented linker
remlink16\link tstfint.obj,,,,,