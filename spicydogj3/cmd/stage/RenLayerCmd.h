#pragma once

#include "cmd/Cmd.h"
class Layer;

class RenLayerCmd : public Cmd {
private:
	Layer*  m_layer;
	CString m_new;
	CString m_old;

public:
	RenLayerCmd(Layer* layer,LPCTSTR newName);
	virtual void Do();
	virtual void Undo();
}; 