#include "stdafx.h"
#include "resource.h"
#include "DelCharCmd.h"
#include "model/char/CharList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DelCharCmd::DelCharCmd(CharList* charList,int pos) :
	Cmd(IDCMD_DEL_CHAR) {
	m_pos      = pos;
	m_charList = charList;
	ASSERT_VALID(m_charList);
	ASSERT(0 <= m_pos && m_pos < m_charList->GetCharCount());
}

void DelCharCmd::Do() {
	ASSERT_VALID(m_charList);
	m_keepers.InheritedFrom(m_charList->m_charList,m_pos,1);
	ASSERT_VALID(m_charList);
}

void DelCharCmd::Undo() {
	ASSERT_VALID(m_charList);
	m_keepers.TransferTo(m_charList->m_charList,m_pos);
	ASSERT_VALID(m_charList);
}


