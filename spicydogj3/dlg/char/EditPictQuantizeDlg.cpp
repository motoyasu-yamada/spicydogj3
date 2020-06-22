#include "stdafx.h"
#include "resource.h"
#include "EditPictQuantizeDlg.h"
#include "dlg/ColorPaletteDlg.h"
#include "utility/PaintMemDC.h"
#include "utility/ReenterLock.h"
#include "utility/WndUtility.h"
#include "utility/StringUtility.h"
#include "utility/LeadToolUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int COLORS_OF_ROW  = 16;
const int HEIGHT_OF_ROW = 12;
const int MARGIN_H = 4;
const int MARGIN_V = 4;

BEGIN_MESSAGE_MAP(EditPictQuantizeDlg, CDialog)
	ON_BN_CLICKED   (IDCR_BUILDTYPE, OnBuildTypeChanged)
	ON_CBN_SELCHANGE(IDCC_PNG_ACTTYPE,     OnSelchangeActtype)
	ON_BN_CLICKED   (IDCC_PNG_TRANS,       OnTransparency)
	ON_CBN_SELCHANGE(IDCC_PNG_COLORBITS,   OnSelchangeColorbits)
	ON_BN_CLICKED   (IDCC_PNG_DITHER,      OnClickDither)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED   (IDCR_BUILDTYPE2,OnBuildTypeChanged)
	ON_BN_CLICKED   (IDCR_BUILDTYPE3,OnBuildTypeChanged)
	ON_BN_CLICKED(IDCB_RESET, OnReset)
	ON_COMMAND(IDCC_PNG_TRANSCOLOR,OnTransparencyColor)
	ON_MESSAGE(WM_CLRBOXEX_SELECT,      OnClrBoxDropdown)
END_MESSAGE_MAP()

EditPictQuantizeDlg::EditPictQuantizeDlg(PictChar* pPict)
:	CDialog(IDD_EDIT_PICT_QUANTIZE),
	m_bInited(FALSE),
	m_bReenterLock(FALSE),
	m_bPalette(FALSE) {

	ASSERT_VALID(pPict);
	m_data = pPict->GetPictData();
	m_param= m_paramOrg= pPict->GetPictOptParam();
	
	//{{AFX_DATA_INIT(EditPictQuantizeDlg)
	//}}AFX_DATA_INIT
	m_psPreview.SetListener(this);
}

EditPictQuantizeDlg::~EditPictQuantizeDlg() {
}

void EditPictQuantizeDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCC_PNG_COLORBITS,m_cbColorCount);
	DDX_Control(pDX, IDCC_PNG_ACTTYPE,  m_cbActType);
	DDX_Control(pDX, IDCS_JPG_COMPRESS_SLIDER, m_sliderJpgCompress);
	DDX_Control(pDX, IDCS_COUNT_OF_COLORS, m_stColors);
	DDX_Control(pDX, IDCS_PREVIEW,m_psPreview);
	DDX_ActTableCtrl(pDX, IDCS_COLORS, m_ctrlAct);
	ColorBoxEx::DDX_Control(pDX,IDCC_PNG_TRANSCOLOR,m_colorTransparency,IDB_CLRBOX_TRANS,RGB(0x00,0x80,0x80));
}

const PictQuantizeParam& EditPictQuantizeDlg::GetQuantizeParam() const {
	return m_param;
}

BOOL EditPictQuantizeDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	m_ctrlAct.SetToolTipFormat(IDS_PICTOPT_ACTTABLE_TOOLTIP);
	m_sliderJpgCompress.SetRange(0,100);
	LoadData();
	m_bInited = TRUE;
	DoOptimize();

	
	return TRUE;
}

void EditPictQuantizeDlg::UpdateCtrls() {
	int n = CDialog::GetCheckedRadioButton(IDCR_BUILDTYPE,IDCR_BUILDTYPE3);
	BOOL bGif = n == IDCR_BUILDTYPE2;
	BOOL bJpg = n == IDCR_BUILDTYPE3;

	EnableDlgItem(this,IDCR_BUILDTYPE,(!bGif) && (!bJpg));
	EnableDlgItem(this,IDCR_BUILDTYPE2,TRUE);
	EnableDlgItem(this,IDCR_BUILDTYPE3,TRUE);

	EnableDlgItem(this,IDCC_PNG_ACTTYPE,        bGif);
	EnableDlgItem(this,IDCSTATIC_PNG_COLORBITS, bGif);
	EnableDlgItem(this,IDCC_PNG_COLORBITS,      bGif);
	EnableDlgItem(this,IDCC_PNG_DITHER,         bGif);
	EnableDlgItem(this,IDCC_PNG_TRANS,          bGif);

	BOOL bTrans = bGif && IsDlgButtonChecked(IDCC_PNG_TRANS);
	m_colorTransparency.EnableWindow(bTrans);

	EnableDlgItem(this,IDCSTATIC_JPG_RATIO,     bJpg);
	EnableDlgItem(this,IDCS_JPG_COMPRESS,       bJpg);
	EnableDlgItem(this,IDCS_JPG_COMPRESS_SLIDER,bJpg);
	EnableDlgItem(this,IDCSTATIC_JPG_PERB,      bJpg);
	EnableDlgItem(this,IDCSTATIC_JPG_PERE,      bJpg);

	m_psPreview.Invalidate();
	m_ctrlAct.Invalidate();
}

void EditPictQuantizeDlg::DoOptimize() {
	PictChar::Optimize(m_cache,m_data,m_param);
}

int GetValueFromComboBox(const CComboBox& cb,int nMin,int nMax,int nDefault) {
	CString str;
	cb.GetWindowText(str);
	int n = _ttoi(str);
	if (nMin <= n && n <= nMax) {
		return n;
	} else {
		return nDefault;
	}
}

void EditPictQuantizeDlg::OnParamUpdated() {
	if (SaveData()) {
		DoOptimize();
		LoadData();
	}
}

BOOL EditPictQuantizeDlg::SaveData() {
	ReenterLock lock(m_bReenterLock);
	if (!lock.Lock()) {
		return FALSE;
	}

	PictQuantizeParam& param = m_param;

	int nId = GetCheckedRadioButton(IDCR_BUILDTYPE,IDCR_BUILDTYPE3);
	if (nId == IDCR_BUILDTYPE) {
		param.m_quantize = PICT_QUANTIZE_PATHTHROUGH;
	} else if (nId == IDCR_BUILDTYPE3) {
		param.m_quantize = PICT_QUANTIZE_JPG;
	} else {
		ASSERT(nId == IDCR_BUILDTYPE2);
		param.m_quantize = PICT_QUANTIZE_PNG;
	}

	if (param.m_quantize == PICT_QUANTIZE_PNG) {
		int n = m_cbActType.GetCurSel();
		if (n == CB_ERR)
			n = 0;
		param.m_pngPaletteType    = (ACTTYPE) n;
		PictChar::CreateAct(param.m_pngPalette,m_data.m_bmp,param);

		param.m_pngColors = GetValueFromComboBox(m_cbColorCount,2,256,param.m_pngColors);
		param.m_pngUseDither  = (IsDlgButtonChecked(IDCC_PNG_DITHER) == 0) ? 0 : 100;
		if (IsDlgButtonChecked(IDCC_PNG_TRANS) != 0) {
			param.m_pngUseTransparency   = TRUE;
			param.m_pngTransparentColor = m_colorTransparency.GetColor();
			ASSERT(0 <= param.m_pngTransparentColor && param.m_pngTransparentColor <= 0xFFFFFF);
		} else
			param.m_pngUseTransparency    = FALSE;
	} else {
		param.m_pngPalette.Empty();
	}

	if (param.m_quantize == PICT_QUANTIZE_JPG) {
		param.m_jpgCompress = m_sliderJpgCompress.GetPos();
	}

	return TRUE;
}

void EditPictQuantizeDlg::OnBuildTypeChanged() {
	OnParamUpdated();
}

void EditPictQuantizeDlg::OnSelchangeActtype() {
	OnParamUpdated();
}

void EditPictQuantizeDlg::OnTransparency() {
	OnParamUpdated();
}

BOOL UpdateTextBySelChange(CComboBox& cb)
{
	int nSel = cb.GetCurSel();
	if (nSel == -1)
		return FALSE;
	CString strSrc;
	cb.GetWindowText(strSrc);
	
	CString strDst;
	cb.GetLBText(nSel,strDst);
	if (strDst == strSrc)
		return FALSE;

	cb.SetWindowText(strDst);
	return TRUE;
}

void EditPictQuantizeDlg::OnSelchangeColorbits() {
	if (UpdateTextBySelChange(m_cbColorCount)) {
		OnParamUpdated();
	}
}

void EditPictQuantizeDlg::OnClickDither() {
	OnParamUpdated();
}

void EditPictQuantizeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	OnParamUpdated();
}

void EditPictQuantizeDlg::OnReset() {
	m_param = m_paramOrg;
	DoOptimize();
	LoadData();
}

LRESULT EditPictQuantizeDlg::OnClrBoxDropdown(WPARAM wparam,LPARAM lparam) {
	COLORREF* pCr = reinterpret_cast<COLORREF*>(lparam);
	CRect rc;
	m_colorTransparency.GetWindowRect(rc);

	ColorPaletteDlg dlg(*pCr,FALSE,rc.left,rc.bottom);
	if (dlg.DoModal() != IDOK)
		return FALSE;
	m_colorTransparency.SetColor(*pCr = dlg.GetColor());
	OnParamUpdated();
	return TRUE;
}

void EditPictQuantizeDlg::OnTransparencyColor() {
	OnParamUpdated();
}

double CalcScale(CSize szDst,CSize szSrc) {
	double x = static_cast<double>(szDst.cx) / szSrc.cx;
	double y = static_cast<double>(szDst.cy) / szSrc.cy;
	return min(x,y);
}

void CalcRect(CRect& rcDraw,CSize szPict,CRect rcDrawbale) {
	double scale = CalcScale(rcDrawbale.Size(),szPict);
	rcDraw.SetRect(0,0,szPict.cx * scale,szPict.cy * scale);
	CSize sz = rcDrawbale.Size() - rcDraw.Size();
	rcDraw.OffsetRect(rcDraw.left + sz.cx/2,rcDraw.top + sz.cy/2);
}

void EditPictQuantizeDlg::DrawPreview(CDC* pDC) {
	CRect rc;
	m_psPreview.GetClientRect(rc);
	pDC->FillSolidRect(rc,GetSysColor(COLOR_BTNFACE));

	if (!m_cache.m_bValid)
		return;
	CSize sz = m_cache.m_sz;
	CRect rcDraw;
	CalcRect(rcDraw,sz,rc);
	LPaintTransparent(*pDC,
		                rcDraw.left,rcDraw.top,rcDraw.Width(),rcDraw.Height(),
		                m_cache.m_bmp,0,0,sz.cx,sz.cy,m_cache.m_bTrans,m_cache.m_crTrans);
}

void EditPictQuantizeDlg::OnPSPaint(PreviewStatic& ps) {
	ASSERT(&m_psPreview == &ps);
	PaintMemDC dc(&ps);
	DrawPreview(&dc);
}

BOOL EditPictQuantizeDlg::OnPSEraseBkgnd(PreviewStatic& ps,CDC& pDC) {
	return TRUE;
}

void EditPictQuantizeDlg::LoadData() {
	PictQuantizeParam& param = m_param;
	int nId = IDCR_BUILDTYPE2;
	if (param.m_quantize == PICT_QUANTIZE_PATHTHROUGH)
		nId = IDCR_BUILDTYPE;
	else if (param.m_quantize == PICT_QUANTIZE_JPG)
		nId = IDCR_BUILDTYPE3;
	else
		ASSERT(param.m_quantize == PICT_QUANTIZE_PNG);
	CheckRadioButton(IDCR_BUILDTYPE,IDCR_BUILDTYPE3,nId);
	
	if (param.m_quantize == PICT_QUANTIZE_PNG) {
		m_cbActType.SetCurSel(param.m_pngPaletteType);
		m_cbColorCount.SetWindowText(FormatString("%d",param.m_pngColors));
		CheckDlgButton(IDCC_PNG_DITHER,(param.m_pngUseDither == 100)? 1 : 0);
		CheckDlgButton(IDCC_PNG_TRANS, param.m_pngUseTransparency ? 1 : 0);
		m_colorTransparency.SetColor(param.m_pngTransparentColor);
	}

	if (param.m_quantize == PICT_QUANTIZE_JPG) {
		m_sliderJpgCompress.SetPos(param.m_jpgCompress);
		SetDlgItemText(IDCS_JPG_COMPRESS,FormatString("%d%%",param.m_jpgCompress));
	}

	BOOL bPallette = FALSE;
	if (m_cache.m_bValid) {
		SetDlgItemText(IDCS_SIZE,  FormatString("%d",m_cache.m_nDataLen));
		SetDlgItemText(IDCS_FORMAT,m_cache.m_nFormat == PICTOPTOUTFORMAT_PNG ? _T("PNG") : _T("JPEG"));
	
		if (m_cache.m_nFormat == PICTOPTOUTFORMAT_PNG) {
			pBITMAPHANDLE pbmph = m_cache.m_bmp.GetHandle();
			HPALETTE hpal = pbmph->hPalette;
			if (hpal != NULL) {
				const int colors = ::GetPaletteEntries(hpal,0,0,NULL);
				Act act;
				act.CreateFromPalette(pbmph->hPalette,colors);
				m_ctrlAct.SetAct(&act);
				if (param.m_pngUseTransparency) {
					m_ctrlAct.SetTransColor(TRUE,param.m_pngTransparentColor);
				} else {
					m_ctrlAct.SetTransColor(FALSE);
				}
				m_stColors.SetWindowText(FormatString(IDS_ACTTABLE_COLORS,colors));
				bPallette= TRUE;
			}
		}
	} else {
		SetDlgItemText(IDCS_SIZE,  _T("-"));
		SetDlgItemText(IDCS_FORMAT,_T("-"));
	}

	if (!bPallette) {
		m_ctrlAct.ResetAct();
		m_stColors.SetWindowText(_T("-"));
	}

	UpdateCtrls();
}