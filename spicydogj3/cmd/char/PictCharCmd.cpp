//////////////////////////////////////////////////////////////////////////
//	これは[SpicyDog]のソースコードです。
//	著作権 スパイシーソフト株式会社
//	このソースコードに関するすべての権利はスパイシーソフトが保有します。
//
//	This is a part of SpicyDog  2001.
//	Copyright (C) 2000 Spicysoft Corporation.
//	All rights reserved.
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PictCharCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


PictSetCenterCmd::PictSetCenterCmd(PictChar* p,CPoint ptNew) 
:	Cmd(IDCMD_PICT_SET_CENTER),
	m_pPictChar(p),
	m_ptCenterNew(ptNew) {
	ASSERT_VALID(m_pPictChar);
	m_ptCenterOld = m_pPictChar->GetCenter();
}

void PictSetCenterCmd::Do() {
	ASSERT_VALID(m_pPictChar);
	m_pPictChar->SetCenter(m_ptCenterNew);
	ASSERT_VALID(m_pPictChar);
}

void PictSetCenterCmd::Undo() {
	ASSERT_VALID(m_pPictChar);
	m_pPictChar->SetCenter(m_ptCenterOld);
	ASSERT_VALID(m_pPictChar);
}




PictSetOptParamCmd::PictSetOptParamCmd(PictChar* p,const PictQuantizeParam& param)
:	Cmd(IDCMD_PICT_SET_QUANTIZE_PARAM),
	m_pPictChar(p), 
	m_paramNew(param) {
	ASSERT_VALID(m_pPictChar);
	m_paramOld = p->GetPictOptParam();
}

void PictSetOptParamCmd::Do() {
	ASSERT_VALID(m_pPictChar);
	m_pPictChar->SetPictOptParam(m_paramNew);
	ASSERT_VALID(m_pPictChar);
}

void PictSetOptParamCmd::Undo() {
	ASSERT_VALID(m_pPictChar);
	m_pPictChar->SetPictOptParam(m_paramOld);
	ASSERT_VALID(m_pPictChar);
}



PictModDataCmd::PictModDataCmd(UINT resource,PictChar* p,const PictData& data) 
:	Cmd(resource),
	m_pPictChar(p),
	m_dataNew(data) {
	ASSERT_VALID(m_pPictChar);
	m_ptCenterOld = m_pPictChar->GetCenter();
	m_dataOld = m_pPictChar->GetPictData();
}

void PictModDataCmd::Do() {
	ASSERT_VALID(m_pPictChar);
	m_pPictChar->SetPictData(m_dataNew);
	ASSERT_VALID(m_pPictChar);
}

void PictModDataCmd::Undo() {
	ASSERT_VALID(m_pPictChar);
	m_pPictChar->SetPictData(m_dataOld);
	m_pPictChar->SetCenter(m_ptCenterOld);
	ASSERT_VALID(m_pPictChar);
}
