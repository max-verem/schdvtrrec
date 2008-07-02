# Microsoft Developer Studio Project File - Name="SchdVtrRec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SchdVtrRec - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SchdVtrRec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SchdVtrRec.mak" CFG="SchdVtrRec - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SchdVtrRec - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SchdVtrRec - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SchdVtrRec - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../temp/Release"
# PROP Intermediate_Dir "../temp/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "C:\projects\airforce\src\vtr_cmd" /I "C:\projects\airforce\src\vtr_srv" /I "C:\projects\airforce\src\mytimecode" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x422 /d "NDEBUG"
# ADD RSC /l 0x422 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../install/SchdVtrRec.exe" /libpath:"C:\projects\airforce\temp\Release\vtr_cmd" /libpath:"C:\projects\airforce\temp\Release\vtr_srv" /libpath:"C:\projects\airforce\temp\Release\mytimecode"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SchdVtrRec - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../temp/Debug"
# PROP Intermediate_Dir "../temp/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "C:\projects\airforce\src\vtr_cmd" /I "C:\projects\airforce\src\vtr_srv" /I "C:\projects\airforce\src\mytimecode" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x422 /d "_DEBUG"
# ADD RSC /l 0x422 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../install/SchdVtrRec_d.exe" /pdbtype:sept /libpath:"C:\projects\airforce\temp\Debug\vtr_cmd" /libpath:"C:\projects\airforce\temp\Debug\vtr_srv" /libpath:"C:\projects\airforce\temp\Debug\mytimecode"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SchdVtrRec - Win32 Release"
# Name "SchdVtrRec - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SchdVtrRec.cpp
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRec.rc
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRecConf.cpp
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRecDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRecIni.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRec.h
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRecConf.h
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRecDlg.h
# End Source File
# Begin Source File

SOURCE=.\SchdVtrRecIni.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\digits\0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digits\9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\digits\SC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SchdVtrRec.ico
# End Source File
# Begin Source File

SOURCE=.\res\SchdVtrRec.rc2
# End Source File
# Begin Source File

SOURCE=.\res\digits\SP.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project