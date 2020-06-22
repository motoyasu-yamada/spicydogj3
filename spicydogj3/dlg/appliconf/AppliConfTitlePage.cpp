#include "stdafx.h"
#include "resource.h"
#include "AppliConfTitlePage.h"
#include "model/AppliConf.h"
#include "utility/DDXUtility.h"
#include "utility/PaintMemDC.h"
#include "utility/LeadToolUtility.h"
#include "utility/StringUtility.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static const int APPLI_ICON_WIDTH  = 24;
static const int APPLI_ICON_HEIGHT = 24;

#define DISPLAY_ERROR_LIST LSettings::DisplayErrorList(AfxGetMainWnd()->GetSafeHwnd())

BEGIN_MESSAGE_MAP(AppliConfTitlePage, CPropertyPageEx)
	ON_EN_CHANGE(IDCE_APP_TITLE,  OnChanged)
	ON_EN_CHANGE(IDCE_APP_VENDOR, OnChanged)
	ON_EN_CHANGE(IDCE_APP_COMMENT,OnChanged)
	ON_COMMAND(IDCB_LOAD_ICON, OnLoadIcon)
	ON_COMMAND(IDCB_CLEAR_ICON,OnClearIcon)
END_MESSAGE_MAP()

/**
	アイコン読込例外
*/
class LoadIconException : public CSimpleException {
private:
	/** エラー理由 */
	CString m_error;
public:
	/** アイコン読み込み時の例外
		@param msg エラー理由を説明する文字列リソースのID
	*/
	LoadIconException(UINT msg) : CSimpleException(TRUE) {
		VERIFY(m_error.LoadString(msg));
	}
	/** アイコン読み込み時の例外
		@param msg   エラー理由を説明する文字列リソースのID
		             エラー理由を説明する下記パラメータを表示する %d を含むこと。
		@param param エラー理由を説明するパラメータ
	*/
	LoadIconException(UINT format,int param) : CSimpleException(TRUE) {
		m_error.Format(format,param);
	}
	/** アイコン読み込み時の例外
		@param msg   エラー理由を説明する文字列リソースのID
		             エラー原因を表示する %s を含むこと。
		@param error エラーが発生した原因となる例外
	*/
	LoadIconException(UINT format,CException* error) : CSimpleException(TRUE) {
		ASSERT(error != NULL);
		CString msg;
		VERIFY(error->GetErrorMessage(msg.GetBuffer(10240),10240));
		msg.ReleaseBuffer();
		m_error.Format(format,msg);
	}
	/** @see CException::GetErrorMessage */
	virtual BOOL GetErrorMessage(LPTSTR lpszError,UINT nMaxError,PUINT pnHelpContext =NULL){
		ASSERT(AfxIsValidAddress(lpszError,nMaxError));
		lstrcpyn(lpszError,m_error,nMaxError);
		return TRUE;
	}
};

AppliConfTitlePage::AppliConfTitlePage(AppliConf* appliConf)
:	CPropertyPageEx(IDD_APPLICONF_TITLE) {
	m_appliConf   = appliConf;
	m_isIconValid = FALSE;
}

AppliConfTitlePage::~AppliConfTitlePage()
{
}

void AppliConfTitlePage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPageEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDCS_ICON,m_iconPreview);
	DDX_Text(pDX, IDCE_APP_TITLE,  m_appliConf->title);
	DDX_Text(pDX, IDCE_APP_VENDOR, m_appliConf->vendor);
	DDX_Text(pDX, IDCE_APP_COMMENT,m_appliConf->comment);
	DDXUtility::DDV_UTF8TextLength(pDX,m_appliConf->title,  1,16,IDCE_APP_TITLE,  IDS_ERR_APPLICONF_TITLE);
	DDXUtility::DDV_UTF8TextLength(pDX,m_appliConf->vendor, 1,16,IDCE_APP_VENDOR, IDS_ERR_APPLICONF_VENDOR);
	DDXUtility::DDV_UTF8TextLength(pDX,m_appliConf->comment,1,64,IDCE_APP_COMMENT,IDS_ERR_APPLICONF_COMMENT);
}

/**
	アイコンプレビュー領域初期化
	アイコン読込
*/
BOOL AppliConfTitlePage::OnInitDialog() {
	if (!CPropertyPageEx::OnInitDialog()) {
		return FALSE;
	}
	m_iconPreview.SetListener(this);
	CRect rcC;
	CRect rcW;
	m_iconPreview.GetClientRect(rcC);
	m_iconPreview.GetWindowRect(rcW);
	CSize border = rcW.Size() - rcC.Size();
	ASSERT(0 <= border.cx && border.cx < APPLI_ICON_WIDTH);
	ASSERT(0 <= border.cy && border.cy < APPLI_ICON_HEIGHT);
	m_iconPreview.SetWindowPos(
		NULL,
		0,0,
		APPLI_ICON_WIDTH  + border.cx,
		APPLI_ICON_HEIGHT + border.cy,
		SWP_NOMOVE | SWP_NOZORDER);
	m_isIconValid = FALSE;

	try {
		LoadIconBitmap(m_appliConf->icon,m_icon);
		m_isIconValid = TRUE;
	} catch(LoadIconException* pe) {
		TCHAR message[10240];
		VERIFY(pe->GetErrorMessage(message,10240));
		TRACE("*** %d \n",message);
		pe->Delete();
	}

	EnableDlgItem(this,IDCB_CLEAR_ICON,m_isIconValid);
	return TRUE;
}

BOOL AppliConfTitlePage::OnSetActive() {
	UpdateData(FALSE);
	return TRUE;
}

void AppliConfTitlePage::OnOK() {
	if (UpdateData(TRUE)) {
		CPropertyPageEx::OnOK();
	}
}

BOOL AppliConfTitlePage::OnApply() {
	if (UpdateData(TRUE)) {
		CancelToClose();
		return TRUE;
	} else
		return FALSE;
}

void AppliConfTitlePage::OnChanged() {
	SetModified(TRUE);
	EnableDlgItem(this,IDCB_CLEAR_ICON,m_isIconValid);
	m_iconPreview.Invalidate();
}

BOOL AppliConfTitlePage::OnKillActive() {	
	return UpdateData(TRUE);
}

/**
	PNG形式アイコン画像を内部形式として読み込む

	@param		buffer PNG形式のバイナリデーター
	@param		bitmap 内部形式
	@exception	LoadIconException - Java[TM]アプリ規定のフォーマットではない場合
	                              - ファイルが破損しているPNG形式場合
*/
void AppliConfTitlePage::LoadIconBitmap(const LBuffer& buffer,LBitmap& bitmap) {
	if (const_cast<LBuffer&>(buffer).GetSize() == 0) {
		throw new LoadIconException(IDS_APPLI_ICON_NULL);
	}

	LMemoryFile file;
	FILEINFO info;
	ZeroMemory(&info,sizeof info);
	if (file.GetInfo(const_cast<LBuffer&>(buffer),&info) != SUCCESS) {
		DISPLAY_ERROR_LIST;
		throw new LoadIconException(IDS_APPLI_ICON_CORRUPTED_HEAD);
	}

	if (info.Format != FILE_PNG) {
		ASSERT(FALSE);
		throw new LoadIconException(IDS_APPLI_ICON_NOT_PNG);
	}
	if (info.BitsPerPixel != 8) {
		ASSERT(FALSE);
		throw new LoadIconException(IDS_APPLI_ICON_INVALID_BITS,info.BitsPerPixel);
	}
	if (info.Width != APPLI_ICON_WIDTH) {
		ASSERT(FALSE);
		throw new LoadIconException(IDS_APPLI_ICON_INVALID_WIDTH,info.Width);
	}
	if (info.Height != APPLI_ICON_HEIGHT) {
		ASSERT(FALSE);
		throw new LoadIconException(IDS_APPLI_ICON_INVALID_HEIGHT,info.Height);
	}

	file.SetBitmap(&bitmap);
	if (file.LoadBitmap(const_cast<LBuffer&>(buffer),24,ORDER_BGR) != SUCCESS) {
		DISPLAY_ERROR_LIST;
		throw new LoadIconException(IDS_APPLI_ICON_CORRUPTED);
	}
	ASSERT(bitmap.GetColorOrder() == ORDER_BGR);
}



/**
	アイコンをファイルから読み込む

	@param path   ファイルパス
	@param icon   読み込んだアイコンのバイナリー・データ
	@param bitmap アイコンを読み込んでビットマップに変換した結果
	@exception LoadIconException
*/
void AppliConfTitlePage::LoadIcon(LPCTSTR path,LBuffer& icon,LBitmap& bitmap) {
	CFile file;
	CFileException error;

	if (!file.Open(path,CFile::modeRead | CFile::shareExclusive,&error)) {
		ASSERT(FALSE);
		throw new LoadIconException(IDS_APPLI_ICON_OPEN_FILE_ERROR,&error);
	}

	DWORD length = file.GetLength();
	icon.Reallocate(length);
	DWORD read   = file.Read(icon.Lock(),length);
	icon.Unlock();
	if (read != length) {
		ASSERT(FALSE);
		throw new LoadIconException(IDS_APPLI_ICON_READ_FILE_ERROR);
	}
	file.Close();

	LoadIconBitmap(icon,bitmap);
}

void AppliConfTitlePage::OnLoadIcon() {
	CFileDialog dlg(TRUE,NULL,NULL,0,LoadString(IDS_APPLI_ICON_FILTER),this);
	if (dlg.DoModal() == IDOK) {
		try {
			LBuffer temp;
			LoadIcon(dlg.GetFileName(),temp,m_icon);
			m_appliConf->icon.Copy(&temp);
			m_isIconValid = TRUE;
		} catch(LoadIconException* error) {
			error->ReportError();
		} catch(...) {
			ASSERT(FALSE);
			AfxMessageBox(IDS_ERR_APPLICONF_ICON,MB_OK | MB_ICONSTOP);
		}
	}
	OnChanged();
}

void AppliConfTitlePage::OnClearIcon() {
	m_appliConf->icon.Free();
	m_icon.Free();
	m_isIconValid = FALSE;
	OnChanged();
}

void AppliConfTitlePage::OnPSPaint(PreviewStatic& ps) {
	PaintMemDC dc(&ps);
	if (m_isIconValid) {
		LPaintTransparent(
			dc,
			0,0,
			APPLI_ICON_WIDTH,APPLI_ICON_HEIGHT,
			m_icon,
			0,0,
			APPLI_ICON_WIDTH,APPLI_ICON_HEIGHT,
			FALSE);
	} else {
		dc.FillSolidRect(
			0,0,
			APPLI_ICON_WIDTH,APPLI_ICON_HEIGHT,
			::GetSysColor(COLOR_BTNFACE));
	}
}

BOOL AppliConfTitlePage::OnPSEraseBkgnd(PreviewStatic&,CDC&) {
	return TRUE;
}

