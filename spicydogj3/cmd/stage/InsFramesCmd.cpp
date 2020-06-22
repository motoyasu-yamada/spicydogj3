#include "stdafx.h"
#include "Resource.h"
#include "InsFramesCmd.h"
#include "model/stage/Layer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

InsFramesCmd::InsFramesCmd(
	Layer* layer,
	int nInsPos,
	int nFrames)
	: Cmd(IDCMD_INS_FRAMES) {
	m_layer = layer;
	ASSERT_VALID(m_layer);
	KeyFrameOwners& owners = m_layer->GetKeyFrameOwners();
	if (nInsPos < owners.GetFrameCount()) {
		m_expand = FALSE;
		m_params.m_insparams.m_insPos = nInsPos;
		m_params.m_insparams.m_frames = nFrames;

		ASSERT(owners.IsValidPos(m_params.m_insparams.m_insPos));
		ASSERT(0 < m_params.m_insparams.m_frames);
	} else {
		m_expand = TRUE;
		m_params.m_expand = (nInsPos + nFrames) - owners.GetFrameCount();
		ASSERT(0 < m_params.m_expand);
	}
}

void InsFramesCmd::Do() {
	ASSERT_VALID(m_layer);
	KeyFrameOwners& owners = m_layer->GetKeyFrameOwners();
	if (!m_expand) {
		owners.MoveFramesForward(m_params.m_insparams.m_insPos,
								   -1,
								   m_params.m_insparams.m_frames);
	} else {
		owners.Expand(m_params.m_expand);
	}
}

void InsFramesCmd::Undo() {
	ASSERT_VALID(m_layer);
	KeyFrameOwners& owners = m_layer->GetKeyFrameOwners();
	if (!m_expand) {
		owners.MoveFramesBackward(m_params.m_insparams.m_insPos+m_params.m_insparams.m_frames,
									-1,
									m_params.m_insparams.m_frames);
	} else {
		owners.Shrink(m_params.m_expand);
	}
}

