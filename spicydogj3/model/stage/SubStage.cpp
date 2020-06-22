#include "stdafx.h"
#include "SubStage.h"

#include "sdom/SDOMException.h"
#include "sdom/sdomn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SubStage::SubStage(){
}

CString SubStage::GetStageName() const {
	return m_name;
}

CString SubStage::GetLabelName() const {
	return GetStageName();
}

void SubStage::SetStageName(LPCTSTR ptsz) {
	ASSERT(AfxIsValidString(ptsz));
	m_name = ptsz;
}

void SubStage::Serialize(SDOMArchive* ar) {
	ar->SerializeString(WSDOMN_NAME,m_name);
	Stage::Serialize(ar);
}

SDOMSerializable* SubStage::factory(LPCWSTR name) {
	if (wcscmp(name,WSDOMN_SUB_STAGE) != 0) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	return new SubStage;
}

LPCWSTR SubStage::namer(SDOMSerializable* object) {
	ASSERT(AfxIsValidAddress(object,sizeof SDOMSerializable));
	if (object->getTypeName() != "SubStage") {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	return WSDOMN_SUB_STAGE;
}