#pragma once

#include "Stage.h"

class SubStage : public Stage {
private:
	friend class RenSubStageCmd;
	CString m_name;
public:
	SubStage();
	virtual CString GetStageName() const;
	virtual CString GetLabelName() const;
	void SetStageName(LPCTSTR);
	static SDOMSerializable* factory(LPCWSTR name);
	static LPCWSTR namer(SDOMSerializable*);
private:
	virtual void Serialize(SDOMArchive*);
};


