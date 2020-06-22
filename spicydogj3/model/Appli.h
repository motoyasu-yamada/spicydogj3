#pragma once

#include <memory>
#include "build/Buildable.h"
#include "sdom/olm.h"
#include "model/stage/Label.h"

class AppliConf;
class CharList;
class Stage;
class MainStage;
class SubStages;
class Builder;
class SDOMArchive;

/** ƒAƒvƒŠ */
class Appli
:	public CObject,
	public SDOMSerializable,
	public Buildable {
private:
	typedef std::auto_ptr<AppliConf> APPLICONFP;
	typedef std::auto_ptr<CharList>  CHARLISTP;  
	typedef TOLMOwner<MainStage>     MAINSTAGEP;
	typedef std::auto_ptr<SubStages> SUBSTAGESP;
	Stage* m_activeStage;

public:
	APPLICONFP appliConf;
	CHARLISTP  charList;
	MAINSTAGEP mainStage;
	SUBSTAGESP subStages;

	Appli();
	virtual ~Appli();
	virtual void Build(Builder*) const;
	virtual void AssertValid() const;
	virtual void Serialize(SDOMArchive*);
	void ActivateStage(const Stage*);
	Stage* GetActiveStage();

	void GetLabelList(LABEL_NAME_LIST&) const;	
};
