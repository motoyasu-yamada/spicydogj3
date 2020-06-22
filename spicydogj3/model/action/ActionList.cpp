#include "stdafx.h"
#include "resource.h"
#include "ActionList.h"

#include "Action.h"
#include "ActionFactory.h"
#include "model/build/Builder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ActionList::ActionList() {
}

ActionList::~ActionList() {
}

void ActionList::CopyFrom(const ActionList* actions) {
	ASSERT_VALID(actions);
	if (GetActionCount() != 0) {
		COLMKeepers del;
		del.InheritedFrom(m_actions,0,GetActionCount());
	}
	COLMKeepers temp;
	int count = actions->GetActionCount();
	for (int n = 0;n < count;n++) {
		const Action* action = actions->GetAction(n);
		ASSERT_VALID(action);
		temp.OwnNew(action->Clone());
	}
	temp.TransferTo(m_actions,0);

	ASSERT(actions->GetActionCount() == GetActionCount());
	ASSERT_VALID(this);
}

void ActionList::AssertValid() const {
	int count = GetActionCount();
	for (int n = 0;n < count;n++) {
		const Action* action = GetAction(n);
		ASSERT_VALID(action);
	}
}

void ActionList::Build(Builder* builder) const {
	int count = GetActionCount();
	if (0xFF <= count) {
		builder->NotifyErr(IDS_EN_ACTION_LIST_OVER_SIZE);
		return;
	}

	if (builder->GetBuildStep() == Builder::BUILDSTEP_LAST) {
		builder->WriteUI8(count);
	}
	for (int n = 0;n < count;n++) {
		const Action* action = GetAction(n);
		ASSERT_VALID(action);
		builder->Build(action);
	}
}

int ActionList::GetActionCount() const {
	return m_actions.GetCount();
}

Action const* ActionList::GetAction(int n) const {
	return m_actions.GetObject(n);
}

Action * ActionList::GetAction(int n) {
	return const_cast<Action*>(m_actions.GetObject(n));
}

void ActionList::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);
	m_actions.Serialize(ar);
	ASSERT_VALID(this);
}

void ActionList::InsActCmd(int pos,Action* action) {
	ASSERT_VALID(this);
	ASSERT(0 <= pos  && pos  <= GetActionCount());
	ASSERT_VALID(action);

	COLMKeepers temp;
	temp.OwnNew(action);
	temp.TransferTo(m_actions,pos);
}

void ActionList::DelActCmd(int pos) {
	ASSERT_VALID(this);
	ASSERT(0 <= pos  && pos  < GetActionCount());
	
	COLMKeepers trash;
	trash.InheritedFrom(m_actions,pos,1);
}

void ActionList::MovActCmd(int left,int right) {
	ASSERT_VALID(this);
	ASSERT(0 <= left  && left  < GetActionCount());
	ASSERT(0 <= right && right < GetActionCount());

	m_actions.Move(left,right);
}
