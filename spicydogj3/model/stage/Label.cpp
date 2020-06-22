#include "STDAFX.H"
#include "Label.H"

#include "model/build/Builder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Label::Label() {
	m_absolutePos = -1;
}

void Label::BuildLabel(Builder* builder,int relativePos) const {
	ASSERT(builder != NULL);
	ASSERT(0 <= relativePos && relativePos <= 0xFFFF);

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		m_absolutePos = builder->GetAbsoluteFramePos(relativePos);
		ASSERT(0 <= m_absolutePos);
		break;
	case Builder::BUILDSTEP_LAST:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

int Label::GetLabelAbsolutePos() const {
	ASSERT(0 <= m_absolutePos);
	return m_absolutePos;
}

	void removeAllComoboBox(CComboBox& combo) {
		while (0 < combo.GetCount()) {
			combo.DeleteString(0);
		}
	}

void Label::SetupLabelComboBox(
	CComboBox& combo,
	const LABEL_NAME_LIST& labels,
	const Label* selected) {

	combo.SetRedraw(FALSE);

	removeAllComoboBox(combo);

	int select = -1;
	LABEL_NAME_LIST::const_iterator i;

	for (i = labels.begin();i != labels.end();i++) {
		const LABEL_NAME& name = *i;
		int current= combo.AddString(name.second);
		ASSERT(name.first != NULL);
		ASSERT(current != -1);

		if (name.first == selected) {
			ASSERT(select == -1);
			select = current;
		}
	}

	if (select != -1) {
		combo.SetCurSel(select);
	}
	combo.SetRedraw(TRUE);
}
