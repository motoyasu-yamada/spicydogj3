#pragma once

#include "sdom/olm.h"
#include "cmd/Cmd.h"

class Char;
class CharList;

/** ƒLƒƒƒ‰ƒNƒ^‚ğ•¡»‚·‚é */
class DupCharCmd : public Cmd {
private:
	int m_ins;
	CharList*   m_charList;
	COLMKeepers m_keepers;

public:
	DupCharCmd(CharList* charList,int ins,Char* dupChar);
	virtual void Do();
	virtual void Undo();
};