#include "stdafx.h"
#include "SimpleAction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void SimpleAction::Serialize(SDOMArchive* ar) {
	ASSERT(ar);
}

void SimpleAction::AssertValid() const {
}

CRuntimeClass* SimpleAction::GetDialogClass() const {
	return NULL;
}
