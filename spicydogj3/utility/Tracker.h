#pragma once

#include "utility/bezier.h"

const CSize DEFAULT_TRACKER_HANDLE_SIZE(6,6);
const CSize DEFAULT_TRACKER_CENTER_SIZE(10,10);

class TrackerMsgPump;

/**
	�V�t�g��ԊǗ����s���N���X
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
	�ړ���p�g���b�J�[
	- �V�t�g�X�e�[�g�ł͊p�x45�x�P�ʂł����ړ��ł��Ȃ�
*/
class MoveTracker : public ShiftRectTracker {
public:
	MoveTracker();
	BOOL TrackMove(CWnd*,CPoint);
};

/**
	�ʏ탊�T�C�Y��p�g���b�J�[
	- �V�t�g�X�e�[�g�ł͓���䗦�ł������T�C�Y�ł��Ȃ�
*/
class ResizeTracker : public ShiftRectTracker {
public:
	ResizeTracker();
	BOOL TrackResize(CWnd*,CPoint);
};

/**
	�V�K��`�쐬 ���o�[�o���h�g���b�J�[
	- �V�t�g�X�e�[�g�ł͐����`����̃g���b�L���O�ɂȂ�
*/
class CreateRectTracker : public ShiftRectTracker {
public:
	CreateRectTracker();
	BOOL TrackCreate(CWnd*,CPoint);
};

/**
	�V�K�ȉ~�쐬�g���b�J�[
	- �V�t�g�X�e�[�g�ł͐��~����̃g���b�L���O�ɂȂ�
*/
class CreateCircleTracker: public ShiftRectTracker {
public:
	CreateCircleTracker();
	BOOL TrackCreate(CWnd*,CPoint);
private:
	virtual void DrawTrackerRect(LPCRECT lpRect,CWnd* pWndClipTo,CDC* pDC,CWnd* pWnd);
};

/** 
	���[�W�����ړ��g���b�J�[
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
	�x�W�F�Ȑ��ړ��g���b�J�[
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
	��]�g���b�J�[
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
	/// �g���b�L���O�ݒ�
	void SetTracker(LPCRECT devRect,CPoint devCenter,double rotate);
	/// �g���b�L���O���ʂ̎擾
	void GetTracker(CPoint& devCenter,double& rotate) const;

	/// �g���b�L���O
	BOOL TrackRotate(CWnd*,CPoint ptMouse);
	/// ��g���b�L���O���̃g���b�J�[�ƃn���h����`�悷��
	void Draw(CDC* pDC) const;
	/// �q�b�g�e�X�g
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
	��]�E���T�C�Y�g���b�J�[
*/
class TrackerEx {
protected:
	/// �f�o�C�X���W�n�̌��^��`
	CRect   m_rc;		
	/// �f�o�C�X���W�n�̒����X
	CPoint  m_center;
	/// ���������X�P�[��
	double  m_scalex;
	/// ���������X�P�[��
	double  m_scaley;
	/// ��]
	double  m_rotate;
public:
	TrackerEx();
	/** 
		�g���b�L���O����ݒ�
		@param prc �f�o�C�X���W�n-���^��`
		@param pt  �f�o�C�X���W�n-���T�C�Y�E��]�̌��_�ʒu
	*/
	void SetTracker(LPCRECT prc,CPoint pt,double scalex,double scaley,double rotate);
	/// �g���b�L���O���ʂ��擾
	void GetTracker(double& scalex,double& scaley,double& rotate) const;
	/// �}�E�X�ʒu���� CRectTracker::hitNothing ...
	int HitTest(CPoint ptWnd) const;
	/// ��`����уn���h���̕`��
	void Draw(CDC*) const;

protected:
	/// �n���h���Ƀq�b�g�������ǂ�������
	BOOL HitPoint(POINT ptHandleCenter,POINT ptMouse) const;
	/// �n���h����`��
	void DrawHandle(CDC*) const;
	/// �n���h����`�悷��e���v���[�g���\�b�h
	virtual void DrawHandle1(CDC*,LPCRECT) const = 0; 
	/// �{�[�_�[�̕`��
	void DrawBorder(CDC*) const;
	/// �����_�̕`��
	void DrawCenter(CDC*) const;
	/// �n���h���ʒu�̎擾
	CPoint GetHandlePos(int nHandle) const;
	/// ��`�g�̃|�����C�������擾
	void GetTrackerPolyline(LPPOINT) const;
public:
	virtual void DrawTracking(CDC*);
}; // class TrackerEx

class RotateTrackerEx : public TrackerEx {
public:
	/// ��]�g���b�L���O
	BOOL TrackRotate(CWnd* pWnd,CPoint);
private:
	/// �n���h����`�悷��e���v���[�g���\�b�h
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
	/// ���T�C�Y�g���b�L���O
	BOOL TrackResize(CWnd* pWnd,CPoint);
	/// �J�[�\���ݒ�
	BOOL SetCursor(CWnd* pWnd,UINT nHitTest) const;
private:
	/// �n���h����`�悷��e���v���[�g���\�b�h
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
	�p�X�̒��_��}������g���b�J�[
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
	�p�X�̒��_�n���h��(���E)�̏C���g���b�J�[
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
	�p�X�̒��_�ړ��g���b�J�[
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