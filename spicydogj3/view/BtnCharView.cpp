#include "stdafx.h"
#include "resource.h"
#include "BtnCharView.h"

#include "cmd/char/ModBtnCharCmd.h"
#include "dlg/char/BtnCharMailEventDlg.h"
#include "dlg/char/BtnCharTelEventDlg.h"
#include "dlg/char/BtnCharScheduleEventDlg.h"
#include "model/Appli.h"
#include "model/char/CharList.h"
#include "utility/PaintMemDC.h"
#include "utility/WndUtility.h"
#include "utility/DDXUtility.h"
#include "SpicydogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(BtnCharView, CharDialog)
	ON_BN_CLICKED(IDCB_MAIL_EVENT,OnEditMailEvent)
	ON_BN_CLICKED(IDCB_SCHEDULE_EVENT,OnEditScheduleEvent)
	ON_BN_CLICKED(IDCB_TEL_EVENT,OnEditTelEvent)
	ON_BN_CLICKED(IDCC_KEY0, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY1, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY2, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY3, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY4, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY5, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY6, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY7, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY8, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEY9, OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYASTER,   OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYDOWN,    OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYLEFT,    OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYPOUND,   OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYRIGHT,   OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYSELECT,  OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYSOFT1,   OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYSOFT2,   OnUpdateData)
	ON_BN_CLICKED(IDCC_KEYUP,      OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT,OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT_AUTO, OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT_EMERGENCY,OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT_GREETING, OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT_RELAY,OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT_SKY,      OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT_STANDARD, OnUpdateData)
	ON_BN_CLICKED(IDCC_MAIL_EVENT_SUPER,    OnUpdateData)
	ON_BN_CLICKED(IDCC_SCHEDULE_EVENT,OnUpdateData)
	ON_BN_CLICKED(IDCC_TEL_EVENT,OnUpdateData)
	ON_CBN_SELCHANGE(IDCC_ACTIVE, OnUpdateData)
	ON_CBN_SELCHANGE(IDCC_NORMAL, OnUpdateData)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(BtnCharView,CharDialog);

CImageList BtnCharView::s_images;
CString    BtnCharView::s_strUnsetItem;
void BtnCharView::clinit() {
	static BOOL clinited = FALSE;
	if (clinited) {
		return;
	}

	if (!CreateImageListColor32(s_images,IDB_CHAR_TYPE,16,16,RGB(0x00,0x80,0x80))) {
		ASSERT(FALSE);
		AfxThrowResourceException();
	}
	if (!s_strUnsetItem.LoadString(IDS_BTN_CHAR_VIEW_NOCHAR_REF)) {
		ASSERT(FALSE);
		AfxThrowResourceException();
	}

	clinited = TRUE;
}

BtnCharView::BtnCharView() {
	m_inited = FALSE;
	clinit();
	Init(IDD_BTN_CHAR);
}

void BtnCharView::InitBtnableCharList() {
	SpicydogDoc* document = DYNAMIC_DOWNCAST(SpicydogDoc,GetDocument());
	ASSERT_VALID(document);
	const CharList* charList = document->appli->charList.get();
	ASSERT_VALID(charList);
	for (int n = 0;n < charList->GetCharCount();n++) {
		const Char* char1 = charList->GetChar(n);
		ASSERT_VALID(char1);
		if ((!char1->GetCharType().canButtonChar) || char1 == GetChar()) {
			continue;
		}
		m_chars.push_back(char1);
	}
}

BOOL BtnCharView::OnInitDialog() {
	CharDialog::OnInitDialog();

	CWnd* wnd = GetDlgItem(IDCS_ACTIONLIST);
	ASSERT_VALID(wnd);
	CRect rc;
	wnd->GetWindowRect(rc);
	ScreenToClient(rc);
	if (m_dlgActionList.GetSafeHwnd() != NULL)
		m_dlgActionList.DestroyWindow();
	m_dlgActionList.Create(GetDocument()->appli.get(),&GetBtnChar()->actions,this,rc);

	m_psPreviewN.SetListener(this);
	m_psPreviewA.SetListener(this);
	m_cbActive.SetImageList(&s_images);
	m_cbNormal.SetImageList(&s_images);
	m_cbActive.ModifyStyle(CBS_SORT,CBS_HASSTRINGS);
	SetButtonIcon(IDCC_KEY1,IDI_KEY1);
	SetButtonIcon(IDCC_KEY2,IDI_KEY2);
	SetButtonIcon(IDCC_KEY3,IDI_KEY3);
	SetButtonIcon(IDCC_KEY4,IDI_KEY4);
	SetButtonIcon(IDCC_KEY5,IDI_KEY5);
	SetButtonIcon(IDCC_KEY6,IDI_KEY6);
	SetButtonIcon(IDCC_KEY7,IDI_KEY7);
	SetButtonIcon(IDCC_KEY8,IDI_KEY8);
	SetButtonIcon(IDCC_KEY9,IDI_KEY9);
	SetButtonIcon(IDCC_KEY0,IDI_KEY0);
	SetButtonIcon(IDCC_KEYASTER,IDI_KEYASTER);
	SetButtonIcon(IDCC_KEYPOUND,IDI_KEYSHARP);
	SetButtonIcon(IDCC_KEYSOFT1,IDI_KEYSOFTKEY1);
	SetButtonIcon(IDCC_KEYSOFT2,IDI_KEYSOFTKEY2);
	SetButtonIcon(IDCC_KEYLEFT,IDI_KEYLEFT);
	SetButtonIcon(IDCC_KEYRIGHT,IDI_KEYRIGHT);
	SetButtonIcon(IDCC_KEYUP,IDI_KEYUP);
	SetButtonIcon(IDCC_KEYDOWN,IDI_KEYDOWN);
	SetButtonIcon(IDCC_KEYSELECT,IDI_KEYSELECT);
	InitBtnableCharList();
	GetBtnChar()->GetState(&m_state);
	UpdateCharList();
	m_inited = TRUE;

	UpdateData(FALSE);
	UpdateControls();

	return TRUE;
}

void BtnCharView::SetButtonIcon(UINT nID,UINT nRes) {
	HICON h = (HICON) ::LoadImage(AfxGetResourceHandle(),
								  MAKEINTRESOURCE(nRes),
								  IMAGE_ICON,
								  16,
								  16,
								  LR_DEFAULTCOLOR);
	ASSERT(h != NULL);
	CButton* p = (CButton*) GetDlgItem(nID);
	ASSERT_VALID(p);
	p->SetIcon(h);
}

void BtnCharView::DoDataExchange(CDataExchange* pDX) {
	CharDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCC_NORMAL, m_cbNormal);
	DDX_Control(pDX, IDCC_ACTIVE, m_cbActive);
	DDX_Control(pDX, IDCS_PREVIEW_N, m_psPreviewN);
	DDX_Control(pDX, IDCS_PREVIEW_A, m_psPreviewA);
	DDX_Check(pDX,IDCC_KEY0,m_state.keyEvent.KEY_0);
	DDX_Check(pDX,IDCC_KEY1,m_state.keyEvent.KEY_1);
	DDX_Check(pDX,IDCC_KEY2,m_state.keyEvent.KEY_2);
	DDX_Check(pDX,IDCC_KEY3,m_state.keyEvent.KEY_3);
	DDX_Check(pDX,IDCC_KEY4,m_state.keyEvent.KEY_4);
	DDX_Check(pDX,IDCC_KEY5,m_state.keyEvent.KEY_5);
	DDX_Check(pDX,IDCC_KEY6,m_state.keyEvent.KEY_6);
	DDX_Check(pDX,IDCC_KEY7,m_state.keyEvent.KEY_7);
	DDX_Check(pDX,IDCC_KEY8,m_state.keyEvent.KEY_8);
	DDX_Check(pDX,IDCC_KEY9,m_state.keyEvent.KEY_9);
	DDX_Check(pDX,IDCC_KEYLEFT,  m_state.keyEvent.KEY_LEFT);
	DDX_Check(pDX,IDCC_KEYRIGHT, m_state.keyEvent.KEY_RIGHT);
	DDX_Check(pDX,IDCC_KEYUP,    m_state.keyEvent.KEY_UP);
	DDX_Check(pDX,IDCC_KEYDOWN,  m_state.keyEvent.KEY_DOWN);
	DDX_Check(pDX,IDCC_KEYASTER, m_state.keyEvent.KEY_ASTER);
	DDX_Check(pDX,IDCC_KEYPOUND, m_state.keyEvent.KEY_POUND);
	DDX_Check(pDX,IDCC_KEYSOFT1, m_state.keyEvent.KEY_SOFT1);
	DDX_Check(pDX,IDCC_KEYSOFT2, m_state.keyEvent.KEY_SOFT2);
	DDX_Check(pDX,IDCC_KEYSELECT,m_state.keyEvent.KEY_SELECT);
	DDX_Check(pDX,IDCC_MAIL_EVENT,m_state.jphoneMailEvent.useEvent);
	DDX_Check(pDX,IDCC_SCHEDULE_EVENT,m_state.jphoneScheduleEvent.useEvent);
	DDX_Check(pDX,IDCC_TEL_EVENT,m_state.jphoneTelEvent.useEvent);

	if (pDX->m_bSaveAndValidate) {
		m_dlgActionList.UpdateCurrentData();

		int nSelNormal = m_cbNormal.GetCurSel();
		int nSelActive = m_cbActive.GetCurSel();
		if (nSelNormal == -1) {
			nSelNormal = 0;
		}
		if (nSelActive == -1) {
			nSelActive = 0;
		}
		m_state.normalState.ReferTo(nSelNormal == 0 ? NULL : m_chars[nSelNormal - 1]);
		m_state.activeState.ReferTo(nSelActive == 0 ? NULL : m_chars[nSelActive - 1]);
	}
}

void BtnCharView::OnUpdateData() {
	if (!m_inited) {
		return;
	}
	UpdateControls();
}

void BtnCharView::UpdateControls() {
	m_psPreviewA.Invalidate();
	m_psPreviewN.Invalidate();
	DDXUtility::UpdateDataEx(this,TRUE);
	EnableDlgItem(this,IDCB_MAIL_EVENT,    m_state.jphoneMailEvent.useEvent);
	EnableDlgItem(this,IDCB_TEL_EVENT,     m_state.jphoneTelEvent.useEvent);
	EnableDlgItem(this,IDCB_SCHEDULE_EVENT,m_state.jphoneScheduleEvent.useEvent);
}

BtnChar* BtnCharView::GetBtnChar() {
	BtnChar* getChar = dynamic_cast<BtnChar*>(GetChar());
	ASSERT_VALID(getChar);
	return getChar;
}

void DeleteComboBozAllItems(CComboBox& cb) {
	while (0 < cb.GetCount())
		cb.DeleteString(0);
}

void BtnCharView::UpdateCharList() {
	m_cbNormal.SetRedraw(FALSE);
	m_cbActive.SetRedraw(FALSE);

	DeleteComboBozAllItems(m_cbNormal);
	DeleteComboBozAllItems(m_cbActive);

	COMBOBOXEXITEM item;
	int iItem       = 0;
	ZeroMemory(&item,sizeof item);
	item.iItem      = iItem++;
	item.mask       = CBEIF_IMAGE | CBEIF_TEXT | CBEIF_SELECTEDIMAGE;
	item.iImage     = item.iSelectedImage = 0;
	item.pszText    =  (LPTSTR)(LPCTSTR)s_strUnsetItem;
	item.cchTextMax =  s_strUnsetItem.GetLength() + 1;
	m_cbNormal.InsertItem(&item);
	m_cbActive.InsertItem(&item);

	int nSelNormal = 0;
	int nSelActive = 0;
	const Char* normal = m_state.normalState;
	const Char* active = m_state.activeState;
	int count = m_chars.size();
	for (int n = 0;n < count;n++) {
		const Char* char1 = m_chars[n];
		const CString strName = char1->GetName();
		item.iItem      = iItem;
		item.iImage     = item.iSelectedImage = char1->GetCharType().iconIndex;
		item.pszText    =  (LPTSTR)(LPCTSTR)strName;
		item.cchTextMax =  strName.GetLength() + 1;

		VERIFY(m_cbNormal.InsertItem(&item) == iItem);
		VERIFY(m_cbActive.InsertItem(&item) == iItem);
		
		if (char1 == normal) {
			nSelNormal  = iItem;
		}
		if (char1 == active) {
			nSelActive = iItem;
		}
		iItem++;
	}

	if (nSelNormal != -1) {
		m_cbNormal.SetCurSel(nSelNormal);
	}
	if (nSelActive != -1) {
		m_cbActive.SetCurSel(nSelActive);
	}
	m_cbNormal.SetRedraw(TRUE);
	m_cbActive.SetRedraw(TRUE);
}

void BtnCharView::OnOK() {
	if (!UpdateData()) {
		return;
	}
	
	Cmd* cmd = new ModBtnCharCmd(GetBtnChar(),
		                         m_state,
								 m_dlgActionList.GetEditActionList());
	GetDocument()->DoCmd(cmd);
	GetDocument()->UpdateAll(NULL);
	CharDialog::OnOK();
}

SpicydogDoc* BtnCharView::GetDocument() {
	SpicydogDoc* document = DYNAMIC_DOWNCAST(SpicydogDoc,CharDialog::GetDocument());
	ASSERT_VALID(document);
	return document;
}

void BtnCharView::OnEditMailEvent() {
	BtnCharMailEventDlg dlg(&m_state.jphoneMailEvent);
	if (dlg.DoModal() == IDOK) {
		m_state.jphoneMailEvent = dlg.mailEvent;
	}
}

void BtnCharView::OnEditTelEvent(){
	BtnCharTelEventDlg dlg(&m_state.jphoneTelEvent);
	if (dlg.DoModal() == IDOK) {
		m_state.jphoneTelEvent = dlg.telEvent;
	}
}

void BtnCharView::OnEditScheduleEvent(){
	BtnCharScheduleEventDlg dlg(&m_state.jphoneScheduleEvent);
	if (dlg.DoModal() == IDOK) {
		m_state.jphoneScheduleEvent = dlg.scheduleEvent;
	}
}

void BtnCharView::OnPSPaint(PreviewStatic& ps) {
	PaintMemDC dc(&ps);

	COLORREF crBkgnd = ::GetSysColor(COLOR_APPWORKSPACE);
	CRect rc;
	ps.GetClientRect(rc);
	int select = (&ps == &m_psPreviewN) ? m_cbNormal.GetCurSel() : m_cbActive.GetCurSel();
	select --;
	if (select == -1) {
		dc.FillSolidRect(rc,crBkgnd);
	} else {
		const Char* charDraw = m_chars[select];
		ASSERT_VALID(charDraw);
		CharPreview preview;
		preview.dc         = &dc;
		preview.rect       = rc;
		preview.bkgndColor = crBkgnd;
		charDraw->DrawPreview(&preview);
	}	
}
