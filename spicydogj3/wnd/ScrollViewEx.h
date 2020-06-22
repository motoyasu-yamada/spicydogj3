#pragma once
/**
	1) You must define these string resources.
		ID_SCROLLVIEWEX_SCALE
		ID_SCROLLVIEWEX_MOUSE

	2) How to work with the tracker
		void OnLButtonDown(UINT,CPoint pt) {
			m_trackerRect.m_rect = m_rc;
			GetViewT()->LPtoDP(&dc,m_trackerRect.m_rect);

			if (m_trackerRect.Track(this,pt)) {
				m_rc = m_trackerRect.m_rect;
				dc.DPtoLP(m_rc);
				Invalidate();
			}
		}

		void OnDraw(CDC* pDC) {
			m_trackerRect.m_rect = m_rc;
			GetViewT()->LPtoDP(pDC,m_trackerRect.m_rect);
			m_trackerRect.Draw(pDC);
		}

		BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
			m_trackerRect.m_rect = m_rc;
			GetViewT()->LPtoDP(m_trackerRect.m_rect);
			return m_trackerRect.SetCursor(pWnd,nHitTest);
		}
*/
#include "utility/bezier.h"
#include "utility/Accel.h"

const UINT WM_SCROLLVIEWEX_BASE     = WM_USER + 0x1000;
const UINT WM_SCROLLVIEWEX_GETSCALE = WM_SCROLLVIEWEX_BASE + 0x0001;
const UINT WM_SCROLLVIEWEX_SETSCALE = WM_SCROLLVIEWEX_BASE + 0x0002;

/*
	CScrollViewの高機能バージョン
	- 正確なセンタリング表示
	- 高度なスクロール領域設定
	- グリッド表示支援
*/
class ScrollViewEx : public CView,public AccelOwner {
public:
	enum SetScaleType { setScale,setScaleDocumentToFitSize,setScaleTotalToFitSize,setResizeParentToFit};
	enum GetScaleType { getScale = setScale,
		                getScaleDocumentToFitSize = setScaleDocumentToFitSize,
						getScaleTotalToFitSize    = setScaleTotalToFitSize};
	/// スケーリングパラメータ構造体
	struct SETSCALEPARAM {
		///  スケールの設定方式
		int m_typeSetScale;
		/// 表示スケール 1=100%
		double m_scale;
	}; // struct SCROLLVIEWEX_SCALE
	/// スケーリングパラメータ構造体
	struct GETSCALEPARAM {
		///  現在のスケール
		int m_typeGetScale;
		/// 表示スケール 1=100%
		double m_scale;
	}; // struct SCROLLVIEWEX_SCALE
	
private:
	/// マッピングモード
	int m_nMapMode;
	/// 論理座標系-スクロール可能領域
	CSize m_totalLog;
	/// デバイス座標系-スクロール可能領域
	CSize m_totalDev;
	/// スクロールページサイズ-デバイス座標系
	CSize m_pageDev;
	/// スクロールラインサイズ-デバイス座標系
	CSize m_lineDev;
	/// センタリングするかどうか
	BOOL m_bCenter;
	/// 初期化されているかどうか
	BOOL m_bInsideUpdate;
	/// 表示倍率 1-等倍
	double m_scale;
	/// 論理ドキュメントサイズ
	CSize  m_documentLog;
	/// ドキュメントサイズとスクロール可能領域の比率
	int    m_nTotalByDocument;
	/// 論理ドキュメント座標系原点位置
	CPoint m_documentCenter;
	/// InitialUpdated済かどうか
	BOOL m_bInitialUpdated;
	// UpdateScrollSizesは再入不可
	BOOL m_bUpdateScrollSizes;

protected:
	DECLARE_DYNAMIC(ScrollViewEx)
	ScrollViewEx();

public:
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static void DPtoLP(CDC* pDC,LPPOINT,int = 1);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static void LPtoDP(CDC* pDC,LPPOINT,int = 1);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static void DPtoLP(CDC* pDC,LPSIZE);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static void LPtoDP(CDC* pDC,LPSIZE);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static void DPtoLP(CDC* pDC,LPRECT);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static void LPtoDP(CDC* pDC,LPRECT);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static HRGN LPtoDP(CDC* pDC,HRGN);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static HRGN DPtoLP(CDC* pDC,HRGN);
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	static void LPtoDP(CDC*,CBEZIER& bezier);

	/// 高精度なデバイス座標と論理座標の変換ルーチン
	void DPtoLP(LPPOINT,int = 1) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	void LPtoDP(LPPOINT,int = 1) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	void DPtoLP(LPSIZE) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	void LPtoDP(LPSIZE) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	void DPtoLP(LPRECT) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	void LPtoDP(LPRECT) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	HRGN DPtoLP(HRGN) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	HRGN LPtoDP(HRGN) const;
	/// 高精度なデバイス座標と論理座標の変換ルーチン
	void LPtoDP(CBEZIER& bezier);

	/// 表示倍率を変更する
	void SetScale(double dScale);
	/// スクロール矩形全体表示をウインドウ枠のサイズに統一する
	void SetScaleDocumentToFitSize();
	/// ドキュメント矩形全体表示をウインドウ枠のサイズに統一する
	void SetScaleTotalToFitSize();
	/// 表示に収まるようウインドウ枠のサイズを調整する
	void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	/// 現在の表示倍率
	double GetScale() const { return m_scale; }
	/// スクロール位置をセンターに戻す
	void ScrollToCenter();
	/// 指定したドキュメント系座標位置を画面中央にする
	void ScrollToCenter(CPoint ptLog);
	/// コントローラからウインドウメッセージ処理プロシージャのアクセスを許可するためのバイパス
	inline LRESULT Default() { return CWnd::Default(); }
	/// ドキュメント矩形(デバイス座標系)を取得
	void GetDocumentRect(LPRECT prc) const;
	/// スクローラブル矩形(デバイス座標系)を取得
	void GetScrollableRect(LPRECT prc) const;
	/// 点(デバイス座標)がドキュメント矩形内にあるかどうか
	BOOL PtInDocumentRect(CPoint devpt) const;
	/// 点(デバイス座標)がスクローラブル矩形内にあるかどうか
	BOOL PtInScrollableRect(CPoint devpt) const;
	/// 指定した論理位置にスクロール
	void ScrollToPosition(CPoint pt);
	/// スクロール位置 - 論理位置
	CPoint GetScrollPosition() const;
	/// 指定したデバイス位置にスクロール
	void ScrollToDevicePosition(POINT ptDev);
	/// スクロール位置 - デバイス位置
	CPoint GetDeviceScrollPosition() const;

protected:
	/// ドキュメント枠を描画する
	void DrawDocumentBorder(CDC*,CBrush*) const;
	/// ドキュメント矩形をクリップする
	void MaskDocumentRect(CDC* pDC,BOOL b) const;

	/// 初期化(OnInitialUpdate)されているかどうか
	inline BOOL IsInitialUpdated() const { return m_bInitialUpdated; }
	virtual void OnInitialUpdate();

	/// 表示をセンタリングする
	void Center(BOOL b);
	/**
		スクロールエリアを設定
		@param sizeDocument     有効な論理座標系サイズ(ドキュメントサイズ)を指定する
		@param nDocumentByTotal ドキュメントサイズに対するスクロール可能領域のサイズ
		@param ptCenter         論理座標系の原点位置
	*/
	void SetScrollSizes(SIZE sizeDocument,int nDocumentByTotal,POINT ptCenter);
	/// ウインドウ可視範囲内を塗りつぶす
	void FillBkgnd(CDC* pDC, CBrush* pBrush) const;
	/// スクロール可能領域を塗りつぶす
	void FillScrollabeRect(CDC* pDC,CBrush* pBrush) const;
	/// ドキュメント矩形を塗りつぶす
	void FillDocumentRect(CDC* pDC,CBrush* pBrush) const;

	/// 論理座標系での画面中央のスクロール位置
	CPoint GetCenterScrollPosition() const;
	/// デバイスコンテキスト調整
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	/// 描画ハンドラ - 継承クラスで必ず実装すること
	virtual void OnDraw(CDC* pDC) = 0;

protected:
	virtual ~ScrollViewEx();
	/// グリッド描画ヘルパー
	void DrawGridHelper(CDC*,CPen&,int);
	/// ドキュメントセンターグリッドを描画
	void DrawCenterLine(CDC*,CPen&);
	/// マウスクリックでビューにフォーカス
	virtual BOOL PreTranslateMessage(LPMSG lpmsg);
	/// センター点を描画する
	void DrawCenter(CDC*,CPoint);
protected:
	/// ターゲット内おさまる最大の等比倍率を求める
	static double CalcScale(CSize szSrc,CSize szDst);
	virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType = adjustBorder);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);
	void UpdateScrollSizes();
	void UpdateBars();
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb) const;
	void GetScrollBarSizes(CSize& sizeSb) const;
	void GetScrollBarState(CSize sizeClient, CSize& needSb,CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient) const;

	//{{AFX_MSG(ScrollViewEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnScale();
	afx_msg void OnUpdateScale(CCmdUI*);
	afx_msg void OnUpdateMouse(CCmdUI*);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class ScrollViewEx

/**
	デバイスコンテキストの座標系を
	デバイス座標系に一時的に戻す
*/
class CSetDCtoDP {
private:
	CDC* m_pDC;
	int m_nSavedDC;
public:
	CSetDCtoDP(CDC* pDC) : m_pDC(pDC) {
		ASSERT_VALID(m_pDC);
		m_nSavedDC = m_pDC->SaveDC();

		m_pDC->SetMapMode(MM_TEXT);
		m_pDC->SetViewportOrg(0, 0);
		m_pDC->SetWindowOrg(0, 0);
	}
	~CSetDCtoDP() {
		ASSERT_VALID(m_pDC);
		VERIFY(m_pDC->RestoreDC(m_nSavedDC));
	}
}; // class CSetDCtoDP