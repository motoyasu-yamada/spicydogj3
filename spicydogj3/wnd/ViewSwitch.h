#pragma once

#include "utility/EventPublisher.H"

class ViewSwitchInfo {
public:
	CView*  m_pView;
	BOOL    m_bClosable;
	UINT    m_nIcon;
	CString m_strDefaultCaption;

public:
	//	コンストラクタ
	ViewSwitchInfo();
	ViewSwitchInfo(CView*,LPCTSTR ptszName,BOOL,UINT);
	//	コピーコンストラクタ
	ViewSwitchInfo(const ViewSwitchInfo&);
	//	代入演算子
	ViewSwitchInfo& operator = (const ViewSwitchInfo&);

	//	ビューのポインタを取得
	CView* GetView() const;
	//	ビューのキャプションの設定
	CString GetCaption() const;
	//	閉じることが可能なビューかどうか
	BOOL IsClosable() const;
	//	ビューが閉じられるかどうか設定
	void SetClosable(BOOL);
	//	ビューのアイコン情報を取得
	UINT GetIcon() const;

//	インプリメント
private:
	void CopyFrom(const ViewSwitchInfo&);
};

//	ビュースイッチのデータ構造変更リスナー
class ViewSwitchListener
{
//	インターフェース
public:
	virtual void OnRemoveView(int n,CView*) {}
	virtual void OnActiveViewChanged(int nNew,int nOld) {}
//	インプリメント
public:
	//	仮想デストラクタ
	virtual ~ViewSwitchListener() {}
};

//	ビュースイッチのデータ構造管理
class ViewSwitch : public CEventPublisher<ViewSwitch,ViewSwitchListener>
{
private:
	typedef std::list<ViewSwitchInfo> VIEWS;
	VIEWS m_Views;
	int   m_nActive;
	
	int m_nRemoved;
	CView* m_pRemovedView;
	int m_nNewActive;
	int m_nOldActive;

public:
	//	コンストラクタ
	ViewSwitch();
	//	スイッチに登録されているビュー数
	int GetViewCount() const;
	//	スイッチに登録されているビューの情報を取得する
	void GetViewInfo(int,ViewSwitchInfo&) const;
	//	スイッチに登録されているビューの情報を変更する
	void SetViewInfo(int,const ViewSwitchInfo&);
	//	カレントビューの変更
	void ActivateView(int);
	//	カレントビューの取得
	int GetActiveView() const;
	//	ビューの新規追加
	int AppendView(CView*,LPCTSTR,BOOL,UINT);
	//	ビューの削除
	void RemoveView(int);

private:
	//	イベント発行
	void FireRemoveView(int,CView*);
	void EnumRemoveView(ViewSwitch*,ViewSwitchListener*);
	void FireActiveViewChanged(int,int);
	void EnumActiveViewChanged(ViewSwitch*,ViewSwitchListener*);
};
