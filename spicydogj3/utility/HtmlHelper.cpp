#include "stdafx.h"
#include "HtmlHelper.h"
#include <HtmlHelp.h>
#include "utility/FileUtility.h"
#pragma comment(lib,"HtmlHelp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HtmlHelper::HtmlHelper()
:	m_inited(FALSE),
	m_uninitialized(FALSE),
	m_dwCookie(0),
	m_hWnd(NULL)
{
}

HtmlHelper::~HtmlHelper()
{
	if (!m_uninitialized)
		Uninitialize();
}

void HtmlHelper::Initialize(HWND hwnd,LPCTSTR ptszPath,LPCTSTR ptszWndType)
{
	if (!m_inited)
	{
		ASSERT(ptszPath && AfxIsValidString(ptszPath));
		ASSERT(ExistsPath(ptszPath));
		ASSERT(ptszWndType == NULL || AfxIsValidString(ptszWndType));
		ASSERT(hwnd != NULL && IsWindow(hwnd));
		
		m_strFilePath = ptszPath;
		m_strWndType = ptszWndType;
		m_hWnd = hwnd;
		
		HtmlHelp(NULL, NULL, HH_INITIALIZE, (DWORD)&m_dwCookie);
		m_inited = m_dwCookie != 0;
		ASSERT(m_inited);
	}
	else
		ASSERT(FALSE);
}

void HtmlHelper::Uninitialize() {
	if (!m_inited) {
		ASSERT(FALSE);
		return;
	}
	if (m_uninitialized) {
		ASSERT(FALSE);
		return;
	}

	VERIFY(HtmlHelp(NULL, NULL, HH_UNINITIALIZE,m_dwCookie));
	m_uninitialized = TRUE;
}

void HtmlHelper::CloseAll() {
	if (!m_inited && m_uninitialized) {
		ASSERT(FALSE);
		return;
	}
	
	if (HtmlHelp(m_hWnd,m_strFilePath, HH_GET_WIN_HANDLE,(DWORD)(LPCTSTR)m_strWndType))  {
		HtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0) ;
	}
}

BOOL HtmlHelper::PreTranslateMessage(MSG* pMsg) {
	if (!m_inited && m_uninitialized) {
		ASSERT(FALSE);
		return FALSE;
	}	
	return (BOOL)HtmlHelp(NULL, NULL, HH_PRETRANSLATEMESSAGE,(DWORD)pMsg);
}

void HtmlHelper::DisplayToc(LPCTSTR ptszPage)
{
	if (!m_inited && m_uninitialized)
		{ ASSERT(FALSE); return;}
	ASSERT(m_hWnd != NULL && ::IsWindow(m_hWnd));

	CString str;
	if (ptszPage == NULL)
		str = m_strFilePath;
	else
	{
		ASSERT(ptszPage != NULL && AfxIsValidString(ptszPage));
		str.Format(_T("%s::%s"),m_strFilePath,ptszPage);
	}

	HtmlHelp(m_hWnd,str, HH_DISPLAY_TOC, NULL);
}

void HtmlHelper::DisplayIndex(LPCTSTR ptszKeyword)
{
	if (!m_inited && m_uninitialized)
		{ ASSERT(FALSE); return;}
	ASSERT(m_hWnd != NULL && ::IsWindow(m_hWnd));

	HtmlHelp(m_hWnd,m_strFilePath,HH_DISPLAY_INDEX,(DWORD)ptszKeyword);
}

void HtmlHelper::DisplaySearch(LPCTSTR ptszQuery)
{	
	if (!m_inited && m_uninitialized)
		{ ASSERT(FALSE); return;}
	ASSERT(m_hWnd != NULL && ::IsWindow(m_hWnd));

	HH_FTS_QUERY q = {sizeof HH_FTS_QUERY,};
	
	q.fUniCodeStrings = FALSE;
	q.pszSearchQuery = ptszQuery;
	q.iProximity = HH_FTS_DEFAULT_PROXIMITY;
	q.fStemmedSearch = FALSE;
	q.fTitleOnly = FALSE;
	q.fExecute = TRUE;
	q.pszWindow = NULL;
	
	HtmlHelp(m_hWnd,m_strFilePath, HH_DISPLAY_SEARCH, (DWORD)&q);
}

