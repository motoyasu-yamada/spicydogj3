#include "stdafx.h"
#include "resource.h"
#include "RenCharCmd.h"
#include "model/char/Char.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

RenCharCmd::RenCharCmd(Char* c,LPCTSTR newName) :
	Cmd(IDCMD_REN_CHAR) {
	ASSERT_VALID(c);
	ASSERT(AfxIsValidString(newName));

	m_strNewName = newName; 
	m_char       = c;
	m_strOldName = c->GetName();
}

void RenCharCmd::Do() {
	m_char->SetName(m_strNewName);
}

void RenCharCmd::Undo() {
	m_char->SetName(m_strOldName);
}
