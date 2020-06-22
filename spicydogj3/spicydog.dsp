# Microsoft Developer Studio Project File - Name="Spicydog" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Spicydog - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "spicydog.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "spicydog.mak" CFG="Spicydog - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Spicydog - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "Spicydog - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Spicydog - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/release"
# PROP Intermediate_Dir "../temp/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /WX /Gm /Gi /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FAs /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FAcs /Fr /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /pdb:none /map:"../output/release.map" /machine:I386 /MAPINFO:EXPORTS /MAPINFO:LINES
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy dll\*.* .\..\output\release	copy dll\release\*.* .\..\output\release	copy newdocument.sdb .\..\output\release	copy index.chm .\..\output\release	xcopy /E /I /Y sdp .\..\output\release\sdp
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Spicydog - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../output/debug"
# PROP Intermediate_Dir "../temp/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /Gi /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FAs /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=DLLをコピー
PostBuild_Cmds=copy dll\*.* .\..\output\debug	copy dll\debug\*.* .\..\output\debug	xcopy /E /I /Y sdp .\..\output\debug\sdp	copy index.chm .\..\output\debug	copy newdocument.sdb .\..\output\debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Spicydog - Win32 Release"
# Name "Spicydog - Win32 Debug"
# Begin Group "xml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xml\xml.h
# End Source File
# End Group
# Begin Group "sdom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sdom\DOMImpl.h
# End Source File
# Begin Source File

SOURCE=.\sdom\MFCUnserializer.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\MFCUnserializer.h
# End Source File
# Begin Source File

SOURCE=.\sdom\OLM.CPP
# End Source File
# Begin Source File

SOURCE=.\sdom\OLM.H
# End Source File
# Begin Source File

SOURCE=.\sdom\OLMAlloc.h
# End Source File
# Begin Source File

SOURCE=.\sdom\OLMTest.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMArchive.h
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMBuffer.h
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMEnum.h
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMException.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMException.h
# End Source File
# Begin Source File

SOURCE=.\sdom\sdomn.h
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMReader.h
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMSerializable.h
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMTest.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\SDOMWriter.h
# End Source File
# Begin Source File

SOURCE=.\sdom\Ver3SDOMReader.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\Ver3SDOMReader.h
# End Source File
# Begin Source File

SOURCE=.\sdom\VerJ3SDOMReader.cpp
# End Source File
# Begin Source File

SOURCE=.\sdom\VerJ3SDOMReader.h
# End Source File
# End Group
# Begin Group "model"

# PROP Default_Filter ""
# Begin Group "char"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\model\char\BtnChar.cpp
# End Source File
# Begin Source File

SOURCE=.\model\char\BtnChar.h
# End Source File
# Begin Source File

SOURCE=.\model\char\Char.cpp
# End Source File
# Begin Source File

SOURCE=.\model\char\Char.h
# End Source File
# Begin Source File

SOURCE=.\model\char\CharDef.h
# End Source File
# Begin Source File

SOURCE=.\model\char\CharList.cpp
# End Source File
# Begin Source File

SOURCE=.\model\char\CharList.h
# End Source File
# Begin Source File

SOURCE=.\model\char\PictChar.cpp
# End Source File
# Begin Source File

SOURCE=.\model\char\PictChar.h
# End Source File
# Begin Source File

SOURCE=.\model\char\SndChar.cpp
# End Source File
# Begin Source File

SOURCE=.\model\char\SndChar.h
# End Source File
# Begin Source File

SOURCE=.\model\char\TextChar.cpp
# End Source File
# Begin Source File

SOURCE=.\model\char\TextChar.h
# End Source File
# End Group
# Begin Group "stage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\model\stage\Animating.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\Animating.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\Coloring.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\Coloring.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\KeyFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\KeyFrame.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\KeyFrameKeepers.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\KeyFrameKeepers.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\KeyFrameOwners.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\KeyFrameOwners.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\Label.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\Layer.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\Layer.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\MainStage.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\MainStage.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\Placing.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\Placing.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\Stage.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\Stage.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\SubStage.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\SubStage.h
# End Source File
# Begin Source File

SOURCE=.\model\stage\SubStages.cpp
# End Source File
# Begin Source File

SOURCE=.\model\stage\SubStages.h
# End Source File
# End Group
# Begin Group "build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\model\build\Buildable.cpp
# End Source File
# Begin Source File

SOURCE=.\model\build\Buildable.h
# End Source File
# Begin Source File

SOURCE=.\model\build\Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\model\build\Builder.h
# End Source File
# Begin Source File

SOURCE=.\model\build\BuildOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\model\build\BuildOutput.h
# End Source File
# Begin Source File

SOURCE=.\model\build\BuildTest.cpp
# End Source File
# Begin Source File

SOURCE=.\model\build\sdc.h
# End Source File
# End Group
# Begin Group "type"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\model\type\Act.cpp
# End Source File
# Begin Source File

SOURCE=.\model\type\Act.h
# End Source File
# Begin Source File

SOURCE=.\model\type\FPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\model\type\FPoint.h
# End Source File
# Begin Source File

SOURCE=.\model\type\FRect.cpp
# End Source File
# Begin Source File

SOURCE=.\model\type\FRect.h
# End Source File
# Begin Source File

SOURCE=.\model\type\FSize.cpp
# End Source File
# Begin Source File

SOURCE=.\model\type\FSize.h
# End Source File
# Begin Source File

SOURCE=.\model\type\type.cpp
# End Source File
# Begin Source File

SOURCE=.\model\type\type.h
# End Source File
# End Group
# Begin Group "action"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\model\action\Action.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\Action.h
# End Source File
# Begin Source File

SOURCE=.\model\action\ActionFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\ActionFactory.h
# End Source File
# Begin Source File

SOURCE=.\model\action\ActionList.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\ActionList.h
# End Source File
# Begin Source File

SOURCE=.\model\action\DelayJumpAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\DelayJumpAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\JumpAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\JumpAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\PlayAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\PlayAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\QuitAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\QuitAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\RandomJumpAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\RandomJumpAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\SetBackgroundColorAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\SetBackgroundColorAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\SetBacklightAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\SetBacklightAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\SetSoftkeyAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\SetSoftkeyAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\SetVibrationAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\SetVibrationAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\SimpleAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\SimpleAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\SoundPlayAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\SoundPlayAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\SoundStopAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\SoundStopAction.h
# End Source File
# Begin Source File

SOURCE=.\model\action\StopAction.cpp
# End Source File
# Begin Source File

SOURCE=.\model\action\StopAction.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\model\AffineMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\model\AffineMatrix.h
# End Source File
# Begin Source File

SOURCE=.\model\Appli.cpp
# End Source File
# Begin Source File

SOURCE=.\model\Appli.h
# End Source File
# Begin Source File

SOURCE=.\model\AppliConf.cpp
# End Source File
# Begin Source File

SOURCE=.\model\AppliConf.h
# End Source File
# Begin Source File

SOURCE=.\model\Selectable.cpp
# End Source File
# Begin Source File

SOURCE=.\model\Selectable.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\ltkrn_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib\LTDIS_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib\ltimg_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib\ltdic_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib\Ltwvc_n.lib
# End Source File
# Begin Source File

SOURCE=.\lib\cppunitd.lib

!IF  "$(CFG)" == "Spicydog - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Spicydog - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\lib\xerces-c_2D.lib"

!IF  "$(CFG)" == "Spicydog - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Spicydog - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\lib\xerces-c_2.lib"

!IF  "$(CFG)" == "Spicydog - Win32 Release"

!ELSEIF  "$(CFG)" == "Spicydog - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\lib\cppunit.lib

!IF  "$(CFG)" == "Spicydog - Win32 Release"

!ELSEIF  "$(CFG)" == "Spicydog - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "utility"

# PROP Default_Filter ""
# Begin Group "utility/toolbar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\utility\toolbar\DialogBarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\toolbar\DialogBarDlg.h
# End Source File
# Begin Source File

SOURCE=.\utility\toolbar\DialogSizingBar.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\toolbar\DialogSizingBar.h
# End Source File
# End Group
# Begin Group "utility/zip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\utility\zip\InfoZip.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\zip\InfoZip.h
# End Source File
# Begin Source File

SOURCE=.\utility\zip\UnzipDLL.h
# End Source File
# Begin Source File

SOURCE=.\utility\zip\ZCallBck.h
# End Source File
# Begin Source File

SOURCE=.\utility\zip\ZipDLL.h
# End Source File
# End Group
# Begin Group "utility/wnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\utility\wnd\BitmapButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\BitmapButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\BitmapStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\BitmapStatic.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\ColorBox.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\ColorBox.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\ColorFaceButton.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\ColorFaceButton.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\FontStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\FontStatic.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\HyperLinkLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\HyperLinkLabel.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\InToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\InToolBar.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\InWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\InWnd.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\PreviewStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\PreviewStatic.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\SplashWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\SplashWnd.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\SplitterCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\SplitterCtrl.h
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\ToolBarWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\wnd\ToolBarWnd.h
# End Source File
# End Group
# Begin Group "utility/smtp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\utility\smtp\AppOctetStream.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\AppOctetStream.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\Base64.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\Base64.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\kconv.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\kconv.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MailMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MailMessage.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MIMECode.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MIMECode.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MIMEContentAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MIMEContentAgent.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MIMEMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\MIMEMessage.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\rfc1468.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\rfc1468.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\SMTP.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\SMTP.h
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\smtpTest.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\TextPlain.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\smtp\TextPlain.h
# End Source File
# End Group
# Begin Group "utility/http"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\utility\http\HttpPost.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\http\HttpPost.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\utility\Accel.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\Accel.h
# End Source File
# Begin Source File

SOURCE=.\utility\AppBugDump.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\AppBugDump.h
# End Source File
# Begin Source File

SOURCE=.\utility\Bezier.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\bezier.h
# End Source File
# Begin Source File

SOURCE=.\utility\ChildDataExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\ChildDataExchange.h
# End Source File
# Begin Source File

SOURCE=.\utility\clinit.h
# End Source File
# Begin Source File

SOURCE=.\utility\CmdExec.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\CmdExec.h
# End Source File
# Begin Source File

SOURCE=.\utility\CursorCapture.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\CursorCapture.h
# End Source File
# Begin Source File

SOURCE=.\utility\DDXUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\DDXUtility.h
# End Source File
# Begin Source File

SOURCE=.\utility\DocumentEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\DocumentEvent.h
# End Source File
# Begin Source File

SOURCE=.\utility\EventPublisher.h
# End Source File
# Begin Source File

SOURCE=.\utility\FileUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\FileUtility.h
# End Source File
# Begin Source File

SOURCE=.\utility\FolderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\FolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\utility\Func.h
# End Source File
# Begin Source File

SOURCE=.\utility\HtmlHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\HtmlHelper.h
# End Source File
# Begin Source File

SOURCE=.\utility\LeadToolUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\LeadToolUtility.h
# End Source File
# Begin Source File

SOURCE=.\utility\MemDC.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\utility\MemoryUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\MemoryUtility.h
# End Source File
# Begin Source File

SOURCE=.\utility\NamingRule.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\NamingRule.h
# End Source File
# Begin Source File

SOURCE=.\utility\PaintMemDC.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\PaintMemDC.h
# End Source File
# Begin Source File

SOURCE=.\utility\PxLib.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\PxLib.h
# End Source File
# Begin Source File

SOURCE=.\utility\ReenterLock.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\ReenterLock.h
# End Source File
# Begin Source File

SOURCE=.\utility\RegUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\RegUtility.h
# End Source File
# Begin Source File

SOURCE=.\utility\ScrollControler.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\ScrollControler.h
# End Source File
# Begin Source File

SOURCE=.\utility\StringUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\StringUtility.h
# End Source File
# Begin Source File

SOURCE=.\utility\Tracker.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\Tracker.h
# End Source File
# Begin Source File

SOURCE=.\utility\Utf8.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\Utf8.h
# End Source File
# Begin Source File

SOURCE=.\utility\WndUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility\WndUtility.h
# End Source File
# End Group
# Begin Group "cppunit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cppunit\TextTestResult.h
# End Source File
# End Group
# Begin Group "cmd"

# PROP Default_Filter ""
# Begin Group "cmd/stage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cmd\stage\AttachLayerCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\AttachLayerCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelFramesCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelFramesCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelKeyFrameLabelCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelKeyFrameLabelCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelLayerCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelLayerCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelSubStageCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\DelSubStageCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsFramesCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsFramesCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsKeyFrameCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsKeyFrameCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsKeyFrameLabelCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsKeyFrameLabelCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsLayerCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsLayerCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsSubStageCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\InsSubStageCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\ModKeyFrameCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\ModKeyFrameCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\ModKeyFrameLabelCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\ModKeyFrameLabelCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\MovLayerCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\MovLayerCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\RenLayerCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\RenLayerCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\RenSubStageCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\stage\RenSubStageCmd.h
# End Source File
# End Group
# Begin Group "cmd/char"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cmd\char\DelCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\DelCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\char\DupCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\DupCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\char\InsCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\InsCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\char\ModBtnCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\ModBtnCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\char\ModSndCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\ModSndCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\char\ModTextCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\ModTextCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\char\PictCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\PictCharCmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\char\RenCharCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\char\RenCharCmd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cmd\Cmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\Cmd.h
# End Source File
# Begin Source File

SOURCE=.\cmd\CmdHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\CmdHistory.h
# End Source File
# Begin Source File

SOURCE=.\cmd\CmdSet.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\CmdSet.h
# End Source File
# Begin Source File

SOURCE=.\cmd\CmdTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\CmdTransaction.h
# End Source File
# Begin Source File

SOURCE=.\cmd\ModActionListCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\cmd\ModActionListCmd.h
# End Source File
# End Group
# Begin Group "dlg"

# PROP Default_Filter ""
# Begin Group "dlg/action"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dlg\action\ActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\ActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\ActionListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\ActionListDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\DelayJumpActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\DelayJumpActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\JumpActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\JumpActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\RandomJumpActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\RandomJumpActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\RandomJumpCaseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\RandomJumpCaseDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetBackgroundColorActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetBackgroundColorActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetBackLightActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetBacklightActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetSoftkeyActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetSoftkeyActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetVibrationActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SetVibrationActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SoundPlayActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\action\SoundPlayActionDlg.h
# End Source File
# End Group
# Begin Group "dlg/appliconf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfCanvasPage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfCanvasPage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfMachiukePage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfMachiukePage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfPlayPage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfPlayPage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfTitlePage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfTitlePage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfTypePage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfTypePage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfWizDonePage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfWizDonePage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfWizHomePage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\appliconf\AppliConfWizHomePage.h
# End Source File
# End Group
# Begin Group "dlg/build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dlg\build\BuildOutputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\build\BuildOutputDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\build\EmuPlayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\build\EmuPlayDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\build\UploadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\build\UploadDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\build\UploadedDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\build\UploadedDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\build\UploadingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\build\UploadingDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\build\ViewADFDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\build\ViewADFDlg.h
# End Source File
# End Group
# Begin Group "dlg/char"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dlg\char\BtnCharMailEventDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\BtnCharMailEventDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\BtnCharScheduleEventDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\BtnCharScheduleEventDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\BtnCharTelEventDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\BtnCharTelEventDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\CharListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\CharListDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\EditPictQuantizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\EditPictQuantizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\ImportPictCharDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\ImportPictCharDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\NewEmptyPictCharDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\NewEmptyPictCharDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\PictEffectPictDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\PictEffectPictDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\PictResizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\PictResizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\char\PictTrimDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\char\PictTrimDlg.h
# End Source File
# End Group
# Begin Group "dlg/pref"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dlg\pref\GridEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\GridEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefEmuPage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefEmuPage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefEtcPage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefEtcPage.h
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefUploadPage.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\pref\PrefUploadPage.h
# End Source File
# End Group
# Begin Group "dlg/stage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dlg\stage\EditAnimatingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditAnimatingDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringCustomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringCustomDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringMeidoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringMeidoDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditColoringSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditPlacingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\EditPlacingDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\KeyFrameActionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\KeyFrameActionDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\KeyFrameLabelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\KeyFrameLabelDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\RenLayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\RenLayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\RenSubStageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\RenSubStageDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\StagesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\StagesDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\TimelineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\TimelineDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\TimelineDlgConf.h
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\TimelineDlgFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\stage\TimelineDlgLayer.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\dlg\AppBugDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\AppBugDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\ColorPaletteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\ColorPaletteDlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\historydlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\historydlg.h
# End Source File
# Begin Source File

SOURCE=.\dlg\VersionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dlg\VersionDlg.h
# End Source File
# End Group
# Begin Group "wnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\wnd\acttablectrl.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\acttablectrl.h
# End Source File
# Begin Source File

SOURCE=.\wnd\AppliFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\AppliFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\wnd\BuildBar.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\BuildBar.h
# End Source File
# Begin Source File

SOURCE=.\wnd\CharListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\CharListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\wnd\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\wnd\ScaleBox.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\ScaleBox.h
# End Source File
# Begin Source File

SOURCE=.\wnd\ScrollViewEx.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\ScrollViewEx.h
# End Source File
# Begin Source File

SOURCE=.\wnd\ToolsBoxBar.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\ToolsBoxBar.h
# End Source File
# Begin Source File

SOURCE=.\wnd\ViewSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\ViewSwitch.h
# End Source File
# Begin Source File

SOURCE=.\wnd\ViewSwitchWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\ViewSwitchWnd.h
# End Source File
# End Group
# Begin Group "view"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\view\BtnCharView.cpp
# End Source File
# Begin Source File

SOURCE=.\view\BtnCharView.h
# End Source File
# Begin Source File

SOURCE=.\view\CharView.cpp
# End Source File
# Begin Source File

SOURCE=.\view\CharView.h
# End Source File
# Begin Source File

SOURCE=.\view\PictCharView.cpp
# End Source File
# Begin Source File

SOURCE=.\view\PictCharView.h
# End Source File
# Begin Source File

SOURCE=.\view\SndCharView.cpp
# End Source File
# Begin Source File

SOURCE=.\view\SndCharView.h
# End Source File
# Begin Source File

SOURCE=.\view\StageView.cpp
# End Source File
# Begin Source File

SOURCE=.\view\StageView.h
# End Source File
# Begin Source File

SOURCE=.\view\TextCharView.cpp
# End Source File
# Begin Source File

SOURCE=.\view\TextCharView.h
# End Source File
# End Group
# Begin Group "cnt"

# PROP Default_Filter ""
# Begin Group "cnt/stage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cnt\stage\MovCharCnt.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt\stage\MovCharCnt.h
# End Source File
# Begin Source File

SOURCE=.\cnt\stage\StageCnt.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt\stage\StageCnt.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cnt\Cnt.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt\Cnt.h
# End Source File
# Begin Source File

SOURCE=.\cnt\CntMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt\CntMgr.h
# End Source File
# Begin Source File

SOURCE=.\cnt\PictCnt.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt\PictCnt.H
# End Source File
# Begin Source File

SOURCE=.\cnt\ScrollViewCnt.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt\ScrollViewCnt.h
# End Source File
# End Group
# Begin Group "res"

# PROP Default_Filter ""
# Begin Group "res/ico"

# PROP Default_Filter "ico"
# Begin Source File

SOURCE=.\res\ico\appli.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\bold.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\document.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\fontstyl.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\italic.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key0.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key2.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key3.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key4.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key5.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key6.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key7.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key8.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\key9.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keyapp.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keyaster.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keydown.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keyleft.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keyright.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keyselec.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keysharp.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keysoft1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keysoft2.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\keyup.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\MSGBOX03.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ico\MSGBOX04.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ico\SDB.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\SDBDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\speaker_.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_b.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_c.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_l.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_lb.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_lt.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_r.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_rb.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_rt.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico\trim_t.ico
# End Source File
# End Group
# Begin Group "res/cur"

# PROP Default_Filter "cur"
# Begin Source File

SOURCE=.\res\cur\center.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\draw_rec.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\drawdot.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\pointer.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\sel_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\sel_rect.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\spoit.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\viewmove.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur\viewzoom.CUR
# End Source File
# End Group
# Begin Group "res/txt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\txt\LICENSE_REMARKS.TXT
# End Source File
# Begin Source File

SOURCE=.\res\txt\STAFF.TXT
# End Source File
# End Group
# Begin Group "res/bmp"

# PROP Default_Filter "bmp"
# Begin Source File

SOURCE=.\res\bmp\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bmphdr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bn_jphone.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bn_spicydog.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\bn_spicysoft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\build.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\chartype.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\charview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\clr_clos.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\clr_dlg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\clr_null.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\clr_spoi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\command.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\history.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\icolayer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\idt_buil.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\idt_dictionary.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\idt_frame.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\idt_laye.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\idt_pict.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\idt_shav.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\idt_stag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\inschar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\itb_etc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\itb_history.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\layerchar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\line.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\paint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\paint1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\paint2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\paint3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\play.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\stages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\tlogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp\toolbar2.bmp
# End Source File
# End Group
# Begin Group "res/act"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\act\gray.act
# End Source File
# Begin Source File

SOURCE=.\res\act\imode.act
# End Source File
# Begin Source File

SOURCE=.\res\act\mac.act
# End Source File
# Begin Source File

SOURCE=.\res\act\mono.act
# End Source File
# Begin Source File

SOURCE=.\res\act\rgb.act
# End Source File
# Begin Source File

SOURCE=.\res\act\web.act
# End Source File
# Begin Source File

SOURCE=.\res\act\win.act
# End Source File
# End Group
# Begin Source File

SOURCE=.\english.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "smaf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\smaf\masmw.cpp
# End Source File
# Begin Source File

SOURCE=.\smaf\masmw.h
# End Source File
# Begin Source File

SOURCE=.\smaf\masmwlib.h
# End Source File
# Begin Source File

SOURCE=.\smaf\SMAFPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\smaf\SMAFPlayer.h
# End Source File
# Begin Source File

SOURCE=.\smaf\SMAFPlayerListener.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Jphone.cpp
# End Source File
# Begin Source File

SOURCE=.\Jphone.h
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\Spicydog.cpp
# End Source File
# Begin Source File

SOURCE=.\Spicydog.h
# End Source File
# Begin Source File

SOURCE=.\SpicydogDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SpicydogDoc.h
# End Source File
# Begin Source File

SOURCE=.\SpicydogDocTempl.cpp
# End Source File
# Begin Source File

SOURCE=.\SpicydogDocTempl.h
# End Source File
# Begin Source File

SOURCE=.\SpicydogPref.cpp
# End Source File
# Begin Source File

SOURCE=.\SpicydogPref.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\test.cpp
# End Source File
# End Target
# End Project
