#pragma once

/** 矩形操作 */
void MoveRectTo(CRect& rc,int x,int y);

/** ウインドウ移動 */
void MoveChildWindowToTop(CWnd& wnd,int nTop);
void MoveChildWindowToBottom(CWnd& wnd,int nBottom);
void MoveChildWindowToRight(CWnd& wnd,int nRight);
void MoveChildWindowToLeft(CWnd&,int);
void MoveChildWindowToLeftNext(CWnd& wnd,const CWnd& wndLeft);

/** ウインドウスリサイズ */
void StretchChildWindowToBottom(CWnd& wnd,int nBottom);
void StretchChildWindowToRight(CWnd& wnd,int nRight);
void StretchChildWindowWidth(CWnd& wnd,int nWidth);
void StretchChildWindowHeight(CWnd& wnd,int nHeight);

/**  ウインドウサイズ取得 */
int GetWindowHeight(const CWnd&);
int	GetWindowWidth(const CWnd&);
int GetChildWindowLeft(const CWnd&);
int GetChildWindowTop(const CWnd&);
int GetChildWindowRight(const CWnd&);
int GetChildWindowBottom(const CWnd&);

/**  親座標系での子ウインドウ位置 */
void GetParentClientRect(const CWnd& wnd,CRect& rc);

/**  ポップアップメニュー */
void TrackDlgBtnPopupMenu(CDialog* pParent,UINT nItemID,UINT nMenuID,int nSubMenu = 0);
void TrackNewPopupMenu(POINT,CWnd*,UINT nID,int nSubMenu = 0);
void TrackNewPopupMenu(CWnd*,UINT nID,int nSubMenu = 0);

/** 	ダイアログの子コントロールの、親ウインドウ座標系での矩形を求める */
void GetDlgItemRect(const CWnd* pParent,UINT nID,CRect& rc);
/** 	すべての子孫ウインドウを使用不可(淡色状態)/可に変更 */
void EnableDescendantWindows(CWnd* hWnd,BOOL b);
/** 	選択されている名前付け規則に応じてコントロールの状態(使用可/不可)を更新 */
void EnableDlgItem(CWnd* pParent,UINT nID,BOOL b);
/** 	ダイアログのリソース上のサイズ */
SIZE GetDlgTemplateSize(UINT nDlgResID);
/**	ウインドウの子孫にメッセージを送信 */
void SendMessageToDescendants(CWnd*,UINT,int = 0,WPARAM =0,LPARAM = 0,BOOL = FALSE);
void LoadWindowPlacement(CWnd* pWnd,LPCTSTR ptszSec,LPCTSTR ptszEntry);
void SaveWindowPlacement(CWnd* pWnd,LPCTSTR ptszSec,LPCTSTR ptszEntry);
CString WindowPlacementToString(const WINDOWPLACEMENT& w);
BOOL StringToWindowPlacement(LPCTSTR ptsz,WINDOWPLACEMENT& w);
void SetWindowPlacement(CWnd* pWnd,const WINDOWPLACEMENT& w);
void GetWindowPlacement(CWnd* pWnd,WINDOWPLACEMENT& w);
BOOL LoadWindowPlacementFromReg(LPCTSTR ptszSec,LPCTSTR ptszEntry,WINDOWPLACEMENT& w);
void SaveWindowPlacementToReg  (LPCTSTR ptszSec,LPCTSTR ptszEntry,const WINDOWPLACEMENT& w);

/// システム256色以上の(カスタムパレットおよびHiColor以上)のイメージリストを作成
BOOL CreateImageListColor32(CImageList& il,UINT nResID,int cx,int cy,COLORREF crMask);

/** 現在アクティブなビューを取得 */
CView* GetActiveView();
/** キーが続けて押されているかどうか */
BOOL GetKeyStillDown(int nKey);
/** アイコンつきのタブを描画する */
void DrawTab(CDC& dc,const CRect& rc,BOOL sel,LPCTSTR text,CImageList& imageList,
			 int iconIndex,int leftMargin);
/** クリックかドラッグしてようとしているかどうか判定 */
BOOL IsToStartDrag(CWnd* pWnd,CPoint ptDown,BOOL bLeftOrRight);
/** リストの現在選択されている列のインデックスを取得する */
int GetListCtrlCurSel(const CListCtrl& listCtrl);
/** ハンドル番号からハンドルの矩形を取得する */
void GetHandleRect(const CRect& rc,int nHandle,CRect& rcHandle,int nHandleSize,int nStyle);
/** \nで分割されたメッセージ文字列を切り出す*/
CString GetMessageString(UINT nID);