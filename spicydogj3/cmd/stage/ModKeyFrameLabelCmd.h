#pragma once
#include "resource.h"
#include "cmd/Cmd.h"
#include "model/stage/KeyFrame.h"

class ModKeyFrameLabelCmd : public Cmd {
private:
	KeyFrame* m_key;
	KeyFrameLabel m_new;
	KeyFrameLabel m_old;
public:
	ModKeyFrameLabelCmd(KeyFrame* key,const KeyFrameLabel* keyFrameLabel);
	virtual void Do();
	virtual void Undo();
};
