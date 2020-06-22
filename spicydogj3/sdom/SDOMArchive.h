#pragma once

#include "SDOMSerializable.h"
#include <map>

#define USE_LEADTOOLS
#define USE_OLM

namespace xercesc_2_2 {
class DOMDocument;
class DOMNode;
};
using namespace xercesc_2_2;

#if defined(USE_OLM)
class COLMObject;
class COLMOwner;
class COLMReferer;
#endif
#if defined(USE_LEADTOOLS)
class LBitmapBase;
class LBuffer;
#endif
class SDOMSerializable;
class SDOMBuffer;
class SDOMEnumType;

/** SDOMとネイティブオブジェクト間でシリアライズを行う */
class SDOMArchive {
private:
	DOMNode* m_current;
	DOMNode* m_reading;
	DOMNode* m_written;
	DOMDocument* m_doc;
	BOOL m_storing;
	typedef std::map<int,COLMObject*> OBJECTMAP;
	typedef std::multimap<int,COLMReferer*> REFERERMAP;
	OBJECTMAP  m_objects;
	REFERERMAP m_referers;

public:
	typedef SDOMSerializable* (*CLASS_FACTORY)(LPCWSTR name);
	typedef LPCWSTR (*CLASS_NAMER)(SDOMSerializable*);

	SDOMArchive();
	DOMDocument* SerializeStore(LPCWSTR name,SDOMSerializable*);
	void SerializeLoad(DOMDocument*,LPCWSTR,SDOMSerializable*);

	BOOL IsStoring() const;
	BOOL IsLoading() const;
	void SerializeString(LPCWSTR name,CString& value);
	void SerializeBYTE(LPCWSTR name,BYTE&);
	void SerializeWORD(LPCWSTR name,WORD&);
	void SerializeDWORD(LPCWSTR name,DWORD&);
	void SerializeBOOL(LPCWSTR name,BOOL&);
	void SerializeDOUBLE(LPCWSTR name,double&);
	void SerializeINT(LPCWSTR name,int&);
	void SerializeUINT(LPCWSTR name,UINT&);
	void SerializeLONG(LPCWSTR name,long&);
	void SerializeSIZE(LPCWSTR name,SIZE& sz);
	void SerializePOINT(LPCWSTR name,POINT& pt);
	void SerializeCOLORREF(LPCWSTR name,COLORREF& cr);
	void SerializeBuffer(LPCWSTR name,SDOMBuffer& buffer);
	void SerializeStatic (LPCWSTR name,SDOMSerializable*);
	void SerializeDynamic(SDOMSerializable*&,CLASS_FACTORY,CLASS_NAMER);
	void SerializeOwner(COLMOwner*,BOOL writeid,CLASS_FACTORY factory,CLASS_NAMER);
	void SerializeReferer(LPCWSTR name,COLMReferer*);
	void SerializeType(LPCWSTR,int&,const SDOMEnumType&);
#if defined(USE_LEADTOOLS)
	void SerializeBitmap(LPCWSTR name,LBitmapBase& bitmap);
	void SerializeBuffer(LPCWSTR name,LBuffer& buffer);
	static void SaveBitmap(const LBitmapBase& bitmap,LBuffer& buffer);
	static void LoadBitmap(const LBuffer& buffer,LBitmapBase& bitmap);
#endif
	BOOL HasNextElement() const;
	void EnterType(int&,const SDOMEnumType&);
	void ExitType();
	void EnterElement(LPCWSTR name);
	void ExitElement();

	static CString encodeID(int idInt);
	static int decodeID(LPCWSTR id);
	static LPCWSTR NILL_NAME ;
	static LPCWSTR NILL_VALUE;
	static int hex2bin(CHAR c);
	static CHAR bin2hex(int n);
	static void hex2bin(LPCTSTR hex,LPVOID pointer);
	static CString bin2hex(LPCVOID pointer,UINT length);
	static UINT hexLength(LPCTSTR hex);
	static CString addSlashes(LPCTSTR);
	static CString stripSlashes(LPCTSTR);
private:
	BOOL alreadyCreated(int id) const;
	COLMObject* getObject(int id) const;
	void queueRequest(int id,COLMReferer*);
	void registObject(int id,COLMObject*);
	void resolveRequests(int id,COLMObject*);
};
