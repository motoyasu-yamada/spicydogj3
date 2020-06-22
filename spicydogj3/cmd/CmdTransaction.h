#pragma once

#include "CmdSet.h"

/**
	複数の処理をまとめてコマンド化する
	CommandBag
		全ての処理を一括、Do/Uno/Redo
	CommandTransaction
		ユーザの操作に応じて各処理を各処理毎にDo
		一括Undo
		一括Redo
*/
class CmdTransaction : public CmdSet {
private:
	BOOL m_commited;
	BOOL m_firstDo;
public:
	CmdTransaction(UINT resource);
	virtual void AddCommand(Cmd*);
	virtual void Commit();
	virtual void Do();
	virtual void Undo();
};
