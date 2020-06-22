#include "stdafx.h"
#include "resource.h"
#include "SoundPlayActionDlg.h"

#include "model/Appli.h"
#include "model/action/SoundPlayAction.h"
#include "model/char/CharList.h"
#include "model/char/SndChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(SoundPlayActionDlg, ActionDlg)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(SoundPlayActionDlg,ActionDlg)
SoundPlayActionDlg::SoundPlayActionDlg()
:	ActionDlg() {
}

void SoundPlayActionDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCC_SOUND_CHARS, m_cbSoundChars);
}

void SoundPlayActionDlg::SetAction(Appli* appli,Action* action) {
	ActionDlg::SetAction(appli,action);
	UpdateSndCharList(appli);
}

void SoundPlayActionDlg::Apply()
{
	int select = m_cbSoundChars.GetCurSel();
	if (select == -1) {
		GetAction()->SetSound(NULL);
	} else {
		ASSERT(0 <= select && select < m_soundChars.size());
		GetAction()->SetSound(m_soundChars[select]);
	}
}

BOOL SoundPlayActionDlg::Create(CWnd* wnd) {
	ASSERT_VALID(wnd);
	return CDialog::Create(IDD_ACTDLG_SOUND_PLAY,wnd);
}

SoundPlayAction* SoundPlayActionDlg::GetAction() {
	ASSERT_VALID(this);
	SoundPlayAction* action = dynamic_cast<SoundPlayAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}

void SoundPlayActionDlg::UpdateSndCharList(const Appli* appli) {
	ASSERT_VALID(appli);

	int select = -1;
	m_cbSoundChars.SetRedraw(FALSE);

	while (0 < m_cbSoundChars.GetCount()) {
		m_cbSoundChars.DeleteString(0);
	}
	m_soundChars.clear();

	const CharList* charList = appli->charList.get();
	const SndChar* current = GetAction()->GetSound();

	for (int n = 0;n < charList->GetCharCount();n++) {
		const Char* c = charList->GetChar(n);
		if (!c->GetCharType().canSoundChar) {
			continue;
		}

		m_soundChars.push_back(dynamic_cast<const SndChar*>(c));
		int add= m_cbSoundChars.AddString(c->GetName());
		if (c == current) {
			select = add;
		}
	}
	if (select != -1) {
		m_cbSoundChars.SetCurSel(select);
	}
	m_cbSoundChars.SetRedraw(TRUE);
}
