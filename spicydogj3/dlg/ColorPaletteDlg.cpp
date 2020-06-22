#include "stdafx.h"
#include "resource.h"
#include "ColorPaletteDlg.h"
#include "utility/PaintMemDC.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CELL_H_COUNT = 16;
const int MAX_COLORS = 256;

BEGIN_MESSAGE_MAP(ColorPaletteDlg, CDialog)
	ON_COMMAND(IDCE_CLRVAL,      OnClrVal)
	ON_BN_CLICKED(IDCB_CLR_DLG,  OnClrDlg)
	ON_BN_CLICKED(IDCB_CLR_NULL, OnClrNull)
	ON_BN_CLICKED(IDCB_CLR_SPOIT,OnClrSpoit)
	ON_EN_UPDATE(IDCE_CLRVAL,    OnUpdateClrval)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDCS_PALETTE,  OnPalette)
	ON_BN_CLICKED(IDCB_CLR_CLOSE,OnClrClose)
	ON_CBN_SELCHANGE(IDCC_ACT,   OnSelchangeAct)
END_MESSAGE_MAP()

ACTTYPE ColorPaletteDlg::s_nDefaultAct = ACTTYPE_IMODE;
ColorPaletteDlg::ColorPaletteDlg(COLORREF cr,BOOL bTrans,int x,int y,CWnd* pParent /*=NULL*/)
:	CDialog(IDD_COLOR_PALETTE, pParent),
	m_crSelect(cr),
	m_crCurrent(cr),
	m_xInit(x),
	m_yInit(y),
	m_bEnableTrans(bTrans)
{
}

void ColorPaletteDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ColorPaletteDlg)
	DDX_Control(pDX, IDCC_ACT, m_cbACTs);
	DDX_Control(pDX, IDCB_CLR_CLOSE, m_btnClose);
	DDX_Control(pDX, IDCE_CLRVAL,   m_editClrVal);
	DDX_Control(pDX, IDCS_PALETTE,  m_ColorPal);
	DDX_Control(pDX, IDCS_CLRBOX,	m_ColorBox);
	DDX_Control(pDX, IDCB_CLR_SPOIT,m_btnSpoit);
	DDX_Control(pDX, IDCB_CLR_NULL, m_btnNull);
	DDX_Control(pDX, IDCB_CLR_DLG,  m_btnDlg);
	//}}AFX_DATA_MAP
}

BOOL ColorPaletteDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	SetupButtons();

	UpdateACTsComboBox();

	m_ColorPal.SetListener(this);
	AdjustPalletteBox();
	
	SetCurrentColor(m_crCurrent);
	m_btnNull.EnableWindow(m_bEnableTrans);

	return TRUE;
}

const Act* ColorPaletteDlg::GetCurAct() const {
	return CDefaultActs::GetDefaultActs()->GetAct(s_nDefaultAct);
}

void ColorPaletteDlg::OnClrDlg() {
	CColorDialog dlg(m_crSelect);
	if (dlg.DoModal() == IDOK) {
		SelectColor(dlg.GetColor());
	} else {
		Cancel();
	}
}

void ColorPaletteDlg::OnClrNull() {
	ASSERT(m_bEnableTrans);
	SelectColor(NULLCOLOR);
}

COLORREF ColorPaletteDlg::GetColor() const
{
	return m_crSelect;
}

void ColorPaletteDlg::OnClrSpoit() {
	ColorPicker picker(this,IDC_SPOIT);
	picker.SetListener(this);
	if (picker.DoPick()) {
		SelectColor(picker.GetColor());
	} else {
		Cancel();
	}
}

void ColorPaletteDlg::OnPickerMove(CPoint,COLORREF cr) {
	SetCurrentColor(cr);
}

/**
	色値を文字列表現に変換する

	@param	cr	色値
	@return		#RRGGBB	色値の文字列表現
*/
CString RGB2STRING(COLORREF cr) {
	CString strVal;
	strVal.Format(_T("#%02X%02X%02X"),GetRValue(cr),GetGValue(cr),GetBValue(cr));
	return strVal;
}

/**
	色表現を意味する文字列を色値に変換する
	
	@param	ptsz	##RRGGBB
	@return			文字列が意味する色値
*/
COLORREF STRING2RGB(LPCTSTR ptsz) {
	ASSERT(ptsz != NULL);
	ASSERT(AfxIsValidString(ptsz));

	COLORREF cr;
	if (ptsz != NULL && *ptsz == _T('#'))
	{
		int r = 0;
		int g = 0;
		int b = 0;
		ptsz++;
		_stscanf(ptsz,"%02X%02X%02X",&r,&g,&b);
		cr = RGB(r,g,b);
	}
	else
		cr = _ttoi(ptsz);
	
	return cr;
}

void ColorPaletteDlg::OnUpdateClrval() 
{
	CString strVal;
	m_editClrVal.GetWindowText(strVal);
	m_crSelect = STRING2RGB(strVal);
}

void ColorPaletteDlg::SetCurrentColor(COLORREF cr)
{
	m_crCurrent = cr;
	m_ColorBox.SetColor(m_crCurrent);
	
	m_editClrVal.SetWindowText(RGB2STRING(m_crCurrent));

	m_ColorPal.Invalidate();
}


void ColorPaletteDlg::OnMouseMove(UINT nFlags, CPoint) 
{
	UNUSED_ALWAYS(nFlags);

	CPoint pt;
	VERIFY(GetCursorPos(&pt));
	COLORREF cr = GetColorFromPoint(pt);
	if (cr != NULLCOLOR && cr != m_crCurrent)
	{
		SetCurrentColor(cr);
	}
}


void ColorPaletteDlg::OnPalette() 
{
	CPoint pt;
	VERIFY(GetCursorPos(&pt));
	SelectColor(GetColorFromPoint(pt));
}

void ColorPaletteDlg::OnPSMouseMove(PreviewStatic&,UINT,CPoint)
{
	CPoint pt;
	VERIFY(GetCursorPos(&pt));
	COLORREF cr = GetColorFromPoint(pt);
	if (cr != m_crCurrent)
	{
		SetCurrentColor(cr);
	}
}

void ColorPaletteDlg::OnClrVal()
{
	OnUpdateClrval();
	SelectColor(m_crSelect);
}

void ColorPaletteDlg::SelectColor(COLORREF cr)
{
	m_crSelect = cr;
	UpdateData(FALSE);
	EndDialog(IDOK);
}

void ColorPaletteDlg::OnOK()
{
	OnClrVal();
}

void ColorPaletteDlg::Cancel()
{
	EndDialog(IDCANCEL);
}

void ColorPaletteDlg::SetupButtons()
{
	VERIFY(m_btnNull.LoadBitmap(IDB_CLR_NULL));
	VERIFY(m_btnDlg.LoadBitmap(IDB_CLR_DLG));
	VERIFY(m_btnSpoit.LoadBitmap(IDB_CLR_SPOIT));
	VERIFY(m_btnClose.LoadBitmap(IDB_CLR_CLOSE));
}

COLORREF ColorPaletteDlg::GetColorFromPoint(const CPoint& pt) const {
	CRect rc;
	m_ColorPal.GetWindowRect(rc);

	if (!rc.PtInRect(pt)) {
		return NULLCOLOR;
	}

	CPoint ptClient(pt);
	m_ColorPal.ScreenToClient(&ptClient);

	int nIndex = (ptClient .x / m_nCellW)  + (ptClient.y / m_nCellH) * CELL_H_COUNT;
	const Act* pAct = GetCurAct();
	if (nIndex < pAct->GetCount())
		return pAct->GetColor(nIndex);
	else
		return RGB(0x00,0x00,0x00);
}

void ColorPaletteDlg::AdjustPalletteBox() {
	//	全てのパレットセルの大きさを統一するように、
	//	パレット領域の幅・高さをセルサイズの公倍数に調整。
	CRect rc;
	m_ColorPal.GetWindowRect(rc);
	ScreenToClient(rc);
	CRect rcDlg;
	GetWindowRect(rcDlg);
	int nMargin = rcDlg.bottom - rc.bottom;

	//	セルのサイズを求める
	m_nCellW = rc.Width() / CELL_H_COUNT;
	m_nCellH = m_nCellW ;

	//	パレット領域のサイズを求める。
	m_nPalW  = m_nCellW * CELL_H_COUNT;
	const int nCellCntH = (MAX_COLORS + CELL_H_COUNT - 1) / CELL_H_COUNT;
	m_nPalH  = m_nCellH * nCellCntH;

	rc.bottom = rc.top  + m_nPalH + 4;
	rc.right  = rc.left + m_nPalW + 4;
	m_ColorPal.MoveWindow(rc);

	rcDlg.bottom = rc.bottom + nMargin;

	if (m_xInit != -1 && m_yInit != -1) {
		MoveRectTo(rcDlg,m_xInit,m_yInit);	
	}

	CWnd* pDesktop = CWnd::GetDesktopWindow();
	ASSERT_VALID(pDesktop);
	CRect rcDesktop;
	pDesktop->GetClientRect(rcDesktop);

	if (rcDesktop.bottom < rcDlg.bottom) {
		int nHeight = rcDlg.Height();
		rcDlg.bottom = rcDlg.top;
		rcDlg.top = rcDlg.bottom - nHeight;
	}
	if (rcDesktop.right < rcDlg.right) {
		int nWidth = rcDlg.Width();
		rcDlg.right = rcDlg.left;
		rcDlg.left = rcDlg.right - nWidth;
	}

	SetWindowPos(NULL,rcDlg.left,rcDlg.top,rcDlg.Width(),rcDlg.Height(),SWP_NOZORDER | SWP_NOACTIVATE);
	
	ShowWindow(SW_SHOW);
	UpdateWindow();

	m_ColorPal.Invalidate();
}

void ColorPaletteDlg::OnPSPaint(PreviewStatic& ps) {
	PaintMemDC dc(&ps);

	CBrush brGrid;
	VERIFY(brGrid.CreateSolidBrush(RGB(0,0,0)));

	dc.FillSolidRect(0,0,m_nPalW+4,m_nPalH+4,RGB(0x80,0x80,0x80));

	int nCount = GetCurAct()->GetCount();
	for (int n = 0;n < nCount;n++)
	{
		const int x = n % CELL_H_COUNT;
		const int y = n / CELL_H_COUNT;
		
		CRect rc(CPoint(2+x * m_nCellW,2+y * m_nCellH),CSize(m_nCellW,m_nCellH));
		COLORREF cr = GetCurAct()->GetColor(n);
		dc.FillSolidRect(rc,cr);
		rc.InflateRect(0,0,1,1);
		dc.FrameRect(rc,&brGrid);
	}

	CRect rc(0,0,m_nPalW+4,m_nPalH+4);
	dc.Draw3dRect(rc,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHILIGHT));
	rc.DeflateRect(1,1);
	dc.Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DLIGHT));
}

BOOL ColorPaletteDlg::OnPSEraseBkgnd(PreviewStatic&,CDC&) {
	return TRUE;
}

void ColorPaletteDlg::OnClrClose() 
{
	Cancel();	
}

void ColorPaletteDlg::OnSelchangeAct() 
{
	int nCurSel = m_cbACTs.GetCurSel(); 
	if (nCurSel != -1)
	{
		ASSERT(ACTTYPE_IMODE <= nCurSel && nCurSel < ACTTYPE_CUSTOM);
		s_nDefaultAct = (ACTTYPE)nCurSel;
		m_ColorPal.Invalidate();
	}
}

void ColorPaletteDlg::UpdateACTsComboBox()
{
	while (m_cbACTs.GetCount() != 0)
		m_cbACTs.DeleteString(0);

	const CDefaultActs* pActs = CDefaultActs::GetDefaultActs();
	ASSERT(ACTTYPE_IMODE == 0);
	m_cbACTs.AddString(pActs->GetActName(ACTTYPE_IMODE));
	m_cbACTs.AddString(pActs->GetActName(ACTTYPE_WEB));
	m_cbACTs.AddString(pActs->GetActName(ACTTYPE_WIN));
	m_cbACTs.AddString(pActs->GetActName(ACTTYPE_MAC));
	m_cbACTs.AddString(pActs->GetActName(ACTTYPE_GRAY));
	m_cbACTs.AddString(pActs->GetActName(ACTTYPE_MONO));
	m_cbACTs.AddString(pActs->GetActName(ACTTYPE_RGB));
	m_cbACTs.SetCurSel(s_nDefaultAct);
}

void ColorPaletteDlg::OnMouseDown(CPoint pt)
{
	CRect rc;
	GetClientRect(rc);
	if (!rc.PtInRect(pt))
		EndDialog(IDCANCEL);
}
