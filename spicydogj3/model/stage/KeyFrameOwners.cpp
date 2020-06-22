#include "STDAFX.H"
#include "KeyFrameOwners.H"

#include "KeyFrame.h"
#include "model/build/Builder.h"
#include "sdom/sdomn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

KeyFrameOwners::KeyFrameOwners() {
	m_frameCount = 0;
}

const KeyFrame* KeyFrameOwners::Find(int nPos,FIND_KEYRAME_TYPE ft) const {
	KeyFrameOwners* pTHIS = const_cast<KeyFrameOwners*>(this);
	OWNERS::iterator i = pTHIS->FindItr(nPos,ft);
	if (i != pTHIS->m_owners.end()) {
		return *i;
	} else {
		return NULL;	
	}
}

KeyFrameOwners::OWNERS::iterator KeyFrameOwners::FindItr(int nFindPos,FIND_KEYRAME_TYPE ft) {
	OWNERS::iterator i  = m_owners.begin();
	OWNERS::iterator ib = m_owners.end();

	while (i != m_owners.end())
	{
		const KeyFrame* p = *i;
		ASSERT_VALID(p);
		
		int nPos = p->GetFramePos();
		switch (ft)
		{
		case FT_EQUAL:
			if (nFindPos == nPos)
				return i;
			else if (nFindPos < nPos)
				return m_owners.end();
			break;
		case FT_END:
			if (nFindPos < nPos)
				return i;
			break;
		case FT_BEGIN:
			if (nFindPos < nPos)
				return ib;
			break;
		default:
			ASSERT(FALSE);
		}
		ib = i++;
	}
	if (ft == FT_BEGIN)
		return ib;

	return m_owners.end();
}

KeyFrameOwners::ConstItr KeyFrameOwners::Begin() const
{
	return m_owners.begin();
}

KeyFrameOwners::ConstItr KeyFrameOwners::End() const
{
	return m_owners.end();
}

BOOL KeyFrameOwners::IsValidRange(int start,int len) const
{
	return (IsValidPos(start)) && 
		   (0 <= len) &&
		   (0 <= start && (start + len) <= GetFrameCount());
}

BOOL KeyFrameOwners::IsValidPos(int start) const
{
	return 0 <= start && start < GetFrameCount();
}

int KeyFrameOwners::ValidateLen(int start,int len) const
{
	ASSERT(0 <= start && start < GetFrameCount());

	if (len == -1)
		len = GetFrameCount() - start;
	
	ASSERT(0 < len);
	return len;
}

int KeyFrameOwners::GetFrameCount() const
{
	return m_frameCount;
}

void KeyFrameOwners::Expand(int nExpand)
{
	ASSERT(0 < nExpand);
	
	m_frameCount += nExpand;

	DEBUG_ONLY(AssertValid());
}

void KeyFrameOwners::Shrink(int nShrink)
{
	ASSERT(0 < nShrink);

	m_frameCount -= nShrink;

	DEBUG_ONLY(AssertValid());
}

void KeyFrameOwners::MoveFramesForward(int start,int len,int offset)
{
	Expand(offset);

	if (start == m_frameCount)
	{
		ASSERT(offset == 1);
		ASSERT(len == 1 || len == -1);
	}
	else
	{
		len = ValidateLen(start,len);
		ASSERT(IsValidRange(start,len));
		ASSERT(0 < offset);

		MoveFrames(start,len,offset);
	}

	DEBUG_ONLY(AssertValid());
}

void KeyFrameOwners::MoveFramesBackward(int start,int len,int offset) {
	if (start == m_frameCount) {
		ASSERT(offset == 1);
		ASSERT(len == 1 || len == -1);
	} else {
		len = ValidateLen(start,len);
		ASSERT(IsValidRange(start,len));
		ASSERT(0 < offset && offset <= start);
		MoveFrames(start,len,-offset);
	}
	Shrink(offset);

	DEBUG_ONLY(AssertValid());
}

void KeyFrameOwners::MoveFrames(int start,int len,int offset) {
	ASSERT(offset != 0);
	int end = start + len -1;
	
	OWNERS::iterator i = m_owners.begin();
	while (i != m_owners.end()) {
		KeyFrame* p = *i++;
		int pos = p->GetFramePos();
		if (start <= pos && pos <= end) {
			p->MoveFramePos(offset);
		}
	}
}
	
SDOMSerializable* KeyFrameOwners::factory(LPCWSTR name) {
	ASSERT(AfxIsValidString(name) && wcscmp(WSDOMN_KEY_FRAME,name) == 0);
	return new KeyFrame;
}

LPCWSTR KeyFrameOwners::namer(SDOMSerializable* object) {
	ASSERT(AfxIsValidAddress(object,sizeof SDOMSerializable));
	return WSDOMN_KEY_FRAME;
}

void KeyFrameOwners::Serialize(SDOMArchive* ar) {
	ar->SerializeINT(WSDOMN_FRAME_LENGTH,m_frameCount);

	if (ar->IsLoading()) {
		while (ar->HasNextElement()) {
			TOLMOwner<KeyFrame>& o = *m_owners.insert(m_owners.end());
			ar->SerializeOwner(&o,true,factory,namer);
		}

	} else {
		OWNERS::iterator i = m_owners.begin();
		while (i != m_owners.end()) {
			TOLMOwner<KeyFrame>& o = *i++;
			ar->SerializeOwner(&o,true,factory,namer); // WSDOMN_KEY_FRAME
		}
	}
}

void KeyFrameOwners::AssertValid() const {
	int nPrevPos = -1;
	OWNERS::const_iterator i;
	for (i= m_owners.begin();i != m_owners.end();i++) {
		const KeyFrame* p = *i;
		ASSERT_VALID(p);

		int nPos = p->GetFramePos();
		ASSERT(IsValidPos(nPos));
		ASSERT(nPrevPos < nPos);
		nPrevPos = nPos;
	}
}

