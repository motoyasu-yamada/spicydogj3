#include "stdafx.h"
#include "Char.h"

#include <afxmt.h>
#include "TextChar.h"
#include "BtnChar.h"
#include "SndChar.h"
#include "PictChar.h"
#include "sdom/SDOMException.h"
#include "sdom/sdomn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CharType::CharType(
	CRuntimeClass* guiClass,
	LPCTSTR charTypeName,
	UINT    displayName,
	LPCWSTR xmlNodeName,
	int  iconIndex,
	BOOL guiModal,
	BOOL canButtonChar,
	BOOL canSoundChar,
	BOOL canAttachLayer,
	BOOL canModifyColoring,
	BOOL canModifyPlacing
) {
	ASSERT_POINTER(guiClass,CRuntimeClass);
	ASSERT(AfxIsValidString(charTypeName));
	ASSERT(AfxIsValidString(xmlNodeName));

	this->guiClass          = guiClass;
	this->charTypeName      = charTypeName;
	this->displayName       = displayName;
	this->xmlNodeName       = xmlNodeName;
	this->iconIndex         = iconIndex;
	this->guiModal          = guiModal;
	this->canButtonChar     = canButtonChar;
	this->canSoundChar      = canSoundChar;
	this->canAttachLayer    = canAttachLayer;
	this->canModifyColoring = canModifyColoring;
	this->canModifyPlacing  = canModifyPlacing;
}

Char::Char() {
}

Char::Char(const Char& c) {
	m_name = c.m_name;
}

Char* Char::Clone() const {
	ASSERT(FALSE);
	return NULL;
}

Char::~Char() {
}

void Char::Serialize(SDOMArchive* ar) {
	ar->SerializeString(WSDOMN_NAME,m_name);
}

CString Char::GetName() const {
	return m_name;
}

void Char::SetName(LPCTSTR ptsz) {
	ASSERT(AfxIsValidString(ptsz));
	m_name = ptsz;
}

SDOMSerializable* Char::factory(LPCWSTR name) {
	if (wcscmp(name,WSDOMN_TEXT_CHAR) == 0) {
		return new TextChar;
	}
	if (wcscmp(name,WSDOMN_BTN_CHAR) == 0) {
		return new BtnChar;
	}
	if (wcscmp(name,WSDOMN_SND_CHAR) == 0) {
		return new SndChar;
	}
	if (wcscmp(name,WSDOMN_PICT_CHAR) == 0) {
		return new PictChar;
	}
	SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	return NULL;
}

LPCWSTR Char::namer(SDOMSerializable* _object) {
	ASSERT_POINTER(_object,sizeof SDOMSerializable);
	Char* object = dynamic_cast<Char*>(_object);
	ASSERT_VALID(object);
	return object->GetCharType().xmlNodeName;
}

void Char::GetBezier(CBEZIER& cb) const {
	cb.SetSize(1);
	BEZIER& b = cb[0];

	CRect rc;
	GetRect(rc);
	b.SetSize(4 * 3 + 1);
	b[0] = b[1] = b[11] = b[12] = CPoint(rc.left,rc.top);
	b[2] = b[3] = b[4]  = CPoint(rc.right,rc.top);
	b[5] = b[6] = b[7]  = CPoint(rc.right,rc.bottom);
	b[8] = b[9] = b[10] = CPoint(rc.left, rc.bottom);
}

CString Char::GetInfo() const {
	return "";
}

CString Char::GetLongInfo() const {
	return "";
}
