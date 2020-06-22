#include "stdafx.h"
#include "FolderDlg.h"
#include "utility/FileUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FolderDlg::FolderDlg
(
	HWND hwndParent,
	LPCTSTR	ptszTitle,
	LPCTSTR ptszRoot,
	UINT uiFlag
)
:	m_strTitle(ptszTitle),
	m_strRoot(ptszRoot),
	m_hwndParent(hwndParent),
	m_hwndMe(NULL),
	m_bDoneModal(FALSE)
{
	UNUSED_ALWAYS(ptszRoot);

	m_bi.ulFlags = uiFlag;
}

CString FolderDlg::GetFolderPath() const
{
	ASSERT (m_bDoneModal);
	return m_strSelected;
}
int FolderDlg::GetFolderImage() const
{
	ASSERT (m_bDoneModal);
	return m_bi.iImage;
}
//------------------------------------------------------------------------
//	ダイアログスタイルの設定/取得
//------------------------------------------------------------------------
UINT FolderDlg::GetStyle() const
{
	ASSERT (!m_bDoneModal && m_hwndMe);
	return m_bi.ulFlags;
}
void FolderDlg::SetStyle(UINT nFlags)
{
	ASSERT (!m_bDoneModal && m_hwndMe);
	m_bi.ulFlags = nFlags;
}
//////////////////////////////////////////////////////////////////////////
//	フォルダ選択メソッド
//////////////////////////////////////////////////////////////////////////
int FolderDlg::DoModal()
{
	//	BROWSE_INFOを初期化する
	TCHAR atszDisplayName[MAX_PATH*2];
	ZeroMemory(&m_bi,sizeof(m_bi));
	m_bi.hwndOwner		= m_hwndParent;
	m_bi.pidlRoot		= NULL;
	m_bi.lpszTitle		= m_strTitle;
	m_bi.lpfn			= BrowseCallbackProc;
	m_bi.lParam			= reinterpret_cast<LPARAM>(this);
	m_bi.pszDisplayName = atszDisplayName;

	LPITEMIDLIST pidl = ::SHBrowseForFolder(&m_bi);
	
	int iResult = IDCANCEL;
	if (pidl != NULL)
	{
		if (SUCCEEDED(::SHGetPathFromIDList(pidl,m_strSelected.GetBuffer(MAX_PATH * 2 +1))))
			iResult = IDOK;
		m_strSelected.ReleaseBuffer();

		LPMALLOC pMalloc = NULL;
		if (SUCCEEDED(::SHGetMalloc(&pMalloc)))
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
		else
			ASSERT(FALSE);
	}

	m_bDoneModal = TRUE;
	m_hwndMe = NULL;
	return iResult;
}

//////////////////////////////////////////////////////////////////////////
//	実装
//////////////////////////////////////////////////////////////////////////
FolderDlg::~FolderDlg()
{
}
//////////////////////////////////////////////////////////////////////////
//	Protected & Priavte
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
//	フォルダダイアログの初期化メッセージの処理
//------------------------------------------------------------------------
void FolderDlg::OnInit()
{
	// ルートフォルダが指定されていない場合は、
	// カレントディレクトリをルートに設定
	if (m_strRoot.IsEmpty())
	{
		m_strRoot = GetCurrentDirectory();
	}

	if (m_hwndMe != NULL && ::IsWindow(m_hwndMe))
	{
		CWnd* pWnd = CWnd::FromHandle(m_hwndMe);
		pWnd->CenterWindow();
		pWnd->SendMessage(BFFM_SETSELECTION,TRUE,reinterpret_cast<LPARAM>(static_cast<LPCTSTR>(m_strRoot)));
	}
	else
		ASSERT(FALSE);
}
//------------------------------------------------------------------------
//	ユーザーが選択されているパスが変更になった
//------------------------------------------------------------------------
void FolderDlg::OnSelChanged(LPCITEMIDLIST pidl)
{
	// ステータスウインドウに現在選択されているパスを表示
	CString strSel;
	BOOL bGet = SHGetPathFromIDList(pidl,strSel.GetBuffer(MAX_PATH));
	strSel.ReleaseBuffer();

	if (bGet && m_hwndMe != NULL && IsWindow(m_hwndMe))
	{
		::SendMessage(m_hwndMe,BFFM_SETSTATUSTEXT,0,reinterpret_cast<LPARAM>(static_cast<LPCTSTR>(strSel)));
	}
	else
		ASSERT(FALSE);
}
//------------------------------------------------------------------------
//	OKボタンのシミュレートをする
//------------------------------------------------------------------------
void FolderDlg::EnableOK(BOOL bEnable)
{
	if (m_hwndMe != NULL && ::IsWindow(m_hwndMe))
	{
		::SendMessage(m_hwndMe, BFFM_ENABLEOK, static_cast<WPARAM>(bEnable), NULL);
	}
	else
		ASSERT(FALSE);
}
//------------------------------------------------------------------------
//	指定した ITEMIDLISTのオブジェクトを選択する
//------------------------------------------------------------------------
void FolderDlg::SetSelection(LPCITEMIDLIST pidl)
{
	if (m_hwndMe != NULL && ::IsWindow(m_hwndMe))
	{
		::SendMessage(m_hwndMe, BFFM_SETSELECTION,FALSE,reinterpret_cast<LPARAM>(pidl));
	}
	else
		ASSERT(FALSE);
}
void FolderDlg::SetSelection(LPCTSTR szPath)
{
	if (m_hwndMe != NULL && ::IsWindow(m_hwndMe))
	{
		::SendMessage(m_hwndMe, BFFM_SETSELECTION, TRUE, reinterpret_cast<LPARAM>(LPCSTR(szPath)));
	}
	else
		ASSERT(FALSE);
}
//------------------------------------------------------------------------
//	ステータステキストを設定する
//------------------------------------------------------------------------
void FolderDlg::SetStatusText(LPCTSTR ptszText)
{
	if (m_hwndMe != NULL && ::IsWindow(m_hwndMe))
		::SendMessage(m_hwndMe, BFFM_SETSTATUSTEXT, NULL, reinterpret_cast<LPARAM>(ptszText));
	else
		ASSERT(FALSE);
}
//------------------------------------------------------------------------
// BrowseCallbackProc
// [動作] SHBrowseForFolder参照
// [引数] SHBrowseForFolder参照
// [戻値] SHBrowseForFolder参照
//
// [使用上の注意]
//
//	-OP	-DATE		-AUTHOR		-MEMO
//	CRE	00/01/05	M.YAMADA
//------------------------------------------------------------------------
int CALLBACK FolderDlg::BrowseCallbackProc
(
	HWND	hwnd,
	UINT	uMsg, 
	LPARAM	lParam,
	LPARAM	lpData
)
{
	FolderDlg* pbff = reinterpret_cast<FolderDlg*>(lpData);
	if (pbff != NULL)
	{
		switch(uMsg) {
			case BFFM_INITIALIZED: 
				pbff->m_hwndMe = hwnd;
				
				pbff->OnInit();
			   break;
			case BFFM_SELCHANGED: 
				if (pbff->m_hwndMe != NULL)
					pbff->OnSelChanged((LPITEMIDLIST)lParam);
			   break;
			default:
			   break;
		 }
	}
	return 0;
}
