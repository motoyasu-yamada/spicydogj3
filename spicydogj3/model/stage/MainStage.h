#pragma once

#include "Stage.h"

class MainStage : public Stage {
public:
	virtual CString GetStageName() const;
	virtual CString GetLabelName() const;
};

