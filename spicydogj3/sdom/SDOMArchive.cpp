//	$Id: SDOMArchive.cpp,v 1.8 2003/07/30 12:10:46 m_yamada Exp $
#include "stdafx.h"
#include "SDOMArchive.h"

#include <atlconv.h>
#include <xercesc/dom/DOM.hpp>
#include "xml/xml.h"
#include "SDOMBuffer.h"
#include "SDOMException.h"
#include "SDOMEnum.h"
#include "DOMImpl.h"

#if defined(USE_OLM)
#include "olm.h"
#endif
#if defined(USE_LEADTOOLS)
#include <ltwrappr.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace xercesc_2_2;

LPCWSTR SDOMArchive::NILL_NAME  = L"xsi:nil";
LPCWSTR SDOMArchive::NILL_VALUE = L"true";

SDOMArchive::SDOMArchive() {
	m_doc     = NULL;
	m_current = NULL;
	m_reading = NULL;
	m_written = NULL;
}

CString SDOMArchive::encodeID(int idInt) {
	CString idString;
	idString.Format("id%d",idInt);
	return idString;
}

int SDOMArchive::decodeID(LPCWSTR idString) {
	ASSERT(AfxIsValidString(idString));
	int idInt;
	if (swscanf(idString,L"id%d",&idInt) != 1) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	return idInt;
}

DOMDocument* SDOMArchive::SerializeStore(LPCWSTR name,SDOMSerializable* object) {
	ASSERT(AfxIsValidString(name));
	ASSERT(AfxIsValidAddress(object,sizeof SDOMSerializable));
	ASSERT(m_doc == NULL);

	m_storing = true;
	m_doc     = DOMImpl::getImpl()->createDocument(NULL,name,NULL);
	m_current = m_doc->getDocumentElement();
	m_reading = NULL;

	typedef std::auto_ptr<DOMDocument> DOCUMENTP;
	DOCUMENTP document;
	document = DOCUMENTP(m_doc);

	object->Serialize(this);

	return document.release();
}

void SDOMArchive::SerializeLoad(DOMDocument* document,LPCWSTR name,SDOMSerializable* object) {
	ASSERT(AfxIsValidAddress(document,sizeof DOMDocument));
	ASSERT(AfxIsValidString(name));
	ASSERT(AfxIsValidAddress(object,sizeof SDOMSerializable));
	ASSERT(m_doc == NULL);

	m_doc     = document;
	m_reading = document->getDocumentElement();
	m_storing = false;

	SerializeStatic(name,object);
}

void SDOMArchive::SerializeString(LPCWSTR name,CString& value) {
	USES_CONVERSION;
	ASSERT(AfxIsValidString(name));

	if (IsStoring()) {
		ASSERT(AfxIsValidAddress(m_current,sizeof DOMNode));
		CString slashed;
		slashed = addSlashes(value);
		DOMNodeP<DOMElement> child;
		DOMNodeP<DOMText>    text;
		child = DOMNodeP<DOMElement>(m_doc->createElement(name));
		text  = DOMNodeP<DOMText>   (m_doc->createTextNode(A2W(slashed)));
		child->appendChild(text.release());
		m_current->appendChild(child.release());

	} else {
		if (m_reading == NULL) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		ASSERT(AfxIsValidAddress(m_reading,sizeof DOMNode));
		CString nodeName = m_reading->getNodeName();
		if (nodeName != name) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		DOMText* text = (DOMText*)(m_reading->getFirstChild());
		if (text == NULL) {
			value = "";
		} else {
			ASSERT(AfxIsValidAddress(text,sizeof DOMText));
			value = text->getNodeValue();
		}
		//TRACE(" %S = %s\n",m_reading->getNodeName(),value);
		m_reading = m_reading->getNextSibling();
		value = stripSlashes(value);
	}
}

template <typename _VT> CString cast_to_string(_VT v) {
	CString value;
	value.Format("%d",v);
	return value;
}

template <> CString cast_to_string<double>(double v) {
	CString value;
	value.Format("%f",v);
	return value;
}

template <> CString cast_to_string<float>(float v) {
	CString value;
	value.Format("%f",v);
	return value;
}

template <typename _VT,int _MIN,int _MAX,_VT (*_FUNC)(LPCSTR)> 
void SerializeNumber(SDOMArchive* archive,LPCWSTR name,_VT& v){
	ASSERT(AfxIsValidAddress(archive,sizeof SDOMArchive));
	ASSERT(AfxIsValidString(name));
	ASSERT(AfxIsValidAddress(&v,sizeof _VT));

	CString value;
	if (archive->IsStoring()) {
		if (v < _MIN && _MAX < v) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		value = cast_to_string(v);
	}
	
	archive->SerializeString(name,value);
	
	if (archive->IsLoading()) {
		v = _FUNC(value);
		if (v < _MIN && _MAX < v) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
	}
}

void SDOMArchive::SerializeBYTE(LPCWSTR name,BYTE& v) {
	SerializeNumber<BYTE,0x00,0xFF,atoi>(this,name,v);
}

void SDOMArchive::SerializeWORD(LPCWSTR name,WORD& v) {
	SerializeNumber<WORD,0x0000,0xFFFF,atoi>(this,name,v);
}

void SDOMArchive::SerializeDWORD(LPCWSTR name,DWORD& v) {
	SerializeNumber<DWORD,0x00000000,0xFFFFFFFF,atoi>(this,name,v);
}

void SDOMArchive::SerializeINT(LPCWSTR name,int& v) {
	SerializeNumber<int,INT_MIN,INT_MAX,atoi>(this,name,v);
}

void SDOMArchive::SerializeUINT(LPCWSTR name,UINT& v) {
	SerializeNumber<UINT,0,UINT_MAX,atoi>(this,name,v);
}

void SDOMArchive::SerializeLONG(LPCWSTR name,long& v) {
	SerializeNumber<long,LONG_MIN,LONG_MAX,atol>(this,name,v);
}

void SDOMArchive::SerializeBOOL(LPCWSTR name,BOOL& b) {
	ASSERT(AfxIsValidAddress(&b,sizeof BOOL));
	ASSERT(AfxIsValidString(name));
	CString value;
	if (IsStoring()) {
		value = b ? ASDOMV_TRUE : ASDOMV_FALSE;
		SerializeString(name,value);
	} else {
		SerializeString(name,value);
		if (value == ASDOMV_TRUE){
			b = true;
		} else if (value == ASDOMV_FALSE){
			b = false;
		} else {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
	}
}

void SDOMArchive::SerializeCOLORREF(LPCWSTR name,COLORREF& cr) {
	ASSERT(AfxIsValidAddress(&cr,sizeof BOOL));
	ASSERT(AfxIsValidString(name));
	CString value;
	if (IsStoring()) {
		value.Format("#%02x%02x%02x",GetRValue(cr),GetGValue(cr),GetBValue(cr));
		
	}

	SerializeString(name,value);
	
	if (IsLoading()) {
		int r,g,b;
		if (sscanf(value,"#%02x%02x%02x",&r,&g,&b) != 3) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		cr = RGB(r,g,b);
	}
}

void SDOMArchive::SerializeDOUBLE(LPCWSTR name,double& v) {
	SerializeNumber<double,INT_MIN,INT_MAX,atof>(this,name,v);
}

void SDOMArchive::SerializeSIZE(LPCWSTR name,SIZE& sz) {
	ASSERT(AfxIsValidAddress(&sz,sizeof SIZE));
	EnterElement(name);
	SerializeLONG(WSDOMN_CX,sz.cx);
	SerializeLONG(WSDOMN_CY,sz.cy);
	ExitElement();
}

void SDOMArchive::SerializePOINT(LPCWSTR name,POINT& pt) {
	ASSERT(AfxIsValidAddress(&pt,sizeof POINT));
	EnterElement(name);
	SerializeLONG(WSDOMN_X,pt.x);
	SerializeLONG(WSDOMN_Y,pt.y);
	ExitElement();
}

void SDOMArchive::SerializeBuffer(LPCWSTR name,SDOMBuffer& buffer) {
	CString value;
	if (IsStoring()) {
		value = buffer.bin2hex();
		SerializeString(name,value);
	} else {
		ASSERT(IsLoading());
		SerializeString(name,value);
		buffer.hex2bin(value);
	}
}

void SDOMArchive::SerializeStatic(LPCWSTR name,SDOMSerializable* p) {
	EnterElement(name);
	p->Serialize(this);
	ExitElement();
}


void SDOMArchive::SerializeType(LPCWSTR name,int& value,const SDOMEnumType& enumType) {
	ASSERT(AfxIsValidString(name));

	CString string;
	if (IsStoring()) {
		string = enumType.Value2Name(value);
	}
	SerializeString(name,string);
	if (IsLoading()) {
		value  = enumType.Name2Value(string);
	}
}

void SDOMArchive::EnterType(int& value,const SDOMEnumType& type) {
	USES_CONVERSION;
	if (IsLoading()) {
		if (m_reading == NULL) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		CString nodeName = m_reading->getNodeName();
		//TRACE("> %s\n",nodeName);
		if (!type.Exists(nodeName)) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		m_current = m_reading;
		m_reading = m_current->getFirstChild();
		value     = type.Name2Value(nodeName);
	} else {
		ASSERT(IsStoring());
		ASSERT(m_reading == NULL);
		DOMNodeP<DOMElement> child;
		child = DOMNodeP<DOMElement>(m_doc->createElement(A2W(type.Value2Name(value))));
		m_current->appendChild(child.get());
		m_current = child.release();
	}
}

void SDOMArchive::ExitType() {
	ExitElement();
}

void SDOMArchive::EnterElement(LPCWSTR name) {
	if (IsLoading()) {
		if (m_reading == NULL) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		CString nodeName = m_reading->getNodeName();
		//TRACE("> %s\n",nodeName);
		if (nodeName != name) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		m_current = m_reading;
		m_reading = m_current->getFirstChild();
		//TRACE(">> %S\n",m_reading == NULL ? L"null" : m_reading->getNodeName());
	} else {
		ASSERT(IsStoring());
		ASSERT(m_reading == NULL);

		DOMNodeP<DOMElement> child;
		
		child = DOMNodeP<DOMElement>(m_doc->createElement(name));
		m_current->appendChild(child.get());
		m_current = child.release();
	}
}

void SDOMArchive::ExitElement() {
	if (IsLoading()) {
		//TRACE("<\n");
		if (m_reading != NULL) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		ASSERT(AfxIsValidAddress(m_current,sizeof DOMNode));
		m_reading = m_current->getNextSibling();
		ASSERT(m_reading == NULL || AfxIsValidAddress(m_reading,sizeof DOMNode));
		m_current = m_current->getParentNode();
		ASSERT(AfxIsValidAddress(m_current,sizeof DOMNode));
	} else {
		ASSERT(IsStoring());
		ASSERT(m_reading == NULL);
		m_written = m_current;
		m_current = m_current->getParentNode();
	}
}

void SDOMArchive::SerializeDynamic(
	SDOMSerializable*&	object,
	CLASS_FACTORY		factory,
	CLASS_NAMER			namer) {
	ASSERT(factory != NULL);
	ASSERT(namer   != NULL);

	LPCWSTR name;
	if (IsLoading()) {
		name = m_reading->getNodeName();
		object = factory(name);
	} else {
		name   = namer(object);
	}

	EnterElement(name);
	object->Serialize(this);
	ExitElement();
}

void SDOMArchive::SerializeReferer(LPCWSTR name,COLMReferer* referer) {
	USES_CONVERSION;
	if (IsStoring()) {
		int id = (int)referer->GetObject();
		if (id == 0) {
			ASSERT(AfxIsValidAddress(m_current,sizeof DOMNode));

			DOMNodeP<DOMElement> child;
			child = DOMNodeP<DOMElement>(m_doc->createElement(name));
			child->setAttribute(NILL_NAME,NILL_VALUE);
			m_current->appendChild(child.release());

		} else {
			SerializeString(name,encodeID(id));
		}
	} else {
		CString string;
		SerializeString(name,string);
		if (!string.IsEmpty()) {
			int id = decodeID(A2W(string));
			if (alreadyCreated(id)) {
				COLMObject* object = getObject(id);
				referer->ReferTo(object);
			} else {
				queueRequest(id,referer);
			}
		}
	}
}

void SDOMArchive::SerializeOwner(
	COLMOwner* owner,
	BOOL writeid,
	CLASS_FACTORY factory,
	CLASS_NAMER namer) {
	USES_CONVERSION;

	ASSERT(factory != NULL);
	ASSERT(namer != NULL);
	ASSERT(owner != NULL);

	if (IsStoring()) {
		COLMObject* object = owner->GetObject();
		ASSERT(object != NULL);
		SDOMSerializable* serializable = dynamic_cast<SDOMSerializable*>(object);
		ASSERT(serializable != NULL);

		SerializeDynamic(serializable,factory,namer);
		
		if (writeid) {
			ASSERT(m_written != NULL);
			ASSERT(wcscmp(m_written->getNodeName(),namer(serializable))==0);
			DOMElement* element = dynamic_cast<DOMElement*>(m_written);
			if (element == NULL) {
				SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
			}
			element->setAttribute(WSDOMN_ID,A2W(encodeID((int)object)));
		}

	} else {
		ASSERT(IsLoading());
		SDOMSerializable* object;
		COLMObject* object2;
		int id;

		if (writeid) {
			ASSERT(m_reading != NULL);
			DOMElement* element = dynamic_cast<DOMElement*>(m_reading);
			if (element == NULL) {
				SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
			}
			ASSERT(AfxIsValidAddress(element,sizeof DOMElement));
			LPCWSTR attribute = element->getAttribute(WSDOMN_ID);
			if (attribute == NULL) {
				SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
			}
			id = decodeID(attribute);
		}

		SerializeDynamic(object,factory,namer);

		ASSERT(object != NULL);
		object2 = dynamic_cast<COLMObject*>(object);
		ASSERT(object2 != NULL);
	
		if (!owner->IsNullObject()) {
			COLMKeeper disposer;
			disposer.InheritedFrom(owner);
		}
		owner->OwnNew(object2);	

		if (writeid) {
			if (id != 0) {
				registObject(id,object2);
				resolveRequests(id,object2);
			}
		}
	}
}

BOOL SDOMArchive::HasNextElement() const {
	ASSERT(IsLoading());
	return m_reading != NULL;
}

BOOL SDOMArchive::alreadyCreated(int id) const {
	ASSERT(IsLoading());
	OBJECTMAP::const_iterator i = m_objects.find(id);
	return i != m_objects.end();
}

void SDOMArchive::registObject(int id,COLMObject* object) {
	ASSERT(IsLoading());
	ASSERT(object != NULL);
	ASSERT(id != 0);
	ASSERT(!alreadyCreated(id));

	m_objects.insert(OBJECTMAP::value_type(id,object));
}

void SDOMArchive::queueRequest(int id,COLMReferer* referer) {
	ASSERT(IsLoading());
	ASSERT(referer != NULL);
	ASSERT(id != 0);
#if defined(_DEBUG)
	REFERERMAP::const_iterator i = m_referers.begin();
	while (i != m_referers.end()) {
		ASSERT(i->second != referer);
		i++;
	}
#endif
	m_referers.insert(REFERERMAP::value_type(id,referer));
	//TRACE("queing COLMReferer(%08x) waiting for instanciation(%d)\n",referer,id);
}

COLMObject* SDOMArchive::getObject(int id) const {
	OBJECTMAP::const_iterator i = m_objects.find(id);
	ASSERT(i != m_objects.end());
	return i->second;
}

void SDOMArchive::resolveRequests(int id,COLMObject* object) {
	ASSERT(IsLoading());
	ASSERT(object != NULL);
	ASSERT(id != 0);

	REFERERMAP::iterator i = m_referers.lower_bound(id);
	if (i == m_referers.end()) {
		return;
	}
	REFERERMAP::iterator e = m_referers.upper_bound(id);
	while (i != e) {
		ASSERT(i->first == id);
		COLMReferer* referer = i->second;
		//TRACE("resolving %08x=>%d(%d):%s\n",referer,id,object,typeid(*object).name());
		referer->ReferTo(object);
		REFERERMAP::iterator d = i++;
		m_referers.erase(d);
	}
}

BOOL SDOMArchive::IsStoring() const{
	return m_storing;
}

BOOL SDOMArchive::IsLoading() const{
	return !m_storing;
}


CString SDOMArchive::bin2hex(LPCVOID pointer,UINT length) {
	CString string;
	LPBYTE src = (LPBYTE)pointer;
	LPBYTE dst  =(LPBYTE)string.GetBuffer(length * 2);
	for (int n = 0;n < length;n++) {
		ASSERT(AfxIsValidAddress(src,1,FALSE));
		ASSERT(AfxIsValidAddress(dst,2,TRUE));
		int byte = *src++;
		*dst++ = bin2hex(byte >> 4);
		*dst++ = bin2hex(byte & 0xF);
	}
	string.ReleaseBuffer();
	return string;
}

UINT SDOMArchive::hexLength(LPCTSTR hex) {
	if (strlen(hex) % 2 != 0) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	return (strlen(hex) + 1) / 2;
}

int SDOMArchive::hex2bin(CHAR c) {
	if ('0' <= c && c <= '9') {
		int n = c - '0';
		ASSERT(0 <= n && n <= 9);
		return n;
	}
	if ('A' <= c && c <= 'F') {
		int n = c - 'A' + 10;
		ASSERT(10 <= n && n <= 15);
		return n;
	}
	SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
}

CHAR SDOMArchive::bin2hex(int n) {
	ASSERT(0x0 <= n && n <= 0xF);
	if (n < 10) {
		CHAR c =  '0' + n;
		ASSERT('0' <= c && c <= '9');
		return c;
	} else {
		CHAR c = 'A' + n - 10;
		ASSERT('A' <= c && c <= 'F');
		return c;
	}
}

void SDOMArchive::hex2bin(LPCTSTR hex,LPVOID pointer) {
	ASSERT(AfxIsValidString(hex));
	ASSERT(AfxIsValidAddress(pointer,hexLength(hex)));

	UINT length = hexLength(hex);
	LPBYTE buffer = (LPBYTE)pointer;
	for (int n = 0;n < length;n++) {
		int high = hex2bin(*hex++);
		int low  = hex2bin(*hex++);
		ASSERT(0 <= high && high < 16);
		ASSERT(0 <= low  && low  < 16);
		int byte = (high << 4) + low;
		ASSERT(0 <= byte && byte <= 255);
		*buffer++ = byte;
	}
}


#if defined(USE_LEADTOOLS)
const L_INT FORMAT = FILE_BMP;
const L_INT BITS   = 24;
const L_INT ORDER  = ORDER_BGR;

void SDOMArchive::SaveBitmap(const LBitmapBase& bitmap,LBuffer& buffer) {
	LMemoryFile file;

	ASSERT(const_cast<LBitmapBase&>(bitmap).GetColorOrder() == ORDER);
	file.SetBitmap(const_cast<LBitmapBase*>(&bitmap));
	if (file.SaveBitmap(&buffer,FORMAT,BITS,ORDER) != SUCCESS) {
#if defined(_DEBUG)
		LSettings::DisplayErrorList(AfxGetMainWnd()->GetSafeHwnd());
#endif	
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void SDOMArchive::LoadBitmap(const LBuffer& buffer,LBitmapBase& bitmap) {
	FILEINFO fi;
	LMemoryFile file;

	ZeroMemory(&fi,sizeof fi);
	if (file.GetInfo(const_cast<LBuffer&>(buffer),&fi) != SUCCESS) {
#if defined(_DEBUG)
		LSettings::DisplayErrorList(AfxGetMainWnd()->GetSafeHwnd());
#endif
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	if (fi.Format != FORMAT) {
		//TRACE("LoadBitmap.Format=%d\n",fi.Format);
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	file.SetBitmap(&bitmap);
	if (file.LoadBitmap(const_cast<LBuffer&>(buffer),BITS,ORDER) != SUCCESS) {
#if defined(_DEBUG)
		LSettings::DisplayErrorList(AfxGetMainWnd()->GetSafeHwnd());
#endif	
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	ASSERT(bitmap.GetColorOrder() == ORDER_BGR);
}

void SDOMArchive::SerializeBitmap(LPCWSTR name,LBitmapBase& bitmap) {
	LBuffer buffer;
	if (IsLoading()) {
		SerializeBuffer(name,buffer);
		LoadBitmap(buffer,bitmap);
	} else {
		ASSERT(IsStoring());
		SaveBitmap(bitmap,buffer);
		SerializeBuffer(name,buffer);
	}
}

void SDOMArchive::SerializeBuffer(LPCWSTR name,LBuffer& buffer) {
	if (IsLoading()) {
		ASSERT(IsLoading());
		CString string;
		SerializeString(name,string);
		
		UINT length = hexLength(string);
		if (length ==0 ) {
			buffer.Reallocate(0);
		} else {
			buffer.Reallocate(length);
			
			LPVOID pointer = buffer.Lock();
			try {
				hex2bin(string,pointer);
				buffer.Unlock();
			} catch(...) {
				buffer.Unlock();
				throw;
			}
		}
	} else {
		ASSERT(IsStoring());
		UINT length = buffer.GetSize();
		LPVOID pointer = buffer.Lock();
		try {
			CString string = bin2hex(pointer,length);
			SerializeString(name,string);
			buffer.Unlock();
		} catch(...) {
			buffer.Unlock();
			throw;
		}
	}	
}

#endif

CString SDOMArchive::stripSlashes(LPCTSTR str) {
	CString newstr;
    LPCTSTR iterate;
	LPCTSTR end;
    int length;
    length  = lstrlen(str);
	iterate = str;
	end     = str + length;

    while (iterate < end) {
		TCHAR tc;
		tc = *iterate++;
		//	ダブルバイトキャラクタ
		if (_istleadbyte(tc)) {
			newstr += tc;
			newstr += *iterate++;
			continue;
		}

		//	シングルバイトキャラクタ
		if (tc != '\\') {
			newstr += tc;
			continue;
		}

		//	スラッシュされているキャラクタ
		tc = *iterate++;
		switch(tc) {
		case 't':
			newstr += '\t';
			break;
		case 'r':
			newstr += '\r';
			break;
		case 'n':
			newstr += '\n';
			break;
		case '\\':
			newstr += '\\';
			break;
		default:
			newstr += tc;
			ASSERT(FALSE);
			break;
		}
    }
    return newstr;
}

CString SDOMArchive::addSlashes(LPCTSTR src) {
    LPCTSTR iterate;
	CString newstr;
    LPCTSTR end;
    int     length;

    if (src == NULL) {
        return "";
    }

    iterate = src;
	length  = lstrlen(src);
    end     = src + length;

    while (iterate < end) {
		TCHAR tc = *iterate;
		//	ダブルバイトキャラクタ
		if (_istleadbyte(tc)) {
			newstr += *iterate++;
			newstr += *iterate++;
			continue;
		}

		//	シングルバイトキャラクタ
		switch (tc) {
			case '\r':
				newstr += "\\r";
				break;
			case '\n':
				newstr += "\\n";
				break;
			case '\t':
				newstr += "\\t";
				break;
			case '\\':
				newstr += "\\\\";
				break;
			default:
				newstr += tc;
				break;
		}
        iterate++;
    }

	return newstr;
}
