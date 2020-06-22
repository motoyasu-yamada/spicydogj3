#pragma once
#include "cmd/Cmd.h"

class SndChar;

class ModSndCharCmd : public Cmd {
private:
	SndChar* m_shape;
	BOOL     m_todo;
	HANDLE   m_old;
	HANDLE   m_new;

public:
	ModSndCharCmd(SndChar*,HANDLE hRep);
	virtual ~ModSndCharCmd();
	virtual void Do();
	virtual void Undo();
};