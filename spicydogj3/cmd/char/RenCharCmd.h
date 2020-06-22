#pragma once

#include "cmd/Cmd.h"


class Char;

/**	キャラクタ名称を変更する */
class RenCharCmd : public Cmd {
private:
	Char*  m_char;
	CString m_strNewName;
	CString m_strOldName;

public:
	RenCharCmd(Char* chr,LPCTSTR newName);
	virtual void Do();
	virtual void Undo();
};