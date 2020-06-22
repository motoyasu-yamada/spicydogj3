#include "stdafx.h"
#include "CursorCapture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CursorCapture::CursorCapture(HWND hwnd)
:	m_hwndOld(NULL),
	m_bCapturing(FALSE)
{
	if (hwnd != NULL) {
		SetCapture(hwnd);
	}
}

CursorCapture::~CursorCapture()
{
	if (IsCapturing())
		ReleaseCapture();
}

BOOL CursorCapture::IsCapturing() const
{
	return m_bCapturing;
}

void CursorCapture::SetCapture(HWND hwnd)
{
	if (::IsWindow(hwnd))
	{
		m_hwndOld = ::SetCapture(hwnd);
		ASSERT(::GetCapture() == hwnd);

		m_bCapturing = TRUE;
	}
	else
		ASSERT(FALSE);
}

void CursorCapture::ReleaseCapture()
{
	if (IsCapturing())
	{
		if (m_hwndOld != NULL && ::IsWindow(m_hwndOld))
		{
			::SetCapture(m_hwndOld);
		}
		else
			::ReleaseCapture();

		m_bCapturing = FALSE;
	}
	else
		ASSERT(FALSE);
}

CursorClipper::CursorClipper(LPCRECT prcClip) {
	GetClipCursor(m_rcOld);
	ClipCursor(prcClip);
}

CursorClipper::~CursorClipper() {
	ClipCursor(m_rcOld);
}
