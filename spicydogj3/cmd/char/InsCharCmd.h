#pragma once

#include "cmd/Cmd.h"
#include "sdom/olm.h"

class Char;
class CharList;

class InsCharCmd  : public Cmd {
private:
	int         m_insPos;
	CharList*   m_charList;
	COLMKeepers m_keepers;

public:
	InsCharCmd(CharList* charList,int pos,Char* insChar);

	virtual void Do();
	virtual void Undo();
};