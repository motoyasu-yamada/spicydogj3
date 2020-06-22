#pragma once

#include "cmd/Cmd.h"
#include "model/char/TextChar.h"

class ModTextCharCmd : public Cmd {
private:
	TextCharState m_stateOld;
	TextCharState m_stateNew;
	TextChar* m_pText;
public:
	ModTextCharCmd(TextChar* pText,const TextCharState& stateNew);
	virtual void Do();
	virtual void Undo();
};