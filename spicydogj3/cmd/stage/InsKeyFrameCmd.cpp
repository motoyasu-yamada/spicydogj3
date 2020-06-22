
#include "stdafx.h"
#include "Resource.h"
#include "InsKeyFrameCmd.h"
#include "model/stage/KeyFrame.h"
#include "model/stage/Layer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

InsKeyFrameCmd::InsKeyFrameCmd(
	Layer* layer,
	int pos,
	KeyFrame* key) :
	Cmd(IDCMD_INS_KEY_FRAME) {

	ASSERT_VALID(key);
	ASSERT_VALID(layer);
	m_layer = layer;
	m_pos   = pos;
	m_keepers.OwnNew(key);

	KeyFrameOwners& owners = layer->GetKeyFrameOwners();
	if (m_pos < owners.GetFrameCount()) {
		if (owners.Find(m_pos,KeyFrameOwners::FT_EQUAL) == NULL) {
			m_mode = MODE_INS;
		} else {
			m_mode = MODE_OVERINS;
		}
	} else {
		m_mode = MODE_EXPAND;
	}
	if (m_mode == MODE_EXPAND) {
		m_expand = m_pos - owners.GetFrameCount() + 1;
	} else {
		m_expand = -1;
	}
}

void InsKeyFrameCmd::Do() {
	KeyFrameOwners& owners = m_layer->GetKeyFrameOwners();
	if (m_mode == MODE_OVERINS) {
		owners.MoveFramesForward(m_pos,-1,1);
	} else if (m_mode == MODE_EXPAND) {
		owners.Expand(m_expand);
	}
	m_keepers.TransferTo(owners,m_pos,1);
}

void InsKeyFrameCmd::Undo() {
	KeyFrameOwners& owners = m_layer->GetKeyFrameOwners();
	m_keepers.InheritedFrom(owners,m_pos,1);
	if (m_mode == MODE_OVERINS) {
		owners.MoveFramesBackward(m_pos + 1,-1,1);
	} else if (m_mode == MODE_EXPAND) {
		owners.Shrink(m_expand);
	}
}
