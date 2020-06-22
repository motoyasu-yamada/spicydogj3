#pragma once

#include "SimpleAction.h"

class QuitAction: public SimpleAction {
public:
	QuitAction();
private:
	virtual void Build(Builder*) const;
	virtual Action* Clone() const;
	virtual UINT GetActionNameResID() const;
};
