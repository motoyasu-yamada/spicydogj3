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
#include "DocumentEvent.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
	ドキュメント変更通知クラス
*/

//	シングルトンオブジェクト
std::auto_ptr<DocumentNotifier> DocumentNotifier::s_pNotifier;

//	コンストラクタ
DocumentNotifier::DocumentNotifier()
:	m_pDocument(NULL)
{
}

//	シングルトンノティファイアーを取得
DocumentNotifier* DocumentNotifier::GetNotifier()
{
	if (s_pNotifier.get() == NULL)
	{
		s_pNotifier = std::auto_ptr<DocumentNotifier>(new DocumentNotifier);
	}

	return s_pNotifier.get();
}

void DocumentNotifier::AddListener(DocumentListener* p) {
	ASSERT(AfxIsValidAddress(p,sizeof DocumentListener));

	DOCUMENTLISTENERS::iterator i = std::find(m_Listeners.begin(),m_Listeners.end(),p);
	ASSERT(i == m_Listeners.end());
	
	m_Listeners.push_back(p);
}

void DocumentNotifier::RemoveListener(DocumentListener* p) {
	ASSERT(AfxIsValidAddress(p,sizeof DocumentListener));

	DOCUMENTLISTENERS::iterator i = std::find(m_Listeners.begin(),m_Listeners.end(),p);
	ASSERT(i != m_Listeners.end());

	m_Listeners.erase(i);
}

void DocumentNotifier::ForceActivateDocument(CDocument* pDoc) {
	m_pDocument = pDoc;
	if (m_pDocument != NULL) {
		ASSERT_VALID(m_pDocument);
	}
	
	DOCUMENTLISTENERS::iterator i = m_Listeners.begin();
	for (;i != m_Listeners.end();i++) {
		DocumentListener*& pListener = *i;
		ASSERT_POINTER(pListener,DocumentListener);
		if (pListener != NULL) {
			pListener->OnDocumentActivated(pDoc);
		}
	}
	UpdateDocument(m_pDocument);
}

void DocumentNotifier::ActivateDocument(CDocument* pDoc) {
	if (m_pDocument == pDoc) {
		TRACE("you activate same document,but i do not reactivate.");
		return;
	}
	ASSERT_VALID(pDoc);
	ForceActivateDocument(pDoc);
}

void DocumentNotifier::UpdateDocument(CDocument* pDoc,LPARAM lHint,CObject* pHint) {
	ASSERT(m_pDocument == pDoc);
	if (m_pDocument == NULL) {
		return;
	}
	ASSERT_VALID(m_pDocument);

	DOCUMENTLISTENERS::iterator i = m_Listeners.begin();
	for (;i != m_Listeners.end();i++)
	{
		DocumentListener*& pListener = *i;
		ASSERT_POINTER(pListener,DocumentListener);
		if (pListener != NULL)
			pListener->OnDocumentUpdated(pDoc,lHint,pHint);
	}
}

void DocumentNotifier::Notify(DWORD dwNotifyCode,CDocument* pDoc,LPARAM lParam,LPVOID pv) {
	ASSERT(m_pDocument == pDoc);
	if (m_pDocument == NULL) {
		return;
	}
	ASSERT_VALID(m_pDocument);

	DOCUMENTLISTENERS::iterator i = m_Listeners.begin();
	for (;i != m_Listeners.end();i++)
	{
		DocumentListener*& pListener = *i;
		ASSERT_POINTER(pListener,DocumentListener);
		if (pListener != NULL)
			pListener->OnNotify(dwNotifyCode,pDoc,lParam,pv);
	}
}

CDocument* DocumentNotifier::GetActiveDocument() const {
	if (m_pDocument == NULL) {
		return NULL;
	}
	ASSERT_VALID(m_pDocument);
	return m_pDocument;
}

void DocumentNotifier::NotifyDocumentExpired(CDocument* pDocument) {
	if (GetActiveDocument() == pDocument) {
		ForceActivateDocument(NULL);
	}
}