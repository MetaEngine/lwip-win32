# Microsoft Developer Studio Project File - Name="lwipwin32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=lwipwin32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lwipwin32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lwipwin32.mak" CFG="lwipwin32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lwipwin32 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "lwipwin32 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lwipwin32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./lwip/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "lwipwin32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./lwip/include" /I "./lwip/include/ipv4" /I "./pcap/Include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "LWIP_DEBUG" /FR /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "lwipwin32 - Win32 Release"
# Name "lwipwin32 - Win32 Debug"
# Begin Group "console"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\lwipwin32.c
# End Source File
# End Group
# Begin Group "lwip"

# PROP Default_Filter ""
# Begin Group "api"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lwip\api\api_lib.c
# End Source File
# Begin Source File

SOURCE=.\lwip\api\api_msg.c
# End Source File
# Begin Source File

SOURCE=.\lwip\api\err.c
# End Source File
# Begin Source File

SOURCE=.\lwip\api\netbuf.c
# End Source File
# Begin Source File

SOURCE=.\lwip\api\netdb.c
# End Source File
# Begin Source File

SOURCE=.\lwip\api\netifapi.c
# End Source File
# Begin Source File

SOURCE=.\lwip\api\sockets.c
# End Source File
# Begin Source File

SOURCE=.\lwip\api\tcpip.c
# End Source File
# End Group
# Begin Group "core"

# PROP Default_Filter ""
# Begin Group "ipv4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lwip\core\ipv4\autoip.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\ipv4\icmp.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\ipv4\igmp.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\ipv4\inet.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\ipv4\inet_chksum.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\ipv4\ip.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\ipv4\ip_addr.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\ipv4\ip_frag.c
# End Source File
# End Group
# Begin Group "snmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lwip\core\snmp\asn1_dec.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\snmp\asn1_enc.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\snmp\mib2.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\snmp\mib_structs.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\snmp\msg_in.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\snmp\msg_out.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\lwip\core\dhcp.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\dns.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\init.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\mem.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\memp.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\netif.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\pbuf.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\raw.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\stats.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\sys.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\tcp.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\tcp_in.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\tcp_out.c
# End Source File
# Begin Source File

SOURCE=.\lwip\core\udp.c
# End Source File
# End Group
# Begin Group "netif"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lwip\netif\etharp.c
# End Source File
# Begin Source File

SOURCE=.\lwip\netif\ethernetif.c
# End Source File
# Begin Source File

SOURCE=.\lwip\netif\loopif.c
# End Source File
# End Group
# Begin Group "arch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lwip\arch\sys_arch.c
# End Source File
# End Group
# End Group
# Begin Group "pcap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pcap\pcapif.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\pcap\Lib\wpcap.lib
# End Source File
# End Target
# End Project
