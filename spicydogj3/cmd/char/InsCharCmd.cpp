#include "stdafx.h"
#include "resource.h"
#include "InsCharCmd.h"
#include "model/char/Char.h"
#include "model/char/CharList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

InsCharCmd::InsCharCmd(CharList* charList,int pos,Char* insChar) : 
	Cmd(IDCMD_INS_CHAR) {
	m_charList = charList;
	m_insPos   = pos;
	if (m_insPos == -1) {
		m_insPos = charList->GetCharCount();
	}
	ASSERT_VALID(insChar);
	m_keepers.OwnNew(insChar);
}

void InsCharCmd::Do() {
	ASSERT_VALID(m_charList);
	m_keepers.TransferTo(m_charList->m_charList,m_insPos);
	ASSERT_VALID(m_charList);
}

void InsCharCmd::Undo() {
	ASSERT_VALID(m_charList);
	m_keepers.InheritedFrom(m_charList->m_charList,m_insPos,1);
	ASSERT_VALID(m_charList);
}
