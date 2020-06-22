#include "stdafx.h"
#include "Player.h"
#include "SpicydogDoc.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "model/stage/Stage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Player::Player(SpicydogDoc* pDocument)
:	m_document(pDocument),
	m_frame(0),
	m_playing(FALSE) {
	ASSERT_VALID(m_document);
}

void Player::FirstFrame() {
	m_frame = 0;
}

void Player::NextFrame() {
	m_frame ++;
	if (GetLastFrame() < m_frame) {
		m_frame -= (GetLastFrame()+1);
		ASSERT_VALID(m_document);
		if (! m_document->appli->appliConf->
			loopPlay) {
			Stop();
		}
	}
}

void Player::PrevFrame() {
	m_frame --;
	if (m_frame < 0) {
		m_frame += (GetLastFrame()+1);
	}
}

void Player::LastFrame() {
	m_frame = GetLastFrame();
}

void Player::SetPlayFrame(int n) {
	m_frame = n;
}

int Player::GetPlayFrame() const {
	return m_frame;
}

int Player::GetLastFrame() const {
	int nCount = m_document->appli->GetActiveStage()->GetFrameCount();
	if (nCount == 0) {
		return 0;
	}
	return nCount - 1;
}

BOOL Player::IsPlaying() const {
	return m_playing;
}

void Player::Play() {
	ASSERT(!m_playing);
	
	Stage* stage = m_document->appli->GetActiveStage();
	stage->UnselectFrame();

	m_playing = TRUE;
}

void Player::Stop() {
	ASSERT(m_playing);
	m_playing = FALSE;
}

void Player::PrepareEdit() {
	if (IsPlaying()) {
		Stop();
	}
}
