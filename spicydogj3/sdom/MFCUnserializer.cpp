//	$Id: MFCUnserializer.cpp,v 1.4 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"

#include "MFCUnserializer.h"

#include "SDOMException.h"
#include <xercesc/dom/DOM.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace xercesc_2_2;

const WORD  NULL_TAG       = 0;           // special tag indicating NULL ptrs
const WORD  NEW_CLASS_TAG  = 0xFFFF;      // special tag indicating new CRuntimeClass
const WORD  CLASS_TAG      = 0x8000;      // 0x8000 indicates class tag (OR'd)
const DWORD BIG_CLASS_TAG  = 0x80000000;  // 0x8000000 indicates big class tag (OR'd)
const WORD  BIG_OBJECT_TAG = 0x7FFF;      // 0x7FFF indicates DWORD object tag
const DWORD MAX_MAP_COUNT  = 0x3FFFFFFE;  // 0x3FFFFFFE last valid mapCount

MFCUnserializer::MFCUnserializer(CFile& file) 
:	m_file(file) {
	m_index = 1;
}

void MFCUnserializer::ReadBuffer(BYTE* buffer,int len) {
	ASSERT(AfxIsValidAddress(buffer,len,TRUE));
	if (len != m_file.Read(buffer,len)) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

int MFCUnserializer::getReadStringLength() {
	BYTE bLen;
	ReadValue(bLen);

	if (bLen < 0xff) {
		return bLen;
	}

	WORD wLen;
	ReadValue(wLen);
	if (wLen == 0xfffe) {
		return (int)-1;
	} else if (wLen == 0xffff) {
		DWORD dwLen;
		ReadValue(dwLen);
		return (int)dwLen;
	} else {
		return wLen;
	}
}

void MFCUnserializer::ReadValue(CString& string) {
	string.Empty();

	UINT nNewLen  = getReadStringLength();
	if (nNewLen == 0) {
		return;
	}
	
	UINT read = m_file.Read(string.GetBuffer(nNewLen),nNewLen);
	string.ReleaseBuffer();
	if (read != nNewLen) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(int& n) {
	if (m_file.Read(&n,sizeof n) != sizeof n) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(SIZE& size) {
	if (m_file.Read(&size,sizeof size) != sizeof size) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(POINT& point) {
	if (m_file.Read(&point,sizeof point) != sizeof point) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(CStringList& stringlist){
	int nNewCount = ReadCount();

	CString string;
	while (nNewCount--) {
		ReadValue(string);
		stringlist.AddTail(string);
	}

}

void MFCUnserializer::ReadValue(BYTE& byte) {
	if (m_file.Read(&byte,sizeof byte) != sizeof byte) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(WORD& word) {
	if (m_file.Read(&word,sizeof word) != sizeof word) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(DWORD& dword) {
	if (m_file.Read(&dword,sizeof DWORD) != sizeof DWORD) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(short& n) {
	if (m_file.Read(&n,sizeof n) != sizeof n) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(double& d) {
	if (m_file.Read(&d,sizeof d) != sizeof d) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void MFCUnserializer::ReadValue(UINT& n) {
	if (m_file.Read(&n,sizeof UINT) != sizeof UINT) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

int MFCUnserializer::ReadCount() {
	WORD wCount;
	ReadValue(wCount);
	if (wCount != 0xFFFF) {
		return wCount;
	}

	DWORD dwCount;
	ReadValue(dwCount);
	return dwCount;
}

BOOL MFCUnserializer::ReadObject(CString& className,int& object) {
	WORD wTag;
	DWORD obTag;
	BOOL bNewObject = false;
	int index;

	ReadValue(wTag);
	if (wTag == NULL_TAG) {
		object = 0;
		goto exit;
	}
	if (wTag == BIG_OBJECT_TAG) {
		ReadValue(obTag);
	} else {
		obTag = ((wTag & CLASS_TAG) << 16) | (wTag & ~CLASS_TAG);
	}

	if (wTag == NEW_CLASS_TAG) {
		loadClass(className);
		ASSERT(!className.IsEmpty());
		registObject(className,object);
		bNewObject = true;
		goto exit;
	}
	
	index = obTag & ~BIG_CLASS_TAG; 
	getClass(index,className);
	
	if (alreadyLoadObject(index)) {
		object = index;
		bNewObject = false;
		goto exit;

	} else {
		registObject(className,object);
		bNewObject = true;
		goto exit;
	}

exit:
	if (object == 0) {
		//TRACE("* NULL\n");
	} else if (bNewObject) {
		//TRACE("* NEW %X %s\n",object,className);
	} else {
		//TRACE("* REF %X %s\n",object,className);
	}
	return bNewObject;
}

void MFCUnserializer::getClass(int index,CString& className) {
	if (m_index <= index) {
		//TRACE("getClass: file[%08X] \n",m_file.GetPosition());
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	className = m_classes[index];
}

void MFCUnserializer::registObject(CString className,int& object) {
	m_classes[m_index] = className;
	m_objects[m_index] = true;
	object = m_index;
	m_index++;
}

BOOL MFCUnserializer::alreadyLoadObject(int index) {
	if (index <= 0 || m_index <= index) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	std::map<int,BOOL>::iterator i = m_objects.find(index);
	if (i == m_objects.end()) {
		return false;
	}
	return i->second;
}

void MFCUnserializer::loadClass(CString& string) {
	WORD wSchema;
	ReadValue(wSchema);

	WORD len;
	ReadValue(len);

	int read = m_file.Read(string.GetBuffer(len),len);
	string.ReleaseBuffer();

	m_classes[m_index] = string;
	m_index++;
}

void MFCUnserializer::Skip(int sz) {
	m_file.Seek(sz,CFile::current);
}


DOMNode* MFCUnserializer::GetObject(int object) const {
	if (object == 0) {
		return NULL;
	}

	std::map<int,DOMNode*>::const_iterator i = m_nodes.find(object);
	if (i == m_nodes.end()) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	DOMNode* node = i->second;
	ASSERT(node == NULL || AfxIsValidAddress(node,sizeof DOMNode));
	return node;
}

void MFCUnserializer::RegistObject(int object,DOMNode* p) {
	ASSERT(p == NULL || AfxIsValidAddress(p,sizeof DOMNode,TRUE));
	std::map<int,DOMNode*>::iterator i = m_nodes.find(object);
	if (i != m_nodes.end()) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	//TRACE("Regist %d\n",object);
	m_nodes[object] = p;
}
