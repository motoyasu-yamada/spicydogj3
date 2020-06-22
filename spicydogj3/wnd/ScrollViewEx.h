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
	CScrollView�̍��@�\�o�[�W����
	- ���m�ȃZ���^�����O�\��
	- ���x�ȃX�N���[���̈�ݒ�
	- �O���b�h�\���x��
*/
class ScrollViewEx : public CView,public AccelOwner {
public:
	enum SetScaleType { setScale,setScaleDocumentToFitSize,setScaleTotalToFitSize,setResizeParentToFit};
	enum GetScaleType { getScale = setScale,
		                getScaleDocumentToFitSize = setScaleDocumentToFitSize,
						getScaleTotalToFitSize    = setScaleTotalToFitSize};
	/// �X�P�[�����O�p�����[�^�\����
	struct SETSCALEPARAM {
		///  �X�P�[���̐ݒ����
		int m_typeSetScale;
		/// �\���X�P�[�� 1=100%
		double m_scale;
	}; // struct SCROLLVIEWEX_SCALE
	/// �X�P�[�����O�p�����[�^�\����
	struct GETSCALEPARAM {
		///  ���݂̃X�P�[��
		int m_typeGetScale;
		/// �\���X�P�[�� 1=100%
		double m_scale;
	}; // struct SCROLLVIEWEX_SCALE
	
private:
	/// �}�b�s���O���[�h
	int m_nMapMode;
	/// �_�����W�n-�X�N���[���\�̈�
	CSize m_totalLog;
	/// �f�o�C�X���W�n-�X�N���[���\�̈�
	CSize m_totalDev;
	/// �X�N���[���y�[�W�T�C�Y-�f�o�C�X���W�n
	CSize m_pageDev;
	/// �X�N���[�����C���T�C�Y-�f�o�C�X���W�n
	CSize m_lineDev;
	/// �Z���^�����O���邩�ǂ���
	BOOL m_bCenter;
	/// ����������Ă��邩�ǂ���
	BOOL m_bInsideUpdate;
	/// �\���{�� 1-���{
	double m_scale;
	/// �_���h�L�������g�T�C�Y
	CSize  m_documentLog;
	/// �h�L�������g�T�C�Y�ƃX�N���[���\�̈�̔䗦
	int    m_nTotalByDocument;
	/// �_���h�L�������g���W�n���_�ʒu
	CPoint m_documentCenter;
	/// InitialUpdated�ς��ǂ���
	BOOL m_bInitialUpdated;
	// UpdateScrollSizes�͍ē��s��
	BOOL m_bUpdateScrollSizes;

protected:
	DECLARE_DYNAMIC(ScrollViewEx)
	ScrollViewEx();

public:
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static void DPtoLP(CDC* pDC,LPPOINT,int = 1);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static void LPtoDP(CDC* pDC,LPPOINT,int = 1);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static void DPtoLP(CDC* pDC,LPSIZE);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static void LPtoDP(CDC* pDC,LPSIZE);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static void DPtoLP(CDC* pDC,LPRECT);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static void LPtoDP(CDC* pDC,LPRECT);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static HRGN LPtoDP(CDC* pDC,HRGN);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static HRGN DPtoLP(CDC* pDC,HRGN);
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	static void LPtoDP(CDC*,CBEZIER& bezier);

	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	void DPtoLP(LPPOINT,int = 1) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	void LPtoDP(LPPOINT,int = 1) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	void DPtoLP(LPSIZE) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	void LPtoDP(LPSIZE) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	void DPtoLP(LPRECT) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	void LPtoDP(LPRECT) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	HRGN DPtoLP(HRGN) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	HRGN LPtoDP(HRGN) const;
	/// �����x�ȃf�o�C�X���W�Ƙ_�����W�̕ϊ����[�`��
	void LPtoDP(CBEZIER& bezier);

	/// �\���{����ύX����
	void SetScale(double dScale);
	/// �X�N���[����`�S�̕\�����E�C���h�E�g�̃T�C�Y�ɓ��ꂷ��
	void SetScaleDocumentToFitSize();
	/// �h�L�������g��`�S�̕\�����E�C���h�E�g�̃T�C�Y�ɓ��ꂷ��
	void SetScaleTotalToFitSize();
	/// �\���Ɏ��܂�悤�E�C���h�E�g�̃T�C�Y�𒲐�����
	void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	/// ���݂̕\���{��
	double GetScale() const { return m_scale; }
	/// �X�N���[���ʒu���Z���^�[�ɖ߂�
	void ScrollToCenter();
	/// �w�肵���h�L�������g�n���W�ʒu����ʒ����ɂ���
	void ScrollToCenter(CPoint ptLog);
	/// �R���g���[������E�C���h�E���b�Z�[�W�����v���V�[�W���̃A�N�Z�X�������邽�߂̃o�C�p�X
	inline LRESULT Default() { return CWnd::Default(); }
	/// �h�L�������g��`(�f�o�C�X���W�n)���擾
	void GetDocumentRect(LPRECT prc) const;
	/// �X�N���[���u����`(�f�o�C�X���W�n)���擾
	void GetScrollableRect(LPRECT prc) const;
	/// �_(�f�o�C�X���W)���h�L�������g��`���ɂ��邩�ǂ���
	BOOL PtInDocumentRect(CPoint devpt) const;
	/// �_(�f�o�C�X���W)���X�N���[���u����`���ɂ��邩�ǂ���
	BOOL PtInScrollableRect(CPoint devpt) const;
	/// �w�肵���_���ʒu�ɃX�N���[��
	void ScrollToPosition(CPoint pt);
	/// �X�N���[���ʒu - �_���ʒu
	CPoint GetScrollPosition() const;
	/// �w�肵���f�o�C�X�ʒu�ɃX�N���[��
	void ScrollToDevicePosition(POINT ptDev);
	/// �X�N���[���ʒu - �f�o�C�X�ʒu
	CPoint GetDeviceScrollPosition() const;

protected:
	/// �h�L�������g�g��`�悷��
	void DrawDocumentBorder(CDC*,CBrush*) const;
	/// �h�L�������g��`���N���b�v����
	void MaskDocumentRect(CDC* pDC,BOOL b) const;

	/// ������(OnInitialUpdate)����Ă��邩�ǂ���
	inline BOOL IsInitialUpdated() const { return m_bInitialUpdated; }
	virtual void OnInitialUpdate();

	/// �\�����Z���^�����O����
	void Center(BOOL b);
	/**
		�X�N���[���G���A��ݒ�
		@param sizeDocument     �L���Ș_�����W�n�T�C�Y(�h�L�������g�T�C�Y)���w�肷��
		@param nDocumentByTotal �h�L�������g�T�C�Y�ɑ΂���X�N���[���\�̈�̃T�C�Y
		@param ptCenter         �_�����W�n�̌��_�ʒu
	*/
	void SetScrollSizes(SIZE sizeDocument,int nDocumentByTotal,POINT ptCenter);
	/// �E�C���h�E���͈͓���h��Ԃ�
	void FillBkgnd(CDC* pDC, CBrush* pBrush) const;
	/// �X�N���[���\�̈��h��Ԃ�
	void FillScrollabeRect(CDC* pDC,CBrush* pBrush) const;
	/// �h�L�������g��`��h��Ԃ�
	void FillDocumentRect(CDC* pDC,CBrush* pBrush) const;

	/// �_�����W�n�ł̉�ʒ����̃X�N���[���ʒu
	CPoint GetCenterScrollPosition() const;
	/// �f�o�C�X�R���e�L�X�g����
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	/// �`��n���h�� - �p���N���X�ŕK���������邱��
	virtual void OnDraw(CDC* pDC) = 0;

protected:
	virtual ~ScrollViewEx();
	/// �O���b�h�`��w���p�[
	void DrawGridHelper(CDC*,CPen&,int);
	/// �h�L�������g�Z���^�[�O���b�h��`��
	void DrawCenterLine(CDC*,CPen&);
	/// �}�E�X�N���b�N�Ńr���[�Ƀt�H�[�J�X
	virtual BOOL PreTranslateMessage(LPMSG lpmsg);
	/// �Z���^�[�_��`�悷��
	void DrawCenter(CDC*,CPoint);
protected:
	/// �^�[�Q�b�g�������܂�ő�̓���{�������߂�
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
	�f�o�C�X�R���e�L�X�g�̍��W�n��
	�f�o�C�X���W�n�Ɉꎞ�I�ɖ߂�
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