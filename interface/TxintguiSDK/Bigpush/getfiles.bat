copy ..\*.h   	.\*.h
copy ..\*.cpp 	.\*.cpp
copy ..\*.rc 	.\*.rc
rem copy ..\*.txt	.\*.txt
rem copy ..\media\heli.x	.\heli.x

dir /on /b .\*.h   >  filelist.txt
dir /on /b .\*.cpp >> filelist.txt
dir /on /b .\*.rc  >> filelist.txt
rem dir /on /b .\*.txt >> filelist.txt
rem dir /on /b .\*.x   >> filelist.txt

pkzip BigpushSource.zip *.* -x*.zip


rem copy BigpushSource.zip A:
rem copy BigpushSource.zip L: