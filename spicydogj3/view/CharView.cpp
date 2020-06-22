#include "stdafx.h"
#include "CharView.H"
#include "model/char/Char.h"
#include "SpicydogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CharView::AttachChar(Char* p){
	m_pChar.ReferTo(p);
}

CharView::~CharView() {
}

BOOL CharView::GetViewCaption(CString& str) const {
	if (m_pChar.IsNullObject()) {
		return FALSE;
	}
	ASSERT_VALID(m_pChar);
	str = GetChar()->GetName();
	return TRUE;
}

const Char* CharView::GetChar() const {
	ASSERT_VALID(m_pChar);
	return m_pChar;
}

Char* CharView::GetChar() {
	ASSERT_VALID(m_pChar);
	return m_pChar; 
}

void CharView::OnActiveViewChanged(int,int) {
	ASSERT_VALID(m_pChar);
}


IMPLEMENT_DYNCREATE(CharDialog,CDialog)
CharDialog::CharDialog() {
}

void CharDialog::Init(UINT nID) {
	m_lpszTemplateName = MAKEINTRESOURCE(nID);
	m_nIDHelp = nID;
}

void CharDialog::AttachChar(SpicydogDoc* document,Char* pC) {
	m_pDocument = document;
	m_pChar = pC;

	ASSERT_VALID(m_pDocument);
	ASSERT_VALID(m_pChar);
}

const Char* CharDialog::GetChar() const {
	ASSERT_VALID(m_pChar);
	return m_pChar;
}

Char* CharDialog::GetChar() {
	ASSERT_VALID(m_pChar);
	return m_pChar;
}

SpicydogDoc* CharDialog::GetDocument() {
	ASSERT_VALID(m_pDocument);
	return m_pDocument;
}

const SpicydogDoc* CharDialog::GetDocument() const {
	ASSERT_VALID(m_pDocument);
	return m_pDocument;
}

CharDialog::~CharDialog() {
}