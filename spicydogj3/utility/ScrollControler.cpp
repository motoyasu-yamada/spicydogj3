#include "stdafx.h"
#include "ScrollControler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ScrollControler::ScrollControler
(
	CWnd* pParent,
	int& nNext,
	UINT nSBCode, 
	UINT nPos, 
	CScrollBar* pScrollBar,
	int nMax,
	int nPage
)
:	m_pParent(pParent),
	m_nNext(nNext),
	m_nSBCode(nSBCode),
	m_nPos(nPos),
	m_pScrollBar(pScrollBar),
	m_nMax(nMax),
	m_nPage(nPage)
{
}

void ScrollControler::Scroll() {
	ASSERT(m_pScrollBar != NULL);
	ASSERT_VALID(m_pScrollBar);

	int nPrev = m_pScrollBar->GetScrollPos();
	if (m_nMax == 0)
		return;
	
	switch (m_nSBCode)
	{
	case SB_ENDSCROLL:
		//m_pParent->Invalidate(FALSE);
		break;
	// 1 行下へスクロール。
	case SB_LINEDOWN:
		m_nNext = nPrev + 1;
		break;
	// 1 行上へスクロール。
	case SB_LINEUP:
		m_nNext = nPrev - 1;
		break;
	// 一番上までスクロール。
	case SB_TOP:
		m_nNext = 0;
		break;
	// 一番下までスクロール。
	case SB_BOTTOM:
		m_nNext = m_nMax-1;
		break;
	// 絶対位置へスクロール。現在位置は nPos で提供。
	case SB_THUMBPOSITION:
	// 指定位置へスクロール ボックスをドラッグ。現在位置は nPos で提供。
	case SB_THUMBTRACK:		
		m_nNext = m_nPos;
		break;
	// 1 ページ下へスクロール。
	case SB_PAGEDOWN:
		m_nNext = nPrev + m_nPage;
		break;
	// 1 ページ上へスクロール。
	case SB_PAGEUP:
		m_nNext = nPrev - m_nPage;
		break;
	default:
		m_nNext = 0;
		ASSERT(FALSE);
		break;
	}
	
	if (m_nNext < 0)
		m_nNext = 0;
	if (m_nMax <= m_nNext)
		m_nNext = m_nMax - 1;

	m_pScrollBar->SetScrollPos(m_nNext);
	return;
}