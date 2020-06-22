#include "STDAFX.H"
#include "SplashWnd.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT IDT_POP			= 2; // ���������^�C�}�[
const UINT IDT_HIDE			= 3; // �x�������^�C�}�[
const UINT MMSECS_HIDE_DELAY_MOUSE_CLICK	= 500;
const UINT MMSECS_HIDE_DELAY_KEY_DOWN		= 500;

BEGIN_MESSAGE_MAP(SplashWnd, CWnd)
	//{{AFX_MSG_MAP(SplashWnd)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

SplashWnd* SplashWnd::s_pSingleInstance = NULL;

void SplashWnd::DoSplash(UINT resid) {
	ASSERT(s_pSingleInstance == NULL);

	try {
		s_pSingleInstance = new SplashWnd(resid);
		if (s_pSingleInstance) {
			s_pSingleInstance->_DoSplash();
		}
	} catch(CException* pe) {
		DEBUG_ONLY(pe->ReportError());
		ASSERT(FALSE);
		pe->Delete();
	}
}

void SplashWnd::EndSplash(UINT n) {
	ASSERT(s_pSingleInstance != NULL);
	s_pSingleInstance->_EndSplash(n);
}

SplashWnd::SplashWnd(UINT resid) {
	if (!m_bmpSplash.LoadBitmap(MAKEINTRESOURCE(resid))) {
		AfxThrowResourceException();
	}

	BITMAP bm;
	m_bmpSplash.GetBitmap(&bm);
	m_szBitmap = CSize(bm.bmWidth,bm.bmHeight);
}

void SplashWnd::PostNcDestroy()  {	
	CWnd::PostNcDestroy();

	ASSERT(s_pSingleInstance == this);
	delete this;
	s_pSingleInstance = NULL;
}

SplashWnd::~SplashWnd() {
}

void SplashWnd::_DoSplash() {
	ASSERT(s_pSingleInstance != NULL);

	if (m_szBitmap == CSize(0,0))
	{
		ASSERT(FALSE);
		return;
	}

	// �f�X�N�g�b�v�̒����ɕ\��
	CRect rcDesktop;
	GetDesktopWindow()->GetClientRect(&rcDesktop);
	CRect rcThis;
	rcThis.SetRect(rcDesktop.CenterPoint()-CSize(m_szBitmap.cx/2,m_szBitmap.cy/2),
				   rcDesktop.CenterPoint()+CSize(m_szBitmap.cx/2,m_szBitmap.cy/2));

	if (!CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,AfxRegisterWndClass(CS_OWNDC),NULL,WS_POPUP,rcThis,NULL,0))
	{
		ASSERT(FALSE);
		return;
	}
	ShowWindow(SW_SHOW);
	UpdateWindow();

	SetForegroundWindow();
}
//------------------------------------------------------------------------
//	�^�C�}�[����
//------------------------------------------------------------------------
void SplashWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	if(nIDEvent == IDT_POP)
	{
		ShowWindow(SW_HIDE);
		DestroyWindow();
	}
	else if (nIDEvent == IDT_HIDE)
	{
		ShowWindow(SW_HIDE);
	}
	else
		ASSERT(FALSE);
}
//////////////////////////////////////////////////////////////////////////
//	�X�v���b�V���X�N���[�������g���K�[�C�x���g/����
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
//	uiWait�_�b��ɍŏI�I�ɃX�v���b�V���E�C���h�E��j�󂷂�B
//------------------------------------------------------------------------
void SplashWnd::_EndSplash(UINT uiWait)
{
	VERIFY(SetTimer(IDT_POP,uiWait,NULL));
}
//------------------------------------------------------------------------
//  uiWait�_�b�x�����ăE�C���h�E���\���ɂ���B
//  - �e�}�E�X/�L�[�{�[�h�C�x���g����Ăяo�����
//------------------------------------------------------------------------
void SplashWnd::HideSplash(UINT uiWait)
{
	VERIFY(SetTimer(IDT_HIDE,uiWait,NULL)); 
}
//------------------------------------------------------------------------
// �}�E�X�̍��N���b�N��AMMSECS_HIDE_DELAY_MOUSE_LICK �_�b��ɔ�\��
//------------------------------------------------------------------------
void SplashWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(nFlags);
	HideSplash(MMSECS_HIDE_DELAY_MOUSE_CLICK);
}
//------------------------------------------------------------------------
// �}�E�X�̍��N���b�N��AMMSECS_HIDE_DELAY_MOUSE_LICK�_�b��ɔ�\��
//------------------------------------------------------------------------
void SplashWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	UNUSED_ALWAYS(nChar);
	UNUSED_ALWAYS(nRepCnt);
	UNUSED_ALWAYS(nFlags);
	HideSplash(MMSECS_HIDE_DELAY_KEY_DOWN);
}
//------------------------------------------------------------------------
//	�X�v���b�V���X�N���[���̕`��
//------------------------------------------------------------------------
void SplashWnd::OnNcPaint() 
{
	CClientDC dc(this);
	CDC dcMem;
	if (dcMem.CreateCompatibleDC(&dc))
	{
		CBitmap* pSaved = dcMem.SelectObject(&m_bmpSplash);
		ASSERT(pSaved != NULL);
			dc.BitBlt(0,0,m_szBitmap.cx,m_szBitmap.cy,&dcMem,0,0,SRCCOPY);
		
		VERIFY(dcMem.SelectObject(pSaved) != NULL);
	}
	else
		ASSERT(FALSE);
}
//------------------------------------------------------------------------
// ������h�~
//------------------------------------------------------------------------
BOOL SplashWnd::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}
