#include "stdafx.h"
#include "resource.h"
#include "DelLayerCmd.h"

#include "model/stage/Layer.h"
#include "model/stage/Stage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DelLayerCmd::DelLayerCmd(Stage* stage,int nRemove)
:	Cmd(IDCMD_DEL_LAYER) {
	m_stage   = stage;
	m_remove = nRemove;
	ASSERT_VALID(m_stage);
}

void DelLayerCmd::Do() {
	m_stage->UnselectLayer();

	COLMOwners& owners = m_stage->GetOwners();
	m_keepers.InheritedFrom(owners,m_remove,1);

	int n = m_remove;
	if ((owners.GetCount() - 1)<= m_remove) {
		n = owners.GetCount() - 1;
	}
	m_stage->SelectLayer(n);
}

void DelLayerCmd::Undo() {
	COLMOwners& owners = m_stage->GetOwners();
	m_keepers.TransferTo(owners,m_remove);
	m_stage->UnselectLayer();
}
