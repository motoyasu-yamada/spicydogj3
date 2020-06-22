#include "stdafx.h"
#include "resource.h"
#include "PictEffectPictDlg.h"
#include "utility/PaintMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PictEffectPictDlg, CDialog)
	//{{AFX_MSG_MAP(PictEffectPictDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

PictEffectPictDlg::PictEffectPictDlg
(
	const LBitmapBase& bmp,
	UINT nEffectNameID,
	UINT nParamNameID,
	UINT nUnitNameID,
	int nMin,
	int nMax,
	int nDefault,
	PICT_EFFECT_PROC proc,
	CWnd* pParent
) :	CDialog(IDD_PICT_EFFECT, pParent),
	m_nMin(nMin),
	m_nMax(nMax),
	m_nDefault(nDefault),
	m_proc(proc)
{
	ASSERT(m_proc != NULL);
	ASSERT(m_nMin <= m_nDefault && m_nDefault <= m_nMax);

	m_bmpSrc.Copy(const_cast<LBitmapBase&>(bmp));

	m_strParamName.LoadString(nParamNameID);
	m_strUnit.LoadString(nUnitNameID);
	m_strEffectName.LoadString(nEffectNameID);

	m_psPreview.SetListener(this);
	m_nParam = m_nDefault;
}

void PictEffectPictDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_PARAM, m_scParam);
	DDX_Control(pDX, IDCS_PREVIEW, m_psPreview);
	DDX_Text(pDX, IDCE_PARAM, m_nParam);
	DDX_Text(pDX, IDCS_PARAMNAME, m_strParamName);
	DDX_Text(pDX, IDCS_UNIT, m_strUnit);
	DDX_Text(pDX, IDCS_EFFECT_NAME, m_strEffectName);
}

void PictEffectPictDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	m_nParam = m_scParam.GetPos();
	UpdateData(FALSE);
	UpdatePreview();
}

BOOL PictEffectPictDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	int n = m_nParam;
	m_scParam.SetRange(m_nMin,m_nMax);
	m_scParam.SetTicFreq((m_nMax - m_nMin)/10);
	m_scParam.SetPos(n);
	UpdateData(TRUE);
	UpdatePreview();
	
	return TRUE;
}

void PictEffectPictDlg::UpdatePreview() {
	CWaitCursor wc;
	if (m_bmpDst.IsAllocated()) {
		m_bmpDst.Free();
	}
	m_bmpDst.Copy(const_cast<LBitmapBase&>(m_bmpSrc));
	VERIFY((*m_proc)(m_bmpDst.GetHandle(),m_nParam));
	m_psPreview.Invalidate();
}

int PictEffectPictDlg::GetEffectParam() const
{
	return m_nParam;
}

void PictEffectPictDlg::OnPSPaint(PreviewStatic& ps) {
	PaintMemDC dc(&ps);

	CRect rc;
	ps.GetClientRect(rc);
	dc.FillSolidRect(rc,GetSysColor(COLOR_APPWORKSPACE));

	BITMAPHANDLE* pbmph = m_bmpDst.GetHandle();
	CRect rcDst(0,0,m_bmpDst.GetWidth(),m_bmpDst.GetHeight());

	CPalette* pOurPalette = CPalette::FromHandle(L_CreatePaintPalette(dc,pbmph));
	CPalette* pSavedPalette = NULL;
	if (pOurPalette != NULL) {
		pSavedPalette = dc.SelectPalette(pOurPalette, FALSE);
		ASSERT(pSavedPalette != NULL);
		dc.RealizePalette();
	}
	
	L_PaintDC(dc,pbmph,NULL,NULL,&rcDst,NULL,SRCCOPY);//,EFFECT_SPIRAL_IN);

	if (pOurPalette != NULL) {
		VERIFY(dc.SelectPalette(pSavedPalette, FALSE) != NULL);
	}
}

BOOL PictEffectPictDlg::OnPSEraseBkgnd(PreviewStatic&,CDC&) {
	return TRUE;
}
