#include "stdafx.h"
#include "resource.h"
#include "SoundPlayAction.h"

#include "sdom/sdomn.h"
#include "model/stage/Label.h"
#include "model/build/Builder.h"
#include "model/char/SndChar.h"
#include "dlg/action/SoundPlayActionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


SoundPlayAction::SoundPlayAction() {
}

SoundPlayAction::~SoundPlayAction() {
}

SoundPlayAction::SoundPlayAction(const SoundPlayAction& right) {
	m_sound = right.m_sound;
}

Action* SoundPlayAction::Clone() const {
	return new SoundPlayAction(*this);
}

void SoundPlayAction::SetSound(const SndChar* sound) {
	if (sound == NULL) {
		ASSERT_VALID(sound);
	}
	ASSERT_VALID(this);
	m_sound.ReferTo(sound);
	ASSERT_VALID(this);
	ASSERT(GetSound() == sound);
}

const SndChar* SoundPlayAction::GetSound() const {
	return m_sound;
}

void SoundPlayAction::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);
	ar->SerializeReferer(WSDOMN_SOUND,&m_sound);
	ASSERT_VALID(this);
}

void SoundPlayAction::AssertValid() const {
	if (m_sound.GetObject() != NULL) {
		ASSERT_VALID(&m_sound);
	}
}

UINT SoundPlayAction::GetActionNameResID() const {
	return IDS_AN_SOUND_PLAY_ACTION;
}

CRuntimeClass* SoundPlayAction::GetDialogClass() const {
	return RUNTIME_CLASS(SoundPlayActionDlg);
}

void SoundPlayAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	const TOLMReferer<SndChar>& c = m_sound;

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (c.IsNullObject()) {
			builder->NotifyErr(IDS_EN_NO_SND_ATTACHED);
			return;
		}
		builder->UseChar(c);
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_SOUND_PLAY);
		builder->WriteCharRef(c);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

