#pragma once

/** ColorBox ウィンドウ
	色(背景色や描画色等)を表示するSTATICエリア */
class ColorBox : public CStatic {
private:
	COLORREF m_crColor;
public:
	DECLARE_DYNAMIC(ColorBox);
	ColorBox();
	virtual ~ColorBox();
	BOOL Create(const CRect&,CWnd* pParent,UINT nID);
	COLORREF GetColor(void) const;
	void SetColor(COLORREF);
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC*);
	DECLARE_MESSAGE_MAP()
};

/**
	ツールバー用のカラー表示 & 選択ボックス
*/
#define WM_SELECT_COLOR	(WM_USER + 0x1000)

class ColorBoxEx : public CWnd {
private:
	COLORREF   m_cr;
	UINT       m_idIcon;
	COLORREF   m_crMask;
	CImageList m_image;
	CRect      m_rcBtn;
	CRect      m_rcColor;
	CRect      m_rcIcon;
	BOOL       m_bDown;
	CToolTipCtrl m_tooltip;
	mutable CString m_strTooltip;
public:
	DECLARE_DYNAMIC(ColorBoxEx);
	ColorBoxEx();
	virtual ~ColorBoxEx();
	/**  ウインドウを作成 */
	BOOL Create(CWnd* pParent,CRect rc,UINT nID,UINT nIconBmpId,COLORREF crMask);
	/**  色を取得 */
	COLORREF GetColor(void) const;
	/**  色を設定 */
	void SetColor(COLORREF);
	/**  ボタンを押す */
	void PressButton();
	/**  クラスを初期化 */
	static void clinit();
	/**  ON_UPDATE_COMMAND_UIハンドラ向けのヘルパ関数 */
	static void OnUpdateCmdUI(CCmdUI* pCmdUI,COLORREF crSet);
	/**  ON_COMMAND処理にて利用 -> Commandを投げたColorBoxExにアクセスして現在色を取得 */
	static COLORREF GetCurrentColor();
	/**  配置用のダミーStaticと同位置にColorBoxExを作成する*/
	static void DDX_Control(CDataExchange* pDX,int nIDC,ColorBoxEx& wnd,UINT nIconBmpId,COLORREF crMask);
	static void DDX_COLORREF(CDataExchange* pDX,int nIDC,COLORREF&);
protected:
	afx_msg BOOL OnNeedText(UINT id,NMHDR * pNMHDR,LRESULT * pResult);
	virtual BOOL PreTranslateMessage(MSG*p);
	afx_msg void OnLButtonDown(UINT,CPoint);
	afx_msg void OnKeyDown(UINT c,UINT r,UINT f);
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd*);
	afx_msg void OnKillFocus(CWnd*);
	afx_msg void OnEnable( BOOL bEnable );
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()
};

/**
	カラーピッカーのマウスが現在拾っている色を通知する
*/
class ColorPickerListener {
public:
	virtual void OnPickerMove(CPoint ptDesktop,COLORREF cr) = 0;
public:
	virtual ~ColorPickerListener() {};
};

/** 画面上から色をピックアップする */
class ColorPicker {
private:
	ColorPickerListener* m_pListener;
	HCURSOR m_hcurPicker;
	CWnd* m_pParent;
	COLORREF m_cr;
public:
	/**	コンストラクタ
		@param wnd   色をピックアップするウインドウ
		@paran curid カーソルリソースID
	*/
	ColorPicker(CWnd*,UINT curid);
	/**	色のピッキングを行う 色が選択されるまでモーダル */
	BOOL DoPick();
	/**	ピッキングした色を取得する */
	COLORREF GetColor() const;
	/**	リスナーを設定 */
 	void SetListener(ColorPickerListener*);
};

#define WM_CLRBOXEX_SELECT	WM_USER + 0x1001
const COLORREF NULLCOLOR = 0xFFFFFFFF;
const COLORREF NULLCOLOR_1 = RGB(0xFF,0xFF,0xFF);
const COLORREF NULLCOLOR_2 = RGB(0x80,0x80,0x80);
const int NULLCOLOR_PATTERN_SIZE = 6;
void FillNullRect (CDC*,LPCRECT);
void FillSolidRect(CDC*,LPCRECT,COLORREF);