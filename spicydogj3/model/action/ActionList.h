#pragma once

#include "model/build/Buildable.h"
#include "sdom/SDOMSerializable.h"
#include "sdom/olm.h"
#include "Action.h"

class Builder;
class SDOMArchive;

/** ƒAƒNƒVƒ‡ƒ“ˆê—— */
class ActionList
:	public CObject,
	public Buildable,
	public SDOMSerializable {
public:
	ActionList();
	virtual ~ActionList();
	void CopyFrom(const ActionList*);
	void InsActCmd(int nPos,Action*);
	void DelActCmd(int nDel);
	void MovActCmd(int nL,int nR);
	int GetActionCount() const;
	const Action* GetAction(int pos) const;
	Action* GetAction(int pos);

protected:
	virtual void Serialize(SDOMArchive*);
	virtual void Build(Builder*) const;
	virtual void AssertValid() const;

private:
	typedef TOLMOwners<Action,false> ACTIONS;
	ACTIONS m_actions;
};

