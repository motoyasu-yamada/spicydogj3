#include "stdafx.h"
#include "resource.h"
#include "SpicydogDoc.h"
#include "Player.h"
#include "SpicydogPref.h"
#include "cmd/Cmd.h"
#include "dlg/appliconf/AppliConfDlg.h"
#include "dlg/build/EmuPlayDlg.h"
#include "dlg/build/UploadDlg.h"
#include "dlg/build/UploadedDlg.h"
#include "dlg/build/UploadingDlg.h"
#include "dlg/build/ViewADFDlg.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "model/build/Builder.h"
#include "sdom/SDOMBuffer.h"
#include "sdom/SDOMException.h"
#include "sdom/SDOMWriter.h"
#include "sdom/Ver3SDOMReader.h"
#include "sdom/VerJ3SDOMReader.h"
#include "utility/DocumentEvent.h"
#include "utility/FileUtility.h"
#include "utility/StringUtility.h"
#include "utility/http/HttpPost.h"
#include "utility/smtp/MailMessage.h"
#include "utility/smtp/RFC1468.h"
#include "utility/smtp/SMTP.h"
#include "wnd/AppliFrameWnd.h"
#include "wnd/MainFrame.h"
#include <AfxInet.h>
#include <memory>
#include <xercesc/dom/DOM.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const LPCTSTR       FNAME_NEW_DOCUMENT = _T("newdocument.sdb");
static const LPCTSTR       USER_AGENT         = "Spicydog3JLITE";
static const LPCTSTR       UPLOAD_HTTP_SERVER = "upload.spicy-dog.com";
static const INTERNET_PORT UPLOAD_HTTP_PORT   = 80;
static const LPCTSTR       UPLOAD_HTTP_OBJECT = "/jlite3/upload.php";

CLIPFORMAT CF_LAYERS;
CLIPFORMAT CF_CHARS;

BEGIN_MESSAGE_MAP(SpicydogDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO_ALL, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO_ALL, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_HISTORY_CLEAR, OnUpdateClearHistory)
	ON_COMMAND(ID_EDIT_UNDO_ALL, OnEditUndoAll)
	ON_COMMAND(ID_EDIT_REDO_ALL, OnEditRedoAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_HISTORY_CLEAR, OnClearHistory)
	ON_COMMAND(ID_APP_CONF,         OnAppConf)
	ON_COMMAND(ID_APP_VIEW_JAD,     OnAppViewADF)
	ON_COMMAND(ID_APP_RUN,          OnEmuPlay)
	ON_COMMAND(ID_APP_PUBLISH_LOCAL,OnPublishLocal)
	ON_COMMAND(ID_APP_PUBLISH_UPLOAD,OnPublishUpload)
	ON_COMMAND(ID_APP_BUILD,OnBuild)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(SpicydogDoc,CDocument,0  | VERSIONABLE_SCHEMA)
SpicydogDoc::SpicydogDoc()
:	m_bUpdatePubMet(TRUE)
{
	appli  = APPLIPTR(new Appli);
	player = PLAYERPTR(new Player(this));
	m_bLoadFromOldVersion = FALSE;
}

SpicydogDoc::~SpicydogDoc() {
	DocumentNotifier::GetNotifier()->NotifyDocumentExpired(this);
}

void SpicydogDoc::OnCloseDocument() {
	DocumentNotifier::GetNotifier()->NotifyDocumentExpired(this);
	CDocument::OnCloseDocument();
}

void SpicydogDoc::DeleteContents()  {
	ClearCmdHistory();
	appli = APPLIPTR(new Appli);
	CDocument::DeleteContents();
}

void SpicydogDoc::OnAppConf() {
	AppliConfDlg dlg(AppliConfDlg::MODE_TAB,*appli->appliConf);
	dlg.DoModal();
	UpdateAll(NULL);
}

void SpicydogDoc::AssertValid() const {
	CDocument::AssertValid();
	ASSERT_VALID(appli.get());
}

void SpicydogDoc::UpdateAll(CView* pSender,LPARAM lHint,CObject* pHint) {
	SetModifiedFlag(TRUE);
	m_bUpdatePubMet = TRUE;
	UpdateAllViews(pSender,lHint,pHint);
	DocumentNotifier::GetNotifier()->UpdateDocument(this,lHint,pHint);
}

void SpicydogDoc::BeginEdit() {
	player->PrepareEdit();
}


void ShowControlBar(CFrameWnd* pFrameWnd,UINT nID) {
	if (pFrameWnd == NULL)
		{ASSERT(FALSE);return;}
	ASSERT_VALID(pFrameWnd);

	CControlBar* pBar = pFrameWnd->GetControlBar(nID);
	if (pBar == NULL)
		{ASSERT(FALSE);return;}
	ASSERT_VALID(pBar);

	pFrameWnd->ShowControlBar(pBar,TRUE, FALSE);
}

BOOL SpicydogDoc::Build(Builder& builder) {
	CWaitCursor wc;

	builder.Build();
	m_bUpdatePubMet = FALSE;
	DocumentNotifier::GetNotifier()->Notify(EVENTNOTIFYCODE_BUILT,this);
	BOOL b = buildOutput.GetResult() == BuildOutput::STATE_NORMAL ||
		     buildOutput.GetResult() == BuildOutput::STATE_WARNING;
	if (!b) {
		CFrameWnd* pFrameWnd = (CFrameWnd*)DYNAMIC_DOWNCAST(CFrameWnd,AfxGetMainWnd());
		if (pFrameWnd != NULL) {
			ShowControlBar(pFrameWnd,ID_VIEW_BUILDOUTPUT);
		} else
			ASSERT(FALSE);
	}

	return b;
}

void SpicydogDoc::OnAppViewADF() {
	BeginEdit();
	
	Builder builder(appli.get(),&buildOutput);
	Build(builder);
	
	ViewADFDlg dlg;
	dlg.m_strJam = builder.GenerateADF("sdp.jar",0);
	dlg.m_strJam.Replace("\n","\r\n");
	dlg.DoModal();
}

void SpicydogDoc::OnBuild() {
	BeginEdit();
	Builder builder(appli.get(),&buildOutput);
	Build(builder);
}

int SpicydogDoc::GetMaxUndoCmd() const {
	int n = ACTIVE_PREF.EtcMaxHistoryCount;
	ASSERT(16 <= n && n < 256);
	return n;
}

void SpicydogDoc::OnEditUndo() {
	if (!CanUndoCmd())
		return;

	UndoCmd();
	UpdateAll(NULL);
}

void SpicydogDoc::OnEditRedo() {
	if (!CanRedoCmd())
		return;
	RedoCmd();
	UpdateAll(NULL);
}

void SpicydogDoc::OnClearHistory() {
	if (GetCmdCount() == 0)
		return;
	ClearCmdHistory();
	UpdateAll(NULL);
}

void SpicydogDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) {
	pCmdUI->Enable(CanRedoCmd());
}
void SpicydogDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) {
	pCmdUI->Enable(CanUndoCmd());
}
void SpicydogDoc::OnUpdateClearHistory(CCmdUI* pCmdUI) {
	pCmdUI->Enable(GetCmdCount() != 0);
}

void SpicydogDoc::OnEditUndoAll() {
	if (!CanUndoCmd())
		return;
	Jump2Cmd(-1);
	UpdateAll(NULL);	
}

void SpicydogDoc::OnEditRedoAll() {
	if (!CanRedoCmd())
		return;
	Jump2Cmd(GetCmdCount()-1);
	UpdateAll(NULL);
}

HACCEL SpicydogDoc::GetDefaultAccelerator() {
	MainFrame* frame = DYNAMIC_DOWNCAST(MainFrame,AfxGetMainWnd());
	ASSERT_VALID(frame);
	return frame->GetActiveAccelerator();
}

BOOL SpicydogDoc::OnNewDocument() {
	BOOL bResult = FALSE;
	if (CDocument::OnOpenDocument(GetAppFilePath(FNAME_NEW_DOCUMENT))) {
		m_strPathName.Empty();
		SetModifiedFlag(FALSE);
		bResult = TRUE;
	} else {
		bResult = CDocument::OnNewDocument();
	}
	return bResult;
}

BOOL SpicydogDoc::OnOpenDocument(LPCTSTR path) {
	ASSERT(AfxIsValidString(path));
	ASSERT(ExistsPath(path));

	return CDocument::OnOpenDocument(path);
}

void SpicydogDoc::OnPublishLocal() {
	BeginEdit();
	Builder builder(appli.get(),&buildOutput);
	if (!Build(builder)) {
		AfxMessageBox(IDS_ERR_PUBLISH_FOR_BUILD_ERR,MB_OK | MB_ICONSTOP);
		return;
	}

	CFileDialog dlg(FALSE,
					LoadString(IDS_JAD_DEFEXT),
					LoadString(IDS_JAD_FILENAME),
					OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
					LoadString(IDS_JAD_FILTER),
					AfxGetMainWnd());

	if (dlg.DoModal() == IDOK) {
		CString directory = GetFileParent(dlg.GetPathName());
		CString jadName   = GetFileName(dlg.GetPathName());
		CString fileName  = GetFileStripedName(jadName);
		CString jarName   = fileName + ".jar";
		builder.GenerateJARFile(CatPath(directory,jarName));	
		builder.GenerateADFFile(CatPath(directory,jadName),jarName);
		builder.GenerateHTMFile(CatPath(directory,fileName + ".htm"),jadName);
	}
}

class UploadException : public CSimpleException {
private:
	CString m_error;
public:
	enum CAUSE { HTTP_ERROR,CORRUPTED_RESULT,ERROR_RESULT};
	UploadException(int cause,int param = 0,LPCTSTR msg = NULL) : CSimpleException(TRUE) {
		DWORD   error;
		CString errmsg;
		switch(cause) {
		case HTTP_ERROR:
			error  = GetLastError();
			errmsg = FormatError(error);
			m_error.Format(IDS_ERR_UPLOAD_HTTP_ERROR,error,errmsg,param);
			break;
		case CORRUPTED_RESULT:
			m_error.Format(IDS_ERR_UPLOAD_CURRUPTED_RESULT);
			break;
		case ERROR_RESULT:
			switch(param) {
			case 200:
				errmsg = LoadString(IDS_ERR_UPLOAD_ERROR_RESULT_200);
				break;
			case 201:
				errmsg = LoadString(IDS_ERR_UPLOAD_ERROR_RESULT_201);
				break;
			case 202:
				errmsg = LoadString(IDS_ERR_UPLOAD_ERROR_RESULT_202);
				break;
			case 300:
				errmsg = LoadString(IDS_ERR_UPLOAD_ERROR_RESULT_300);
				break;
			case 100:
			default:
				ASSERT(FALSE);
				break;
			}
			m_error.Format(IDS_ERR_UPLOAD_ERROR_RESULT,param,errmsg,msg);
			break;
		}
	}
	virtual BOOL GetErrorMessage(LPTSTR lpszError,UINT nMaxError,PUINT pnHelpContext =NULL){
		lstrcpyn(lpszError,m_error,nMaxError);
		return TRUE;
	}
};

void UploadSDC(LPCTSTR sdcPath,LPCTSTR appliTitle,LPCTSTR appliType,CString& url) {
	HttpPost upload;
	upload.AddFormData("appli_type", appliType);
	upload.AddFormData("appli_title",appliTitle);
	upload.AddFormFile("sdc",        "sdp.sdc",sdcPath,"application/octet-stream");
	upload.Commit();
	
	CString       userAgent = USER_AGENT;
	CString       server    = UPLOAD_HTTP_SERVER;
	INTERNET_PORT port      = UPLOAD_HTTP_PORT;
	CString object          = UPLOAD_HTTP_OBJECT;
	DWORD   status;
	typedef std::auto_ptr<CHttpConnection>  CONNECTION_P;
	typedef std::auto_ptr<CInternetSession> SESSION_P;
	typedef std::auto_ptr<CHttpFile>        FILE_P;

	SESSION_P     session;
	CONNECTION_P  connection;
	FILE_P        file;

	session    = SESSION_P   (new CInternetSession(userAgent));
	ASSERT_VALID(session.get());
	connection = CONNECTION_P(session->GetHttpConnection(server,port));
	ASSERT_VALID(connection.get());
	file       = FILE_P      (connection->OpenRequest(CHttpConnection::HTTP_VERB_POST,object));
	ASSERT_VALID(file.get());

	file->SendRequest(upload.GetHeader(),
			          upload.GetData(),
					  upload.GetDataLength());
	TRACE("%s\n",upload.GetData());

	file->QueryInfoStatusCode(status);
	if( status != HTTP_STATUS_OK ) { 
		throw new UploadException(UploadException::HTTP_ERROR,status);
	}

	CString strLine;
	if (!file->ReadString(strLine)) {
		throw new UploadException(UploadException::CORRUPTED_RESULT);
	}

	int result = _ttoi(strLine);
	if (result != 100) {
		CString msg;
		VERIFY(file->ReadString(msg));
		TRACE("%s\n",msg);
		while (file->ReadString(msg)) {
			TRACE("%s\n",msg);
		}
		throw new UploadException(UploadException::ERROR_RESULT,result,msg);
	}
	VERIFY(file->ReadString(url));
	TRACE("url=" + url);

	session->Close();
}


/**
*	メール送信時のエラー警告ダイアログ
*/
class SendMailErrDlg : public CDialog {
private:
	/// エラーメッセージ
	CString m_strErrMsg;
public:
	/**
	*	メール送信時のエラーを警告ダイアログを表示してユーザに通知
	*/
	static void DisplaySendMailErr(LPCTSTR msg) {
		ASSERT(AfxIsValidString(msg));
		SendMailErrDlg dlg(msg);
		dlg.DoModal();
	}

private:
	/**
	*	ダイアログ初期化
	*/
	SendMailErrDlg::SendMailErrDlg(LPCTSTR msg) :
		CDialog(IDD_SEND_MAIL_ERR) {
		ASSERT(AfxIsValidString(msg));
		m_strErrMsg = msg;
	}
	/**
	*	エラーメッセージをダイアログに設定
	*/
	virtual BOOL OnInitDialog() {
		if (!CDialog::OnInitDialog()) {
			return FALSE;
		}
		SetDlgItemText(IDCE_DETAIL,m_strErrMsg);
		return TRUE;
	}
};

/**
*	メールを送信する
*		メール送信時にエラーが発生した場合はユーザに警告画面を表示する
*/
void SendMail(LPCTSTR tos,LPCTSTR from,LPCTSTR server,LPCTSTR subject,LPCTSTR body) {
	ASSERT(AfxIsValidString(tos));
	ASSERT(AfxIsValidString(from));
	ASSERT(AfxIsValidString(server));
	ASSERT(AfxIsValidString(subject));
	ASSERT(AfxIsValidString(body));

	CSMTP smtp(server);
	CRFC1468 rfc;

	CStringArray tolist;
	Split(tos,',',tolist);

	if (!smtp.Connect()) {
		SendMailErrDlg::DisplaySendMailErr(smtp.GetLastError());
		return;
	}

	for (int n = 0;n < tolist.GetSize();n++) {
		CMailMessage msg;
		CString to;
		to = tolist.ElementAt(n);
		to.TrimLeft();
		to.TrimRight();
		msg.m_sMailerName = USER_AGENT;
		msg.m_sFrom       = from;
		msg.m_sSubject    = rfc.EncodeSubject(subject);
		msg.m_sBody       = rfc.EncodeBody   (body);
		msg.AddMultipleRecipients(to);
		if (!smtp.SendMessage(&msg)) {
			SendMailErrDlg::DisplaySendMailErr(smtp.GetLastError());
			break;
		}
	}

	if (!smtp.Disconnect()) {
		smtp.GetLastError();
		return;
	}
}

void SpicydogDoc::OnPublishUpload() {
	BeginEdit();
	BeginEdit();
	Builder builder(appli.get(),&buildOutput);
	if (!Build(builder)) {
		AfxMessageBox(IDS_ERR_CANNOT_RUN_FOR_BUILD_ERR,MB_OK | MB_ICONSTOP);
		return;
	}
	CString sdcPath = CreateTempFile("sdc");
	builder.GenerateSDCFile(sdcPath);

	UploadDlg dlg(AfxGetMainWnd());
	if (dlg.DoModal() == IDOK) {
		CString url;
		try {
			CWaitCursor wc;
			UploadingDlg dlgUploading;
			dlgUploading.Create();

			dlgUploading.SetProgress(IDS_UPLOADING);
			const AppliConf* conf = appli->appliConf.get();
			UploadSDC(sdcPath,conf->title,conf->appliType,url);

			if (ACTIVE_PREF.FwdMailUse) {
				dlgUploading.SetProgress(IDS_FOWARDING_DOWNLOAD_URL);
				CString warning;
				CString appliType;
				appliType = appli->appliConf->appliType;
				if (appliType == APPLI_TYPE_JPHONEF ||
					appliType == APPLI_TYPE_JPHONEFM) {
					VERIFY(warning.LoadString(IDS_MAIL_WARNING_ONLY_F));
				} else {
					VERIFY(warning.LoadString(IDS_MAIL_WARNING_ONLY_G));
				}
				SendMail (ACTIVE_PREF.FwdMailTo,
						  ACTIVE_PREF.FwdMailFrom,
						  ACTIVE_PREF.FwdMailSMTPServer,
						  ACTIVE_PREF.FwdMailSubject,
						  ACTIVE_PREF.FwdMailBody
						  + "\r\n" 
						  + LoadString(IDS_FORWARDING_DOWNLOAD_URL_HEAD)
						  + "\r\n"
						  + url 
						  + "\r\n" 
						  + warning);
			}
		} catch(CException* e) {
			e->ReportError();
			AfxMessageBox(IDS_ERR_UPLOAD,MB_OK | MB_ICONSTOP);
			return;
		} catch(...) {
			AfxMessageBox(IDS_ERR_UPLOAD,MB_OK | MB_ICONSTOP);
			return;
		}	
		UploadedDlg dlgDone(url);
		dlgDone.DoModal();
	}
}

void SpicydogDoc::OnEmuPlay() {
	BeginEdit();
	Builder builder(appli.get(),&buildOutput);
	if (!Build(builder)) {
		AfxMessageBox(IDS_ERR_CANNOT_RUN_FOR_BUILD_ERR,MB_OK | MB_ICONSTOP);
		return;
	}
	CString temp    = CreateTempDirectory("sdp");

	EmuPlayDlg dlg(temp,&builder,FALSE);
	dlg.DoModal();
}


void SpicydogDoc::UpdateFrameCounts() {
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible()) {
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL)
				pFrame->m_nWindow = -1;     // unknown
		}
	}

	int nFrames = 0;
	pos = GetFirstViewPosition();
	while (pos != NULL) {
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible()) {
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL && pFrame->m_nWindow == -1 ) {
				ASSERT_VALID(pFrame);
				if (pFrame->IsKindOf(RUNTIME_CLASS(AppliFrameWnd))) {
					// not yet counted (give it a 1 based number)
					pFrame->m_nWindow = ++nFrames;
				}
			}
		}
	}

	int iFrame = 1;
	pos = GetFirstViewPosition();
	while (pos != NULL) {
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		ASSERT(::IsWindow(pView->m_hWnd));
		if (pView->IsWindowVisible()) {
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL && pFrame->m_nWindow == iFrame) {
				ASSERT_VALID(pFrame);
				if (nFrames == 1)
					pFrame->m_nWindow = 0;      // the only one of its kind
				pFrame->OnUpdateFrameTitle(TRUE);
				iFrame++;
			}
		}
	}
	ASSERT(iFrame == nFrames + 1);
}

void SpicydogDoc::Serialize(CArchive& ar) {
	typedef std::auto_ptr<DOMDocument> DOCUMENTP;
	DOCUMENTP document;
	CFile* file = ar.GetFile();

	if (ar.IsLoading()) {
		typedef std::auto_ptr<SDOMReader> READERP;
		READERP readerJ;
		readerJ = READERP(new VerJ3SDOMReader);
		READERP readerI;
		readerI = READERP(new Ver3SDOMReader);
		if (readerI->CanRead(file)) {
			if (AfxMessageBox(IDS_WARNING_LOAD_FROM_OLD_VERSION,MB_OKCANCEL) != IDOK) {
				AfxMessageBox(IDS_CANCEL_LOAD_FROM_OLD_VERSION,MB_OK);
				AfxThrowUserException();
			}
			document = DOCUMENTP(readerI->Read(file));
			m_bLoadFromOldVersion = TRUE;
		} else {
			document = DOCUMENTP(readerJ->Read(file));
		}
		SDOMArchive archive;
		archive.SerializeLoad(document.get(),WSDOMN_SPICYDOG,appli.get());

	} else {
		typedef std::auto_ptr<SDOMWriter> WRITERP;
		WRITERP writerJ;
		writerJ = WRITERP(new SDOMWriter);
		SDOMArchive archive;
		document = DOCUMENTP(archive.SerializeStore(WSDOMN_SPICYDOG,appli.get()));
		writerJ->Write(file,document.get());
		document = DOCUMENTP(NULL);
	}
}

BOOL SpicydogDoc::OnSaveDocument(LPCTSTR lpszPathName) {
	if (m_bLoadFromOldVersion) {
		if (AfxMessageBox(IDS_WARNING_SAVE_FROM_OLD_VERSION,MB_OKCANCEL) != IDOK) {
			AfxMessageBox(IDS_CANCEL_SAVE_FROM_OLD_VERSION,MB_OK);
			return FALSE;
		}
	}
	BOOL success = CDocument::OnSaveDocument(lpszPathName);
	if (success && m_bLoadFromOldVersion) {
		m_bLoadFromOldVersion = FALSE;
	}
	return success;
}
