#include "stdafx.h"
#include "resource.h"
#include "QuitAction.h"

#include "model/build/Builder.h"
#include "model/stage/label.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

QuitAction::QuitAction() {
}

void QuitAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_QUIT);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

Action* QuitAction::Clone() const {
	return new QuitAction;
}

UINT QuitAction::GetActionNameResID() const {
	return IDS_AN_QUIT_ACTION;
}

