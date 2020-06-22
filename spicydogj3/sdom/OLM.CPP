//	$Id: OLM.CPP,v 1.5 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"
#include "olm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/// OLMの各種生成・消滅イベントをトレースするかどうか
//#define USE_TRACE_OLM_LIFE
#if defined(USE_TRACE_OLM_LIFE)
	#define TRACE_OLM_LIFE ::AfxTrace
#else
	#define TRACE_OLM_LIFE 1 ? (void)0 : ::AfxTrace
#endif

#if defined(_DEBUG)

BOOL assertValidObject(CObject const* p,BOOL allowNull,LPCTSTR file,int line) {
	if (p == NULL) {
		return allowNull;
	}
	AfxAssertValidObject(p,file,line);
	return true;
}
#define ASSERT_VALID_EX(p,b) ASSERT(assertValidObject(static_cast<CObject const*>(p),true,__FILE__,__LINE__))

template<typename _T>
const char* class_name(const _T* p) {
	return typeid(*p).name();
}

#else
	#define class_name(p) NULL
	#define ASSERT_VALID_EX(p,b)
#endif



COLMObject::COLMObject() {
}

COLMObject::~COLMObject() {
	TRACE_OLM_LIFE("%08X:COLMObject::~COLMObject(%s)\n",this,class_name(this));

	REFERERS referers;
	REFERERS::iterator copy;
	for (copy = m_referers.begin();copy != m_referers.end();copy++) {
		referers.push_back(*copy);
	}
	ASSERT(referers.size() == m_referers.size());
	
	REFERERS::iterator i;
	for (i = referers.begin(); i != referers.end();i++) {
		COLMReferer* pReferer = *i;
		ASSERT(AfxIsValidAddress(pReferer,sizeof COLMReferer));
		ASSERT(pReferer->GetKeptOLMObject() == this || pReferer->GetKeptOLMObject() == NULL);
		pReferer->ClearRefererer();
		
		TRACE_OLM_LIFE("%08X:COLMObject::~COLMObject referer=%08X\n",this,pReferer);
	}

	ASSERT(m_referers.size() == 0);
}

void COLMObject::AppendReferer(COLMReferer* pReferer) const {
	TRACE_OLM_LIFE("%08X:COLMObject::AppendReferer(%s) referer=%08X\n",this,class_name(this),pReferer);

	ASSERT(AfxIsValidAddress(this,sizeof COLMObject));
	ASSERT(AfxIsValidAddress(pReferer,sizeof COLMReferer));
	ASSERT(std::find(m_referers.begin(),m_referers.end(),pReferer) == m_referers.end());

	m_referers.push_back(pReferer);
}

void COLMObject::RemoveReferer(COLMReferer* pReferer) const {
	TRACE_OLM_LIFE("%08X:COLMObject::RemoveReferer(%s) referer=%08X\n",this,class_name(this),pReferer);

	ASSERT(AfxIsValidAddress(this,sizeof COLMObject));
	ASSERT(AfxIsValidAddress(pReferer,sizeof COLMReferer));
	ASSERT(pReferer->GetKeptOLMObject() == this);
	ASSERT(0 < m_referers.size());
	REFERERS::iterator i = std::find(m_referers.begin(),m_referers.end(),pReferer);
	ASSERT(i != m_referers.end());
	
	m_referers.erase(i);
}

void COLMObject::OnRemovedOLMObject(COLMObject* pObj)
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMObject));
	ASSERT_VALID(pObj);
	ASSERT(pObj == this);
	
	REFERERS referers(m_referers);
	ASSERT(referers.size() == m_referers.size());

	REFERERS::iterator i;
	for (i = referers.begin(); i != referers.end();i++)
	{
		COLMReferer* pReferer = *i;
		ASSERT(AfxIsValidAddress(pReferer,sizeof COLMReferer));
		ASSERT(pReferer->GetObject() == pObj);
		ASSERT_VALID(pReferer);
	
		pReferer->OnRemovedOLMObject(pObj);
	}
}

void COLMObject::OnRevivalOLMObject(COLMObject* pObj)
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMObject));
	ASSERT(pObj == this);

	REFERERS::iterator i;
	for (i = m_referers.begin(); i != m_referers.end();i++)
	{
		COLMReferer* pReferer = *i;
		ASSERT(AfxIsValidAddress(pReferer,sizeof COLMReferer));
		ASSERT(pReferer->IsNullObject());
		ASSERT(pReferer->GetKeptOLMObject() == this);
		
		pReferer->OnRevivalOLMObject(pObj);
	}
}

int COLMObject::GetReferenceCount() const {
	ASSERT_VALID(this);
	return m_referers.size();
}


COLMObjectPtr::COLMObjectPtr() {
	m_pObj = NULL;
}

COLMObjectPtr::COLMObjectPtr(const COLMObjectPtr& src) {
	m_pObj = NULL;
}

COLMObjectPtr::~COLMObjectPtr() {
	TRACE_OLM_LIFE("%08X:COLMObjectPtr::~COLMObjectPtr(%s) m_pObj=%08x\n",this,class_name(this),m_pObj);
	ASSERT(m_pObj == NULL);
	m_pObj = NULL;
}

void COLMObjectPtr::DeleteObject() {
	TRACE_OLM_LIFE("%08X:COLMObjectPtr::DeleteObject(%s) m_pObj=%08x\n",this,class_name(this),m_pObj);
	ASSERT(m_pObj != NULL);
	delete m_pObj;
	m_pObj = NULL;
}

COLMObjectPtr& COLMObjectPtr::operator =(const COLMObjectPtr& src) {
	m_pObj = NULL;
	return *this;
}

BOOL COLMObjectPtr::IsNullObject() const {
	return m_pObj == NULL;
}


COLMObjectPtr::operator const CObject*() const {
	ASSERT_VALID(this);
	return m_pObj;
}

const COLMObject& COLMObjectPtr::operator*() const {
	ASSERT_VALID(this);
	return *m_pObj;
}

COLMObject* COLMObjectPtr::GetObject() {
	ASSERT_VALID(this);
	return m_pObj;
}

void COLMObjectPtr::SetObject(COLMObject* pObject) {
	ASSERT_VALID(this);
	m_pObj = pObject;
	ASSERT_VALID(this);
}

const COLMObject* COLMObjectPtr::GetObject() const {
	ASSERT_VALID(this);
	return m_pObj;
}

BOOL COLMObjectPtr::operator ==(const CObject*& p) const {
	ASSERT_VALID(this);
	return m_pObj == p;
}

void COLMObjectPtr::AssertValid() const {
	if (!IsNullObject()) {
		ASSERT_VALID(m_pObj);
	}
	ASSERT(AfxIsValidAddress(this,sizeof COLMObjectPtr));
}


COLMOwner::COLMOwner() {
}


COLMOwner::~COLMOwner() {
	if (!IsNullObject()) {
		DeleteObject();
	}
}

COLMOwner::COLMOwner(const COLMOwner& src) {
	ASSERT(src.IsNullObject());
}

void COLMOwner::OwnNew(COLMObject* po) {
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwner));
	ASSERT(IsNullObject());
	SetObject(po);
	ASSERT_VALID_EX(po,true);
}

void COLMOwner::InheritedFrom(COLMOwner* p) {
	ASSERT_VALID(this);
	ASSERT(IsNullObject());
	ASSERT(p != NULL);
	ASSERT(p != this);
	
	COLMObject* pSrcObj = p->GetObject();
	ASSERT(pSrcObj != NULL);
	
	p->SetObject(NULL);
	SetObject(pSrcObj);

	ASSERT_VALID(this);
}




COLMKeeper::COLMKeeper() {
}

COLMKeeper::COLMKeeper(const COLMKeeper& src) {
	ASSERT(src.IsNullObject());
}

COLMKeeper::~COLMKeeper() {
	if (!IsNullObject()) {
		DeleteObject();
	}
}

void COLMKeeper::OwnNew(COLMObject* pSrcObj) {
	ASSERT_VALID(this);
	ASSERT(IsNullObject());
	ASSERT_VALID_EX(pSrcObj,TRUE);

	SetObject(pSrcObj);

	ASSERT_VALID(this);
}

void COLMKeeper::InheritedFrom(COLMOwner* pOwner) {
	ASSERT_VALID(this);
	ASSERT(pOwner != NULL);
	ASSERT(IsNullObject());
	
	SetObject(pOwner->GetObject());
	if (!IsNullObject()) {
		Backup(GetObject());
		pOwner->SetObject(NULL);
	}
	
	ASSERT(pOwner->GetObject() == NULL);
}

void COLMKeeper::TransferTo(COLMOwner* pOwner) {
	ASSERT_VALID(this);
	ASSERT(pOwner != NULL);
	ASSERT(pOwner->IsNullObject());
	
	pOwner->SetObject(GetObject());
	if (!IsNullObject()) {
		Restore(GetObject());
		SetObject(NULL);
	}
	
	ASSERT(GetObject() == NULL);
}

void COLMKeeper::Backup(COLMObject* pObj) {
	ASSERT_VALID(this);
	ASSERT_VALID(pObj);
	ASSERT(GetObject() == pObj);
	
	if (!IsNullObject()) {
		GetObject()->OnRemovedOLMObject(pObj);
	}
}

void COLMKeeper::Restore(COLMObject* pObj) {
	ASSERT_VALID(this);
	ASSERT_VALID(pObj);
	ASSERT(GetObject() == pObj);
	
	if (!IsNullObject()) {
		GetObject()->OnRevivalOLMObject(pObj);
	}
}


COLMReferer::COLMReferer() {
	m_pKept     = NULL;
	m_pListener = NULL;
}

COLMReferer::COLMReferer(const COLMReferer& r) {
	SetObject(const_cast<COLMObject*>(r.GetObject()));
	m_pKept     = r.m_pKept;
	m_pListener = NULL;
}

COLMReferer& COLMReferer::operator =(const COLMReferer& r) {
	SetObject(const_cast<COLMObject*>(r.GetObject()));
	m_pKept     = r.m_pKept;
	m_pListener = NULL;
	return *this;
}

const COLMObject* COLMReferer::GetKeptOLMObject() const {
	ASSERT(AfxIsValidAddress(this,sizeof COLMReferer));

	if (!IsNullObject()) {
		TRACE_OLM_LIFE("%08X:COLMReferer::GetKeptOLMObject m_pObj=%08X(%s)\n",
			   this,
			   GetObject(),
			   class_name(GetObject()));
		return GetObject();
	}
	
	if (m_pKept != NULL) {
		TRACE_OLM_LIFE("%08X:COLMReferer::GetKeptOLMObject m_pKept=%08X(%s)\n",
			this,
			m_pKept,
			class_name(m_pKept));
		return m_pKept;
	}

	return NULL;
}

COLMReferer::~COLMReferer() {
	ASSERT(AfxIsValidAddress(this,sizeof COLMReferer));
	if (!IsNullObject()) {
		ASSERT(m_pKept == NULL);
		TRACE_OLM_LIFE("%08x:COLMReferer::~COLMReferer m_pObj=%08X(%s)\n",
			this,
			GetObject(),
			class_name(GetObject()));
		GetObject()->RemoveReferer(this);
		SetObject(NULL);
	}

	if (m_pKept != NULL) {
		TRACE_OLM_LIFE("%08x:COLMReferer::~COLMReferer m_pKept=%08X(%s)\n",
			this,
			m_pKept,
			class_name(m_pKept));
		m_pKept->RemoveReferer(this);
		m_pKept = NULL;
	}
}

void COLMReferer::ReferTo(COLMObject const* pObj) {
	ASSERT_VALID(this);

	if (pObj == GetObject()) {
		return;
	}

	ClearRefererer();
	
	ASSERT(IsNullObject());
	ASSERT(m_pKept == NULL);

	SetObject(const_cast<COLMObject*>(pObj));
	if (!IsNullObject()) {
		GetObject()->AppendReferer(this);
	}
	ASSERT_VALID(this);
}

void COLMReferer::ClearRefererer() {
	ASSERT_VALID(this);

	if (!IsNullObject()) {
		GetObject()->RemoveReferer(this);
		SetObject(NULL);
	}

	if (m_pKept != NULL) {
		ASSERT_VALID(m_pKept);
		m_pKept->RemoveReferer(this);
		m_pKept = NULL;
	}
}

void COLMReferer::AppendListener(COLMRefererListener* p) {
	ASSERT(AfxIsValidAddress(this,sizeof COLMReferer));

	ASSERT(m_pListener == NULL);
	m_pListener = p;
	ASSERT(m_pListener != NULL);
}

void COLMReferer::RemoveListener(COLMRefererListener* p)
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMReferer));

	ASSERT(m_pListener != NULL);
	ASSERT(m_pListener == p);
	m_pListener = NULL;
	ASSERT(m_pListener == NULL);
}

void COLMReferer::OnRemovedOLMObject(COLMObject* p) {
	ASSERT(AfxIsValidAddress(this,sizeof COLMReferer));
	ASSERT(p == GetObject());
	ASSERT(m_pKept == NULL);

	m_pKept = GetObject();
	SetObject(NULL);

	if (m_pListener != NULL) {
		m_pListener->OnRemovedOLMObject(p);
	}
}

void COLMReferer::OnRevivalOLMObject(COLMObject* p)
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMReferer));
	ASSERT(p == m_pKept);
	ASSERT(GetObject() == NULL);

	SetObject(m_pKept);
	m_pKept = NULL;
}


COLMOwners::COLMOwners() {
}

COLMOwners::~COLMOwners() {
}

int COLMOwners::FindPos(const COLMObject* p) const {
	int nPos = 0;
	OWNERS::const_iterator i;
	for (i = m_owners.begin();i != m_owners.end();i++)
	{
		const COLMOwner& o = *i;
		if (o.GetObject() == p) {
			return nPos;
		}
		nPos++;
	}
	ASSERT(FALSE);
	return -1;
}

void COLMOwners::ReorderFront(const std::list<COLMObject*>& panes) {
	const COLMObject* pEndPane = m_owners.rbegin()->GetObject();
	
	OWNERS::iterator i = m_owners.begin();
	while (i != m_owners.end()) {
		
		COLMObject* pPane = i->GetObject();
		ASSERT_VALID(pPane);

		OWNERS::iterator iMove = i;
		i++;
		
		std::list<COLMObject*>::const_iterator iFind = std::find(panes.begin(),panes.end(),pPane);
		
		if (iFind != panes.end()) {
			
			COLMKeeper k;
			k.InheritedFrom(&(*iMove));
			m_owners.erase(iMove);
			
			COLMOwner& o = *m_owners.insert(m_owners.end());
			k.TransferTo(&o);
		}

		if (pPane == pEndPane) {
			break;
		}
	}
}

void COLMOwners::ReorderBack(const std::list<COLMObject*>& panes)
{
	OWNERS::iterator i = m_owners.begin();
	i ++;
	OWNERS::iterator iInsert = m_owners.begin();
	while (i != m_owners.end())
	{
		COLMObject* pPane = i->GetObject();
		ASSERT_VALID(pPane);

		OWNERS::iterator iMove = i;
		i++;
		std::list<COLMObject*>::const_iterator iFind = std::find(panes.begin(),panes.end(),pPane);
		if (iFind != panes.end())
		{
			COLMKeeper k;
			k.InheritedFrom(&(*iMove));
			m_owners.erase(iMove);
			COLMOwner& o = *m_owners.insert(iInsert);
			k.TransferTo(&o);
		}
	}
}

void COLMOwners::ReorderUp(const std::list<COLMObject*>& panes)
{
	OWNERS::reverse_iterator iPrev = m_owners.rbegin();
	OWNERS::reverse_iterator i = m_owners.rbegin();
	i++;

	while (i != m_owners.rend())
	{
		COLMObject* pPane = i->GetObject();
		ASSERT_VALID(pPane);

		std::list<COLMObject*>::const_iterator iFind = std::find(panes.begin(),panes.end(),pPane);
		if (iFind != panes.end()) {
			COLMOwner& o = *i;
			COLMOwner& oPrev = *iPrev;
			o.m_pObj = oPrev.m_pObj;
			oPrev.m_pObj = pPane;
		}

		iPrev++;
		i++;
	}
}

void COLMOwners::ReorderDown(const std::list<COLMObject*>& panes)
{
	OWNERS::iterator iPrev = m_owners.begin();
	OWNERS::iterator i = m_owners.begin();
	i++;

	while (i != m_owners.end()) {
		COLMObject* pPane = i->GetObject();
		ASSERT_VALID(pPane);

		std::list<COLMObject*>::const_iterator iFind = std::find(panes.begin(),panes.end(),pPane);
		if (iFind != panes.end())
		{
			COLMOwner& oPrev = *iPrev;
			COLMOwner& o = *i;

			o.m_pObj = oPrev.m_pObj;
			oPrev.m_pObj = pPane;
		}

		iPrev++;
		i++;
	}
}

void COLMOwners::LoadOrderSnap(const COLMOwnersOrderSnap& snap)
{
	ASSERT(snap.m_snap.size() == m_owners.size());

	std::list<COLMObject*>::const_iterator iR = snap.m_snap.begin();
	OWNERS::iterator iL = m_owners.begin();
	while (iL != m_owners.end())
	{
		COLMOwner& o = *iL;
		o.m_pObj = *iR;
		iL++;
		iR++;
	}
}

void COLMOwners::SaveOrderSnap(COLMOwnersOrderSnap& snap) const
{
	ASSERT(snap.m_snap.size() == 0);
	snap.m_snap.resize(m_owners.size());

	std::list<COLMObject*>::iterator iR = snap.m_snap.begin();
	OWNERS::const_iterator iL = m_owners.begin();
	while (iL != m_owners.end())
	{
		const COLMOwner& o = *iL;
		*iR = o.m_pObj;
		iL++;
		iR++;
	}
}

COLMOwners::ConstItr COLMOwners::Begin() const
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));

	return m_owners.begin();
}

COLMOwners::ConstItr COLMOwners::End() const
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));
	return m_owners.end();
}

void COLMOwners::Move(int nL,int nR)
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));

	COLMKeeper k1;
	COLMKeeper k2;

	COLMOwner& o1 = *GetItr(nL);
	COLMOwner& o2 = *GetItr(nR);

	k1.InheritedFrom(&o1);
	k2.InheritedFrom(&o2);

	k1.TransferTo(&o2);
	k2.TransferTo(&o1);
}

int COLMOwners::GetCount() const {
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));
	return m_owners.size();
}

const COLMObject* COLMOwners::GetObject(int nPos) const {
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));
	const COLMObject* po = const_cast<COLMOwners*>(this)->GetItr(nPos)->GetObject();
	ASSERT_VALID(po);
	return po;
}

COLMObject* COLMOwners::GetObject(int nPos) {
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));
	COLMObject* po = GetItr(nPos)->GetObject();
	ASSERT_VALID(po);

	return po;
}

COLMOwners::Itr COLMOwners::GetItr(int nPos) {
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));

	if (GetCount() == nPos)
		return m_owners.end();

	Itr i = m_owners.begin();
	ASSERT(0 <= nPos && nPos < GetCount());
	std::advance(i,nPos);
	ASSERT(i != m_owners.end());
	
	return i;
}

void COLMOwners::AssertValid() const {
	ASSERT(AfxIsValidAddress(this,sizeof COLMOwners));

	OWNERS::const_iterator i;
	for (i= m_owners.begin();i != m_owners.end();i++) {
		const COLMObject* p = i->GetObject();
		ASSERT_VALID(p);
	}
}

void COLMOwners::Serialize(SDOMArchive* ar,BOOL writeid,SDOMArchive::CLASS_FACTORY factory,SDOMArchive::CLASS_NAMER namer) {
	if (ar->IsLoading()) {
		Load(ar,writeid,factory,namer);
	} else {
		Save(ar,writeid,factory,namer);
	}
}

void COLMOwners::Save(SDOMArchive* ar,BOOL writeid,SDOMArchive::CLASS_FACTORY factory,SDOMArchive::CLASS_NAMER namer) {
	ASSERT(AfxIsValidAddress(ar,sizeof SDOMArchive));
	ASSERT(ar->IsStoring());
	
	OWNERS::const_iterator i;
	for (i= m_owners.begin();i != m_owners.end();i++) {
		COLMOwner& owner = const_cast<COLMOwner&>(*i);
		ASSERT_VALID(&owner);
		ar->SerializeOwner(&owner,writeid,factory,namer);
	}
}

void COLMOwners::Load(SDOMArchive* ar,BOOL writeid,SDOMArchive::CLASS_FACTORY factory,SDOMArchive::CLASS_NAMER namer) {
	ASSERT(AfxIsValidAddress(ar,sizeof SDOMArchive));
	ASSERT(ar->IsLoading());
	
	m_owners.clear();
	ASSERT(m_owners.size() == 0);

	while (ar->HasNextElement()) {
		COLMOwner& owner = *m_owners.insert(m_owners.end());
		ar->SerializeOwner(&owner,writeid,factory,namer);
	}
}

COLMKeepers::~COLMKeepers() {
}

void COLMKeepers::InheritedFrom(COLMOwners& kfowners,int nStart,int nLen)
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMKeepers));
	ASSERT_VALID(&kfowners);

	OWNERS& owners = kfowners.m_owners;
	
	OWNERS::iterator i = kfowners.GetItr(nStart);
	ASSERT(i != owners.end());
	ASSERT(m_keepers.size() == 0);

	while (0 < (nLen--))
	{
		const COLMObject* p = i->GetObject();
		ASSERT_VALID(p);

		COLMKeeper& k = *(m_keepers.insert(m_keepers.end()));
		COLMOwner& o = *i;
		ASSERT_VALID(&o);

		k.InheritedFrom(&o);
		ASSERT_VALID(&k);
		
		OWNERS::iterator iDelete = i++;
		owners.erase(iDelete);
	}

	DEBUG_ONLY(kfowners.AssertValid());
}

void COLMKeepers::TransferTo(COLMOwners& kfowners,int nStart)
{
	ASSERT_VALID(&kfowners);
	ASSERT(AfxIsValidAddress(this,sizeof COLMKeepers));
	OWNERS& owners = kfowners.m_owners;

	OWNERS::iterator io = kfowners.GetItr(nStart);
	KEEPERS::iterator ik;
	for (ik =m_keepers.begin();ik != m_keepers.end();ik++)
	{
		COLMKeeper& k = *ik;
		COLMOwner& o = *owners.insert(io);

		k.TransferTo(&o);
	}
	m_keepers.clear();

	DEBUG_ONLY(kfowners.AssertValid());
}

void COLMKeepers::OwnNew(COLMObject* p)
{
	ASSERT(AfxIsValidAddress(this,sizeof COLMKeepers));
	ASSERT_VALID(p);
	COLMKeeper& k = *m_keepers.insert(m_keepers.end());
	k.OwnNew(p);
}

void COLMDelExKeepers::InheritedFrom(COLMOwners& os,const OLM_OWNER_DEL_EX_LIST& ls)
{
	ASSERT(m_dellist.size() == 0);
	m_dellist = ls;
	ASSERT(m_dellist.size() == ls.size());

	ASSERT(m_keepers.size() == 0);
	{
		int nDelPos = os.GetCount();
		OLM_OWNER_DEL_EX_LIST::const_reverse_iterator i;
		for (i = ls.rbegin(); i != ls.rend(); i++)
		{
			DEL_EX_KEEPER& k = *m_keepers.insert(m_keepers.end());
			k.first = os.FindPos(*i);
		}
	}
	ASSERT(m_keepers.size() == m_dellist.size());

	{
		DEL_EX_KEEPERS::iterator i;
		for (i = m_keepers.begin();i != m_keepers.end();i++)
		{
			DEL_EX_KEEPER& k = *i;
			k.second.InheritedFrom(os,k.first,1);
		}
	}
}

void COLMDelExKeepers::TransferTo(COLMOwners& os) {
	DEL_EX_KEEPERS::reverse_iterator i;
	for (i = m_keepers.rbegin();i != m_keepers.rend();i++)
	{
		DEL_EX_KEEPER& k = *i;
		k.second.TransferTo(os,k.first);
	}

	ASSERT(m_keepers.size() == m_dellist.size());
	m_keepers.clear();
	m_dellist.clear();
}


template void  _OLMConstruct<COLMOwner,COLMOwner>(COLMOwner*,const COLMOwner&);  
//template class TOLMAllocator<COLMOwner>;
template void  _OLMConstruct<COLMKeeper,COLMKeeper>(COLMKeeper*,const COLMKeeper&);  
//template class TOLMAllocator<COLMKeeper>;
