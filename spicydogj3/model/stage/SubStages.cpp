#include "stdafx.h"
#include "SubStages.h"

#include "SubStage.h"
#include "model/build/Builder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SubStages::SubStages() {
}

SubStages::~SubStages() {
}

int SubStages::GetSubStageCount() const {
	return m_stagesSub.GetCount();
}

SubStage* SubStages::GetSubStage(int n) {
	ASSERT(0 <= n && n < GetSubStageCount());
	SubStage* p = m_stagesSub.GetObject(n);
	ASSERT_VALID(p);
	return p;
}

const SubStage* SubStages::GetSubStage(int n) const {
	ASSERT(0 <= n && n < GetSubStageCount());
	const SubStage* p = m_stagesSub.GetObject(n);
	ASSERT_VALID(p);
	return p;
}

void SubStages::Serialize(SDOMArchive* ar) {
	m_stagesSub.Serialize(ar);
}

void SubStages::AssertValid() {
	ASSERT_VALID(&m_stagesSub);
	ASSERT(m_stagesSub.GetCount() <= 0xFF);
}

void SubStages::Build(Builder* builder) const {
	for (int n = 0;n < m_stagesSub.GetCount();n++) {
		builder->Build(m_stagesSub.GetObject(n));
	}
}

int SubStages::GetSubStageIndex(const Stage* stage) const {
	for (int n = 0;n < m_stagesSub.GetCount();n++) {
		if (stage == m_stagesSub.GetObject(n)) {
			return n;
		}
	}
	ASSERT(FALSE);
	return -1;
}

void SubStages::GetLabelList(LABEL_NAME_LIST& ls) const {
	for (int n = 0;n < m_stagesSub.GetCount();n++) {
		m_stagesSub.GetObject(n)->GetLabelList(ls);
	}
}

BOOL SubStages::Exists(const Stage* find) const {
	for (int n = 0;n < m_stagesSub.GetCount();n++) {
		if (m_stagesSub.GetObject(n) == find) {
			return true;
		}
	}
	return false;
}
