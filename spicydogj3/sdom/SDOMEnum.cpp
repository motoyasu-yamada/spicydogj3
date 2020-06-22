//	$Id: SDOMEnum.cpp,v 1.3 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"

#include "SDOMEnum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL SDOMEnumType::Exists(int value) const {
	return m_i2s.find(value) != m_i2s.end();
}

BOOL SDOMEnumType::Exists(LPCTSTR name) const {
	return m_s2i.find(name) != m_s2i.end();
}

LPCTSTR SDOMEnumType::Value2Name(int value) const {
	ASSERT(Exists(value));
	return const_cast<SDOMEnumType*>(this)->m_i2s[value];
}

int SDOMEnumType::Name2Value(LPCTSTR name) const {
	ASSERT(Exists(name));
	return const_cast<SDOMEnumType*>(this)->m_s2i[name];
}

void SDOMEnumType::Define(int value,LPCTSTR name) {
	ASSERT(!Exists(value));
	ASSERT(!Exists(name));
	m_i2s[value] = name;
	m_s2i[name]  = value;
}

SDOMEnumTypeItem::SDOMEnumTypeItem(const SDOMEnumType& type,int value,LPCTSTR name) {
	ASSERT(AfxIsValidString(name));
	const_cast<SDOMEnumType&>(type).Define(value,name);
}