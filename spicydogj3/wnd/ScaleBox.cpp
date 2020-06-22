#include "stdafx.h"
#include "resource.h"
#include "ScaleBox.h"
#include "ScrollViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT ID_SCALE      = 20000;
const double SCALES[]    = {0.12,0.25,0.50,0.75,1,1.5,2,4,5,8,10};
const int SCALE_COUNT    = (sizeof SCALES / sizeof SCALES[0]);
const UINT ID_SCALE_LAST = ID_SCALE + SCALE_COUNT - 1;
const int FONTPOINT_TRI =  90;
const int FONTPOINT_NUM = 100;
const int SCALEBOX_COMBOBOX_HEIGHT = 20;
const int SCALEBOX_TOOLBAR_HEIGHT  = 24;
const LPCTSTR FONTFACE_TRI = _T("Marlett");
const int LAYOUT_MARGIN    = 2;
const int TRI_CX           = 10; 
const int SCALEBOX_MARGIN  = 4;

BEGIN_MESSAGE_MAP(ScaleBox, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_GETDLGCODE()
	ON_WM_ENABLE()
	ON_MESSAGE(WM_SCROLLVIEWEX_GETSCALE,OnGetScaleParam)
	ON_MESSAGE(WM_SCROLLVIEWEX_SETSCALE,OnSetScaleParam)
	ON_COMMAND_RANGE          (ID_SCALE,ID_SCALE_LAST,OnScaleMenu)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SCALE,ID_SCALE_LAST,OnUpdateScaleMenu)
	ON_COMMAND_EX(ID_SCALEBOX_SCALE,   OnScaleToFitSize)
	ON_COMMAND_EX(ID_SCALEBOX_DOCUMENT,OnScaleToFitSize)
	ON_COMMAND_EX(ID_SCALEBOX_TOTAL,   OnScaleToFitSize)
	ON_UPDATE_COMMAND_UI(ID_SCALEBOX_SCALE,   OnUpdateScaleToFitSize)
	ON_UPDATE_COMMAND_UI(ID_SCALEBOX_DOCUMENT,OnUpdateScaleToFitSize)
	ON_UPDATE_COMMAND_UI(ID_SCALEBOX_TOTAL,   OnUpdateScaleToFitSize)
END_MESSAGE_MAP()

CFont ScaleBox::s_fontTri;
CFont ScaleBox::s_fontNum;

IMPLEMENT_DYNAMIC(ScaleBox,CWnd)

void ScaleBox::clinit() {
	static clinited = FALSE;
	if (clinited) {
		return;
	}
	if (!s_fontTri.CreatePointFont(FONTPOINT_TRI,FONTFACE_TRI)) {
		AfxThrowResourceException();
	}
	
	CFont* defaultFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	if (defaultFont == NULL) {
		AfxThrowResourceException();
	}
	LOGFONT logFont;
	VERIFY(defaultFont->GetLogFont(&logFont));
	ASSERT(AfxIsValidString(logFont.lfFaceName));

	if (!s_fontNum.CreatePointFont(FONTPOINT_NUM,logFont.lfFaceName)) {
		AfxThrowResourceException();
	}
	clinited = TRUE;
}

ScaleBox::ScaleBox()
:	m_nScaleMode(modeScale),
	m_scale(1) {
	clinit();
}

ScaleBox::~ScaleBox() {
}

CString ScaleBox::Scale2String(double scale) {
	CString str;
	int nscale = scale * 100 * 10;
	const int percentScale = nscale / 10;
	const int decimalScale = nscale % 10;
	if (decimalScale == 0 ){
		str.Format(_T("%d"),percentScale);
	} else {
		str.Format(_T("%d.%d"),percentScale,decimalScale);
	}
	return str;
}

BOOL ScaleBox::CompareScale(double r,double l) {
	int ir = r * 100 * 10;
	int il = l * 100 * 10;
	return ir == il;
}

BOOL ScaleBox::IsValidScale(double scale) {
	return (0.05 <= scale && scale <= 10);
}

BOOL ScaleBox::CompareScaleParamWithCurrent(int mode,double scale) const {
	if (m_nScaleMode == mode && CompareScale(m_scale,scale)) {
		return TRUE;
	}
	return FALSE;
}

void ScaleBox::SetScale(int mode,double scale) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	if (CompareScaleParamWithCurrent(mode,scale))
		return;

	m_nScaleMode = mode;
	if (m_nScaleMode == modeScale || scale != -1) {
		ASSERT(IsValidScale(scale));
		m_scale = scale;
	}

	Invalidate();
}

void ScaleBox::SelectScale(int mode,double scale) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	if (CompareScaleParamWithCurrent(mode,scale))
		return;

	SetScale(mode,scale);

	CWnd* pPostWnd = AfxGetMainWnd();
	ASSERT(pPostWnd != NULL && ::IsWindow(pPostWnd->GetSafeHwnd()));
	WPARAM wparam = MAKEWPARAM(ID_SCROLLVIEWEX_SCALE,0);
	LPARAM lparam = (LPARAM) m_hWnd;
	VERIFY(pPostWnd->PostMessage(WM_COMMAND,wparam,lparam));
}

void ScaleBox::OnScaleMenu(UINT n) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	SelectScale(modeScale,SCALES[n - ID_SCALE]);
}

void ScaleBox::OnUpdateScaleMenu(CCmdUI* pcmd) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	int n = pcmd->m_nID - ID_SCALE;
	if (CompareScale(SCALES[n],m_scale)) {
		pcmd->SetCheck();
	}
}

LRESULT ScaleBox::OnSetScaleParam(WPARAM wparam,LPARAM lparam) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	ScrollViewEx::SETSCALEPARAM& param = *reinterpret_cast<ScrollViewEx::SETSCALEPARAM*>(lparam);
	switch (m_nScaleMode) {
	case modeScale:
		param.m_typeSetScale = ScrollViewEx::setScale;
		param.m_scale = m_scale;
		break;
	case modeScaleDocumentToFitSize:
		param.m_typeSetScale = ScrollViewEx::setScaleDocumentToFitSize;
		break;
	case modeScaleTotalToFitSize:
		param.m_typeSetScale = ScrollViewEx::setScaleTotalToFitSize;
		break;
	default:
		ASSERT(FALSE);
	}

	return TRUE;
}

LRESULT ScaleBox::OnGetScaleParam(WPARAM wparam,LPARAM lparam) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	const ScrollViewEx::GETSCALEPARAM* pParam = (ScrollViewEx::GETSCALEPARAM*)lparam;
	SetScale(pParam->m_typeGetScale,pParam->m_scale);

	return TRUE;
}

BOOL ScaleBox::Create(CWnd* pParent,CPoint ptTopLeft,int cx,UINT nID) {
	CRect rc(ptTopLeft,CSize(cx,SCALEBOX_COMBOBOX_HEIGHT + SCALEBOX_TOOLBAR_HEIGHT));

	m_rcTri.SetRect(cx - TRI_CX - LAYOUT_MARGIN,
		            LAYOUT_MARGIN,
					cx - LAYOUT_MARGIN,
					SCALEBOX_COMBOBOX_HEIGHT - LAYOUT_MARGIN);
	m_rcNum.SetRect(LAYOUT_MARGIN,
		            LAYOUT_MARGIN,
		            m_rcTri.left-LAYOUT_MARGIN,
		            SCALEBOX_COMBOBOX_HEIGHT-LAYOUT_MARGIN);
	m_rcCob.SetRect(0,0,cx,SCALEBOX_COMBOBOX_HEIGHT);

	return CWnd::Create(NULL,NULL,WS_CHILD | WS_VISIBLE,rc,pParent,nID);
}

int ScaleBox::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rc;
	rc.SetRectEmpty();
	VERIFY(m_editNum.Create(rc,this,0));
	m_editNum.SetFont(&s_fontNum);
	
	VERIFY(m_toolbar.Create(IDT_SCALEBOX,this));
	m_toolbar.SetWindowPos(NULL,0,SCALEBOX_COMBOBOX_HEIGHT + SCALEBOX_MARGIN,0,0,SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);

	return 0;
}

void ScaleBox::OnPaint() {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	CPaintDC dc(this);
	
	BOOL bEnabled = IsWindowEnabled();
	dc.FillSolidRect(m_rcCob,::GetSysColor(bEnabled ? COLOR_WINDOW : COLOR_BTNFACE));

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(::GetSysColor(bEnabled ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT));
	
	CFont* saved = dc.SelectObject(&s_fontTri);
	ASSERT_VALID(saved);
	dc.DrawText(_T("6"),m_rcTri,DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	dc.SelectObject(&s_fontNum);
	CString str = Scale2String(m_scale) + _T("%");
	dc.DrawText(str,m_rcNum,DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	VERIFY(dc.SelectObject(saved) != NULL);

	dc.Draw3dRect(m_rcCob,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_3DHILIGHT));
	CRect rc = m_rcCob;
	rc.DeflateRect(1,1);
	dc.Draw3dRect(rc,::GetSysColor(COLOR_3DSHADOW),::GetSysColor(COLOR_3DLIGHT));
}

void ScaleBox::OnLButtonDown(UINT nFlags,CPoint pt) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));

	if (m_rcTri.PtInRect(pt)) {	
		EnterSelectScale();
	} else if (m_rcNum.PtInRect(pt)) {
		EnterEditScale();
	} else
		CWnd::OnLButtonDown(nFlags,pt);
}

void ScaleBox::EnterSelectScale() {
	CMenu menu;
	if (menu.CreatePopupMenu()) {
		for (int n = 0; n < SCALE_COUNT;n++) {
			VERIFY(menu.AppendMenu(MF_STRING,
								   ID_SCALE + n,
								   Scale2String(SCALES[n]) + _T("%") 
								   ));
		}

		CPoint pt;
		GetCursorPos(&pt);
		VERIFY(menu.TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,pt.x,pt.y,this));
	}
}

void ScaleBox::EnterEditScale() {
	CRect rc = m_rcNum;
	rc.DeflateRect(1,1);
	if (m_editNum.DoEdit(Scale2String(m_scale),rc)) {
		double scale = _ttof(m_editNum.GetText());
		scale /= 100;
		SelectScale(modeScale,scale);
	}

}

BOOL ScaleBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	CPoint pt;
	VERIFY(GetCursorPos(&pt));
	ScreenToClient(&pt);

	if (m_rcTri.PtInRect(pt)) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	} else if (m_rcNum.PtInRect(pt)) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

UINT ScaleBox::OnGetDlgCode() {
	return DLGC_BUTTON;
}

void ScaleBox::OnEnable(BOOL bEnable) {
	Invalidate();
}

void ScaleBox::OnScaleToFitSize(UINT nID) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));
	
	if (nID == ID_SCALEBOX_SCALE) {
		SelectScale(modeScale,m_scale);
	} else if (nID == ID_SCALEBOX_DOCUMENT) {
		SelectScale(modeScaleDocumentToFitSize,-1);
	} else {
		ASSERT(nID == ID_SCALEBOX_TOTAL);
		SelectScale(modeScaleTotalToFitSize,-1);
	}
}

void ScaleBox::OnUpdateScaleToFitSize(CCmdUI* pcmd) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ScaleBox)));
	UINT nID = pcmd->m_nID;
	pcmd->Enable(IsWindowEnabled());
	if (nID == ID_SCALEBOX_SCALE) {
		pcmd->SetCheck(m_nScaleMode == modeScale? 1 : 0);
	} else if (nID == ID_SCALEBOX_DOCUMENT) {
		pcmd->SetCheck(m_nScaleMode == modeScaleDocumentToFitSize ? 1 : 0);
	} else {
		ASSERT(nID == ID_SCALEBOX_TOTAL);
		pcmd->SetCheck(m_nScaleMode == modeScaleTotalToFitSize    ? 1 : 0);
	}
}
