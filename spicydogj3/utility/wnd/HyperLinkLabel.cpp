#include "stdafx.h"
#include "HyperlinkLabel.h"
#include "utility/FileUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(HyperlinkLabel, CStatic)
	//{{AFX_MSG_MAP(HyperlinkLabel)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

HyperlinkLabel::HyperlinkLabel()
:	
	m_pfuncOpenUrl(NULL),
	m_hcurOver	  (NULL),
	m_bMouseOver  (FALSE),
	m_bMouseDown  (FALSE),
	m_crFore(RGB(0x00,0x00,0xFF)),
	m_crOver(RGB(0xFF,0x00,0x00)),
	m_crDown(RGB(0xFF,0x00,0x00))
{
}
HyperlinkLabel::~HyperlinkLabel()
{
}

void HyperlinkLabel::SetColors
(
	COLORREF crFore,
	COLORREF crOver,
	COLORREF crDown
)
{
	m_crFore = crFore;
	m_crOver = crOver;
	m_crDown = crDown;
    if (::IsWindow(m_hWnd))
        Invalidate();
}

void HyperlinkLabel::SetCursor(HCURSOR hcurOver)
{
	m_hcurOver = hcurOver;
}

void HyperlinkLabel::SetCaption(LPCTSTR ptszCaption)
{
	ASSERT(ptszCaption != NULL && AfxIsValidString(ptszCaption));

	m_strCaption = ptszCaption;
	if (::IsWindow(m_hWnd))
	{
		if (!m_strCaption.IsEmpty())
			SetWindowText(m_strCaption);
		else
			SetWindowText(NULL);

		AdjustSize();

		RedrawWindow();
	}
}

void HyperlinkLabel::SetUrl(LPCTSTR ptszUrl)
{
	ASSERT(ptszUrl != NULL && AfxIsValidString(ptszUrl));

	m_strUrl = ptszUrl;

	if (::IsWindow(GetSafeHwnd())) {
		m_ToolTip.UpdateTipText(m_strUrl, this,ID_TOOLTIP);
	}
}

void HyperlinkLabel::OpenUrl(LPCTSTR ptsz) {
	ASSERT(ptsz != NULL && AfxIsValidString(ptsz));

	if (m_pfuncOpenUrl == NULL) {
		InternalOpenUrl(ptsz);
	} else {
		m_pfuncOpenUrl(ptsz);
	}
}

void HyperlinkLabel::InternalOpenUrl(LPCTSTR ptsz) {
	ASSERT(ptsz != NULL && AfxIsValidString(ptsz));
	CWaitCursor wc;
	OpenURL(ptsz);
}

void HyperlinkLabel::AdjustSize() {
	if (!::IsWindow(m_hWnd))
		return;

	CRect rc;
	GetWindowRect(rc);
	CWnd* pParent = GetParent();
	ASSERT_VALID(pParent);
	if (pParent != NULL)
		pParent->ScreenToClient(rc);

	CSize szText;
	CClientDC dc(this);
	CFont* saved = dc.SelectObject(GetFont());
	ASSERT_VALID(saved);
	szText = dc.GetOutputTextExtent(m_strCaption);
	VERIFY(dc.SelectObject(saved) == GetFont());

	SetWindowPos(NULL,0,0,szText.cx,szText.cy,SWP_NOMOVE | SWP_NOZORDER);
}


//------------------------------------------------------------------------
//	URLを開くカスタムプロシージャを設定する
//------------------------------------------------------------------------
HyperlinkLabel::PFUNC_OPEN_URL HyperlinkLabel::SetUrlCallback(PFUNC_OPEN_URL pfunc)
{
	PFUNC_OPEN_URL pfuncOld = m_pfuncOpenUrl;
	m_pfuncOpenUrl = pfunc;
	return pfuncOld;
}

BOOL HyperlinkLabel::PreTranslateMessage(MSG* pMsg) 
{
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------
//	サブクラス化前に初期化処理を行う
//------------------------------------------------------------------------
void HyperlinkLabel::PreSubclassWindow() 
{
    CStatic::PreSubclassWindow();

	EnableClick();
	SetUnderlineFont();
	EnableUrlToolTips();

    AdjustSize();
}

void HyperlinkLabel::EnableClick()
{
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
}

void HyperlinkLabel::SetUnderlineFont()
{
    LOGFONT lf;
    GetFont()->GetLogFont(&lf);
    lf.lfUnderline = TRUE;				//	UnderLine
    m_font.CreateFontIndirect(&lf);
    SetFont(&m_font);
}

void HyperlinkLabel::EnableUrlToolTips()
{
	EnableToolTips(TRUE);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this, m_strUrl,NULL,ID_TOOLTIP);
	m_ToolTip.Activate(TRUE);
}

//------------------------------------------------------------------------
//	ラベルがクリックされたら指定されたURLを開く
//------------------------------------------------------------------------
void HyperlinkLabel::OnClicked() 
{
	CWaitCursor wc;
	if (0 < m_strUrl.GetLength()) 
	{
		m_bMouseDown = TRUE;
		RedrawWindow();

		OpenUrl(m_strUrl);
		
		RedrawWindow();
		m_bMouseDown = FALSE;
	}
}
//------------------------------------------------------------------------
//	マウスがボタンの上にホバーしたのを検地する
//------------------------------------------------------------------------
void HyperlinkLabel::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bMouseOver)
		CStatic::OnMouseMove(nFlags, point);
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		if (rcClient.PtInRect(point))
		{
			m_bMouseOver = TRUE;
			Invalidate();

			ZeroMemory(&m_tme,sizeof(TRACKMOUSEEVENT));
			m_tme.cbSize = sizeof(TRACKMOUSEEVENT);
			m_tme.dwFlags= TME_LEAVE;
			m_tme.dwHoverTime = 0;
			m_tme.hwndTrack   = GetSafeHwnd();
			_TrackMouseEvent(&m_tme);	//	WM_MOUSELEAVEを取得する
		}
	}
}
//------------------------------------------------------------------------
//	マウスがボタン領域かたはなれた
//------------------------------------------------------------------------
LRESULT HyperlinkLabel::OnMouseLeave(WPARAM,LPARAM)
{		
	m_bMouseOver = FALSE;
	Invalidate();

	return 0;
}

//------------------------------------------------------------------------
//	マウスオーバカーソルを設定する
//------------------------------------------------------------------------
BOOL HyperlinkLabel::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
    if (m_hcurOver)
    {	
		if (!m_strUrl.IsEmpty())
			::SetCursor(m_hcurOver);
		else
			::SetCursor(::LoadCursor(NULL,IDC_NO));
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------
//	マウスの状態(ダウン/オーバー)によってラベル色を変更する
//------------------------------------------------------------------------
HBRUSH HyperlinkLabel::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(this);
	ASSERT(nCtlColor == CTLCOLOR_STATIC);

	if (m_bMouseDown)
		pDC->SetTextColor(m_crDown);
	else if (m_bMouseOver)
		pDC->SetTextColor(m_crOver);
	else
		pDC->SetTextColor(m_crFore);

	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}
