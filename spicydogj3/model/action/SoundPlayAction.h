#pragma once

#include "Action.h"

class SndChar;

/** サウンド再生アクション */
class SoundPlayAction : public Action {
public:
	SoundPlayAction();
	void SetSound(const SndChar*);
	const SndChar* GetSound() const;

private:
	SoundPlayAction(const SoundPlayAction&);
	virtual ~SoundPlayAction();
	virtual Action* Clone() const;
	virtual CRuntimeClass* GetDialogClass() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual UINT GetActionNameResID() const;

	TOLMReferer<SndChar> m_sound;
};
