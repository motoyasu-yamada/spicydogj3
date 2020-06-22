#pragma once

#include "cmd/Cmd.h"
#include "model/stage/KeyFrameKeepers.h"

class Layer;
class DelFramesCmd : public Cmd {
private:
	Layer* m_layer;
	KeyFrameKeepers m_keepers;
	int m_nDelPos;
	int m_nFrames;

public:
	DelFramesCmd(Layer* layer,int nDelPos,int nFrames);
	virtual void Do();
	virtual void Undo();
};
