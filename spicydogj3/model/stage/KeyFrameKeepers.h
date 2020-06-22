#pragma once

#include "sdom/olm.h"
#include "KeyFrameOwners.h"

class KeyFrarme;

/** UNDO��`REDO�O�ɃL�[�t���[���̏��L�����Ǘ����� */
class KeyFrameKeepers {
	friend class KeyFrameOwners;	
public:
	typedef KeyFrameOwners::OWNERS OWNERS;
	typedef KeyFrameOwners::OWNER  OWNER;
	typedef TOLMKeeper<KeyFrame> KEEPER;
	typedef TOLMAllocator<KEEPER> ALLOCATOR;
	typedef std::list<KEEPER,ALLOCATOR> KEEPERS;

	virtual void OwnNew(KeyFrame*);
	virtual void InheritedFrom(KeyFrameOwners&,int,int);
	virtual void TransferTo(KeyFrameOwners&,int,int);

private:
	KEEPERS m_keepers;
};
