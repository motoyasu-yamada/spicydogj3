#include "stdafx.h"
#include "resource.h"
#include "DelayJumpAction.h"

#include "dlg/action/DelayJumpActionDlg.h"
#include "model/build/Builder.h"
#include "model/stage/label.h"
#include "sdom/sdomn.h"
#include "sdom/SDOMArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DelayJumpAction::DelayJumpAction() {
	m_delay = 0;
}

DelayJumpAction::DelayJumpAction(const DelayJumpAction& right) {
	m_delay = right.m_delay;
	m_label = right.m_label;
}

DelayJumpAction::~DelayJumpAction() {
}

Action* DelayJumpAction::Clone() const {
	return new DelayJumpAction(*this);
}

void DelayJumpAction::SetLabel(UINT delay,const Label* label) {
	ASSERT(0 <= m_delay && m_delay <= 0xFFFFFFFF);
	m_delay = delay;
	m_label.ReferTo(label);
}

UINT DelayJumpAction::GetDelay() const {
	ASSERT_VALID(this);
	return m_delay;
}

const Label* DelayJumpAction::GetLabel() const {
	ASSERT_VALID(this);
	return m_label;
}

void DelayJumpAction::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);
	ar->SerializeUINT(WSDOMN_DELAY,m_delay);
	ar->SerializeReferer(WSDOMN_LABEL,&m_label);
	ASSERT_VALID(this);
}

CRuntimeClass* DelayJumpAction::GetDialogClass() const {
	ASSERT_VALID(this);
	return RUNTIME_CLASS(DelayJumpActionDlg);
}

UINT DelayJumpAction::GetActionNameResID() const {
	ASSERT_VALID(this);
	return IDS_AN_DELAY_JUMP_ACTION;
}

void DelayJumpAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (!builder->IsValidLabel(m_label)) {
			builder->NotifyErr(IDS_EN_NO_LABEL_ATACHED);
		}
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_DELAY_JUMP);
		builder->WriteUI32(m_delay);
		builder->WriteLabelRef(m_label);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void DelayJumpAction::AssertValid() const {
	ASSERT(0 <= m_delay && m_delay <= 0xFFFFFFFF);
	ASSERT_VALID(&m_label);
}
