#pragma once

#include "cmd/Cmd.h"

class Char;
class Layer;

class AttachLayerCharCmd : public Cmd {
private:
	Layer* m_layer;
	Char* m_new;
	Char* m_old;
public:
	AttachLayerCharCmd(Layer* layer,Char* chr);
	virtual void Do();
	virtual void Undo();
private:
	void AssertValid() const;
};

