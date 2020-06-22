#pragma once

#include "sdom/olm.h"

class KeyFrame;

/** キーフレーム所有者の集合 */
class KeyFrameOwners : public SDOMSerializable {
	friend class KeyFrameKeepers;
public:
	enum FIND_KEYRAME_TYPE { FT_EQUAL,FT_BEGIN,FT_END};
	typedef TOLMOwner<KeyFrame> OWNER;
	typedef TOLMAllocator<OWNER> ALLOCATOR;
	typedef std::list<OWNER,ALLOCATOR> OWNERS;
	typedef OWNERS::const_iterator ConstItr;

	KeyFrameOwners();
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;

	int GetFrameCount() const;
	void Expand(int);
	void Shrink(int);

	void MoveFramesForward(int nStart,int nLen,int nOffset);	
	void MoveFramesBackward(int nStart,int nLen,int nOffset);
	const KeyFrame* Find(int nPos,FIND_KEYRAME_TYPE ft) const;
	
	OWNERS::const_iterator Begin() const;
	OWNERS::const_iterator End() const;

	BOOL IsValidRange(int nStart,int nLen) const;
	BOOL IsValidPos(int nStart) const;

private:
	static SDOMSerializable* factory(LPCWSTR name);
	static LPCWSTR namer(SDOMSerializable* object);

	int    m_frameCount;
	OWNERS m_owners;
	int ValidateLen(int nStart,int nLen) const;
	OWNERS::iterator FindItr(int nPos,FIND_KEYRAME_TYPE ft);
	void MoveFrames(int nStart,int nLen,int nOffset);
};
