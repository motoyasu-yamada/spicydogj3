#include "stdafx.h"
#include "resource.h"
#include "SetBacklightAction.h"

#include "sdom/sdomn.h"
#include "model/build/Builder.h"
#include "dlg/action/SetBacklightActionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


SetBacklightAction::SetBacklightAction() {
	m_enabled = 1;
}

SetBacklightAction::SetBacklightAction(const SetBacklightAction& right) {
	m_enabled = right.m_enabled;
}

SetBacklightAction::~SetBacklightAction() {
}

Action* SetBacklightAction::Clone() const {
	return new SetBacklightAction(*this);
}

BOOL SetBacklightAction::GetEnabled() const {
	return m_enabled;
}

void SetBacklightAction::SetEnabled(BOOL enabled) {
	m_enabled = enabled;
}

void SetBacklightAction::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);
	ar->SerializeBOOL(WSDOMN_ENABLED,m_enabled);
	ASSERT_VALID(this);
}

void SetBacklightAction::AssertValid() const {
}

UINT SetBacklightAction::GetActionNameResID() const {
	return IDS_AN_SET_BKLIGHT_ACTION;
}

void SetBacklightAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());
	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_SET_BACKLIGHT);
		builder->WriteUI8(m_enabled ? 1 : 0);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

CRuntimeClass* SetBacklightAction::GetDialogClass() const {
	return RUNTIME_CLASS(SetBacklightActionDlg);
}