#include "stdafx.h"
#include "resource.h"
#include "BuildOutput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BuildOutput::BuildOutput()
:	m_State(STATE_NONE) {
}

BuildOutput::BuildOutput(const BuildOutput& r) {
	CopyFrom(r);
}

BuildOutput& BuildOutput::operator =(const BuildOutput& r) {
	CopyFrom(r);
	return *this;
}

int BuildOutput::GetItemCount() const {
	return m_Items.GetCount();
}

const BUILD_OUTPUT_ITEM& BuildOutput::GetItem(int n) const {
	ASSERT(0 <= n);
	ASSERT(n < m_Items.GetCount());

	POSITION pos =m_Items.FindIndex(n);
	ASSERT(pos != NULL);
	BUILD_OUTPUT_ITEM_LIST& list = const_cast<BUILD_OUTPUT_ITEM_LIST&>(m_Items);
	BUILD_OUTPUT_ITEM& boi = list.GetAt(pos);
	return boi;
}

BuildOutput::STATE BuildOutput::GetResult() const {
	return m_State;
}

void BuildOutput::BeginBuildOutput() {
	m_nErr  = 0;
	m_nWarn = 0;
	m_State = STATE_ERROR;
	while (0 < m_Items.GetCount()) {
		m_Items.RemoveHead();
	}
}

void BuildOutput::EndBuildOutput(int length,int maxlength) {
	m_nLength    = length;
	m_nMaxLength = maxlength;
	if (m_nMaxLength != -1 &&
		m_nLength    != -1 &&
		m_nMaxLength < m_nLength) {
		CString message;
		message.Format(IDS_ERR_OVERSIZED_BUILT_JAR,maxlength);
		CString location;
		location.LoadString(IDS_OVERSIZED_BUILT_JAR_);
		AppendItem(BUILD_OUTPUT_ITEM::TYPE_ERROR,location,message);
	}

	if (m_nErr != 0) {
		m_State = STATE_ERROR;
	} else if (m_nWarn != 0) {
		m_State = STATE_WARNING;
	} else {
		m_State = STATE_NORMAL;
	}
}

int BuildOutput::GetJarSize() const {
	return m_nLength;
}

int BuildOutput::GetMaxJarSize() const {
	return m_nMaxLength;
}

int BuildOutput::GetErrCount() const {
	return m_nErr;
}

int BuildOutput::GetWarnCount() const {
	return m_nWarn;
}

void BuildOutput::NotifyErr (LPCTSTR ptszLoc,LPCTSTR ptszMsg)
{
	ASSERT(ptszMsg && AfxIsValidString(ptszMsg));
	ASSERT(ptszLoc && AfxIsValidString(ptszLoc));

	AppendItem(BUILD_OUTPUT_ITEM::TYPE_ERROR,ptszLoc,ptszMsg);
	m_nErr++;
}

void BuildOutput::AppendItem(BUILD_OUTPUT_ITEM::TYPE nType,LPCTSTR ptszLoc,LPCTSTR ptszMsg) {
	ASSERT(ptszMsg && AfxIsValidString(ptszMsg));
	ASSERT(ptszLoc && AfxIsValidString(ptszLoc));

	BUILD_OUTPUT_ITEM i;
	i.m_nType  = nType;
	i.m_strLoc = ptszLoc;
	i.m_strMsg = ptszMsg;
	m_Items.AddTail(i);
}

void BuildOutput::NotifyWarn(LPCTSTR ptszLoc,LPCTSTR ptszMsg) {
	ASSERT(ptszMsg && AfxIsValidString(ptszMsg));
	ASSERT(ptszLoc && AfxIsValidString(ptszLoc));

	AppendItem(BUILD_OUTPUT_ITEM::TYPE_WARNING,ptszLoc,ptszMsg);
	m_nWarn++;
}

void BuildOutput::CopyFrom(const BuildOutput& r) {
	m_State = r.m_State;

	while (0 < m_Items.GetCount()) {
		m_Items.RemoveHead();
	}
	POSITION pos = r.m_Items.GetHeadPosition();
	while (pos != NULL) {
		m_Items.AddTail(r.m_Items.GetNext(pos));
	}
}

int BuildOutput::BuildOutputState2Icon(BuildOutput::STATE s) {
	switch(s) {
	case BuildOutput::STATE_NONE:
		return 0;
	case BuildOutput::STATE_NORMAL:
		return 1;
	case BuildOutput::STATE_WARNING:
		return 2;
	case BuildOutput::STATE_ERROR:
		return 4;
	}
	ASSERT(FALSE);
	return 4;
}

int BuildOutput::BuildOutputItem2Icon(BUILD_OUTPUT_ITEM::TYPE t) {
	switch(t) {
	case BUILD_OUTPUT_ITEM::TYPE_WARNING:
		return 0;
	case BUILD_OUTPUT_ITEM::TYPE_ERROR:
		return 1;
	}
	ASSERT(FALSE);
	return 1;
}

CString BuildOutput::BuildOutputItem2Text(BUILD_OUTPUT_ITEM::TYPE t) {
	UINT nID = IDS_BUILD_OUTPUT_ITEM_TYPE_ERROR;
	switch(t) {
	case BUILD_OUTPUT_ITEM::TYPE_WARNING:
		nID = IDS_BUILD_OUTPUT_ITEM_TYPE_WARNING;
		break;
	case BUILD_OUTPUT_ITEM::TYPE_ERROR:
		nID  = IDS_BUILD_OUTPUT_ITEM_TYPE_ERROR;
		break;
	default:
		ASSERT(FALSE);
	}
	CString load;
	load.LoadString(nID);
	return load;
}
