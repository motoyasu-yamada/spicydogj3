#include "stdafx.h"
#include "ReenterLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


ReenterLock::ReenterLock(BOOL& b)
:	m_bLockFlag(b),
	m_bLocking(FALSE) {
}

void ReenterLock::InitReenterLock(BOOL& b) {
	b = FALSE;
}

BOOL ReenterLock::Lock() {
	BOOL bCanEnter;

	ASSERT(!m_bLocking);
	if (!m_bLockFlag) {
		m_bLockFlag = TRUE;
		m_bLocking = TRUE;
		bCanEnter = TRUE;
	} else {
		m_bLocking = FALSE;
		bCanEnter = FALSE;
	}
	return bCanEnter;
}

void ReenterLock::Unlock() {
	ASSERT(m_bLocking);
	ASSERT(m_bLockFlag);
	m_bLockFlag = FALSE;
	m_bLocking = FALSE;
}

ReenterLock::~ReenterLock() {
	if (m_bLocking) {
		Unlock();
	}

	ASSERT(!m_bLocking);
}
