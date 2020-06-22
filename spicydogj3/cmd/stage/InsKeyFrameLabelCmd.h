#pragma once

#include "cmd/Cmd.h"
#include "sdom/olm.h"
#include "model/stage/KeyFrame.h"

class InsKeyFrameLabelCmd : public Cmd {
private:
	KeyFrame* m_key;
	KeyFrameLabel m_new;
	KeyFrameLabel m_old;
public:
	InsKeyFrameLabelCmd(KeyFrame* key,const KeyFrameLabel* keyFrameLabel);
	virtual void Do();
	virtual void Undo();
};

