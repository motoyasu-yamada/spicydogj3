#pragma once

#include "sdom/SDOMSerializable.h"
#include "sdom/olm.h"
#include "model/build/Buildable.h"
#include "SubStage.h"

class SubStages
:	public CObject,
	public SDOMSerializable,
	public Buildable {
	friend class InsSubStageCmd;
	friend class DelSubStageCmd;
	friend class RenSubStageCmd;

private:
	TOLMOwners<SubStage,true> m_stagesSub;

public:
	SubStages();
	virtual ~SubStages();

	virtual void AssertValid();
	virtual void Serialize(SDOMArchive*);
	virtual void Build(Builder*) const;

	int GetSubStageCount() const;
	SubStage* GetSubStage(int);
	const SubStage* GetSubStage(int) const;
	int GetSubStageIndex(const Stage*) const;
	BOOL Exists(const Stage*) const;

	void GetLabelList(LABEL_NAME_LIST& ls) const;
};
