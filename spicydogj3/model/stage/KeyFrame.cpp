#include "stdafx.h"
#include "KeyFrame.h"

#include "sdom/sdomn.h"
#include "sdom/SDOMArchive.h"
#include "model/Appli.h"
#include "model/action/ActionList.h"
#include "model/build/Builder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

KeyFrameState::KeyFrameState() {
}

KeyFrameState& KeyFrameState::operator =(const KeyFrameState& right) {
	animating = right.animating;
	placing   = right.placing;
	coloring  = right.coloring;
	return *this;
}

void KeyFrameState::Interpolate(
	const KeyFrameState* begin,
	const KeyFrameState* end,
	int n,
	int len) {

	int easing = begin->animating.easing;

	ASSERT(AfxIsValidAddress(begin,sizeof KeyFrameState));
	ASSERT(AfxIsValidAddress(end,  sizeof KeyFrameState));
	if (begin->animating.animating) {
		placing.Interpolate(
			&begin->placing,
			&end->placing,
			n,
			len,
			begin->animating.easing);
		
		coloring.Interpolate(
			&begin->coloring,
			&end->coloring,
			n,
			len,
			begin->animating.easing);

		animating.animating = true;

	} else {
		*this = *begin;
	}
}

KeyFrameLabel::KeyFrameLabel() {
	useLabel = FALSE;

}

KeyFrameLabel& KeyFrameLabel::operator =(const KeyFrameLabel& src) {
	useLabel     = src.useLabel;
	labelName    = src.labelName;
	labelComment = src.labelComment;
	return *this;
}

KeyFrame::KeyFrame() {
	m_empty    = FALSE;
	m_framePos = 0;
}

KeyFrame::KeyFrame(int frame,BOOL empty) {
	m_empty    = empty;
	m_framePos = frame;
}

KeyFrame* KeyFrame::Clone() const {
	KeyFrame* clone    = new KeyFrame;
	clone->m_state    = m_state;
	clone->m_actions.CopyFrom(&m_actions);
	clone->m_empty    = m_empty;
	clone->m_framePos = m_framePos;

	ASSERT(!clone->m_label.useLabel);
	ASSERT_VALID(clone);
	return clone;
}

KeyFrame::~KeyFrame() {
}

void KeyFrame::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);

	ar->SerializeINT(WSDOMN_POS,         m_framePos);
	ar->SerializeBOOL(WSDOMN_EMPTY,       m_empty);
	ar->SerializeStatic(WSDOMN_PLACING,  &m_state.placing);
	ar->SerializeStatic(WSDOMN_COLORING, &m_state.coloring);
	ar->SerializeStatic(WSDOMN_ANIMATING,&m_state.animating);
	ar->SerializeBOOL(WSDOMN_USE_LABEL,    m_label.useLabel);
	ar->SerializeString(WSDOMN_LABEL_NAME,   m_label.labelName);
	ar->SerializeString(WSDOMN_LABEL_COMMENT,m_label.labelComment);
	ar->SerializeStatic(WSDOMN_ACTION_LIST,&m_actions);

	ASSERT_VALID(this);
}

void KeyFrame::AssertValid() const {
	CObject::AssertValid();
	ASSERT(0 <= m_framePos);
	ASSERT_VALID(&m_actions);
}

void KeyFrame::BuildKeyFrameLabel(Builder* builder) const {
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));
	ASSERT(0 <= m_framePos && m_framePos <= 0xFFFF);
	ASSERT(m_label.useLabel);
	Label::BuildLabel(builder,m_framePos);
}

ActionList* KeyFrame::GetActionList() {
	return &m_actions;
}

BOOL KeyFrame::IsEmpty() const {
	return m_empty;
}

int KeyFrame::GetFramePos() const {
	return m_framePos;
}

void KeyFrame::SetFramePos(int nPos) {
	ASSERT(0 <= nPos);
	m_framePos = nPos;
}

void KeyFrame::MoveFramePos(int nOffset) {
	m_framePos += nOffset;
	ASSERT(0 <= m_framePos);
}

void KeyFrame::SetState(const KeyFrameState* state) {
	m_state = *state;
	ASSERT_VALID(this);
}

void KeyFrame::GetState(KeyFrameState* state) const {
	ASSERT_VALID(this);
	*state = m_state;
}

void KeyFrame::SetLabel(const KeyFrameLabel* label) {
	m_label = * label;
	ASSERT_VALID(this);
}

void KeyFrame::GetLabel(KeyFrameLabel* label) const {
	ASSERT_VALID(this);
	*label = m_label;
}

BOOL KeyFrame::HasAction() const {
	return 0 < m_actions.GetActionCount();
}

BOOL KeyFrame::HasLabel() const {
	return m_label.useLabel;
}

CString KeyFrame::GetLabelName() const {
	return  m_label.labelName;
}

const int LABEL_NAME_MIN    = 1;
const int LABEL_NAME_MAX    = 32;
const int LABEL_COMMENT_MIN = 0;
const int LABEL_COMMENT_MAX = 256;

BOOL IsValidLabelName(LPCTSTR ptszName) {
	if (ptszName == NULL)
		return FALSE;

	int length = _tcslen(ptszName);
	if (!(LABEL_NAME_MIN <= length && length <= LABEL_NAME_MAX)) {
		return FALSE;
	}

	LPCTSTR ptszI = ptszName;
	int c;
	while((c = *ptszI) != NULL) {
		if (!__iscsym(c))
			return FALSE;

		ptszI = _tcsinc(ptszI);
	}

	return TRUE;
}

void KeyFrameLabel::DDV_Label(CDataExchange* pDX,const Appli* appli,KeyFrameLabel* label,LPCTSTR oldName) {
	CString strOld = oldName;
	CString str    = label->labelName;
	if (pDX->m_bSaveAndValidate) {
		if (!strOld.IsEmpty() && strOld == str) {
			return;
		}

		if (!IsValidLabelName(str)) {
			pDX->Fail();
		}
	}
}

