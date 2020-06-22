#include "STDAFX.H"

#include "buildable.h"
#include "builder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


Buildable::Buildable() {
	m_length   = -1;
	m_useCount = 0;
}

Buildable::~Buildable() {
}

void Buildable::PreBuild(Builder* builder) const {
	switch (builder->GetBuildStep()){
	case Builder::BUILDSTEP_FIRST:
		m_length   = 0;
		m_useCount = 0;
		break;
	case Builder::BUILDSTEP_LAST:
		m_startPos = builder->GetWritingPos();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void Buildable::PostBuild(Builder* builder) const {
	ASSERT(m_useCount == 0);
	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		break;
	case Builder::BUILDSTEP_LAST:
		m_length = builder->GetWritingPos() - m_startPos;
		m_useCount = builder->GetCharUseCount(this);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

int Buildable::GetBuiltSize() const {
	ASSERT(-1 <= m_length);
	ASSERT( 0 <=  m_useCount);
	return m_length;
}

int Buildable::GetUseCount() const {
	ASSERT(-1 <= m_length);
	ASSERT( 0 <=  m_useCount);
	return m_useCount;
}

