#include "STDAFX.H"
#include "Cmd.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Cmd::Cmd(UINT resource) {
	VERIFY(m_name.LoadString(resource));
	m_iconIndex = resource;
}

CString Cmd::GetName() const { 
	ASSERT(!m_name.IsEmpty());
	return m_name; 
}

int Cmd::GetIconIndex() const {
	return m_iconIndex; 
}

Cmd::~Cmd() {
}

void Cmd::SetCmdName(UINT nID,...) {
	CString strFormat;
	if (nID != (UINT)-1) {
		strFormat.LoadString(nID);
	} else {
		strFormat = m_name;
	}
	va_list arg;
	va_start(arg,nID);
	m_name.FormatV(strFormat,arg);
	va_end(arg);
}
