#include "stdafx.h"
#include "BtnChar.h"
#include "resource.h"
#include "sdom/sdomn.h"
#include "model/build/builder.h"
#include "view/BtnCharView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CharType BtnChar::BTN_CHAR_TYPE(
	RUNTIME_CLASS(BtnCharView),
	"BtnChar",
	IDS_CHAR_DISPLAY_NAME_BTN,
	WSDOMN_BTN_CHAR,
	4,
	true,
	false, 
	false,
	true,
	false,
	false
);

/** @todo */
BOOL JphoneScheduleEvent::IsValidCriteria(LPCTSTR criteria) {
	ASSERT(AfxIsValidString(criteria));
	return strlen(criteria) <= 255;
}

/** @todo */
BOOL JphoneTelEvent::IsValidCriteria(LPCTSTR criteria) {
	ASSERT(AfxIsValidString(criteria));
	return strlen(criteria) <= 255;
}

void KeyEvent::Serialize(SDOMArchive* ar) {
	ar->SerializeBOOL(WSDOMN_KEY_0,KEY_0);
	ar->SerializeBOOL(WSDOMN_KEY_1,KEY_1);
	ar->SerializeBOOL(WSDOMN_KEY_2,KEY_2);
	ar->SerializeBOOL(WSDOMN_KEY_3,KEY_3);
	ar->SerializeBOOL(WSDOMN_KEY_4,KEY_4);
	ar->SerializeBOOL(WSDOMN_KEY_5,KEY_5);
	ar->SerializeBOOL(WSDOMN_KEY_6,KEY_6);
	ar->SerializeBOOL(WSDOMN_KEY_7,KEY_7);
	ar->SerializeBOOL(WSDOMN_KEY_8,KEY_8);
	ar->SerializeBOOL(WSDOMN_KEY_9,KEY_9);
	ar->SerializeBOOL(WSDOMN_KEY_ASTER, KEY_ASTER);
	ar->SerializeBOOL(WSDOMN_KEY_POUND, KEY_POUND);
	ar->SerializeBOOL(WSDOMN_KEY_LEFT,  KEY_LEFT);
	ar->SerializeBOOL(WSDOMN_KEY_UP,    KEY_UP);
	ar->SerializeBOOL(WSDOMN_KEY_RIGHT, KEY_RIGHT);
	ar->SerializeBOOL(WSDOMN_KEY_DOWN,  KEY_DOWN );
	ar->SerializeBOOL(WSDOMN_KEY_SELECT,KEY_SELECT);
	ar->SerializeBOOL(WSDOMN_KEY_SOFT_1, KEY_SOFT1);
	ar->SerializeBOOL(WSDOMN_KEY_SOFT_2, KEY_SOFT2);
}

void JphoneMailEvent::Serialize(SDOMArchive* ar) {
	ar->SerializeBOOL(WSDOMN_USE_EVENT,     useEvent);
	ar->SerializeBOOL(WSDOMN_STANDARD_MAIL, standardMail);
	ar->SerializeBOOL(WSDOMN_GREETING_MAIL, greetingMail);
	ar->SerializeBOOL(WSDOMN_RELAY_MAIL,    relayMail);
	ar->SerializeBOOL(WSDOMN_AUTO_MAIL,     autoMail);
	ar->SerializeBOOL(WSDOMN_EMERGENCY_MAIL,emergencyMail);
	ar->SerializeBOOL(WSDOMN_SUPER_MAIL,    superMail);
	ar->SerializeBOOL(WSDOMN_SKY_MAIL,      skyMail);
}

void JphoneScheduleEvent::Serialize(SDOMArchive* ar) {
	ar->SerializeBOOL(WSDOMN_USE_EVENT, useEvent);
	ar->SerializeBOOL(WSDOMN_RECEIVE_ALL,receiveAll);
	ar->SerializeString(WSDOMN_CRITERIA,   criteria);
	ASSERT(IsValidCriteria(criteria));
}

void JphoneTelEvent::Serialize(SDOMArchive* ar) {
	ar->SerializeBOOL(WSDOMN_USE_EVENT,  useEvent);
	ar->SerializeBOOL(WSDOMN_RECEIVE_ALL,receiveAll);
	ar->SerializeString(WSDOMN_CRITERIA,   criteria);
	ASSERT(IsValidCriteria(criteria));
}

BtnCharState::BtnCharState() {
	jphoneMailEvent.useEvent       = FALSE;
	jphoneMailEvent.autoMail       = TRUE;
	jphoneMailEvent.relayMail      = TRUE;
	jphoneMailEvent.emergencyMail  = TRUE;
	jphoneMailEvent.greetingMail   = TRUE;
	jphoneMailEvent.skyMail        = TRUE;
	jphoneMailEvent.standardMail   = TRUE;
	jphoneMailEvent.superMail      = TRUE;
	jphoneTelEvent.useEvent        = FALSE;
	jphoneTelEvent.receiveAll      = TRUE;
	jphoneScheduleEvent.useEvent   = FALSE;
	jphoneScheduleEvent.receiveAll = TRUE;
}

BtnCharState::BtnCharState(const BtnCharState& state) {
	CopyFrom(&state);
}

BtnCharState& BtnCharState::operator=(const BtnCharState& state) {
	CopyFrom(&state);
	return *this;
}

void BtnCharState::CopyFrom(const BtnCharState* src) {
	ASSERT(AfxIsValidAddress(src,sizeof BtnCharState));
	
	normalState         = src->normalState;
	activeState         = src->activeState;
	keyEvent            = src->keyEvent;
	jphoneMailEvent     = src->jphoneMailEvent;
	jphoneScheduleEvent = src->jphoneScheduleEvent;
	jphoneTelEvent      = src->jphoneTelEvent;
}

BtnChar::BtnChar() {
}

BtnChar::BtnChar(const BtnChar& src)
:	Char(src) {	
	actions.CopyFrom(&src.actions);
	m_state = src.m_state;
}

const CharType& BtnChar::GetCharType() const {
	return BTN_CHAR_TYPE;
}

Char* BtnChar::Clone() const {
	return new BtnChar(*this);
}

void BtnChar::GetState(BtnCharState* state) const {
	ASSERT_POINTER(state,BtnCharState);
	ASSERT_VALID(this);
	*state = m_state;
}

void BtnChar::SetState(const BtnCharState* state) {
	ASSERT_POINTER(state,BtnCharState);
	m_state = *state;
	ASSERT_VALID(this);
}

void BtnChar::Serialize(SDOMArchive* ar) {
	ASSERT(AfxIsValidAddress(ar,sizeof SDOMArchive));

	Char::Serialize(ar);
	ar->SerializeReferer(WSDOMN_NORMAL_STATE_CHAR,&m_state.normalState);
	ar->SerializeReferer(WSDOMN_ACTIVE_STATE_CHAR,&m_state.activeState);
	ar->SerializeStatic (WSDOMN_KEY_EVENT,   &m_state.keyEvent);
	ar->SerializeStatic (WSDOMN_JPHONE_MAIL_EVENT,    &m_state.jphoneMailEvent);
	ar->SerializeStatic (WSDOMN_JPHONE_SCHEDULE_EVENT,&m_state.jphoneScheduleEvent);
	ar->SerializeStatic (WSDOMN_JPHONE_TEL_EVENT,     &m_state.jphoneTelEvent);
	ar->SerializeStatic (WSDOMN_ACTION_LIST,          &actions);
}

void BtnChar::AssertValid() const {
	Char::AssertValid();
	JphoneScheduleEvent::IsValidCriteria(m_state.jphoneScheduleEvent.criteria);
	JphoneTelEvent::IsValidCriteria(m_state.jphoneTelEvent.criteria);
}

void BtnChar::Build(Builder* builder) const {
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetName());
	switch(builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		builder->UseChar(m_state.normalState);
		builder->UseChar(m_state.activeState);
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteChar(this,CHARID_BUTTON);
		builder->WriteCharRef(m_state.normalState);
		builder->WriteCharRef(m_state.activeState);
		builder->WriteUI32(GetKeyEventBitmap());
		builder->WriteUI8(GetJPhoneMailEventBitmap());
		if (m_state.jphoneScheduleEvent.useEvent) {
			builder->WriteUI8(1);
			if (m_state.jphoneScheduleEvent.receiveAll) {
				builder->WriteSJIS("");
			} else {
				builder->WriteSJIS(m_state.jphoneScheduleEvent.criteria);
			}
		} else {
			builder->WriteUI8(0);
		}
		if (m_state.jphoneTelEvent.useEvent) {
			builder->WriteUI8(1);
			if (m_state.jphoneTelEvent.receiveAll) {
				builder->WriteSJIS("");
			} else {
				builder->WriteSJIS(m_state.jphoneTelEvent.criteria);
			}
		} else {
			builder->WriteUI8(0);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	builder->Build(&actions);
}

void BtnChar::SetBit(int& bits,int bit,BOOL on) {
	if (!on) {
		return;
	}
	bits += (1 << bit);
}

int BtnChar::GetKeyEventBitmap() const {
	int bits = 0;
	const KeyEvent& ke = m_state.keyEvent;
	SetBit(bits, 0,ke.KEY_0);
	SetBit(bits, 1,ke.KEY_1);
	SetBit(bits, 2,ke.KEY_2);
	SetBit(bits, 3,ke.KEY_3);
	SetBit(bits, 4,ke.KEY_4);
	SetBit(bits, 5,ke.KEY_5);
	SetBit(bits, 6,ke.KEY_6);
	SetBit(bits, 7,ke.KEY_7);
	SetBit(bits, 8,ke.KEY_8);
	SetBit(bits, 9,ke.KEY_9);
	SetBit(bits,10,ke.KEY_ASTER);
	SetBit(bits,11,ke.KEY_POUND);
	SetBit(bits,12,ke.KEY_UP);
	SetBit(bits,13,ke.KEY_LEFT);
	SetBit(bits,14,ke.KEY_RIGHT);
	SetBit(bits,15,ke.KEY_DOWN);
	SetBit(bits,16,ke.KEY_SELECT);
	SetBit(bits,17,ke.KEY_SOFT1);
	SetBit(bits,18,ke.KEY_SOFT2);
	ASSERT(0 <= bits && bits <= 0xFFFFFFFF);
	return bits;
}

int BtnChar::GetJPhoneMailEventBitmap() const {
	int bits = 0;
	const JphoneMailEvent& jme = m_state.jphoneMailEvent;
	SetBit(bits, 0,jme.standardMail);
	SetBit(bits, 1,jme.greetingMail);
	SetBit(bits, 2,jme.relayMail);
	SetBit(bits, 3,jme.autoMail);
	SetBit(bits, 4,jme.emergencyMail);
	SetBit(bits, 5,jme.superMail);
	SetBit(bits, 6,jme.skyMail);
	ASSERT(0 <= bits && bits <= 0xFF);
	return bits;
}

const int PREVIEW_RECT_DEFLATE = 1;
void BtnChar::DrawPreview(const CharPreview* preview) const {
	int cx = preview->rect.CenterPoint().x;
	CRect rcNormal = preview->rect;
	rcNormal.right = cx;
	CRect rcActive = preview->rect;
	rcActive.left  = cx;

	CDC* pDC = preview->dc;
	ASSERT_VALID(pDC);
	pDC->FillSolidRect(preview->rect,preview->bkgndColor);
	pDC->Draw3dRect(rcNormal,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_3DLIGHT));
	pDC->Draw3dRect(rcActive,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_3DLIGHT));

	if (!m_state.normalState.IsNullObject()) {
		rcNormal.DeflateRect(PREVIEW_RECT_DEFLATE,PREVIEW_RECT_DEFLATE,
			                 PREVIEW_RECT_DEFLATE,PREVIEW_RECT_DEFLATE);
		CharPreview normalPreview = *preview;
		normalPreview.rect = rcNormal;
		m_state.normalState->DrawPreview(&normalPreview);
	}

	if (!m_state.activeState.IsNullObject()) {
		rcActive.DeflateRect(PREVIEW_RECT_DEFLATE,PREVIEW_RECT_DEFLATE,
			                 PREVIEW_RECT_DEFLATE,PREVIEW_RECT_DEFLATE);
		CharPreview activePreview = *preview;
		activePreview.rect = rcActive;
		m_state.activeState->DrawPreview(&activePreview);
	}
}

void BtnChar::GetBezier(CBEZIER& bezier) const {
	if (!m_state.normalState.IsNullObject()) {
		m_state.normalState->GetBezier(bezier);
	}
}

void BtnChar::Draw(CDC* pDC,const AffineMatrix* m,const Coloring* c) const {
	if (!m_state.normalState.IsNullObject()) {
		m_state.normalState->Draw(pDC,m,c);
	}
}

void BtnChar::GetRect(CRect& rc) const {
	if (m_state.activeState.IsNullObject()) {
		rc.SetRectEmpty();
	} else {
		m_state.activeState->GetRect(rc);
	}
}

