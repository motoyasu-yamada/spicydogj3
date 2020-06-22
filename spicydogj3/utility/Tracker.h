#pragma once

#include "utility/bezier.h"

const CSize DEFAULT_TRACKER_HANDLE_SIZE(6,6);
const CSize DEFAULT_TRACKER_CENTER_SIZE(10,10);

class TrackerMsgPump;

/**
	シフト状態管理を行うクラス
*/
class ShiftRectTracker : public CRectTracker {
protected:
	typedef void (*ADJUSTFUNC)(int handle,LPRECT cur,LPCRECT org);
private:
	ADJUSTFUNC m_funcAdjust;
	RECT m_rcOrg;
	UINT m_nMask;
	CSize m_szTrackBorder;
protected:
	ShiftRectTracker(ADJUSTFUNC,UINT nMask);
public:
	void SetTrackerBorder(CSize);
protected:
	BOOL Track(CWnd* pWnd,CPoint point, BOOL bAllowInvert = FALSE);
private:
	virtual void DrawTrackerRect(LPCRECT lpRect,CWnd* pWndClipTo,CDC* pDC,CWnd* pWnd);
	virtual BOOL IsShifted() const;
	virtual void AdjustRect(int nHandle,LPRECT lpRect);
	virtual UINT GetHandleMask() const;
};

/**
	移動専用トラッカー
	- シフトステートでは角度45度単位でしか移動できない
*/
class MoveTracker : public ShiftRectTracker {
public:
	MoveTracker();
	BOOL TrackMove(CWnd*,CPoint);
};

/**
	通常リサイズ専用トラッカー
	- シフトステートでは同一比率でしかリサイズできない
*/
class ResizeTracker : public ShiftRectTracker {
public:
	ResizeTracker();
	BOOL TrackResize(CWnd*,CPoint);
};

/**
	新規矩形作成 ラバーバンドトラッカー
	- シフトステートでは正方形限定のトラッキングになる
*/
class CreateRectTracker : public ShiftRectTracker {
public:
	CreateRectTracker();
	BOOL TrackCreate(CWnd*,CPoint);
};

/**
	新規楕円作成トラッカー
	- シフトステートでは正円限定のトラッキングになる
*/
class CreateCircleTracker: public ShiftRectTracker {
public:
	CreateCircleTracker();
	BOOL TrackCreate(CWnd*,CPoint);
private:
	virtual void DrawTrackerRect(LPCRECT lpRect,CWnd* pWndClipTo,CDC* pDC,CWnd* pWnd);
};

/** 
	リージョン移動トラッカー
*/
class MoveRgnTracker {
private:
	CSize m_szOffset;
public:
	MoveRgnTracker();
	CPoint GetMoveOffset() const;
	BOOL TrackMove(CRgn&,CWnd*,CPoint);
private:
	void DrawTrackerRgn (CRgn*,CDC*) const;
};

/** 
	ベジェ曲線移動トラッカー
*/
class MoveBezierTracker {
private:
	CSize m_szOffset;
	CBEZIER* m_pBezier;
public:
	MoveBezierTracker();
	void SetTracker(CBEZIER*);
	CPoint GetMoveOffset() const;
	BOOL TrackMove(CWnd*,CPoint);
private:
	struct MoveBezierTrackerParam {
		CPoint m_ptOrg;
	};
public:
	void DrawTracking (CDC*);
	void OnMouseMove(TrackerMsgPump*,MoveBezierTrackerParam&);
};

/**
	回転トラッカー
*/
class RotateTracker {
private:
	CRect  m_rc;
	CPoint m_devCenter;
	double m_rotate;
public:
	enum {hitRotateCenter = -2};
public:
	RotateTracker();
	/// トラッキング設定
	void SetTracker(LPCRECT devRect,CPoint devCenter,double rotate);
	/// トラッキング結果の取得
	void GetTracker(CPoint& devCenter,double& rotate) const;

	/// トラッキング
	BOOL TrackRotate(CWnd*,CPoint ptMouse);
	/// 非トラッキング時のトラッカーとハンドルを描画する
	void Draw(CDC* pDC) const;
	/// ヒットテスト
	int HitTest(CPoint pt) const;
private:
	BOOL TrackCenter(CWnd*,CPoint ptMouse);
	BOOL TrackHandle(CWnd*,CPoint ptMouse);
	void DrawBorder(CDC*) const;
	void DrawCenter(CDC*) const;
	void DrawHandle(CDC*) const;
	CPoint GetHandlePos(int nHandle) const;
	void GetTrackerPolyline(LPPOINT) const;
	BOOL HitPoint(POINT ptTarget,POINT ptMouse) const;

	struct RotateTrackParam {
		double m_rotatebase;
		double m_rotatestart;
	};
	struct CenterTrackParam {
	};
public:
	void OnMouseMove(TrackerMsgPump*,CenterTrackParam&);
	void OnMouseMove(TrackerMsgPump*,RotateTrackParam&);
	virtual void DrawTracking(CDC*);
}; // class RotateTracker

/**
	回転・リサイズトラッカー
*/
class TrackerEx {
protected:
	/// デバイス座標系の原型矩形
	CRect   m_rc;		
	/// デバイス座標系の中央店
	CPoint  m_center;
	/// 垂直方向スケール
	double  m_scalex;
	/// 水平方向スケール
	double  m_scaley;
	/// 回転
	double  m_rotate;
public:
	TrackerEx();
	/** 
		トラッキング情報を設定
		@param prc デバイス座標系-原型矩形
		@param pt  デバイス座標系-リサイズ・回転の原点位置
	*/
	void SetTracker(LPCRECT prc,CPoint pt,double scalex,double scaley,double rotate);
	/// トラッキング結果を取得
	void GetTracker(double& scalex,double& scaley,double& rotate) const;
	/// マウス位置判定 CRectTracker::hitNothing ...
	int HitTest(CPoint ptWnd) const;
	/// 矩形およびハンドルの描画
	void Draw(CDC*) const;

protected:
	/// ハンドルにヒットしたかどうか判定
	BOOL HitPoint(POINT ptHandleCenter,POINT ptMouse) const;
	/// ハンドルを描画
	void DrawHandle(CDC*) const;
	/// ハンドルを描画するテンプレートメソッド
	virtual void DrawHandle1(CDC*,LPCRECT) const = 0; 
	/// ボーダーの描画
	void DrawBorder(CDC*) const;
	/// 中央点の描画
	void DrawCenter(CDC*) const;
	/// ハンドル位置の取得
	CPoint GetHandlePos(int nHandle) const;
	/// 矩形枠のポリライン情報を取得
	void GetTrackerPolyline(LPPOINT) const;
public:
	virtual void DrawTracking(CDC*);
}; // class TrackerEx

class RotateTrackerEx : public TrackerEx {
public:
	/// 回転トラッキング
	BOOL TrackRotate(CWnd* pWnd,CPoint);
private:
	/// ハンドルを描画するテンプレートメソッド
	virtual void DrawHandle1(CDC*,LPCRECT) const; 
	struct RotateTrackParam {
		double m_rotatebase;
		double m_rotatestart;
	};
public:
	void OnMouseMove(TrackerMsgPump*,RotateTrackParam&);
};

class ResizeTrackerEx : public TrackerEx {
public:
	/// リサイズトラッキング
	BOOL TrackResize(CWnd* pWnd,CPoint);
	/// カーソル設定
	BOOL SetCursor(CWnd* pWnd,UINT nHitTest) const;
private:
	/// ハンドルを描画するテンプレートメソッド
	virtual void DrawHandle1(CDC*,LPCRECT) const; 
	struct ResizeTrackParam {
		int m_nHandle;
		double m_scalingold;
		double m_scalexold;
		double m_scaleyold;
	};
public:
	void OnMouseMove(TrackerMsgPump*,ResizeTrackParam&);
};

/**
	パスの頂点を挿入するトラッカー
*/
class InsPathPtTracker {
private:
	enum InsType { insFirst,insNext,insIns};
	InsType m_instype;
	BOOL m_bFirst;
	CPoint m_pt;
	CPoint m_ptCurveL;
	CPoint m_ptCurveR;
	CPoint m_ptPrev;
	CPoint m_ptPrevCurve;
	CPoint m_ptNext;
	CPoint m_ptNextCurve;
public:
	BOOL TrackAddPointFirst(CWnd*,CPoint pt);
	BOOL TrackAddPointNext (CWnd*,CPoint pt,CPoint ptPrev,CPoint ptPrevCurve);
	BOOL TrackAddPointIns  (CWnd*,CPoint pt,CPoint ptPrev,CPoint ptPrevCurve,CPoint ptNextCurve,CPoint ptNext);
	void GetTracker(CPoint& pt,CPoint& ptCurveL,CPoint& ptCurveR) const;
private:
	BOOL _TrackAddPoint(CWnd*,CPoint pt);

	struct InsPathPtTrackerParam {};
public:
	void OnMouseMove(TrackerMsgPump*,InsPathPtTrackerParam&);
	virtual void DrawTracking(CDC*);
}; // class InsPathPtTracker

/**
	パスの頂点ハンドル(左右)の修正トラッカー
*/
class ModPathPtTracker {
public:
	enum PointType {pointCorner = 0,pointSmoothCurve = 1,pointCurve = 2};
private:
	enum HandleType {handleLeft,handleRight};
	PointType m_pointType;
	HandleType m_handle;
	CPoint m_pt;
	CPoint m_ptCurveL;
	CPoint m_ptCurveR;
	BOOL m_bHasPrev;
	CPoint m_ptPrev;
	CPoint m_ptPrevCurve;
	BOOL m_bHasNext;
	CPoint m_ptNext;
	CPoint m_ptNextCurve;

public:
	ModPathPtTracker();
	void SetPoint(PointType type,CPoint,CPoint,CPoint);
	void SetPrevPoint(CPoint ptPrev,CPoint ptPrevCurve);
	void SetNextPoint(CPoint ptNext,CPoint ptNextCurve);

	BOOL TrackHandleL(CWnd*,CPoint pt);
	BOOL TrackHandleR(CWnd*,CPoint pt);
	void GetTracker(int&,CPoint&,CPoint&,CPoint&) const;

private:
	BOOL Track(HandleType,CWnd*,CPoint);
	struct ModPathPtTrackerParam {};
public:
	void OnMouseMove(TrackerMsgPump*,ModPathPtTrackerParam&);
	virtual void DrawTracking(CDC*);
}; // class ModPathPtTracker

/**
	パスの頂点移動トラッカー
*/
class MovPathPtTracker {
public:
	struct BPOINT {
		BOOL m_bSel;
		POINT m_pt;
		POINT m_ptHandleL;
		POINT m_ptHandleR;
	}; // struct BPOINT
	typedef BPOINT*        PBPOINT;
	typedef BPOINT const*  PCBPOINT;

private:
	struct BEIZER {
		PCBPOINT m_pPts;
		UINT m_nLen;
	}; // struct BEIZER
	typedef std::list<BEIZER> BEIZERS;
	BEIZERS m_beizers;
	CPoint m_ptMoved;
public:
	MovPathPtTracker();
	void AppendBeizer(PCBPOINT,UINT nLen);
	BOOL TrackMove(CWnd*,CPoint);
	CPoint GetTracked() const;

private:
	void ClearAllBeizers();
	void DrawBeizer (CDC*,const BEIZER&) const;
	void DrawBeizerLine(CDC*,const BEIZER&) const;
	void DrawBeizerPts(CDC*,const BEIZER&) const;
	void GetMovedBPOINT(BPOINT&,PCBPOINT) const;
	void DrawBeizers(CDC*) const;
	struct MovPathPtTrackerParam {
		CPoint m_ptOld;
		CPoint m_ptStart;
	};
public:
	void OnMouseMove(TrackerMsgPump*,MovPathPtTrackerParam&);
	virtual void DrawTracking(CDC*);
}; // class MovPathPtTracker