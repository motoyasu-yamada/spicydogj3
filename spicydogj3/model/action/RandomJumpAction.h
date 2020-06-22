#pragma once

#include "Action.h"
#include "sdom/olm.h"

#include <list>

class Label;

class RandomJumpCase :
	public CObject,
	public SDOMSerializable {
public:
	RandomJumpCase();
	RandomJumpCase(const RandomJumpCase&);
	RandomJumpCase& operator = (const RandomJumpCase&);
	void SetCase(int  probability,const Label*);
	int GetProbability() const;
	const Label* GetLabel() const;

private:
	void CopyFrom(const RandomJumpCase&);
	virtual void Serialize(SDOMArchive*);	
	virtual void AssertValid() const;
	static BOOL IsValidProbability(int probability);

	int m_probability;
	TOLMReferer<Label> m_label;
};

class RandomJumpAction :
	public Action {
public:
	enum {MAX_CASE_COUNT = 0xFF,MIN_CASE_VAL = 0x01,MAX_CASE_VAL = 0xFF};

	RandomJumpAction();
	int GetTotalCase() const;
	int GetCaseCount() const;
	void GetCase(int pos,RandomJumpCase& get) const;
	void SetCase(int pos,const RandomJumpCase& set);
	void AppendCase(const RandomJumpCase& insert);
	void DeleteCase(int pos);

protected:
	RandomJumpAction(const RandomJumpAction&);
	virtual ~RandomJumpAction();
	virtual Action* Clone() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual UINT GetActionNameResID() const;
	virtual CRuntimeClass* GetDialogClass() const;
	typedef std::list<RandomJumpCase> CASES;
	CASES m_cases;
};
