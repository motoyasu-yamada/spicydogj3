#pragma once

/**
	カスタムドキュメントテンプレート
	(標準と異なる点)
		- ドキュメントが正規に作成(新規、読込)後にフレームを作成
		- ドキュメントの新規作成時にアプリ設定ダイアログを開く
*/
class SpicydogDocTempl : public CMultiDocTemplate {
public:
	SpicydogDocTempl(UINT nId,CRuntimeClass* pDocC,CRuntimeClass* pFrameC,CRuntimeClass* pViewC);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName,BOOL bMakeVisible);
};

