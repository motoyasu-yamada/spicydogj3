#pragma once
#include "cmd/Cmd.h"
#include "sdom/OLM.h"

class SubStage;

/** サブステージの名称変更 */
class RenSubStageCmd : public Cmd {
private:
	SubStage* m_subStage;
	CString m_strNew;
	CString m_strOld;
public:
	RenSubStageCmd(SubStage*,LPCTSTR);
	virtual void Do();
	virtual void Undo();
};