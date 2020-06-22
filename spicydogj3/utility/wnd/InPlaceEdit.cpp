#include "stdafx.h"
#include "InPlaceEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(InPlaceEdit, CEdit)
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

InPlaceEdit::InPlaceEdit() {
}

InPlaceEdit::~InPlaceEdit() {
}

CString InPlaceEdit::GetText() const {
	CString str;
	GetWindowText(str);
	return str;
}

BOOL InPlaceEdit::Create(CRect& rc,CWnd* pParent,UINT nCtrlID) {
	return CEdit::Create(WS_CHILD | ES_RIGHT,rc,pParent,nCtrlID);
}

BOOL InPlaceEdit::DoEdit(LPCTSTR ptsz,LPCRECT prc) {
	EnterEdit(ptsz,prc);
	BOOL b = TrackEdit();
	ExitEdit();
	return b;
}

BOOL InPlaceEdit::TrackEdit() {
	if (GetCapture() != NULL) {
		return FALSE;
	}

	::AfxLockTempMaps();
	VERIFY(SetCapture() == NULL);
	ASSERT(GetCapture() == this);

	BOOL bEnter = FALSE;
	for (;;) {
		if (GetCapture() != this)
			goto loopexit;

		MSG msg;
		VERIFY(GetMessage(&msg,NULL,0,0));
		UINT   message = msg.message;
		WPARAM vcode   = msg.wParam;
		if (message == WM_KEYDOWN && (vcode == VK_ESCAPE || vcode == VK_RETURN )) {
			continue;
		} else if (message == WM_KEYUP && vcode == VK_RETURN) {
			bEnter = TRUE;
			goto loopexit;
		} else if (message == WM_KEYUP && vcode == VK_ESCAPE ||
			       message == WM_LBUTTONDOWN) {
			goto loopexit;
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
loopexit:
	VERIFY(ReleaseCapture());
	::AfxUnlockTempMaps(FALSE);
	return bEnter;
}

void InPlaceEdit::EnterEdit(LPCTSTR ptszDefault,LPCRECT prc) {
	CRect rc;
	if (prc == NULL) {
		GetParent()->GetClientRect(rc);
	} else {
		rc = *prc;
	}
	MoveWindow(rc);
	ShowWindow(SW_SHOW);
	EnableWindow(TRUE);
	SetFocus();

	SetSel(0,-1);
	ReplaceSel(ptszDefault);
	SetSel(0,-1);
}

void InPlaceEdit::ExitEdit() {
	EnableWindow(FALSE);
	ShowWindow(SW_HIDE);
	GetParent()->SetFocus();
}

