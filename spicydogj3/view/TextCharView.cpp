#include "stdafx.h"
#include "resource.h"
#include "TextCharView.h"

#include "dlg/ColorPaletteDlg.h"
#include "cmd/char/ModTextCharCmd.h"
#include "SpicydogDoc.h"
#include "utility/DDXUtility.h"
#include "utility/WndUtility.h"
#include "utility/PaintMemDC.h"
#include "utility/ReenterLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(TextCharView, CharDialog)
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_EN_UPDATE(IDCE_TEXT,OnUpdateData)
	ON_EN_UPDATE(IDCE_H,   OnUpdateData)
	ON_EN_UPDATE(IDCE_W,   OnUpdateData)
	ON_CBN_SELCHANGE(IDCC_FONTFACE,OnUpdateData)
	ON_CBN_SELCHANGE(IDCC_FONTSIZE,OnUpdateData)
	ON_BN_CLICKED(IDC_COLORBOX,    OnUpdateData)
	ON_BN_CLICKED(IDCC_AUTORESIZE, OnUpdateData)
	ON_BN_CLICKED(IDCC_ITALIC,     OnUpdateData)
	ON_BN_CLICKED(IDCC_BOLD,       OnUpdateData)
	ON_BN_CLICKED(IDCC_UNDERLINE,  OnUpdateData)
	ON_MESSAGE(WM_CLRBOXEX_SELECT, OnTextColor)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(TextCharView, CharDialog)
TextCharView::TextCharView()
:	m_inited(FALSE),
	m_bReenterLock(FALSE)
{
	Init(IDD_TEXT_CHAR);
	m_psPreview.SetListener(this);
}

TextCharView::~TextCharView() {
}

void TextCharView::DoDataExchange(CDataExchange* pDX) {
	CharDialog::DoDataExchange(pDX);

	ColorBoxEx::DDX_Control(pDX,IDC_COLORBOX,m_colorbox,IDB_COLORBOX,RGB(0x00,0x80,0x80));
	DDX_Control(pDX, IDCS_PREVIEW, m_psPreview);
	DDX_Control(pDX, IDCE_TEXT, m_editText);
	DDX_Control(pDX, IDCS_H, m_spinH);
	DDX_Control(pDX, IDCS_W, m_spinW);
	DDX_Control(pDX, IDCC_FONTSIZE,m_cbFontSize);
	DDX_Control(pDX, IDCC_FONTFACE,m_cbFontFace);
	DDX_CBIndex(pDX, IDCC_FONTSIZE,m_state.fontSize);
	DDX_CBIndex(pDX, IDCC_FONTFACE,m_state.fontFace);
	DDX_Text(pDX, IDCE_TEXT,      m_state.text);
	DDX_Text(pDX, IDCE_W,         m_state.width);
	DDX_Text(pDX, IDCE_H,         m_state.height);
	DDX_Check(pDX,IDCC_ITALIC,    m_state.fontStyleItalic);
	DDX_Check(pDX,IDCC_BOLD,      m_state.fontStyleBold);
	DDX_Check(pDX,IDCC_UNDERLINE, m_state.fontStyleUnderline);
	DDX_Check(pDX,IDCC_AUTORESIZE,m_state.autoResize);
	ColorBoxEx::DDX_COLORREF(pDX,IDC_COLORBOX,m_state.textColor);
	DDV_MinMaxInt(pDX,m_state.width, 0,MAX_TEXT_WIDTH);
	DDV_MinMaxInt(pDX,m_state.height,0,MAX_TEXT_HEIGHT);
}

SpicydogDoc* TextCharView::GetDocument() {
	return DYNAMIC_DOWNCAST(SpicydogDoc,CharDialog::GetDocument());
}

void TextCharView::OnSize(UINT nType, int cx, int cy) {
	if (m_inited) {
		CRect rc;
		m_editText.GetWindowRect(rc);
		ScreenToClient(rc);
		m_editText.SetWindowPos(NULL,0,0,cx,cy - rc.top,SWP_NOMOVE | SWP_NOZORDER);
	}
	CharDialog::OnSize(nType, cx, cy);	
}

void TextCharView::SetButtonIcon(UINT nID,UINT nRes) {
	HICON h = (HICON) ::LoadImage(AfxGetResourceHandle(),
								  MAKEINTRESOURCE(nRes),
								  IMAGE_ICON,
								  16,
								  16,
								  LR_DEFAULTCOLOR);
	ASSERT(h != NULL);
	CButton* p = (CButton*) GetDlgItem(nID);
	ASSERT_VALID(p);
	if (h == NULL || p == NULL) {
		AfxThrowResourceException();
	}
	p->SetIcon(h);
}


BOOL TextCharView::OnInitDialog() {
	CharDialog::OnInitDialog();
	GetTextChar()->GetState(m_state);
	m_spinW.SetRange32(0,MAX_TEXT_WIDTH);
	m_spinH.SetRange32(0,MAX_TEXT_HEIGHT);
	SetButtonIcon(IDCC_BOLD,     IDI_FONTSTYLE_BOLD);
	SetButtonIcon(IDCC_ITALIC,   IDI_FONTSTYLE_ITALIC);
	SetButtonIcon(IDCC_UNDERLINE,IDI_FONTSTYLE_UNDERLINE);
	m_inited = TRUE;
	UpdateData(FALSE);
	UpdateControls();
	return TRUE;
}

void TextCharView::OnOK() {
	if (UpdateData(TRUE)) {
		GetDocument()->DoCmd(new ModTextCharCmd(GetTextChar(),m_state));
		GetDocument()->UpdateAll(NULL);
		CharDialog::OnOK();
	}
}

TextChar* TextCharView::GetTextChar() {
	TextChar* textChar = dynamic_cast<TextChar*>(GetChar());
	ASSERT_VALID(textChar);
	return textChar;
}

void TextCharView::OnUpdateData() {
	ReenterLock lock(m_bReenterLock);
	if (!lock.Lock()) {
		return;
	}
	
	UpdateControls();
	BOOL autoSize = IsDlgButtonChecked(IDCC_AUTORESIZE);
	if (!autoSize) {
		return;
	}

	TextChar::UpdateAutoSize(&m_state);
	SetDlgItemInt(IDCE_W,m_state.width);
	SetDlgItemInt(IDCE_H,m_state.height);
}

void TextCharView::UpdateControls() {
	if (!m_inited) {
		return;
	}
	UpdateData(TRUE);

	EnableDlgItem(this,IDCC_FONTFACE, TRUE);
	EnableDlgItem(this,IDCC_FONTSIZE, TRUE);
	EnableDlgItem(this,IDCC_BOLD,     TRUE);
	EnableDlgItem(this,IDCC_ITALIC,   TRUE);
	EnableDlgItem(this,IDCC_UNDERLINE,TRUE);

	BOOL autoSize   = m_state.autoResize;
	EnableDlgItem(this,IDCE_W,!autoSize);
	EnableDlgItem(this,IDCE_H,!autoSize);
	EnableDlgItem(this,IDCS_W,!autoSize);
	EnableDlgItem(this,IDCS_H,!autoSize);

	m_psPreview.Invalidate();
}

void TextCharView::EnableCustomFontProps(BOOL b) {
	EnableDlgItem(this,IDCC_FONTSIZE,b);
	EnableDlgItem(this,IDCC_BOLD,b);
	EnableDlgItem(this,IDCC_ITALIC,b);
}

LRESULT TextCharView::OnTextColor(WPARAM,LPARAM lparam) {
	COLORREF* pCr = reinterpret_cast<COLORREF*>(lparam);
	CRect rc;
	m_colorbox.GetWindowRect(rc);
	
	ColorPaletteDlg dlg(*pCr,FALSE,rc.left,rc.bottom);
	if (dlg.DoModal() == IDOK) {
		*pCr = dlg.GetColor();
		UpdateControls();
		return TRUE;
	} else {
		return FALSE;
	}
}

void TextCharView::OnPSPaint(PreviewStatic& ps) {
	PaintMemDC dc(&ps);
	CRect rc;
	ps.GetClientRect(rc);

	if (DDXUtility::UpdateDataEx(this,TRUE)) {
		TextChar::DrawPreview(&m_state,&dc,rc,::GetSysColor(COLOR_APPWORKSPACE));
	}
}

