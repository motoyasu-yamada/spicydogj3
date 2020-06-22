//	$Id: SDOMException.cpp,v 1.2 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"
#include "SDOMException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int SDOMException::CAUSE_CORRUPTED       = -1;
const int SDOMException::CAUSE_UNKNOWN_VERSION = -2;

int SDOMException::GetCause() const {
	return m_cause;
}

void SDOMException::throwException(int cause) {
	ASSERT(cause == CAUSE_CORRUPTED ||
		   cause == CAUSE_UNKNOWN_VERSION);
	ASSERT(cause == CAUSE_UNKNOWN_VERSION);
	s_exception.m_cause = cause;
	throw &s_exception;
}

SDOMException::SDOMException() 
:	CException(FALSE) {
}

SDOMException SDOMException::s_exception;