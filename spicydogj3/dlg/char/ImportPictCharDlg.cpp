#include "stdafx.h"
#include "resource.h"
#include "ImportPictCharDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(ImportPictCharDlg, CFileDialog)
END_MESSAGE_MAP()

ImportPictCharDlg::ImportPictCharDlg (CWnd* parentWnd) :
	CFileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,parentWnd) {

	ASSERT_VALID(parentWnd);
	SetTemplate(0,IDD_IMPORT_PICT);

	if (m_strFilter.LoadString(IDS_PICT_FILTER)) {
		LPTSTR pch = m_strFilter.GetBuffer(0); // modify the buffer in place
		// MFC delimits with '|' not '\0'
		while ((pch = _tcschr(pch, '|')) != NULL) {
			*pch++ = '\0';
		}
		m_ofn.lpstrFilter = m_strFilter;
		// do not call ReleaseBuffer() since the string contains '\0' characters
	}

	m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	m_buffer.SetSize(MAX_PATH * 512);
	m_paths = m_buffer.GetData();
	m_ofn.lpstrFile = m_paths;
	m_ofn.nMaxFile  = MAX_PATH * 512;

	m_pathThrough = TRUE;
}

void ImportPictCharDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDCC_PATHTHROUGH, m_pathThrough);
}

BOOL ImportPictCharDlg::OnFileNameOK() {
	return !UpdateData(TRUE);
}
