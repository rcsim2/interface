# Microsoft Developer Studio Generated NMAKE File, Based on Bigpush.dsp
!IF "$(CFG)" == ""
CFG=Bigpush - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Bigpush - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Bigpush - Win32 Release" && "$(CFG)" !=\
 "Bigpush - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Bigpush.mak" CFG="Bigpush - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Bigpush - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Bigpush - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Bigpush - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Bigpush.exe"

!ELSE 

ALL : "$(OUTDIR)\Bigpush.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\bigpush.obj"
	-@erase "$(INTDIR)\Bigpush.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Bigpush.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Bigpush.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Bigpush.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Bigpush.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Bigpush.pdb" /machine:I386 /out:"$(OUTDIR)\Bigpush.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bigpush.obj" \
	"$(INTDIR)\Bigpush.res"

"$(OUTDIR)\Bigpush.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Bigpush - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Bigpush.exe"

!ELSE 

ALL : "$(OUTDIR)\Bigpush.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\bigpush.obj"
	-@erase "$(INTDIR)\Bigpush.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Bigpush.exe"
	-@erase "$(OUTDIR)\Bigpush.ilk"
	-@erase "$(OUTDIR)\Bigpush.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\Bigpush.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Bigpush.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Bigpush.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\Bigpush.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Bigpush.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\bigpush.obj" \
	"$(INTDIR)\Bigpush.res"

"$(OUTDIR)\Bigpush.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Bigpush - Win32 Release" || "$(CFG)" ==\
 "Bigpush - Win32 Debug"
SOURCE=.\bigpush.cpp

"$(INTDIR)\bigpush.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Bigpush.rc
DEP_RSC_BIGPU=\
	".\bsc3.bmp"\
	".\icon1.ico"\
	

"$(INTDIR)\Bigpush.res" : $(SOURCE) $(DEP_RSC_BIGPU) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

