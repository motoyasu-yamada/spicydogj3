#include "stdafx.h"
#include "resource.h"
#include "SetBackgroundColorAction.h"

#include "sdom/sdomn.h"
#include "model/build/Builder.h"
#include "dlg/action/SetBackgroundColorActionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


SetBackgroundColorAction::SetBackgroundColorAction() {
	m_color = RGB(0x00,0x00,0x00);
}

SetBackgroundColorAction::SetBackgroundColorAction(const SetBackgroundColorAction& right) {
	m_color = right.m_color;
}

SetBackgroundColorAction::~SetBackgroundColorAction() {
}

Action* SetBackgroundColorAction::Clone() const {
	ASSERT_VALID(this);
	return new SetBackgroundColorAction(*this);
}

void SetBackgroundColorAction::SetColor(COLORREF cr) {
	ASSERT(0 <= cr && cr <= RGB(0xFF,0xFF,0xFF));
	m_color = cr;
	ASSERT_VALID(this);
}

COLORREF SetBackgroundColorAction::GetColor() const {
	ASSERT_VALID(this);
	return m_color;
}

void SetBackgroundColorAction::AssertValid() const {
	ASSERT(0 <= m_color && m_color <= RGB(0xFF,0xFF,0xFF));
}

void SetBackgroundColorAction::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);
	ar->SerializeCOLORREF(WSDOMN_COLOR,m_color);
	ASSERT_VALID(this);
}

UINT SetBackgroundColorAction::GetActionNameResID() const {
	ASSERT_VALID(this);
	return IDS_AN_SET_BKGND_COLOR_ACTION;
}

void SetBackgroundColorAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_SET_BACKGROUND_COLOR);
		builder->WriteColor(m_color);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

CRuntimeClass* SetBackgroundColorAction::GetDialogClass() const {
	ASSERT_VALID(this);
	return RUNTIME_CLASS(SetBackgroundColorActionDlg);
}

