#pragma once

#include <list>
#include <memory>

/**
	ドキュメント変更リスナー
*/
class DocumentListener
{
public:
	virtual void OnDocumentActivated(CDocument*) {}
	virtual void OnDocumentUpdated(CDocument*,LPARAM,CObject*) {}
	virtual void OnNotify(DWORD dwNotifyCode,CDocument*,LPARAM,LPVOID pv) {}
	virtual ~DocumentListener() {}
};

/**
	ドキュメント変更通知クラス
*/
class DocumentNotifier
{
private:
	typedef std::list<DocumentListener*> DOCUMENTLISTENERS;
//	データメンバ
private:
	//	シングルトンオブジェクト
	static std::auto_ptr<DocumentNotifier> s_pNotifier;
	//	アクティブドキュメント
	CDocument*	m_pDocument;
	//	リスナ
	DOCUMENTLISTENERS m_Listeners;

//	インターフェース
protected:
	//	コンストラクタ
	DocumentNotifier();
public:
	/**
		ドキュメントが消滅したことを通知
		OnCloseDocumentとデストラクタから呼び出すことが可能
		@param pDocument 消滅しようとしているドキュメント
	*/
	void NotifyDocumentExpired(CDocument* pDocument);
	//	シングルトンノティファイアーを取得
	static DocumentNotifier* GetNotifier();
	//	現在アクティブなドキュメントを取得
	CDocument* GetActiveDocument() const;
	//	ドキュメントイベントリスナを登録
	void AddListener(DocumentListener*);
	//	ドキュメントイベントリスナを削除
	void RemoveListener(DocumentListener*);
	//	ドキュメントをアクティブになったことを通知
	void ActivateDocument(CDocument*);
	//	ドキュメントが更新されたことを通知
	void UpdateDocument(CDocument*,LPARAM = 0,CObject* = NULL);
	//	ドキュメントイベントを通知する
	void Notify(DWORD dwNotifyCode,CDocument*,LPARAM = 0,LPVOID pv = NULL);
	void ForceActivateDocument(CDocument*);
};

