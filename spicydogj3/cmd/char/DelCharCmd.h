#pragma once

#include "sdom/olm.h"
#include "cmd/Cmd.h"

class Char;
class CharList;

/** �L�����N�^���폜���� */
class DelCharCmd : public Cmd {
private:
	int m_pos;
	CharList*   m_charList;
	COLMKeepers m_keepers;

public:
	DelCharCmd(CharList* charList,int nDel);
	virtual void Do();
	virtual void Undo();
};
