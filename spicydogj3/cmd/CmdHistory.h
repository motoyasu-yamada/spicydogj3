#pragma once
class Cmd;
#include <memory>
#include <list>

/** óöóä«óù */
class CmdHistory {
private:
	typedef std::auto_ptr<Cmd> CMDPTR;
	typedef std::list<CMDPTR> COMMANDS;

private:
	COMMANDS m_History;
	COMMANDS::iterator m_NextCmd;

public:
	CmdHistory();
	virtual void DoCmd(Cmd* pCommand);
	virtual void UndoCmd();
	virtual void RedoCmd();
	virtual void ClearCmdHistory();
	int GetCmdCount() const;

	const Cmd* GetCmd(int) const;
	int GetCurCmdPos() const;
	void Jump2Cmd(int);
protected:
	BOOL CanRedoCmd() const;
	BOOL CanUndoCmd() const;
	virtual int GetMaxUndoCmd() const { return -1; }
};
