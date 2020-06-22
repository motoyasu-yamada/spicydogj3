#include "stdafx.h"
#include "resource.h"
#include "PlayAction.h"

#include "model/build/Builder.h"
#include "model/stage/label.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

PlayAction::PlayAction() {
}

void PlayAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_PLAY);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

Action* PlayAction::Clone() const {
	return new PlayAction;
}

UINT PlayAction::GetActionNameResID() const {
	return IDS_AN_PLAY_ACTION;
}

