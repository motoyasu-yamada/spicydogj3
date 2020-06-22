#pragma once

#include "ActionDlg.H"
#include <vector>

class SoundPlayAction;
class SndChar;

typedef std::vector<const SndChar*> SNDCHARS;

class SoundPlayActionDlg : public ActionDlg {
public:
	DECLARE_DYNCREATE(SoundPlayActionDlg)
	SoundPlayActionDlg();

private:
	void UpdateSndCharList(const Appli* appli);
	SoundPlayAction* GetAction();
	virtual BOOL Create(CWnd*);
	virtual void SetAction(Appli*,Action*);
	virtual void Apply();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	CComboBox m_cbSoundChars;
	SNDCHARS  m_soundChars;

	DECLARE_MESSAGE_MAP()
};

