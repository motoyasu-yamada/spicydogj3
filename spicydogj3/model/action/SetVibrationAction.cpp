#include "stdafx.h"
#include "resource.h"
#include "SetVibrationAction.h"

#include "sdom/sdomn.h"
#include "model/build/Builder.h"
#include "dlg/action/SetVibrationActionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SetVibrationAction::SetVibrationAction() {
}

SetVibrationAction::SetVibrationAction(const SetVibrationAction& right) {
	m_enabled = right.m_enabled;
}

SetVibrationAction::~SetVibrationAction() {
}

Action* SetVibrationAction::Clone() const {
	return new SetVibrationAction(*this);
}

void SetVibrationAction::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);
	ar->SerializeBOOL(WSDOMN_ENABLED,m_enabled);
	ASSERT_VALID(this);
}

void SetVibrationAction::AssertValid() const {
}

UINT SetVibrationAction::GetActionNameResID() const {
	return IDS_AN_SET_VIBRATION_ACTION;
}

void SetVibrationAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());
	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_SET_VIBRATION);
		builder->WriteUI8(m_enabled ? 1 : 0);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

CRuntimeClass* SetVibrationAction::GetDialogClass() const {
	ASSERT_VALID(this);
	return RUNTIME_CLASS(SetVibrationActionDlg);
}

void SetVibrationAction::SetVibration(BOOL enabled) {
	m_enabled = enabled;
	ASSERT_VALID(this);
}

BOOL SetVibrationAction::GetVibration() const {
	ASSERT_VALID(this);
	return m_enabled;
}


