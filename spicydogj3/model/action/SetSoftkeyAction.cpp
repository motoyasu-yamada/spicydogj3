#include "stdafx.h"
#include "resource.h"
#include "SetSoftkeyAction.h"

#include "sdom/sdomn.h"
#include "model/build/Builder.h"
#include "dlg/action/SetSoftkeyActionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SetSoftkeyAction::SetSoftkeyAction() {
	m_which = SOFTKEY_WHICH_LEFT;
}

SetSoftkeyAction::SetSoftkeyAction(const SetSoftkeyAction& right) {
	m_which = right.m_which;
	m_label = right.m_label;
}

Action* SetSoftkeyAction::Clone() const {
	return new SetSoftkeyAction(*this);
}

void SetSoftkeyAction::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);
	ar->SerializeINT(WSDOMN_WHICH,m_which);
	ar->SerializeString(WSDOMN_LABEL,m_label);
	ASSERT_VALID(this);
}

SetSoftkeyAction::~SetSoftkeyAction() {
}

UINT SetSoftkeyAction::GetActionNameResID() const {
	return IDS_AN_SET_SOFTKEY;
}

void SetSoftkeyAction::AssertValid() const {
	ASSERT(m_which == SOFTKEY_WHICH_LEFT  ||
		   m_which == SOFTKEY_WHICH_RIGHT);
	ASSERT(!m_label.IsEmpty() || strlen(m_label) <= 4);
}

CRuntimeClass* SetSoftkeyAction::GetDialogClass() const {
	return RUNTIME_CLASS(SetSoftkeyActionDlg);
}

void SetSoftkeyAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());
	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (4 < m_label.GetLength()) {
			builder->NotifyErr(IDS_EN_SOFTKEY_LABEL_OVER_LENGTH);
			return;
		}
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_SET_SOFTKEY);
		builder->WriteUI8(m_which);
		builder->WriteSJIS(m_label);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

CString SetSoftkeyAction::GetLabel() const {
	ASSERT_VALID(this);
	return m_label;
}

int SetSoftkeyAction::GetWhich() const {
	ASSERT_VALID(this);
	return m_which;
}

void SetSoftkeyAction::SetSoftkey(int which,LPCTSTR label) {
	ASSERT(AfxIsValidString(label));
	ASSERT(m_which == SOFTKEY_WHICH_LEFT  ||
		   m_which == SOFTKEY_WHICH_RIGHT);

	m_label = label;
	m_which = which;

	ASSERT_VALID(this);
}
