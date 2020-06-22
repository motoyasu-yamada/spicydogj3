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
	/** SMAF�t�@�C���`�� */
	enum SMAF_TYPE { SMAF_TYPE_PHASE,SMAF_TYPE_MA,SMAF_TYPE_ERROR };
	int     m_smaftype;
	/** YAMAHA���C�u���� */
	CMASMW* m_yamaha;
	/** �Đ���� */
	IDSTATE m_state;
	/** �f�[�^ */
	PBYTE	m_data;
	/** �f�[�^�T�C�Y */
	DWORD	m_datalen;
	/** �Đ��� */
	DWORD   m_playlen;
	/** �Đ��ʒu */
	DWORD   m_playpos;
	/** �t�@�C�� */
	LONG m_fileid;
	/** �C�x���g���X�i�[ */
	SMAFPlayerListener* m_listener;
	/** SMAFPlayer�̓V���O���g�� */
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
	/** �t�F�[�Y�Đ����̃R�[���o�b�N */
	void CallBack(struct event*	eve);
	/** �t�F�[�Y�Đ����̃R�[���o�b�N */
	static void _CallBack(struct event* eve);
	/** MA�Đ����̃R�[���o�b�N */
	void CallBack(BYTE id);
	/** MA�Đ����̃R�[���o�b�N */
	static LONG _CallBack(BYTE id);
	/** @see CObject::AssertValid */
	virtual void AssertValid() const;
};

