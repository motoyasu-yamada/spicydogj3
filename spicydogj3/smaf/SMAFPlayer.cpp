#include "stdafx.h"
#include "SMAFPlayer.h"
#include "SMAFPlayerListener.h"
#include "smaf/masmw.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TSingletonPtr<SMAFPlayer> SMAFPlayer::s_player;

SMAFPlayer* SMAFPlayer::QueryPlayer() {
	return s_player.get();
}

SMAFPlayer::SMAFPlayer() {
	m_yamaha    = NULL;
	m_state		= ID_STATE_NODATA;
	m_data		= NULL;
	m_datalen	= 0;
	m_playpos	= 0;
	m_yamaha    = new CMASMW;
	m_smaftype  = SMAF_TYPE_ERROR;
}

SMAFPlayer::~SMAFPlayer() {
	if (m_yamaha != NULL) {
		ASSERT_POINTER(m_yamaha,CMASMW);
		delete m_yamaha;
		m_yamaha = NULL;
	}
	m_listener = NULL;
}

void SMAFPlayer::AssertValid() const {
	ASSERT_POINTER(m_yamaha,CMASMW);
	ASSERT_POINTER(m_listener,SMAFPlayerListener);
	ASSERT(m_smaftype == SMAF_TYPE_MA ||
		   m_smaftype == SMAF_TYPE_PHASE ||
		   m_smaftype == SMAF_TYPE_ERROR);
}

BOOL SMAFPlayer::Initialize(SMAFPlayerListener* listener) {
	ASSERT_POINTER(listener,SMAFPlayerListener);
	m_listener = listener;
	
	LONG r;
	r= m_yamaha->Initialize();
	if (!(MASMW_SUCCESS <= r)) {
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(this);
	return TRUE;
}

BOOL SMAFPlayer::InitSMAF() {
	LONG r;
	switch(m_smaftype) {
	case SMAF_TYPE_MA:
		r = m_yamaha->Open(MASMW_CNVID_MMF, m_fileid, 0, NULL);
		if (!(MASMW_SUCCESS <= r)) {
			ASSERT(FALSE);
			return FALSE;
		}
		r = m_yamaha->Standby(MASMW_CNVID_MMF, m_fileid, NULL);
		if (!(MASMW_SUCCESS <= r)) {
			ASSERT(FALSE);
			return FALSE;
		}
		m_playlen  = m_yamaha->Control(MASMW_CNVID_MMF, m_fileid, MASMW_GET_LENGTH, NULL, NULL);
		m_state    = ID_STATE_READY;
		break;
	case SMAF_TYPE_PHASE:
		m_playlen  = CalcLength();
		m_state    = ID_STATE_READY;
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

BOOL SMAFPlayer::LoadSMAF(HANDLE h) {
	ExitSMAF();
	Release();

	LONG r;

	// データのメモリ展開
	m_datalen = GlobalSize(h);
	m_data = new BYTE[m_datalen];
	LPCVOID src = GlobalLock(h);
	CopyMemory(m_data,src,m_datalen);
	GlobalUnlock(h);
	ASSERT(AfxIsValidAddress(m_data,m_datalen));
	
	// MAロード処理
	m_smaftype = SMAF_TYPE_MA;
	r = m_yamaha->Create(MASMW_CNVID_MMF);
	if (!(MASMW_SUCCESS <= r)) {
		ASSERT(FALSE);
		return FALSE;
	}
	r = m_yamaha->Load(MASMW_CNVID_MMF,m_data,m_datalen,1,_CallBack,NULL);
	if(MASMW_SUCCESS <= r) {
		m_fileid = r;
		InitSMAF();
		ExitSMAF();
		Release();
		AfxMessageBox(IDS_ERR_CAN_NOT_LOAD_MMF,MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	ExitSMAF();


	// フェーズロード処理
	ASSERT(m_smaftype == SMAF_TYPE_ERROR);
	r = m_yamaha->Phrase_Initialize();
	if (!(MASMW_SUCCESS <= r)) {
		ASSERT(FALSE);
		return FALSE;
	}
	m_smaftype = SMAF_TYPE_PHASE;
	m_yamaha->Phrase_SetEvHandler(_CallBack);
	r = m_yamaha->Phrase_SetData(0,m_data,m_datalen,1);
	if(MASMW_SUCCESS <= r) {
		info dat;
		ZeroMemory(&dat,sizeof dat);
		m_yamaha->Phrase_GetInfo(&dat);
		TRACE("MakerID %d\n",    dat.MakerID);
		TRACE("DeviceID %d\n",   dat.DeviceID);
		TRACE("VersionID %d\n",  dat.VersionID);
		TRACE("MaxVoice %d\n",   dat.MaxVoice);
		TRACE("MaxChannel %d\n", dat.MaxChannel);
		TRACE("SupportSMAF %d\n",dat.SupportSMAF);
		TRACE("Latency %d\n",    dat.Latency);
		AfxMessageBox(IDS_WARNING_SMAF_PHRASE_MA2,MB_OK);
		return InitSMAF();
	}
	ExitSMAF();

	ASSERT(FALSE);
	Release();
	return FALSE;
}

void SMAFPlayer::ExitSMAF() {
	switch(m_smaftype) {
	case SMAF_TYPE_PHASE:
		switch(GetStatus()) {
		case ID_STATE_PAUSE :
		case ID_STATE_PLAYING :
			Stop();
		case ID_STATE_READY :
			m_yamaha->Phrase_RemoveData(0);
			break;
		}
		m_yamaha->Phrase_Kill();
		m_yamaha->Phrase_Terminate();
		break;
	case SMAF_TYPE_MA:
		switch(GetStatus()) {
		case ID_STATE_PAUSE :
		case ID_STATE_PLAYING :
			Stop();
		case ID_STATE_READY :
			m_yamaha->Close(MASMW_CNVID_MMF, m_fileid, NULL);
			m_yamaha->UnLoad(MASMW_CNVID_MMF, m_fileid, NULL);
			break;
		}
		m_yamaha->Delete(MASMW_CNVID_MMF);
		break;
	case SMAF_TYPE_ERROR:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_smaftype = SMAF_TYPE_ERROR;
}

void SMAFPlayer::Destroy() {
	ExitSMAF();
	Release();
}

int SMAFPlayer::GetLength(void) const {
	return m_playlen;
}


int SMAFPlayer::GetStatus() const {
	return m_state;
}

void SMAFPlayer::SetVolume(int vol) {
	ASSERT(this);
	ASSERT(0 <= vol && vol <= 127);

	switch(m_smaftype) {
	case SMAF_TYPE_PHASE:
		m_yamaha->Phrase_SetVolume(0, vol);
		break;
	case SMAF_TYPE_MA:
		m_yamaha->Control(MASMW_CNVID_MMF, m_fileid, MASMW_SET_VOLUME, &vol, NULL);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void SMAFPlayer::SetPanpot(int pan) {
	ASSERT(this);
	ASSERT(0 <= pan && pan <= 127);

	switch(m_smaftype) {
	case SMAF_TYPE_PHASE:
		m_yamaha->Phrase_SetPanpot(0, pan);
		break;
	case SMAF_TYPE_MA:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

int SMAFPlayer::GetPosition() const {
	ASSERT(this);
	switch(m_smaftype) {
	case SMAF_TYPE_PHASE:
		return m_yamaha->Phrase_GetPosition(0);
	case SMAF_TYPE_MA:
		return m_yamaha->Control(MASMW_CNVID_MMF,m_fileid, MASMW_GET_POSITION, NULL, NULL);
	default:
		ASSERT(FALSE);
		return 0;
	}
}

void SMAFPlayer::Play(int vol,int pan,BOOL repeat) {
	ASSERT_VALID(this);
	ASSERT(0 <= vol && vol <= 127);
	ASSERT(-127 <= pan && pan <= 127);
	switch(m_smaftype) {
	case SMAF_TYPE_MA:
		m_yamaha->Close(MASMW_CNVID_MMF, m_fileid, NULL);
		m_yamaha->Open(MASMW_CNVID_MMF, m_fileid, 0, NULL);
		m_yamaha->Standby(MASMW_CNVID_MMF, m_fileid, NULL);
		m_yamaha->Seek(MASMW_CNVID_MMF, m_fileid,0, 0, NULL);
		m_yamaha->Control(MASMW_CNVID_MMF, m_fileid, MASMW_SET_VOLUME, &vol, NULL);
		m_yamaha->Start(MASMW_CNVID_MMF, m_fileid,repeat ? 0 : 1, NULL);
		break;
	case SMAF_TYPE_PHASE:
		m_yamaha->Phrase_SetVolume(0, vol);
		m_yamaha->Phrase_SetPanpot(0, pan);
		m_yamaha->Phrase_Play(0,repeat ? 0 : 1);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_state = ID_STATE_PLAYING;
}

void SMAFPlayer::Restart(int vol,int pan) {
	ASSERT_VALID(this);
	ASSERT(0 <= vol && vol <= 127);
	ASSERT(-127 <= pan && pan <= 127);
	switch(m_smaftype) {
	case SMAF_TYPE_MA:
		m_yamaha->Control(MASMW_CNVID_MMF,m_fileid,MASMW_SET_VOLUME,&vol,NULL);
		m_yamaha->Restart(MASMW_CNVID_MMF,m_fileid,NULL);
		break;
	case SMAF_TYPE_PHASE:
		m_yamaha->Phrase_SetVolume(0, vol);
		m_yamaha->Phrase_SetPanpot(0, pan);
		m_yamaha->Phrase_Restart(0);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_state = ID_STATE_PLAYING;
}

void SMAFPlayer::Pause() {
	ASSERT_VALID(this);
	switch(m_smaftype) {
	case SMAF_TYPE_MA:
		m_yamaha->Pause(MASMW_CNVID_MMF,m_fileid,NULL);
		break;
	case SMAF_TYPE_PHASE:
		m_yamaha->Phrase_Pause(0);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_state = ID_STATE_PAUSE;
}

void SMAFPlayer::Release() {
	if(m_data != NULL) {
		delete [] m_data;
		m_data	= NULL;
		m_datalen	= 0L;
	}
	m_state		= ID_STATE_NODATA;
}

void SMAFPlayer::Stop() {
	ASSERT_VALID(this);
	switch(m_smaftype) {
	case SMAF_TYPE_MA:
		m_yamaha->Stop(MASMW_CNVID_MMF,m_fileid, NULL);
		m_yamaha->Seek(MASMW_CNVID_MMF,m_fileid, 0, 0, NULL);
		break;
	case SMAF_TYPE_PHASE:
		m_yamaha->Phrase_Stop(0);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_state = ID_STATE_READY;
}

void SMAFPlayer::_CallBack(struct event* eve) {
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	QueryPlayer()->CallBack(eve);
}

LONG SMAFPlayer::_CallBack(BYTE id) {
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	QueryPlayer()->CallBack(id);
	return (MASMW_SUCCESS);
}

void SMAFPlayer::CallBack(BYTE id) {
	ASSERT_VALID(this);
	switch(id) {
	/*	End of Sequence Event	*/
	case 0x7F :			
		Stop();
		m_listener->OnAutoStop();
		break;
	/*	Repeat Event	*/
	case 0x7E :
		break;
	default :
		if(id < 0x7E) {
			/*	User Event	*/
		}
		break;
	}
}

void SMAFPlayer::CallBack(struct event* eve) {
	switch(eve->mode) {
	// 停止
	case -1 :
		Stop();
		m_listener->OnAutoStop();
		break;
	// ループ
	case -2 :
		break;
	default :
		if(eve->mode < 0x7E) {
			/*	User Event	*/
		}
		break;
	}
}


DWORD SMAFPlayer::CalcLength2(DWORD dwStart, DWORD dwEnd) const {
	ASSERT_VALID(this);
	DWORD	dwDiff = dwEnd - dwStart;
	DWORD	m_playpos;
	if(dwDiff <= 1)	return (dwEnd);

	m_playpos = dwStart + (dwDiff / 2);
	if(m_yamaha->Phrase_Seek(0, m_playpos) < 0) {
		m_playpos = CalcLength2(dwStart, m_playpos);
	} else {
		m_playpos = CalcLength2(m_playpos, dwEnd);
	}

	return (m_playpos);
}

DWORD SMAFPlayer::CalcLength() const {
	ASSERT_VALID(this);
	DWORD	m_playpos = 10000;
	while(m_yamaha->Phrase_Seek(0, (long)m_playpos) == 0) {
		m_playpos += 10000;
	}

	m_playpos = CalcLength2(m_playpos - 10000, m_playpos);
	m_yamaha->Phrase_Seek(0, 0);
	return m_playpos;
}
