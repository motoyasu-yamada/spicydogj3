#pragma once

#include "Cmd.h"
#include <memory>
#include <list>

/**	コマンド集合クラス */
class CmdSet : public Cmd {
	typedef std::auto_ptr<Cmd> CMDPTR;
	typedef std::list<CMDPTR> CMDLIST;
	CMDLIST m_commands;
	BOOL m_canAdd;

public:
	CmdSet(UINT resource);
	virtual void AddCommand(Cmd*);
	virtual void Do();
	virtual void Undo();
protected:
	int GetCount() const;
};
