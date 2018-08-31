# Microsoft Developer Studio Project File - Name="HLE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HLE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HLE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HLE.mak" CFG="HLE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HLE - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HLE - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HLE - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\Bin\HL409.dll"

!ELSEIF  "$(CFG)" == "HLE - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\Bin\HL409_D.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HLE - Win32 Release"
# Name "HLE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\HLE.cpp
# End Source File
# Begin Source File

SOURCE=.\HLE.def
# End Source File
# Begin Source File

SOURCE=.\HLE.rc
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

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\10.ico
# End Source File
# Begin Source File

SOURCE=.\res\align_bo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\align_ce.bmp
# End Source File
# Begin Source File

SOURCE=.\res\align_le.bmp
# End Source File
# Begin Source File

SOURCE=.\res\align_mi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\align_ri.bmp
# End Source File
# Begin Source File

SOURCE=.\res\align_to.bmp
# End Source File
# Begin Source File

SOURCE=.\res\arrange.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BMP00002.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\click.cur
# End Source File
# Begin Source File

SOURCE=.\res\ctrl_bl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctrl_br.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctrl_lr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctrl_pt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctrl_tb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctrl_tr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctrl_upl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_c.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_s.cur
# End Source File
# Begin Source File

SOURCE=.\res\d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DcpOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\DcpOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\Diagonal2.cur
# End Source File
# Begin Source File

SOURCE=.\res\Drag.CUR
# End Source File
# Begin Source File

SOURCE=.\res\draw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\H_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\hand_cur.cur
# End Source File
# Begin Source File

SOURCE=.\res\HL.ico
# End Source File
# Begin Source File

SOURCE=.\res\HLDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\HLE.rc2
# End Source File
# Begin Source File

SOURCE=.\res\horizontal.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00016.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00017.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00018.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00019.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00020.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00021.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00022.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00023.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00024.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00025.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00026.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00027.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00028.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00029.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00030.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00031.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00032.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon10.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon11.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon12.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon13.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon14.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon15.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon16.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon17.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon18.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon19.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon20.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon22.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon23.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon24.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon25.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon26.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon8.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon_122.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_arc.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_cir.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon_con.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon_dot.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_ell.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_fil.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_las.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon_mar.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon_mot.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon_non.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_spl.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_uni.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_xy1.ico
# End Source File
# Begin Source File

SOURCE=.\res\loop.ico
# End Source File
# Begin Source File

SOURCE=.\RES\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MLine.cur
# End Source File
# Begin Source File

SOURCE=.\res\MlineH.cur
# End Source File
# Begin Source File

SOURCE=.\res\mouserot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\move.cur
# End Source File
# Begin Source File

SOURCE=.\res\Normal.cur
# End Source File
# Begin Source File

SOURCE=.\res\objchang.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\objchange.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Precision.cur
# End Source File
# Begin Source File

SOURCE=.\res\scale.cur
# End Source File
# Begin Source File

SOURCE=.\res\scale_hi.ico
# End Source File
# Begin Source File

SOURCE=.\res\sel_rect.ico
# End Source File
# Begin Source File

SOURCE=.\res\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabimage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Title.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Trffc20.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\DcpConnct.avi
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
