#include "stdafx.h"
#include "resource.h"
#include "ModSndCharCmd.h"
#include "model/char/SndChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ModSndCharCmd::ModSndCharCmd(SndChar* shape,HANDLE h)
:	Cmd(IDCMD_MOD_SND_CHAR),
	m_shape(shape),
	m_new(h),
	m_old(shape->GetHandle()),
	m_todo(TRUE)
{
	ASSERT(m_new != NULL);
	ASSERT(m_old != NULL);
	ASSERT_VALID(m_shape);
}

void DestroyHandle(HANDLE h) {
	if (h != NULL) {
		VERIFY(GlobalFree(h) == NULL);
	}
}

ModSndCharCmd::~ModSndCharCmd() {
	if (m_todo) {
		DestroyHandle(m_new);
	} else {
		DestroyHandle(m_old);
	}
}

void ModSndCharCmd::Do() {
	ASSERT(m_todo);
	VERIFY(m_shape->ReplaceHandle(m_new) == m_old);
	m_todo = FALSE;
}

void ModSndCharCmd::Undo() {
	ASSERT(!m_todo);
	VERIFY(m_shape->ReplaceHandle(m_old) == m_new);
	m_todo = TRUE;
}