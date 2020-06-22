#include "stdafx.h"
#include "Cnt.h"
#include "CntMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Cnt::~Cnt() {
}

void Cnt::OnDraw(CDC* pDC) {
}

CntMgr* Cnt::GetCntMgr() {
	ASSERT(m_pMgr != NULL);
	return m_pMgr;
}

const CntMgr* Cnt::GetCntMgr() const {
	ASSERT(m_pMgr != NULL);
	return m_pMgr;
}

void Cnt::AttachControlerMgr(CntMgr* pMgr) {
	ASSERT(pMgr != NULL);
	m_pMgr = pMgr;
}

void Cnt::RejectEnter() {
	GetCntMgr()->ActivateDefaultControler();
}

void Cnt::OnEnter() {
}

void Cnt::OnExit() {
}

void Cnt::OnLButtonDblClk(UINT,CPoint){
	ContinueMsgRouting(TRUE);
}

void Cnt::OnLButtonDown(UINT,CPoint) {
	ContinueMsgRouting(TRUE);
}

void Cnt::OnLButtonUp(UINT,CPoint) {
	ContinueMsgRouting(TRUE);
}

void Cnt::OnRButtonDblClk(UINT,CPoint) {
	ContinueMsgRouting(TRUE);
}

void Cnt::OnRButtonDown(UINT,CPoint) {
	ContinueMsgRouting(TRUE);
}

void Cnt::OnRButtonUp(UINT,CPoint) {
	ContinueMsgRouting(TRUE);
}

void Cnt::OnMouseMove(UINT,CPoint) {
	ContinueMsgRouting(TRUE);
}

void Cnt::OnKeyDown(UINT,UINT,UINT) {
	ContinueMsgRouting(TRUE);
}
void Cnt::OnKeyUp(UINT,UINT,UINT) {
	ContinueMsgRouting(TRUE);
}

BOOL Cnt::OnSetCursor(CWnd*,UINT,UINT) {
	ContinueMsgRouting(TRUE);
	return FALSE;
}

void Cnt::ContinueMsgRouting(BOOL b) {
	m_bContinueMsgRouting = b;
}

BOOL Cnt::IsContinueMsgRouting() const {
	return m_bContinueMsgRouting;
}


	