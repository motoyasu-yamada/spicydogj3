#include "stdafx.h"
#include "ChildDataExchange.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



ChildDataExchange::ChildDataExchange(CWnd* pdlg,CDataExchange* pDX)
:	m_pDX(pDX),
	m_pwndSaved(m_pDX->m_pDlgWnd),
	m_pwndTarget(pdlg)
{
	ASSERT_VALID(m_pwndTarget);
	m_pDX->m_pDlgWnd = m_pwndTarget;
}

ChildDataExchange::~ChildDataExchange() {
	ASSERT_VALID(m_pwndTarget);
	ASSERT_VALID(m_pwndSaved);
	ASSERT(m_pDX->m_pDlgWnd == m_pwndTarget);
	m_pDX->m_pDlgWnd = m_pwndSaved;
}
