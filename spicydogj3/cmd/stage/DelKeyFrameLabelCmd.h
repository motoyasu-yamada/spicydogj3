#pragma once

#include "cmd/Cmd.h"
#include "model/stage/KeyFrame.h"

class KeyFrame;
class DelKeyFrameLabelCmd : public Cmd {
private:
	KeyFrame*     m_key;
	KeyFrameLabel m_new;
	KeyFrameLabel m_old;
public:
	DelKeyFrameLabelCmd(KeyFrame* key);
	virtual void Do();
	virtual void Undo();
};
