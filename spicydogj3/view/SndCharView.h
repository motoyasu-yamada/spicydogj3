#pragma once

#include "view/CharView.h"
#include "model/char/SndChar.h"
#include "utility/wnd/PreviewStatic.h"
#include "smaf/SMAFPlayerListener.h"

class SMAFPlayer;
class SndCharView 
:	public CharDialog,
	public PreviewStatic::PaintEventListener,
	public SMAFPlayerListener {
	DECLARE_DYNCREATE(SndCharView)
private:
	BOOL		  m_oRepeat;
	PreviewStatic m_staticPos;
	CSliderCtrl	  m_SliderPanpot;
	CSliderCtrl	  m_SliderVolume;
	UINT		  m_hTimer;
	SMAFPlayer*   m_player;
protected:
	SndCharView();
private:
	void		UpdateControls();
	void		Release();
	void		Play();
	void		Restart();
	void		Pause();
	void		Stop();
	DWORD		CalcLength(DWORD dwStart, DWORD dwEnd);
	DWORD		GetLength();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnCheckRepeat();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReload();
	virtual void OnAutoStop();
	virtual ~SndCharView();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnPSPaint(PreviewStatic&);
	SndChar* GetSndChar();

	DECLARE_MESSAGE_MAP()
};
