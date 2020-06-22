#pragma once

/** インライン編集用 */
class InPlaceEdit : public CEdit {
public:
	InPlaceEdit();
	virtual ~InPlaceEdit();
	BOOL Create(CRect& rc,CWnd* pParent,UINT nCtrlID);
	BOOL DoEdit(LPCTSTR,LPCRECT);
	CString GetText() const;

protected:
	void EnterEdit(LPCTSTR ptszDefault,LPCRECT prc);
	BOOL TrackEdit();
	void ExitEdit();
	DECLARE_MESSAGE_MAP()
};


