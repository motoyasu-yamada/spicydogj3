#include "stdafx.h"
#include "resource.h"
#include "MainStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString MainStage::GetStageName() const {
	CString name;
	name.LoadString(IDS_MAIN_STAGE_NAME);
	return name;
}

CString MainStage::GetLabelName() const {
	return GetStageName();
}