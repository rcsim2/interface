copy ..\*.asm  	.\*.asm
copy ..\*.def 	.\*.def
copy ..\*.rc 	.\*.rc
copy ..\*.bat	.\*.bat
copy ..\*.vxd	.\*.vxd

dir /on /b .\*.asm >  filelist.txt
dir /on /b .\*.def >> filelist.txt
dir /on /b .\*.rc  >> filelist.txt
dir /on /b .\*.bat >> filelist.txt
dir /on /b .\*.vxd >> filelist.txt

pkzip VmyxdSource.zip *.* -x*.zip


rem copy VmyxdSource.zip A:
rem copy VmyxdSource.zip L: