#include "stdafx.H"
#include "Cmd.H"
#include "CmdHistory.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CmdHistory::CmdHistory() :
	m_NextCmd(m_History.end()) {
}

void CmdHistory::DoCmd(Cmd* pCmd) {
	// 実行済コマンド位置より先にあるコマンドは切り捨てる
	if (m_NextCmd != m_History.end()) {
		m_History.erase(m_NextCmd,m_History.end());
	}

	//	最大コマンド保存数以上は切り捨て
	int nMaxUndo = GetMaxUndoCmd();
	if (nMaxUndo != -1) {
		ASSERT(0 < nMaxUndo);
		int nOver = m_History.size() - nMaxUndo;
		while (0 < nOver) {
			m_History.erase(m_History.begin());
			nOver--;
		}
	}

	pCmd->Do();	
	m_History.push_back(CMDPTR(pCmd));
	m_NextCmd = m_History.end();
}

void CmdHistory::RedoCmd() {
	ASSERT(m_NextCmd != m_History.end());
	CMDPTR& pCmd = *m_NextCmd;
	m_NextCmd++;
	pCmd->Do();
}

void CmdHistory::UndoCmd() {
	ASSERT(m_NextCmd != m_History.begin());
	m_NextCmd--;
	CMDPTR& pCmd = *m_NextCmd;
	pCmd->Undo();
}

const Cmd* CmdHistory::GetCmd(int n) const {
	ASSERT(0 < GetCmdCount());
	ASSERT(0 <= n && n < GetCmdCount());
	COMMANDS::const_iterator i =m_History.begin();
	std::advance(i,n);
	ASSERT(i != m_History.end());
	return i->get();
}

int CmdHistory::GetCurCmdPos() const {
	ASSERT(0 < GetCmdCount());
	COMMANDS::iterator ib = const_cast<CmdHistory*>(this)->m_History.begin();
	return std::distance(ib,m_NextCmd) - 1;
}

void CmdHistory::Jump2Cmd(int n) {
	ASSERT(0 < GetCmdCount());
	ASSERT(-1 <= n && n < GetCmdCount());

	int nCur;
	while ((nCur = GetCurCmdPos()) != n) {
		if (nCur < n) {
			RedoCmd();
		} else {
			ASSERT(n < nCur);
			UndoCmd();
		}
	}
}

void CmdHistory::ClearCmdHistory() {
	if (m_History.size() == 0) {
		ASSERT(m_NextCmd == m_History.end());
		return;
	}
	m_NextCmd = m_History.end();
	m_History.clear();
}


BOOL CmdHistory::CanRedoCmd() const {
	return m_NextCmd != const_cast<CmdHistory*>(this)->m_History.end();
}

BOOL CmdHistory::CanUndoCmd() const {
	return m_NextCmd != const_cast<CmdHistory*>(this)->m_History.begin();
}

int CmdHistory::GetCmdCount() const {
	return m_History.size();
}

