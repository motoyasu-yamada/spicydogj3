#pragma once

//	スイッチウインドウの高さ
extern const int VIEWSWITCHWND_HEIGHT;

class ViewSwitch;
#include <vector>



/**	ビュースイッチの表示管理クラス */
class ViewSwitchWnd : public CWnd {
private:
	/**	ビュースイッチ表示ウインドウの、タブ上の閉じるボタン */
	class VirtualButton {
	private:
		CWnd*	m_pParent;
		BOOL	m_bVisible;
		BOOL    m_bPushed;
		CRect   m_rcOrg;

	public:
		VirtualButton();
		void Move(CPoint);
		void Move(const CRect&);
		void GetRect(CRect&);
		BOOL HitTest(CPoint);
		void Show(BOOL);
		BOOL IsVisible() const;
		void Invalidate();

		void OnPaint(CDC* pDC);
		void OnLButtonDown(CPoint);
		BOOL Create(CWnd*);
	}; // class VirtualButton

	//	ビュータイプアイコン (0:ムービー)
	CImageList m_ImageList;
	//	ビューの管理情報
	ViewSwitch& m_switch;
	int m_nActiveView;
	CView* m_pActiveView;

	//	ビュータブの表示情報管理データ
	struct ViewTabInfo {
		CRect m_rc;
	};
	typedef std::vector<ViewTabInfo> VIEWTABS;
	VIEWTABS m_ViewTabs;
	//	閉じるボタン
	VirtualButton m_btnClose;
	//	タブのフォント
	CFont m_fontTab;

// インターフェース
public:
	//	コンストラクタ
	ViewSwitchWnd(ViewSwitch&);
	//	ウインドウ生成
	BOOL Create(CWnd*);
	//	ビュースイッチに登録されているビュー情報が変更された	
	void Update();
	//	レイアウトを変更する
	void RecalcLayout();

// インプリメンテーション
public:
	virtual ~ViewSwitchWnd();
private:
	//	レイアウトを再計算する
	void RecalcChildsLayout();
	//	ビュー切替タブを表示する
	void DrawViewTab(CDC*,BOOL bActivated,const CRect&,UINT nIcon,LPCTSTR ptszCaption);
	//	閉じるボタンのレイアウトを再計算する
	void RecalcCloseBtn();
	//	ビュー切替タブレイアウトを再計算する
	void RecalcViewTabLayout();
	//	閉じるボタンを生成する
	void CreateCloseButton();
	//	タブのヒットテスト
	int HitTabs(CPoint pt);
	
	void DrawBkgnd(CDC&);
	void DrawViewTabs(CDC&);

protected:
	//{{AFX_MSG(ViewSwitchWnd)
	afx_msg void OnCloseBtn();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class ViewSwitchWnd

//{{AFX_INSERT_LOCATION}}

