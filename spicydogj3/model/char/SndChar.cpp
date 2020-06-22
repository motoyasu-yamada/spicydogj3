#include "stdafx.h"
#include "SndChar.h"
#include "resource.h"

#include "sdom/sdomn.h"
#include "view/SndCharView.h"
#include "model/build/Builder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SndChar::SndChar() {
}

CharType SndChar::SND_CHAR_TYPE(
	RUNTIME_CLASS(SndCharView),
	"SndChar",
	IDS_CHAR_DISPLAY_NAME_SOUND,
	WSDOMN_SND_CHAR,
	5,
	true,
	false,
	true,
	false,
	false,
	false);

SndChar::SndChar(const SndChar& from)
: Char(from) {
	m_data = from.m_data;
}

SndChar::SndChar(HANDLE h) {
	m_data.Attach(h);
}

const CharType& SndChar::GetCharType() const {
	return SND_CHAR_TYPE;
}

Char* SndChar::Clone() const {
	return new SndChar(*this);
}


void SndChar::Build(Builder* builder) const {
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));
	UINT length = m_data.GetSize();;
	GlobalAllocatorPtr p(m_data.GetHandle());
	const BYTE* src = (LPBYTE)(LPCVOID)p;
	switch(builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteChar(this,CHARID_SOUND);
		ASSERT(AfxIsValidAddress(p,length));
		TRACE("%c%c%c%c\n",src[0],src[1],src[2],src[3]);
		builder->WriteBuffer(p,length);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

SndChar::~SndChar() {
}

void SndChar::Serialize(SDOMArchive* ar) {
	Char::Serialize(ar);
	ar->SerializeBuffer(WSDOMN_DATA,m_data);
}

void SndChar::AssertValid() const {
	Char::AssertValid();
	ASSERT(0 < m_data.GetSize());
}

HANDLE SndChar::GetHandle() const {
	return m_data.GetHandle();
}

HANDLE SndChar::ReplaceHandle(HANDLE h) {
	HANDLE    hOld;

	hOld = m_data.Detach();
	m_data.Attach(h);

	return hOld;
}

void SndChar::DrawPreview(const CharPreview*) const {
}

void SndChar::Draw(CDC*,const AffineMatrix*,const  Coloring*) const {
	ASSERT(FALSE);
}

void SndChar::GetRect(class CRect&) const {
	ASSERT(FALSE);
}