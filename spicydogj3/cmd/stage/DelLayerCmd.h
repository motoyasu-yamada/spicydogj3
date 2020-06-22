#pragma once

#include "cmd/Cmd.h"
#include "sdom/olm.h"
class Layer;
class Stage;

class DelLayerCmd : public Cmd {
private:
	Stage*      m_stage;
	COLMKeepers m_keepers;
	int m_remove;
public:
	DelLayerCmd(Stage* stage,int remove);
	virtual void Do();
	virtual void Undo();
};