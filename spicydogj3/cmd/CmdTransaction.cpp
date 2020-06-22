#include "STDAFX.H"
#include "CmdTransaction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CmdTransaction::CmdTransaction(UINT resource)
:	CmdSet(resource) {
	m_commited = false;
	m_firstDo  = true;

}

void CmdTransaction::AddCommand(Cmd* pcmd) {
	ASSERT(!m_commited);
	ASSERT(pcmd != NULL);

	pcmd->Do();
	CmdSet::AddCommand(pcmd);
}

void CmdTransaction::Commit() {
	m_commited = TRUE;
}

void CmdTransaction::Do() {
	ASSERT(m_commited);
	if (m_firstDo) {
		m_firstDo = FALSE;
	} else {
		CmdSet::Do();
	}
}

void CmdTransaction::Undo() {
	ASSERT(m_commited);
	CmdSet::Undo();
}

