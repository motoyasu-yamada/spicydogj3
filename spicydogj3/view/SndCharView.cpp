#include "stdafx.h"
#include "resource.h"
#include "SndCharView.h"
#include "SpicydogDoc.h"
#include "utility/WndUtility.h"
#include "cmd/char/ModSndCharCmd.h"
#include "smaf/masmw.h"
#include "smaf/SMAFPlayer.h"
#include "utility/PaintMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_USER_ON_AUTOSTOP = WM_USER + 1000;
const UINT ELAPSE_TIMER = 10;

BEGIN_MESSAGE_MAP(SndCharView, CharDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDCB_PLAY,  OnPlay)
	ON_BN_CLICKED(IDCB_STOP,  OnStop)
	ON_BN_CLICKED(IDCB_PAUSE, OnPause)
	ON_BN_CLICKED(IDCC_REPEAT,OnCheckRepeat)
	ON_BN_CLICKED(IDCB_RELOAD,OnReload)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


IMPLEMENT_DYNCREATE(SndCharView, CharDialog)
SndCharView::SndCharView() {
	Init(IDD_SND_CHAR);

	m_hTimer = 0;
	m_player = SMAFPlayer::QueryPlayer();
}

SndCharView::~SndCharView()
{
}

void SndCharView::DoDataExchange(CDataExchange* pDX) {
	CharDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDCS_POSITION,m_staticPos);
	DDX_Control(pDX,IDCS_PANPOT,  m_SliderPanpot);
	DDX_Check  (pDX,IDCC_REPEAT,  m_oRepeat);
	DDX_Control(pDX,IDCS_VOLUME,  m_SliderVolume);
}

SndChar* SndCharView::GetSndChar() {
	SndChar* snd = dynamic_cast<SndChar*>(GetChar());
	ASSERT_VALID(snd);
	return snd;
}


BOOL SndCharView::OnInitDialog() {
	CharDialog::OnInitDialog();
	ASSERT(m_player);

	m_SliderVolume.SetRange(0, 127);
	m_SliderVolume.SetTicFreq(10);
	m_SliderVolume.SetPageSize(10);
	m_SliderVolume.SetLineSize(0);
	m_SliderVolume.SetPos(127);

	m_SliderPanpot.SetRange(0, 127);
	m_SliderPanpot.SetTicFreq(10);
	m_SliderPanpot.SetPageSize(10);
	m_SliderPanpot.SetLineSize(0);
	m_SliderPanpot.SetPos(64);

	UpdateData(FALSE);

	UpdateControls();
	ASSERT(m_player);
	if (m_player->Initialize(this)) {
		m_hTimer = SetTimer(1,ELAPSE_TIMER,NULL);
		m_player->LoadSMAF(GetSndChar()->GetHandle());
	} else {
		OnCancel();
		return FALSE;
	}
	
	m_staticPos.SetListener(this);
	m_staticPos.Invalidate();
	UpdateControls();
	return TRUE;
}

void SndCharView::UpdateControls(void) {
	ASSERT(m_player);
	BOOL onPlaying = FALSE;
	BOOL onStoping = FALSE;
	BOOL onValid   = TRUE;
	BOOL cancel    = TRUE;

	switch(m_player->GetStatus()) {
	case ID_STATE_NODATA :
		onValid = FALSE;
		break;
	case ID_STATE_PLAYING :
		onPlaying = TRUE;
		cancel    = FALSE;
		break;
	case ID_STATE_SEEK :
	case ID_STATE_PAUSE :
	case ID_STATE_READY :
		onStoping = TRUE;
		cancel    = TRUE;
		break;
	}
	
	EnableDlgItem(this,IDCB_PLAY,    onStoping);
	EnableDlgItem(this,IDOK,         onStoping);
	EnableDlgItem(this,IDCC_REPEAT,  onStoping);
	EnableDlgItem(this,IDCS_POSITION,onValid);
	EnableDlgItem(this,IDCB_PAUSE,onPlaying);
	EnableDlgItem(this,IDCB_STOP, onPlaying);
	EnableDlgItem(this,IDCANCEL,   cancel);
	EnableDlgItem(this,IDCB_RELOAD,cancel);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND | (cancel ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
	m_staticPos.Invalidate();
}

void SndCharView::Release(void) {
	ASSERT(m_player);
	m_player->Release();
	UpdateControls();
}

void SndCharView::Play(void)
{
	ASSERT(m_player);
	m_player->Play(m_SliderVolume.GetPos(),m_SliderPanpot.GetPos(),m_oRepeat);
	UpdateControls();
}

void SndCharView::Restart(void)
{
	ASSERT(m_player);
	m_player->Restart(m_SliderVolume.GetPos(),m_SliderPanpot.GetPos());
	UpdateControls();
}

void SndCharView::Pause() {
	ASSERT(m_player);
	m_player->Pause();
	UpdateControls();
}

void SndCharView::Stop() {
	ASSERT(m_player);
	m_player->Stop();
	UpdateControls();
}

void SndCharView::OnTimer(UINT nIDEvent) {
	ASSERT(m_player);
	if(m_player->GetStatus() == ID_STATE_PLAYING) {
		m_staticPos.Invalidate();
	}

	CDialog::OnTimer(nIDEvent);
}

void SndCharView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ASSERT(m_player);
	if((DWORD)(&m_SliderVolume) == (DWORD)pScrollBar) {
		if(m_player->GetStatus() == ID_STATE_PLAYING) {
			m_player->SetVolume(m_SliderVolume.GetPos());
		}
	} else if((DWORD)(&m_SliderPanpot) == (DWORD)pScrollBar){
		if(m_player->GetStatus() == ID_STATE_PLAYING) {
			m_player->SetPanpot(m_SliderPanpot.GetPos());
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void SndCharView::OnPlay()
{
	ASSERT(m_player);

	if(m_player->GetStatus() == ID_STATE_READY) {
		Play();
	} else if(m_player->GetStatus() == ID_STATE_PAUSE) {
		Restart();
	}
}

void SndCharView::OnStop() 
{
	ASSERT(m_player);

	if(	(m_player->GetStatus() == ID_STATE_PLAYING)	||
		(m_player->GetStatus() == ID_STATE_PAUSE) ) {
		Stop();
	}
}

void SndCharView::OnPause()
{
	ASSERT(m_player);

	if(m_player->GetStatus() == ID_STATE_PLAYING) {
		Pause();
	}
}

void SndCharView::OnCheckRepeat()
{
	UpdateData(TRUE);
}

void SndCharView::OnDestroy(void) {
	CDialog::OnDestroy();
	m_player->Destroy();

	if(m_hTimer != 0) {
		KillTimer(m_hTimer);
		m_hTimer = 0;
	}
}

void SndCharView::OnReload() {
	SndChar* snd = GetSndChar();

	CString strFilter;
	VERIFY(strFilter.LoadString(IDS_SND_FILTER));
	CFileDialog dlg(TRUE,NULL,NULL,0,strFilter,this);
	if (dlg.DoModal() != IDOK) {
		return;
	}
	CFile file(dlg.GetPathName(),CFile::modeRead | CFile::shareDenyNone);
	UINT length = file.GetLength();
	HGLOBAL h = GlobalAlloc(0,length);
	LPVOID p = GlobalLock(h);
	ASSERT(AfxIsValidAddress(p,length));
	file.Read(p,length);
	GlobalUnlock(h);

	GetDocument()->DoCmd(new ModSndCharCmd(snd,h));
	GetDocument()->UpdateAll(NULL);

	ASSERT(m_player);
	VERIFY(m_player->LoadSMAF(GetSndChar()->GetHandle()));
	UpdateControls();
}

void SndCharView::OnAutoStop() {
	UpdateControls();
}

inline COLORREF MixColor(double ratio,COLORREF left,COLORREF right) {
	return RGB(
			GetRValue(left) * (1-ratio) + GetRValue(right) * ratio,
			GetGValue(left) * (1-ratio) + GetGValue(right) * ratio,
			GetBValue(left) * (1-ratio) + GetBValue(right) * ratio);
}

void GradientFillH(CDC& dc,CRect& rc,COLORREF left,COLORREF right) {
	int width  = rc.Width();
	int height = rc.Height();
	for (int x = rc.left;x < rc.right;x++) {
		COLORREF cr = MixColor(static_cast<double>(x-rc.left)/width,left,right);
		dc.FillSolidRect(x,rc.top,1,height,cr);
	}
}

void GradientFillV(CDC& dc,CRect& rc,COLORREF left,COLORREF right) {
	int width  = rc.Width();
	int height = rc.Height();
	for (int y = rc.top;y < rc.bottom;y++) {
		COLORREF cr = MixColor(static_cast<double>(y-rc.top)/height,left,right);
		dc.FillSolidRect(rc.left,y,width,1,cr);
	}
}

void SndCharView::OnPSPaint(PreviewStatic& s) {
	PaintMemDC dc(&s);
	CRect rc;
	s.GetClientRect(rc);

	dc.Draw3dRect(rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DLIGHT));
	rc.DeflateRect(1,1);
	dc.Draw3dRect(rc,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHIGHLIGHT));
	rc.DeflateRect(1,1);

	CRect rc1 = rc;
	CRect rc2 = rc;
	rc1.bottom = rc.top + rc.Height() / 2;
	rc2.top    = rc1.bottom;
	GradientFillV(dc,rc1,RGB(0x55,0x55,0x55),RGB(0x00,0x00,0x00));
	GradientFillV(dc,rc2,RGB(0x00,0x00,0x00),RGB(0x77,0x77,0x77));
	
	int status;
	status = m_player->GetStatus();
	if (ID_STATE_READY  == status ||
		ID_STATE_NODATA == status) {
		return;
	}

	CRect rcB  = rc;
	rcB.right = rcB.left + rcB.Width() * m_player->GetPosition() / m_player->GetLength();
	GradientFillH(dc,rcB,RGB(0x00,0x00,0x33),RGB(0x00,0x00,0x80));
	//TRACE("%d / %d \n",m_player->GetPosition(),m_player->GetLength());
}

