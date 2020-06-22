#pragma once

class ChildDataExchange {
private:
	CDataExchange* m_pDX;
	CWnd* m_pwndTarget;
	CWnd* m_pwndSaved;
public:
	ChildDataExchange(CWnd*,CDataExchange*);
	~ChildDataExchange();
}; 