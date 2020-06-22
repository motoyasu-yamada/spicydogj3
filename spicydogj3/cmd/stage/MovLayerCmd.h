#pragma once

#include "cmd/Cmd.h"
class Stage;
class MovLayerCmd : public Cmd {
private:
	Stage* m_stage;
	int    m_dstPos;
	int    m_srcPos;
	BOOL   m_noMove;
public:
	MovLayerCmd(Stage*,int dst,int src);
	virtual void Do();
	virtual void Undo();
};