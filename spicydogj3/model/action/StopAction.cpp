#include "stdafx.h"
#include "resource.h"
#include "StopAction.h"

#include "model/build/Builder.h"
#include "model/stage/label.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

StopAction::StopAction() {
}

void StopAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_STOP);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

Action* StopAction::Clone() const {
	return new StopAction;
}

UINT StopAction::GetActionNameResID() const {
	return IDS_AN_STOP_ACTION;
}

