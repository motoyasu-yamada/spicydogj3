#include "stdafx.h"
#include "resource.h"
#include "SoundStopAction.h"

#include "model/build/Builder.h"
#include "model/stage/Label.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SoundStopAction::SoundStopAction() {
}

void SoundStopAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_SOUND_STOP);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

Action* SoundStopAction::Clone() const {
	return new SoundStopAction;
}

UINT SoundStopAction::GetActionNameResID() const {
	return IDS_AN_SOUND_STOP_ACTION;
}

