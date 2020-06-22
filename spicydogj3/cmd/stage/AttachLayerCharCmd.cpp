#include "stdafx.h"
#include "resource.h"
#include "AttachLayerCharCmd.h"
#include "model/stage/Layer.h"
#include "model/char/Char.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AttachLayerCharCmd::AttachLayerCharCmd(Layer* layer,Char* chr)
:	Cmd(IDCMD_ATTACH_LAYER_CHAR),
	m_layer(layer),
	m_new(chr),
	m_old(layer->GetAttachedChar()) {
	DEBUG_ONLY(AssertValid());
}

void AttachLayerCharCmd::Do() {
	DEBUG_ONLY(AssertValid());
	m_layer->AttachChar(m_new);
	DEBUG_ONLY(AssertValid());
}

void AttachLayerCharCmd::Undo() {
	DEBUG_ONLY(AssertValid());
	m_layer->AttachChar(m_new);
	DEBUG_ONLY(AssertValid());
}

void AttachLayerCharCmd::AssertValid() const {
	ASSERT_VALID(m_layer);
	if (m_old != NULL) {
		ASSERT_VALID(m_old);
	}
	ASSERT_VALID(m_new);
}
