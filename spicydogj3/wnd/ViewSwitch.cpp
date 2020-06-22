#include "STDAFX.H"
#include "ViewSwitch.H"
#include "view/CharView.h"
#include "model/char/Char.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//	ビュー毎の情報
//	コンストラクタ
ViewSwitchInfo::ViewSwitchInfo()
:	m_pView(NULL),
	m_bClosable(FALSE),
	m_nIcon(0)
{
}

ViewSwitchInfo::ViewSwitchInfo(CView* pView,LPCTSTR ptszName,BOOL bClosable,UINT nIcon)
:	m_pView(pView),
	m_bClosable(bClosable),
	m_nIcon(nIcon),
	m_strDefaultCaption(ptszName)
{
	ASSERT_VALID(pView);
}

//	コピーコンストラクタ
ViewSwitchInfo::ViewSwitchInfo(const ViewSwitchInfo& rhs)
{
	CopyFrom(rhs);
}

//	代入演算子
ViewSwitchInfo& ViewSwitchInfo::operator = (const ViewSwitchInfo& rhs)
{
	ASSERT(this != NULL);
	ASSERT(AfxIsValidAddress(this,sizeof this,TRUE));

	CopyFrom(rhs);

	return *this;
}

//	ビューのポインタを取得
CView* ViewSwitchInfo::GetView() const
{
	ASSERT(m_pView != NULL);
	return m_pView;
}

CString ViewSwitchInfo::GetCaption() const {
	const CharView* pView = dynamic_cast<const CharView*>(m_pView);
	CString str = m_strDefaultCaption;
	pView->GetViewCaption(str);
	return str;
}

BOOL ViewSwitchInfo::IsClosable() const {
	return m_bClosable;
}

//	ビューが閉じられるかどうか設定
void ViewSwitchInfo::SetClosable(BOOL b)
{
	m_bClosable = b;
}

//	ビューのアイコン情報を取得
UINT ViewSwitchInfo::GetIcon() const
{
	return m_nIcon;
}

//	インプリメント
void ViewSwitchInfo::CopyFrom(const ViewSwitchInfo& rhs)
{
	m_pView      = rhs.m_pView;
	m_strDefaultCaption = rhs.m_strDefaultCaption;
	m_nIcon      = rhs.m_nIcon;
	m_bClosable  = rhs.m_bClosable;
}


ViewSwitch::ViewSwitch()
:	m_nActive(0) {
}

int ViewSwitch::GetViewCount() const {
	return m_Views.size();
}

void ViewSwitch::GetViewInfo(int n,ViewSwitchInfo& info) const {
	ASSERT(0 <= n);
	ASSERT(n < GetViewCount());

	VIEWS::const_iterator i = m_Views.begin();
	std::advance(i,n);

	if (i != m_Views.end())
	{
		info = *i;
	}
	else
		ASSERT(FALSE);
}

void ViewSwitch::SetViewInfo(int n,const ViewSwitchInfo& info) {
	ASSERT(0 <= n);
	ASSERT(n < GetViewCount());

	VIEWS::iterator i = m_Views.begin();
	std::advance(i,n);

	if (i != m_Views.end())
	{
		*i = info;
		ASSERT_VALID(info.GetView());
	}
	else
		ASSERT(FALSE);
}

void ViewSwitch::ActivateView(int n) {
	ASSERT(0 <= n);
	ASSERT(n < GetViewCount());

	FireActiveViewChanged(n,m_nActive);
	m_nActive = n;
}

int ViewSwitch::GetActiveView() const {
	ASSERT(0 <= m_nActive);
	ASSERT(m_nActive < GetViewCount());

	return m_nActive;
}

int ViewSwitch::AppendView(CView* pView,LPCTSTR ptszCaption,BOOL b,UINT nIcon) {
	m_Views.push_back(ViewSwitchInfo(pView,ptszCaption,b,nIcon));
	return GetViewCount() -1;
}

void ViewSwitch::RemoveView(int n) {
	ASSERT(0 <= n);
	ASSERT(n < GetViewCount());

	//	通知
	{
		ViewSwitchInfo info;
		GetViewInfo(n,info);
		FireRemoveView(n,info.GetView());
	}

	VIEWS::iterator i = m_Views.begin();
	std::advance(i,n);
	m_Views.erase(i);
	if (GetViewCount() <= m_nActive)
	{
		ASSERT(0 < GetViewCount());
		ActivateView(GetViewCount() -1);
	}

	ASSERT(0 <= m_nActive);
	ASSERT(m_nActive < GetViewCount());
}

//	イベント発行
void ViewSwitch::FireRemoveView(int nRemove,CView* pView)
{
	ASSERT(0 <= nRemove);
	ASSERT(nRemove < GetViewCount());
	ASSERT_VALID(pView);

	m_nRemoved = nRemove;
	m_pRemovedView = pView;

	EnumListeners(this,EnumRemoveView);
}

void ViewSwitch::EnumRemoveView(ViewSwitch* viewSwitch,ViewSwitchListener* listener) {
	ASSERT(AfxIsValidAddress(viewSwitch,sizeof ViewSwitch));
	ASSERT(AfxIsValidAddress(listener,sizeof ViewSwitchListener));
	listener->OnRemoveView(m_nRemoved,m_pRemovedView);
}

void ViewSwitch::FireActiveViewChanged(int nNew,int nOld)
{
	ASSERT(0 <= nNew && nNew < GetViewCount());
	ASSERT(0 <= nOld && nOld < (GetViewCount() + 1));
	
	m_nOldActive = nOld;
	m_nNewActive = nNew;

	EnumListeners(this,EnumActiveViewChanged);
}

void ViewSwitch::EnumActiveViewChanged(ViewSwitch*,ViewSwitchListener* pListener)
{
	pListener->OnActiveViewChanged(m_nNewActive,m_nOldActive);
}