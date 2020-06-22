#include "stdafx.h"
#include "KeyFrameKeepers.h"

#include "KeyFrame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void KeyFrameKeepers::InheritedFrom(KeyFrameOwners& kfowners,int start,int len) {
	int end = start + len -1;
	OWNERS& owners = kfowners.m_owners;
	OWNERS::iterator i = owners.begin();
	while (i != owners.end()) {
		const KeyFrame* keyFrame = *i;
		int pos = keyFrame->GetFramePos();
		if (start <= pos && pos <= end) {
			KEEPER& k = *(m_keepers.insert(m_keepers.end()));
			OWNER& o = *i;
			k.InheritedFrom(&o);
			OWNERS::iterator iDelete = i++;
			owners.erase(iDelete);
		} else {
			i++;
		}
	}

	DEBUG_ONLY(kfowners.AssertValid());
}

void KeyFrameKeepers::TransferTo(KeyFrameOwners& kfowners,int start,int len) {
	int end = start + len -1;
	OWNERS& owners = kfowners.m_owners;
	OWNERS::iterator io = kfowners.FindItr(start,KeyFrameOwners::FT_END);
	KEEPERS::iterator ik;
	for (ik =m_keepers.begin();ik != m_keepers.end();ik++) {
		TOLMKeeper<KeyFrame>& k = *ik;
		TOLMOwner<KeyFrame>& o = *owners.insert(io);

		k.TransferTo(&o);
	}
	m_keepers.clear();

	DEBUG_ONLY(kfowners.AssertValid());
}

void KeyFrameKeepers::OwnNew(KeyFrame* keyFrame) {
	ASSERT_VALID(keyFrame);
	TOLMKeeper<KeyFrame>& k = *m_keepers.insert(m_keepers.end());
	k.OwnNew(keyFrame);
}
