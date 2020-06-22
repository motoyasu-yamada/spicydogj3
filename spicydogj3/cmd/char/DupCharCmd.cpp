#include "stdafx.h"
#include "resource.h"
#include "DupCharCmd.h"
#include "model/char/Char.h"
#include "model/char/CharList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DupCharCmd::DupCharCmd(CharList* charList,int ins,Char* dupChar) :
	Cmd(IDCMD_DUP_CHAR) {
	m_charList = charList;
	m_ins = ins;
	ASSERT_VALID(m_charList);
	ASSERT_VALID(dupChar);
	m_keepers.OwnNew(dupChar);
}

void DupCharCmd::Do() {
	ASSERT_VALID(m_charList);
	m_keepers.TransferTo(m_charList->m_charList,m_ins);
	ASSERT_VALID(m_charList);
}

void DupCharCmd::Undo() {
	ASSERT_VALID(m_charList);
	m_keepers.InheritedFrom(m_charList->m_charList,m_ins,1);
	ASSERT_VALID(m_charList);
}

