#include "stdafx.h"
#include "MemoryUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

GlobalAllocator::GlobalAllocator()
:	m_hg(NULL)
{
}

GlobalAllocator::GlobalAllocator(DWORD dwSize,UINT nFlags)
:	m_hg(NULL)
{
	Alloc(dwSize,nFlags);
}

GlobalAllocator::GlobalAllocator(const GlobalAllocator& r)
:	m_hg(NULL) {
	CopyFrom(r);
}

GlobalAllocator& GlobalAllocator::operator = (const GlobalAllocator& r) { 
	CopyFrom(r);
	return *this;
}

void GlobalAllocator::CopyFrom(const GlobalAllocator& r) {
	if (m_hg != NULL) {
		Free();
	}
	ASSERT(m_hg == NULL);
	if (r.m_hg == NULL)
		return;

	int nSize = r.GetSize();
	ASSERT(0 <= nSize);
	Alloc(nSize);

	GlobalAllocatorPtr ptrR(r.GetHandle());
	ASSERT(AfxIsValidAddress(ptrR,nSize,FALSE));
	GlobalAllocatorPtr ptrL(GetHandle());
	ASSERT(AfxIsValidAddress(ptrL,nSize,TRUE));

	CopyMemory(ptrL,ptrR,nSize);
}

void GlobalAllocator::Alloc(DWORD dwSize,UINT nFlags) {
	ASSERT(m_hg == NULL);
	m_hg = ::GlobalAlloc(nFlags,dwSize);
	if (m_hg == NULL) {
		AfxThrowMemoryException();
	}
#if defined(_DEBUG)
	ASSERT(dwSize == ::GlobalSize(m_hg));
	LPVOID pv = ::GlobalLock(m_hg);
	ASSERT(AfxIsValidAddress(pv,dwSize,TRUE));
	::GlobalUnlock(m_hg);
#endif
	m_nStyle = nFlags;
}

void GlobalAllocator::Free() {
	HGLOBAL hg = Detach();
	if (hg != NULL) {
#if defined(_DEBUG)
		LPVOID pv = ::GlobalLock(hg);
		ASSERT(AfxIsValidAddress(pv,::GlobalSize(hg),TRUE));
		::GlobalUnlock(hg);
#endif
		::GlobalFree(hg);
	} else {
		ASSERT(FALSE);
	}
}

void GlobalAllocator::Realloc(DWORD dwSize,UINT nFlags) {
	HGLOBAL hg = ::GlobalReAlloc(m_hg,dwSize,nFlags);
	if (hg == NULL) {
		AfxThrowMemoryException();
	}
	Detach();
	Attach(hg);
#if defined(_DEBUG)
	ASSERT(dwSize == ::GlobalSize(m_hg));
	LPVOID pv = ::GlobalLock(m_hg);
	ASSERT(AfxIsValidAddress(pv,dwSize,TRUE));
	::GlobalUnlock(m_hg);
#endif
}

void GlobalAllocator::Attach(HGLOBAL hg) {
	ASSERT(hg != NULL);
	m_hg = hg;
#if defined(_DEBUG)
	LPVOID pv = ::GlobalLock(m_hg);
	ASSERT(AfxIsValidAddress(pv,::GlobalSize(m_hg),TRUE));
	::GlobalUnlock(m_hg);
#endif
}

HGLOBAL GlobalAllocator::Detach()
{
#if defined(_DEBUG)
	ASSERT(m_hg != NULL);
	LPVOID pv = ::GlobalLock(m_hg);
	ASSERT(AfxIsValidAddress(pv,::GlobalSize(m_hg),TRUE));
	::GlobalUnlock(m_hg);
#endif

	HGLOBAL hg = m_hg;
	m_hg     = NULL;
	m_nStyle = 0;
	return hg;
}

UINT GlobalAllocator::GetSize() const
{
	ASSERT(m_hg != NULL);
	return ::GlobalSize(m_hg);
}

HGLOBAL GlobalAllocator::GetHandle() const {
	return m_hg;
}

GlobalAllocator::~GlobalAllocator() {
	if (m_hg != NULL) {
		Free();
	}
}


GlobalAllocatorPtr::GlobalAllocatorPtr(HGLOBAL hg)
:	m_nLocked(0),
	m_hg(NULL),
	m_pv(NULL)
{
	Lock(hg);
}

GlobalAllocatorPtr::GlobalAllocatorPtr(GlobalAllocator& a)
:	m_nLocked(0),
	m_hg(NULL),
	m_pv(NULL)
{
	Lock(a.GetHandle());
}

LPVOID GlobalAllocatorPtr::Lock(HGLOBAL hg)
{
	ASSERT(hg   != NULL);
	ASSERT(GlobalSize(hg) != 0);
	ASSERT(m_hg == NULL);
	ASSERT(m_pv == NULL);

	m_hg = hg;
	m_pv = ::GlobalLock(m_hg);
	if (m_pv == NULL) {
		AfxThrowMemoryException();
	}
	ASSERT(AfxIsValidAddress(m_pv,::GlobalSize(hg)));
	return m_pv;
}
void GlobalAllocatorPtr::Unlock()
{
	ASSERT(m_hg != NULL);
	ASSERT(m_pv != NULL);
	
	::GlobalUnlock(m_hg);
	m_pv = NULL;
	m_hg = NULL;
}

GlobalAllocatorPtr::operator LPVOID ()
{
	return m_pv;
}

GlobalAllocatorPtr::operator LPCVOID () const
{
	return m_pv;
}

//	ƒCƒ“ƒvƒŠƒƒ“ƒg
GlobalAllocatorPtr::~GlobalAllocatorPtr()
{
	if (m_pv != NULL)
	{
		Unlock();
	}
}
