#pragma once

#include <afxtempl.h>

/**
	î≠çsèÓïÒ
*/
struct BUILD_OUTPUT_ITEM {
	enum TYPE {TYPE_WARNING,TYPE_ERROR};
	TYPE m_nType;
	CString m_strLoc;
	CString m_strMsg;
};

/**
*/
class BuildOutput {
	friend class Builder;
public:
	enum STATE {STATE_NORMAL,STATE_WARNING,STATE_ERROR,STATE_NONE,STATE_OVERSIZE};
	typedef CList<BUILD_OUTPUT_ITEM,BUILD_OUTPUT_ITEM&> BUILD_OUTPUT_ITEM_LIST;

private:
	STATE m_State;
	BUILD_OUTPUT_ITEM_LIST m_Items;
	int m_nErr;
	int m_nWarn;
	int m_nLength;
	int m_nMaxLength;

public:
	BuildOutput();
	BuildOutput(const BuildOutput&);
	BuildOutput& operator =(const BuildOutput&);
	STATE GetResult() const;
	int GetErrCount() const;
	int GetWarnCount() const;
	int GetJarSize() const;
	int GetMaxJarSize() const;

	int GetItemCount() const;
	const BUILD_OUTPUT_ITEM& GetItem(int) const;
	static int BuildOutputState2Icon(BuildOutput::STATE);
	static int BuildOutputItem2Icon(BUILD_OUTPUT_ITEM::TYPE);
	static CString BuildOutputItem2Text(BUILD_OUTPUT_ITEM::TYPE);

private:
	void BeginBuildOutput();
	void EndBuildOutput(int length,int maxlength);

	void NotifyErr (LPCTSTR,LPCTSTR);
	void NotifyWarn(LPCTSTR,LPCTSTR);

	void AppendItem(BUILD_OUTPUT_ITEM::TYPE,LPCTSTR,LPCTSTR);
	void CopyFrom(const BuildOutput&);
};