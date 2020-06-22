#include "stdafx.h"
#include "resource.h"
#include "Jphone.h"
#include "utility/FileUtility.h"
#include "utility/StringUtility.h"
#include "utility/RegUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR JPHONESDK_REG_PATH = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\jbmidp.jar";
LPCTSTR JPHONESDK_DEFAULT  = "jbmidp.jar";

BOOL Jphone::BrowseEmuPath(CString& path) {
	CFileDialog dlg 
		(TRUE,
		 LoadString(IDS_DEFEXT_EMU),
		 LoadString(IDS_FILENAME_EMU),
         OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		 LoadString(IDS_FILTER_EMU),
		 AfxGetMainWnd());
	if (dlg.DoModal() == IDOK) {
		path = dlg.GetPathName();
		return TRUE;
	}
	return FALSE;
}

BOOL Jphone::DetectEmuPath(CString& path) {
	CString temp = QueryRegValue(HKEY_LOCAL_MACHINE,JPHONESDK_REG_PATH,NULL,JPHONESDK_DEFAULT);
	if (temp.IsEmpty() || !ExistsPath(temp)) {
		return FALSE;
	}
	path = temp;
	return TRUE;
}
