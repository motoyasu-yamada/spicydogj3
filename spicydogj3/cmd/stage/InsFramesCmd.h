#pragma once

#include "cmd/Cmd.h"
#include "model/stage/KeyFrameOwners.h"
class Layer;
class InsFramesCmd : public Cmd
{
private:
	Layer* m_layer;
	BOOL   m_expand;
	union {
		struct {
			int m_insPos;
			int m_frames;
		} m_insparams;
		int m_expand;
	} m_params;
public:
	InsFramesCmd(Layer*,int nInsPos,int nFrames);
	virtual void Do();
	virtual void Undo();
};
