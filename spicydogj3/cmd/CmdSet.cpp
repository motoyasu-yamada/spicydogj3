#include "STDAFX.H"
#include "CmdSet.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CmdSet::CmdSet(UINT resource) 
:	Cmd(resource) 
{
	m_canAdd = true;
}

void CmdSet::AddCommand(Cmd* p) {
	ASSERT(AfxIsValidAddress(p,sizeof Cmd));
	ASSERT(m_canAdd);

	m_commands.push_back(CMDPTR(p));
}

void CmdSet::Do() {
	ASSERT(0 < m_commands.size());
	m_canAdd = false;

	CMDLIST::iterator i;
	for (i = m_commands.begin();i != m_commands.end();i++) {
		(*i)->Do();
	}
}

void CmdSet::Undo() {
	ASSERT(0 < m_commands.size());
	m_canAdd = false;

	CMDLIST::reverse_iterator i;
	for( i = m_commands.rbegin();i != m_commands.rend();i++) {
		(*i)->Undo();
	}
}

int CmdSet::GetCount() const {
	ASSERT(!m_canAdd);
	ASSERT(0 < m_commands.size());

	return m_commands.size(); 
}
