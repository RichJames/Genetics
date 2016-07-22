# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Forge.mak" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : $(OUTDIR)/Forge.exe $(OUTDIR)/Forge.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /Zp4 /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /Zp4 /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"Forge.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_MSC_VER"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"FORGE.res" /d "NDEBUG" /d "_MSC_VER" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"Forge.bsc" 
BSC32_SBRS= \
	$(INTDIR)/FPUTIL.SBR \
	$(INTDIR)/TREEX.SBR \
	$(INTDIR)/EvOptCfg.sbr \
	$(INTDIR)/FSM.SBR \
	$(INTDIR)/Forge.sbr \
	$(INTDIR)/FPCONST.SBR \
	$(INTDIR)/DIAGNOSE.SBR \
	$(INTDIR)/RANDDEV.SBR \
	$(INTDIR)/DIAGWIN.SBR \
	$(INTDIR)/FONTUTIL.SBR \
	$(INTDIR)/TAOPTCFG.SBR \
	$(INTDIR)/GAFLOAT.SBR \
	$(INTDIR)/PDOPTCFG.SBR \
	$(INTDIR)/GAOPTCFG.SBR \
	$(INTDIR)/ForgeTST.sbr \
	$(INTDIR)/ForgeBB.sbr \
	$(INTDIR)/ForgeGA.sbr \
	$(INTDIR)/ForgePD.sbr \
	$(INTDIR)/ForgeTA.sbr \
	$(INTDIR)/ForgeEV.sbr \
	$(INTDIR)/ForgeRA.sbr \
	$(INTDIR)/RAOptCfg.sbr

$(OUTDIR)/Forge.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /INCREMENTAL:no /PDB:$(OUTDIR)/"Forge.pdb" /MACHINE:I386\
 /OUT:$(OUTDIR)/"Forge.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/FPUTIL.OBJ \
	$(INTDIR)/TREEX.OBJ \
	$(INTDIR)/EvOptCfg.obj \
	$(INTDIR)/FSM.OBJ \
	$(INTDIR)/FORGE.res \
	$(INTDIR)/Forge.obj \
	$(INTDIR)/FPCONST.OBJ \
	$(INTDIR)/DIAGNOSE.OBJ \
	$(INTDIR)/RANDDEV.OBJ \
	$(INTDIR)/DIAGWIN.OBJ \
	$(INTDIR)/FONTUTIL.OBJ \
	$(INTDIR)/TAOPTCFG.OBJ \
	$(INTDIR)/GAFLOAT.OBJ \
	$(INTDIR)/PDOPTCFG.OBJ \
	$(INTDIR)/GAOPTCFG.OBJ \
	$(INTDIR)/ForgeTST.obj \
	$(INTDIR)/ForgeBB.obj \
	$(INTDIR)/ForgeGA.obj \
	$(INTDIR)/ForgePD.obj \
	$(INTDIR)/ForgeTA.obj \
	$(INTDIR)/ForgeEV.obj \
	$(INTDIR)/ForgeRA.obj \
	$(INTDIR)/RAOptCfg.obj

$(OUTDIR)/Forge.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : $(OUTDIR)/Forge.exe $(OUTDIR)/Forge.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32  
# ADD BASE CPP /nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"Forge.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"Forge.pdb"\
 /c 
CPP_OBJS=.\WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_MSC_VER"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"FORGE.res" /d "_DEBUG" /d "_MSC_VER" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"Forge.bsc" 
BSC32_SBRS= \
	$(INTDIR)/FPUTIL.SBR \
	$(INTDIR)/TREEX.SBR \
	$(INTDIR)/EvOptCfg.sbr \
	$(INTDIR)/FSM.SBR \
	$(INTDIR)/Forge.sbr \
	$(INTDIR)/FPCONST.SBR \
	$(INTDIR)/DIAGNOSE.SBR \
	$(INTDIR)/RANDDEV.SBR \
	$(INTDIR)/DIAGWIN.SBR \
	$(INTDIR)/FONTUTIL.SBR \
	$(INTDIR)/TAOPTCFG.SBR \
	$(INTDIR)/GAFLOAT.SBR \
	$(INTDIR)/PDOPTCFG.SBR \
	$(INTDIR)/GAOPTCFG.SBR \
	$(INTDIR)/ForgeTST.sbr \
	$(INTDIR)/ForgeBB.sbr \
	$(INTDIR)/ForgeGA.sbr \
	$(INTDIR)/ForgePD.sbr \
	$(INTDIR)/ForgeTA.sbr \
	$(INTDIR)/ForgeEV.sbr \
	$(INTDIR)/ForgeRA.sbr \
	$(INTDIR)/RAOptCfg.sbr

$(OUTDIR)/Forge.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /INCREMENTAL:yes /PDB:$(OUTDIR)/"Forge.pdb" /DEBUG\
 /MACHINE:I386 /OUT:$(OUTDIR)/"Forge.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/FPUTIL.OBJ \
	$(INTDIR)/TREEX.OBJ \
	$(INTDIR)/EvOptCfg.obj \
	$(INTDIR)/FSM.OBJ \
	$(INTDIR)/FORGE.res \
	$(INTDIR)/Forge.obj \
	$(INTDIR)/FPCONST.OBJ \
	$(INTDIR)/DIAGNOSE.OBJ \
	$(INTDIR)/RANDDEV.OBJ \
	$(INTDIR)/DIAGWIN.OBJ \
	$(INTDIR)/FONTUTIL.OBJ \
	$(INTDIR)/TAOPTCFG.OBJ \
	$(INTDIR)/GAFLOAT.OBJ \
	$(INTDIR)/PDOPTCFG.OBJ \
	$(INTDIR)/GAOPTCFG.OBJ \
	$(INTDIR)/ForgeTST.obj \
	$(INTDIR)/ForgeBB.obj \
	$(INTDIR)/ForgeGA.obj \
	$(INTDIR)/ForgePD.obj \
	$(INTDIR)/ForgeTA.obj \
	$(INTDIR)/ForgeEV.obj \
	$(INTDIR)/ForgeRA.obj \
	$(INTDIR)/RAOptCfg.obj

$(OUTDIR)/Forge.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\FPUTIL.CPP
DEP_FPUTI=\
	.\FPUTIL.H\
	.\DIAGNOSE.H

$(INTDIR)/FPUTIL.OBJ :  $(SOURCE)  $(DEP_FPUTI) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TREEX.CPP
DEP_TREEX=\
	.\TREEX.H\
	.\DIAGNOSE.H

$(INTDIR)/TREEX.OBJ :  $(SOURCE)  $(DEP_TREEX) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EvOptCfg.CPP
DEP_EVOPT=\
	.\FORGERES.H\
	.\EvOptCfg.H\
	.\BOOL.H

$(INTDIR)/EvOptCfg.obj :  $(SOURCE)  $(DEP_EVOPT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FSM.CPP
DEP_FSM_C=\
	.\FSM.H\
	.\DIAGNOSE.H\
	.\BOOL.H\
	.\ROULETTE.H\
	.\RANDDEV.H

$(INTDIR)/FSM.OBJ :  $(SOURCE)  $(DEP_FSM_C) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FORGE.RC
DEP_FORGE=\
	.\FORGE.ICO\
	.\FORGERES.H

$(INTDIR)/FORGE.res :  $(SOURCE)  $(DEP_FORGE) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Forge.cpp
DEP_FORGE_=\
	.\DIAGWIN.H\
	.\FONTUTIL.H\
	.\FORGERES.H\
	.\ROULETTE.H\
	.\BOOL.H\
	.\FPCONST.H\
	.\FPUTIL.H\
	.\BINTREEK.H\
	.\SORTS.H\
	.\GAFLOAT.H\
	.\FSM.H\
	.\GAOPTCFG.H\
	.\PDOPTCFG.H\
	.\TAOPTCFG.H\
	.\EvOptCfg.H\
	.\DIAGNOSE.H\
	.\RANDDEV.H\
	.\TREEX.H

$(INTDIR)/Forge.obj :  $(SOURCE)  $(DEP_FORGE_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FPCONST.CPP
DEP_FPCON=\
	.\FPCONST.H

$(INTDIR)/FPCONST.OBJ :  $(SOURCE)  $(DEP_FPCON) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DIAGNOSE.CPP
DEP_DIAGN=\
	.\DIAGNOSE.H

$(INTDIR)/DIAGNOSE.OBJ :  $(SOURCE)  $(DEP_DIAGN) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RANDDEV.CPP
DEP_RANDD=\
	.\RANDDEV.H

$(INTDIR)/RANDDEV.OBJ :  $(SOURCE)  $(DEP_RANDD) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DIAGWIN.CPP
DEP_DIAGW=\
	.\DIAGWIN.H\
	.\DIAGNOSE.H

$(INTDIR)/DIAGWIN.OBJ :  $(SOURCE)  $(DEP_DIAGW) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FONTUTIL.CPP
DEP_FONTU=\
	.\FONTUTIL.H

$(INTDIR)/FONTUTIL.OBJ :  $(SOURCE)  $(DEP_FONTU) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TAOPTCFG.CPP
DEP_TAOPT=\
	.\FORGERES.H\
	.\TAOPTCFG.H\
	.\BOOL.H

$(INTDIR)/TAOPTCFG.OBJ :  $(SOURCE)  $(DEP_TAOPT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GAFLOAT.CPP
DEP_GAFLO=\
	.\GAFLOAT.H\
	.\RANDDEV.H

$(INTDIR)/GAFLOAT.OBJ :  $(SOURCE)  $(DEP_GAFLO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PDOPTCFG.CPP
DEP_PDOPT=\
	.\FORGERES.H\
	.\PDOPTCFG.H\
	.\BOOL.H

$(INTDIR)/PDOPTCFG.OBJ :  $(SOURCE)  $(DEP_PDOPT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GAOPTCFG.CPP
DEP_GAOPT=\
	.\FORGERES.H\
	.\GAOPTCFG.H\
	.\BOOL.H

$(INTDIR)/GAOPTCFG.OBJ :  $(SOURCE)  $(DEP_GAOPT) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ForgeTST.cpp

$(INTDIR)/ForgeTST.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ForgeBB.cpp

$(INTDIR)/ForgeBB.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ForgeGA.cpp

$(INTDIR)/ForgeGA.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ForgePD.cpp

$(INTDIR)/ForgePD.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ForgeTA.cpp

$(INTDIR)/ForgeTA.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ForgeEV.cpp

$(INTDIR)/ForgeEV.obj :  $(SOURCE)  $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ForgeRA.cpp
DEP_FORGER=\
	.\FORGERES.H\
	.\RANDDEV.H\
	.\ROULETTE.H\
	.\BOOL.H\
	.\FSM.H\
	.\RAOptCfg.h\
	.\DIAGNOSE.H\
	.\SIMPLE.H

$(INTDIR)/ForgeRA.obj :  $(SOURCE)  $(DEP_FORGER) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RAOptCfg.cpp

$(INTDIR)/RAOptCfg.obj :  $(SOURCE)  $(INTDIR)

# End Source File
# End Group
# End Project
################################################################################
