#include "stdafx.h"
#include "resource.h"
#include "Spicydog.h"

#include "wnd/MainFrame.h"
#include <afxsock.h>
#include <atlconv.h>
#include <ltwrappr.h>
#include <memory>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "dlg/AppBugDlg.h"
#include "dlg/VersionDlg.h"
#include "model/Appli.h"
#include "model/build/Builder.h"
#include "model/build/BuildOutput.h"
#include "SpicydogDoc.h"
#include "SpicydogPref.h"
#include "SpicydogDocTempl.h"
#include "sdom/Ver3SDOMReader.h"
#include "sdom/SDOMArchive.h"
#include "utility/AppBugDump.h"
#include "utility/StringUtility.h"
#include "utility/WndUtility.h"
#include "utility/wnd/SplashWnd.h"
#include "wnd/AppliFrameWnd.h"
#include "view/StageView.h"

#if defined(_DEBUG)
#pragma comment(lib,"xerces-c_2D.lib")
#else
#pragma comment(lib,"xerces-c_2.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if defined(_DEBUG)
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#endif

const LPCTSTR SECTIONNAME_WINDOWPLACEMENT  = "WindowPlacement";
const LPCTSTR ENTRYNAME_MAINFRAME          = "MainFrame";

BEGIN_MESSAGE_MAP(SpicydogApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW,        CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN,       CWinApp::OnFileOpen)
END_MESSAGE_MAP()

LONG __stdcall DumpApp(PEXCEPTION_POINTERS pInfo);
SpicydogApp theApp;

#if defined(_DEBUG)
void test();
#endif

SpicydogApp::SpicydogApp()
:	m_nFileNew(0),
	m_ddestate(ddestateNone) {
	_tsetlocale(LC_ALL,_T("jpn"));

#if !defined(_DEBUG)
	::SetUnhandledExceptionFilter(DumpApp);
#endif
}

BOOL SpicydogApp::InitInstance() {
	if (!InitEnv()) {
		return FALSE;
	}

	CraeteDocTemplates();
	InitShell();

#if !defined(_DEBUG)
	SplashWnd::DoSplash(IDB_SPLASH);
#endif
	if (!InitPref()) {
		return FALSE;
	}
	ACTIVE_PREF.LoadConfig(this);
#if defined(_DEBUG)
	test();
	// runTest();
#endif
	if (!InitMainWnd()) {
		return FALSE;
	}
	ShowMainWnd();
#if !defined(_DEBUG)
	SplashWnd::EndSplash(1000);
#endif

	if (!ProcessShell()) {
		return FALSE;
	}
	if (m_ddestate == ddestateWaitOpen) {
		return TRUE;
	}
	ASSERT(m_ddestate == ddestateNone);
	return TRUE;
}

BOOL SpicydogApp::OnIdle(LONG lCount) {
	if (m_ddestate == ddestateOpened) {
		m_ddestate = ddestateNone;
	}
	return CWinApp::OnIdle(lCount);
}

BOOL SpicydogApp::InitEnv() {
	DEBUG_ONLY(AfxEnableMemoryTracking(afxMemDF | delayFreeMemDF | checkAlwaysMemDF));
	if (!AfxSocketInit()) {
		AfxMessageBox(IDS_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	if (!AfxOleInit()) {
		return FALSE;
	}
	AfxEnableControlContainer();
	VERIFY(LBase::LoadLibraries(LT_ALL_LEADLIB) != 0);
	LSettings settings;
	settings.UnlockSupport(L_SUPPORT_GIFLZW,"sg8Z2XkjL");

    XMLPlatformUtils::Initialize();

#ifdef _AFXDLL
	Enable3dControls();		
#else
	Enable3dControlsStatic();	
#endif
	
	CF_LAYERS = ::RegisterClipboardFormat("/spicydogj3/layers");
	CF_CHARS  = ::RegisterClipboardFormat("/spicydogj3/chars");
	
	return TRUE;
}

void SetRegistryKey(CWinApp* pApp,LPCTSTR ptszCompanyName,LPCTSTR ptszAppName){
	ASSERT_VALID(pApp);
	ASSERT(pApp->m_pszRegistryKey == NULL);
	ASSERT(pApp->m_pszAppName != NULL);
	ASSERT(ptszCompanyName != NULL);

	BOOL bEnable = AfxEnableMemoryTracking(FALSE);
	free((void*)pApp->m_pszRegistryKey);
	pApp->m_pszRegistryKey = _tcsdup(ptszCompanyName);
	free((void*)pApp->m_pszProfileName);
	pApp->m_pszProfileName = _tcsdup(ptszAppName);
	AfxEnableMemoryTracking(bEnable);
}

void SetRegistryKey(CWinApp* pApp,UINT nComID,UINT nAppID) {
	ASSERT_VALID(pApp);
	SetRegistryKey(pApp,
		           LoadString(nComID).Left(255),
		           LoadString(nAppID).Left(255));
}

BOOL SpicydogApp::InitPref() {
	::SetRegistryKey(this,LoadString(IDS_REGISTRY_KEY_COMPANY),
						  LoadString(IDS_REGISTRY_KEY_APPLICATION));
	LoadStdProfileSettings(6);
	return TRUE;
}

BOOL SpicydogApp::InitMainWnd() {
	MainFrame* frame = new MainFrame;
	if (!frame->LoadFrame(IDR_MAINFRAME)) {
		return FALSE;
	}
	m_pMainWnd = frame;
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

void SpicydogApp::InitShell() {
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
}

BOOL SpicydogApp::ProcessShell() {
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE){ 
		m_ddestate = ddestateWaitOpen;
	}
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	return TRUE;
}

BOOL SpicydogApp::ShowMainWnd() {
	LoadWindowPlacement(m_pMainWnd,SECTIONNAME_WINDOWPLACEMENT,ENTRYNAME_MAINFRAME);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int SpicydogApp::ExitInstance() {
	SpicydogPref::SaveConfig(this);
	LBase::UnloadLibraries(LT_ALL_LEADLIB);
	XMLPlatformUtils::Terminate();

	return CWinApp::ExitInstance();
}

void SpicydogApp::UpdateApplication() {
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos != NULL) {
		CDocTemplate* templ = GetNextDocTemplate(pos);
		ASSERT_VALID(templ);

		POSITION pos2 = templ->GetFirstDocPosition();
		while (pos2 != NULL) {
			SpicydogDoc* document = DYNAMIC_DOWNCAST(SpicydogDoc,templ->GetNextDoc(pos2));
			ASSERT_VALID(document);
			document->UpdateAll(NULL,SpicydogDoc::UPDATE_APPLICATION);
		}
	}
}

void SpicydogApp::OnAppAbout() {
	VersionDlg aboutDlg;
	aboutDlg.DoModal();
}

LONG __stdcall DumpApp(PEXCEPTION_POINTERS pInfo) {
	try {
		AppBugDump dump(pInfo);
		AppBugDlg dlg(dump);
		dlg.DoModal();

	} catch(CException* pe) {
		pe->ReportError();
		pe->Delete();
	
	} catch(...) {
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void SpicydogApp::CraeteDocTemplates() {
	AddDocTemplate(new SpicydogDocTempl(IDR_SDBTYPE,
		                               RUNTIME_CLASS(SpicydogDoc),
									   RUNTIME_CLASS(AppliFrameWnd),
									   RUNTIME_CLASS(StageView)));
}

#if defined(_DEBUG)
void SpicydogApp::runTest() {
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  CppUnit::Outputter* outputter = new CppUnit::CompilerOutputter(&runner.result(),std::cout);
  runner.setOutputter(outputter);
  runner.run("",true,true,true);
}
#endif

/*  
void build(LPCTSTR sdbPath,LPCTSTR outputDir) {
	std::cout << "Spicydog Builder J3\n"
		      << "InputPath:" << sdbPath   << "\n"
			  << "OutputDir:" << outputDir << "\n";

	typedef std::auto_ptr<DOMDocument> DOCUMENTP;
	typedef std::auto_ptr<SDOMReader> READERP;
	DOCUMENTP document;
	READERP reader3;
	reader3 = READERP(new Ver3SDOMReader);

	CFile input3 (sdbPath,CFile::modeRead);
	document = DOCUMENTP(reader3->Read(&input3));
	input3.Close();

	SDOMArchive archive;
	std::auto_ptr<Appli> appli(new Appli);
	archive.SerializeLoad(document.get(),WSDOMN_SPICYDOG,appli.get());

	BuildOutput output;
	Builder builder(appli.get(),&output);
	builder.Build();
	if (output.GetResult() == BuildOutput::STATE_NORMAL) {
		builder.GenerateJARFile(CString(outputDir) + "\\sdp.jar");	
		builder.GenerateADFFile(CString(outputDir) + "\\sdp.jad","sdp.jar");
		builder.GenerateHTMFile(CString(outputDir) + "\\sdp.htm","sdp.jad");
	}
}
*/
