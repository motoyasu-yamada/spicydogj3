#pragma once

#include "cmd/Cmd.h"
#include "sdom/olm.h"

class Stage;
class Layer;

class InsLayerCmd : public Cmd {
private:
	Stage*      m_stage;
	COLMKeepers m_keepers;
	int         m_pos;
public:
	InsLayerCmd(Stage*,int pos,Layer* insert);
	virtual void Do();
	virtual void Undo();
};