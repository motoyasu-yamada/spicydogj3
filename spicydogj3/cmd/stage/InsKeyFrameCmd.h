#pragma once

#include "cmd/Cmd.h"
#include "model/stage/KeyFrameKeepers.h"
class Layer;
class KeyFrame;
class InsKeyFrameCmd : public Cmd {
	enum MODE { MODE_EXPAND,MODE_INS,MODE_OVERINS };
private:
	Layer*          m_layer;
	KeyFrameKeepers m_keepers;
	int  m_pos;
	int  m_expand;
	MODE m_mode;


public:
	InsKeyFrameCmd(Layer* layer,int,KeyFrame*);
	virtual void Do();
	virtual void Undo();
};
