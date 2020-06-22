//	$Id: SMAFPlayer.h,v 1.1 2003/06/01 04:37:15 m_yamada Exp $
#pragma once
#include "utility/MemoryUtility.h"

typedef enum {
	ID_STATE_NODATA = 0,
	ID_STATE_READY,
	ID_STATE_PLAYING,
	ID_STATE_PAUSE,
	ID_STATE_SEEK
} IDSTATE;

class CMASMW;
class SMAFPlayerListener;

class SMAFPlayer : public CObject {
private:
	/** SMAFファイル形式 */
	enum SMAF_TYPE { SMAF_TYPE_PHASE,SMAF_TYPE_MA,SMAF_TYPE_ERROR };
	int     m_smaftype;
	/** YAMAHAライブラリ */
	CMASMW* m_yamaha;
	/** 再生状態 */
	IDSTATE m_state;
	/** データ */
	PBYTE	m_data;
	/** データサイズ */
	DWORD	m_datalen;
	/** 再生長 */
	DWORD   m_playlen;
	/** 再生位置 */
	DWORD   m_playpos;
	/** ファイル */
	LONG m_fileid;
	/** イベントリスナー */
	SMAFPlayerListener* m_listener;
	/** SMAFPlayerはシングルトン */
	static TSingletonPtr<SMAFPlayer> s_player;
public:
	static SMAFPlayer* QueryPlayer();
	virtual ~SMAFPlayer();
	BOOL Initialize(SMAFPlayerListener*);
	void Play(int,int,BOOL);
	void Restart(int vol,int pan);
	int GetPosition() const;
	int GetLength() const;
	void Pause();
	void Stop();
	BOOL LoadSMAF(HANDLE h);
	int GetStatus() const;
	void Release();
	void SetPanpot(int pan);
	void SetVolume(int volume);
	void Destroy();
private:
	friend class TSingletonPtr<SMAFPlayer>;
	BOOL InitSMAF();
	void ExitSMAF();
	SMAFPlayer();
	DWORD CalcLength() const;
	DWORD CalcLength2(DWORD dwStart, DWORD dwEnd) const;
	/** フェーズ再生時のコールバック */
	void CallBack(struct event*	eve);
	/** フェーズ再生時のコールバック */
	static void _CallBack(struct event* eve);
	/** MA再生時のコールバック */
	void CallBack(BYTE id);
	/** MA再生時のコールバック */
	static LONG _CallBack(BYTE id);
	/** @see CObject::AssertValid */
	virtual void AssertValid() const;
};

