# Microsoft Developer Studio Project File - Name="hnmloader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=hnmloader - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hnmloader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hnmloader.mak" CFG="hnmloader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hnmloader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "hnmloader - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hnmloader - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../tmp/hnmloader/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../bin/d2hackmap.exe"

!ELSEIF  "$(CFG)" == "hnmloader - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../tmp/hnmloader/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../bin/d2hackmap.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "hnmloader - Win32 Release"
# Name "hnmloader - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GameWalker.cpp
# End Source File
# Begin Source File

SOURCE=.\hnmloader.cpp
# End Source File
# Begin Source File

SOURCE=.\hnmloader.rc
# End Source File
# Begin Source File

SOURCE=.\IntegrityWalker.cpp
# End Source File
# Begin Source File

SOURCE=.\ListViewBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ListViewDiabloII.cpp
# End Source File
# Begin Source File

SOURCE=.\ListViewImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\ListViewIntegrityScan.cpp
# End Source File
# Begin Source File

SOURCE=.\ListViewModule.cpp
# End Source File
# Begin Source File

SOURCE=.\MaphackChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleWalker.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteExecute.cpp
# End Source File
# Begin Source File

SOURCE=.\RevealMapWorker.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\winAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\winMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\winProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\winTabControl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoHandle.h
# End Source File
# Begin Source File

SOURCE=.\AutoList.h
# End Source File
# Begin Source File

SOURCE=.\AutoToolTip.h
# End Source File
# Begin Source File

SOURCE=.\ConfigLoader.h
# End Source File
# Begin Source File

SOURCE=.\CoolButton.h
# End Source File
# Begin Source File

SOURCE=.\DialogBase.h
# End Source File
# Begin Source File

SOURCE=.\GameWalker.h
# End Source File
# Begin Source File

SOURCE=.\IntegrityWalker.h
# End Source File
# Begin Source File

SOURCE=.\ListViewBase.h
# End Source File
# Begin Source File

SOURCE=.\ListViewDiabloII.h
# End Source File
# Begin Source File

SOURCE=.\ListViewImpl.h
# End Source File
# Begin Source File

SOURCE=.\ListViewIntegrityScan.h
# End Source File
# Begin Source File

SOURCE=.\ListViewModule.h
# End Source File
# Begin Source File

SOURCE=.\MaphackChecker.h
# End Source File
# Begin Source File

SOURCE=.\MemoryHelper.h
# End Source File
# Begin Source File

SOURCE=.\men_fun_bind.h
# End Source File
# Begin Source File

SOURCE=.\ModuleLoader.h
# End Source File
# Begin Source File

SOURCE=.\ModuleWalker.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\RemoteExecute.h
# End Source File
# Begin Source File

SOURCE=.\RevealMapWorker.h
# End Source File
# Begin Source File

SOURCE=.\SocketSystem.h
# End Source File
# Begin Source File

SOURCE=.\ToolTip.h
# End Source File
# Begin Source File

SOURCE=.\winAbout.h
# End Source File
# Begin Source File

SOURCE=.\WinMain.h
# End Source File
# Begin Source File

SOURCE=.\winMenu.h
# End Source File
# Begin Source File

SOURCE=.\winProperties.h
# End Source File
# Begin Source File

SOURCE=.\winTabControl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\32x32x16_OkBor.ico
# End Source File
# Begin Source File

SOURCE=.\res\32x32x16_Screw.ico
# End Source File
# Begin Source File

SOURCE=.\res\aboutme.ico
# End Source File
# Begin Source File

SOURCE=.\res\CancelBor.ico
# End Source File
# Begin Source File

SOURCE=.\res\d2x.ico
# End Source File
# Begin Source File

SOURCE=.\res\editconfig.ico
# End Source File
# Begin Source File

SOURCE=.\res\exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\install.ico
# End Source File
# Begin Source File

SOURCE=.\res\iscan.ico
# End Source File
# Begin Source File

SOURCE=.\res\OkBor.ico
# End Source File
# Begin Source File

SOURCE=.\res\portrait.ico
# End Source File
# Begin Source File

SOURCE=.\res\revealac.ico
# End Source File
# Begin Source File

SOURCE=.\res\TrayIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\unload.ico
# End Source File
# End Group
# End Target
# End Project
