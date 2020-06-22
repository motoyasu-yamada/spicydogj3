#include "stdafx.h"
#include "resource.h"
#include "JumpAction.h"

#include "dlg/action/JumpActionDlg.h"
#include "model/stage/Label.h"
#include "model/build/Builder.h"
#include "sdom/sdomn.h"
#include "sdom/SDOMArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

JumpAction::JumpAction() {
}

JumpAction::JumpAction(const JumpAction& right) {
	m_label = right.m_label;
}

JumpAction::~JumpAction() {
}

Action* JumpAction::Clone() const {
	return new JumpAction(*this);
}

void JumpAction::AssertValid() const {
	ASSERT_VALID(&m_label);
}

void JumpAction::Serialize(SDOMArchive* ar) {
	ar->SerializeReferer(WSDOMN_JUMPTO,&m_label);
}

UINT JumpAction::GetActionNameResID() const {
	return IDS_AN_JUMP_ACTION;
}

const Label* JumpAction::GetLabel() const {
	ASSERT_VALID(this);
	return m_label;
}

void JumpAction::SetLabel(const Label* label) {
	m_label.ReferTo(label);
	ASSERT_VALID(this);
}

void JumpAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	const TOLMReferer<Label>& label = m_label;
	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (!builder->IsValidLabel(label)) {
			builder->NotifyErr(IDS_EN_NO_LABEL_ATACHED);
		}
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_JUMP);
		builder->WriteLabelRef(label);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

CRuntimeClass* JumpAction::GetDialogClass() const {
	return RUNTIME_CLASS(JumpActionDlg);
}
