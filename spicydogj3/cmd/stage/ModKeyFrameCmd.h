#pragma once

#include "cmd/Cmd.h"
#include "model/stage/KeyFrame.h"

class ModKeyFrameCmd : public Cmd {
private:
	KeyFrame*     m_key;
	KeyFrameState m_new;
	KeyFrameState m_old;
public:
	ModKeyFrameCmd(KeyFrame* key,const KeyFrameState* newState);
	virtual void Do();
	virtual void Undo();
};
