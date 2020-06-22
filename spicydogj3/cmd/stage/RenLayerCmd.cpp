#include "stdafx.h"
#include "resource.h"
#include "RenLayerCmd.h"
#include "model/stage/Layer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RenLayerCmd::RenLayerCmd(Layer* layer,LPCTSTR newName) :
	Cmd(IDCMD_MOV_LAYER) {
	m_layer = layer;
	m_new   = newName;
	m_old   = layer->GetName();
	ASSERT_VALID(m_layer);
}

void RenLayerCmd::Do() {
	ASSERT_VALID(m_layer);
	m_layer->SetName(m_new);
	ASSERT_VALID(m_layer);
}

void RenLayerCmd::Undo() {
	ASSERT_VALID(m_layer);
	m_layer->SetName(m_old);
	ASSERT_VALID(m_layer);
}
