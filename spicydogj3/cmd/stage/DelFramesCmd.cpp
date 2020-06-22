#include "stdafx.h"
#include "resource.h"
#include "DelFramesCmd.h"
#include "model/stage/Layer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DelFramesCmd::DelFramesCmd(
	Layer* layer,
	int nDelPos,
	int nFrames)
	: Cmd(IDCMD_DEL_FRAMES) {

	m_nDelPos = nDelPos;
	m_nFrames = nFrames;
	m_layer   = layer;
	ASSERT(0 <= m_nDelPos || m_nDelPos < layer->GetFrameCount());
	ASSERT(0 < m_nFrames);
}

void DelFramesCmd::Do() {
	KeyFrameOwners& owners = m_layer->GetKeyFrameOwners();
	m_keepers.InheritedFrom(owners,m_nDelPos,m_nFrames);
	owners.MoveFramesBackward(m_nDelPos + m_nFrames,-1,m_nFrames);
}

void DelFramesCmd::Undo() {
	KeyFrameOwners& owners = m_layer->GetKeyFrameOwners();
	owners.MoveFramesForward(m_nDelPos,-1,m_nFrames);
	m_keepers.TransferTo(owners,m_nDelPos,m_nFrames);
}
