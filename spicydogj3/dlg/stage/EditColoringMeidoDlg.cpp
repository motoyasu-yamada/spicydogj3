#include "stdafx.h"
#include "resource.h"
#include "EditColoringMeidoDlg.h"
#include "dlg/stage/EditColoringDlg.h"
#include "model/stage/Coloring.h"
#include "utility/ChildDataExchange.h"
#include "utility/ReenterLock.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(EditColoringMeidoDlg, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

EditColoringMeidoDlg::EditColoringMeidoDlg(Coloring* coloring)
:	m_coloring(coloring),
	m_bUpdateLock(FALSE),
	m_bInited(FALSE)
{
	ASSERT_POINTER(m_coloring,Coloring);
	ReenterLock::InitReenterLock(m_bUpdateLock);
}


void EditColoringMeidoDlg::DoDataExchange(CDataExchange* pDX) {
	ReenterLock lock(m_bUpdateLock);
	if (!lock.Lock()) {
		return;
	}
	ASSERT_POINTER(&m_coloring,Coloring);
	ChildDataExchange cde(this,pDX);

	if (!pDX->m_bSaveAndValidate) {
		if (m_coloring->addRed == m_coloring->addGreen &&
			m_coloring->addBlue == m_coloring->addGreen) {
			//	–¾“x•‰
			if (m_coloring->addRed == 0) {
				m_nMeido = 100 * m_coloring->mulRed - 100;
			// ³
			} else {
				m_nMeido = MulDiv(100,m_coloring->addRed,255);
			}
		} else {
			m_nMeido = 0;
		}

		if (100 < m_nMeido)
			m_nMeido = 100;
		if (m_nMeido < -100)
			m_nMeido = -100;

		if (m_bInited)
			m_scMeido.SetPos(m_nMeido);
	}

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_MEIDO, m_scMeido);
	DDX_Control(pDX, IDCE_MEIDO, m_editMeido);

	if (pDX->m_bSaveAndValidate) {
		m_nMeido = m_scMeido.GetPos();
		if (m_nMeido <= 0) {
			ASSERT(-100 <= m_nMeido);
			m_coloring->addRed   = 0;
			m_coloring->addGreen = 0;
			m_coloring->addBlue  = 0;
		
			m_coloring->mulRed   = 
			m_coloring->mulGreen = 
			m_coloring->mulBlue  = static_cast<double>(100 + m_nMeido)/100;
		} else if (0 < m_nMeido) {
			m_coloring->addRed   =
			m_coloring->addGreen = 
			m_coloring->addBlue  = MulDiv(m_nMeido,255,100);

			m_coloring->mulRed   = 
			m_coloring->mulGreen = 
			m_coloring->mulBlue  = static_cast<double>(100 - m_nMeido)/100;
		}
	}
}

BOOL EditColoringMeidoDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_scMeido.SetRange(-100,100);
	m_scMeido.SetTicFreq(50);
	m_scMeido.SetPos(m_nMeido);
	UpdateMeidoText();

	m_bInited = TRUE;

	return TRUE;
}

void EditColoringMeidoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ASSERT_VALID(pScrollBar);
	ASSERT((CWnd*)pScrollBar == &m_scMeido);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdateMeidoText();
	UpdateData(TRUE);
	EditColoringDlg::Invalidate(this);
}

void EditColoringMeidoDlg::UpdateMeidoText()
{
	ReenterLock lock(m_bUpdateLock);
	if (!lock.Lock())
		return;
	lock.Unlock();

	m_nMeido = m_scMeido.GetPos();
	m_editMeido.SetWindowText(FormatString("%d",m_nMeido));
}
