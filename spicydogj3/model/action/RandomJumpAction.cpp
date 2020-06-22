#include "stdafx.h"
#include "resource.h"
#include "RandomJumpAction.h"

#include "sdom/sdomn.h"
#include "model/build/Builder.h"
#include "model/stage/Label.h"
#include "dlg/action/RandomJumpActionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


RandomJumpAction::RandomJumpAction()  {
	RandomJumpCase rcase;
	rcase.SetCase(1,NULL);
	AppendCase(rcase);
}

RandomJumpAction::RandomJumpAction(const RandomJumpAction& right) {
	CASES::const_iterator i = right.m_cases.begin();
	while (i != right.m_cases.end()) {
		m_cases.push_back(*i++);
	}
}

RandomJumpAction::~RandomJumpAction() {
}

Action* RandomJumpAction::Clone() const {
	return new RandomJumpAction(*this);
}

void RandomJumpAction::AssertValid() const {
	ASSERT(m_cases.size());
	CASES::const_iterator i;
	
	i = m_cases.begin();
	while (i != m_cases.end()) {
		ASSERT_VALID(&*i++);
	}
}

void RandomJumpAction::Serialize(SDOMArchive* ar) {
	ar->EnterElement(WSDOMN_CASES);

	if (ar->IsStoring()) {
		CASES::const_iterator i;
		
		i = m_cases.begin();
		while (i != m_cases.end()) {
			const RandomJumpCase& rcase = *i++;
			ar->SerializeStatic(WSDOMN_CASE,const_cast<RandomJumpCase*>(&rcase));
		}

	} else {
		m_cases.clear();
		while (ar->HasNextElement()) {
			RandomJumpCase& rcase = *m_cases.insert(m_cases.end());
			ar->SerializeStatic(WSDOMN_CASE,&rcase);
		}
	}

	ar->ExitElement();
}

UINT RandomJumpAction::GetActionNameResID() const {
	return IDS_AN_RANDOM_JUMP_ACTION;
}

void RandomJumpAction::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetActionName());

	std::list<RandomJumpCase>::const_iterator i;
	int total = GetTotalCase();
	int n = 0;
	BOOL caseValid  = true;
	BOOL labelValid = true;

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (GetCaseCount() <= 0) {
			builder->NotifyErr(IDS_EN_RANDOM_JUMP_NOCASE);
		}
		if (MAX_CASE_COUNT < GetCaseCount()) {
			builder->NotifyErr(IDS_EN_RANDOM_JUMP_OVERSIZE);
		}
		
		for (i = m_cases.begin();i != m_cases.end();i++) {
			int probability = i->GetProbability();
			const Label* label = i->GetLabel();

			if (probability < MIN_CASE_VAL ||
				MAX_CASE_VAL < probability) {
				caseValid   = false;
			}
			if (label == NULL || 
				!builder->IsValidLabel(label)) {
				labelValid  = false;
			}
		}
		if (!caseValid) {
			builder->NotifyErr(IDS_EN_RANDOM_JUMP_INVALID_PROBABILITY);
		}
		if (!labelValid) {
			builder->NotifyErr(IDS_EN_RANDOM_JUMP_INVALID_LABEL);
		}
		break;

	case Builder::BUILDSTEP_LAST:
		builder->WriteAction(ACTIONID_RANDOM_JUMP);
		builder->WriteUI8(GetCaseCount());

		for (i = m_cases.begin();i != m_cases.end();i++) {
			n += i->GetProbability();
			ASSERT(0 <= n && n <= total);
			builder->WriteUI8(n * 255 / total);
			builder->WriteLabelRef(i->GetLabel());
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}



int RandomJumpAction::GetTotalCase() const {
	int n = 0;
	std::list<RandomJumpCase>::const_iterator i;
	for (i = m_cases.begin();i != m_cases.end();i++) {
		n += i->GetProbability();
	}
	return n;
}

int RandomJumpAction::GetCaseCount() const {
	return m_cases.size();
}

void RandomJumpAction::GetCase(int n,RandomJumpCase& rcase) const {
	CASES::const_iterator i = m_cases.begin();
	std::advance(i,n);
	rcase = *i;
}

void RandomJumpAction::SetCase(int n,const RandomJumpCase& rcase) {
	ASSERT(0 <= n && n < GetCaseCount());
	ASSERT_VALID(&rcase);
	CASES::iterator i = m_cases.begin();
	std::advance(i,n);
	*i = rcase;
}

void RandomJumpAction::AppendCase(const RandomJumpCase& d) {
	ASSERT(GetCaseCount() < RandomJumpAction::MAX_CASE_COUNT);
	m_cases.push_back(d);
}

void RandomJumpAction::DeleteCase(int n) {
	ASSERT(0 <= n && n < GetCaseCount());
	std::list<RandomJumpCase>::iterator i = m_cases.begin();
	std::advance(i,n);
	m_cases.erase(i);
}

CRuntimeClass* RandomJumpAction::GetDialogClass() const {
	return RUNTIME_CLASS(RandomJumpActionDlg);
}


RandomJumpCase::RandomJumpCase() {
	m_probability = 1;
}

RandomJumpCase::RandomJumpCase(const RandomJumpCase& right) {
	CopyFrom(right);
}

RandomJumpCase& RandomJumpCase::operator = (const RandomJumpCase& right) {
	CopyFrom(right);
	return *this;
}

void RandomJumpCase::CopyFrom(const RandomJumpCase& right) {
	m_probability = right.m_probability;
	m_label.ReferTo(right.m_label);
}

void RandomJumpCase::SetCase(int  probability,const Label* label) {
	ASSERT(label == NULL || AfxIsValidAddress(label,sizeof Label));
	ASSERT(IsValidProbability(probability));

	m_probability = probability;
	m_label.ReferTo(label);

	ASSERT_VALID(this);
}

int RandomJumpCase::GetProbability() const {
	ASSERT_VALID(this);

	int probability = m_probability;

	ASSERT(IsValidProbability(probability));
	return probability;
}

const Label* RandomJumpCase::GetLabel() const {
	ASSERT_VALID(this);

	const Label* label = m_label;
	ASSERT(label == NULL || AfxIsValidAddress(label,sizeof Label));
	return label;
}

void RandomJumpCase::Serialize(SDOMArchive* ar) {
	ar->SerializeINT(WSDOMN_PROBABILITY,m_probability);
	ar->SerializeReferer(WSDOMN_JUMPTO,&m_label);
}

BOOL RandomJumpCase::IsValidProbability(int probability) {
	return 1 <= probability && probability <= 255;
}

void RandomJumpCase::AssertValid() const {
	ASSERT(IsValidProbability(m_probability));
	ASSERT_VALID(&m_label);
}
