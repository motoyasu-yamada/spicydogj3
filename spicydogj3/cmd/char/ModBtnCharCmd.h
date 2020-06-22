#pragma once

#include "cmd/Cmd.h"
#include "model/char/BtnChar.h"

class ModBtnCharCmd : public Cmd {
private:
	BtnChar*     m_char;
	BtnCharState m_stateOld;
	BtnCharState m_stateNew;
	ActionList   m_actionListNew;
	ActionList   m_actionListOld;

public:
	ModBtnCharCmd(BtnChar* pCharSrc,const BtnCharState& stateOld,const ActionList&);
	virtual void Do();
	virtual void Undo();
};