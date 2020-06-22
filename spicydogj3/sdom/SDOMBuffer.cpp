//	$Id: SDOMBuffer.cpp,v 1.5 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"
#include "SDOMBuffer.h"

#include "SDOMException.h"
#include "SDOMArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SDOMBuffer::SDOMBuffer()
:	m_hg(NULL)
{
}

SDOMBuffer::SDOMBuffer(DWORD dwSize)
:	m_hg(NULL)
{
	GlobalAllocator::Alloc(dwSize);
}

SDOMBuffer::SDOMBuffer(const SDOMBuffer& r)
:	m_hg(NULL) {
	CopyFrom(r);
}

SDOMBuffer& SDOMBuffer::operator = (const SDOMBuffer& r) { 
	CopyFrom(r);
	return *this;
}

void SDOMBuffer::CopyFrom(const SDOMBuffer& r) {
	GlobalAllocator::CopyFrom(r);
}

SDOMBuffer::~SDOMBuffer() {
}

CString SDOMBuffer::bin2hex() const {
	GlobalAllocatorPtr p(GetHandle());
	CString hex = SDOMArchive::bin2hex(p,GetSize());
	return hex;
}

void SDOMBuffer::hex2bin(LPCTSTR hex) {
	UINT length = SDOMArchive::hexLength(hex);
	GlobalAllocator::Alloc(length);
	
	GlobalAllocatorPtr p(GetHandle());
	SDOMArchive::hex2bin(hex,p);
}
