#pragma once

//	�X�C�b�`�E�C���h�E�̍���
extern const int VIEWSWITCHWND_HEIGHT;

class ViewSwitch;
#include <vector>



/**	�r���[�X�C�b�`�̕\���Ǘ��N���X */
class ViewSwitchWnd : public CWnd {
private:
	/**	�r���[�X�C�b�`�\���E�C���h�E�́A�^�u��̕���{�^�� */
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

	//	�r���[�^�C�v�A�C�R�� (0:���[�r�[)
	CImageList m_ImageList;
	//	�r���[�̊Ǘ����
	ViewSwitch& m_switch;
	int m_nActiveView;
	CView* m_pActiveView;

	//	�r���[�^�u�̕\�����Ǘ��f�[�^
	struct ViewTabInfo {
		CRect m_rc;
	};
	typedef std::vector<ViewTabInfo> VIEWTABS;
	VIEWTABS m_ViewTabs;
	//	����{�^��
	VirtualButton m_btnClose;
	//	�^�u�̃t�H���g
	CFont m_fontTab;

// �C���^�[�t�F�[�X
public:
	//	�R���X�g���N�^
	ViewSwitchWnd(ViewSwitch&);
	//	�E�C���h�E����
	BOOL Create(CWnd*);
	//	�r���[�X�C�b�`�ɓo�^����Ă���r���[��񂪕ύX���ꂽ	
	void Update();
	//	���C�A�E�g��ύX����
	void RecalcLayout();

// �C���v�������e�[�V����
public:
	virtual ~ViewSwitchWnd();
private:
	//	���C�A�E�g���Čv�Z����
	void RecalcChildsLayout();
	//	�r���[�ؑփ^�u��\������
	void DrawViewTab(CDC*,BOOL bActivated,const CRect&,UINT nIcon,LPCTSTR ptszCaption);
	//	����{�^���̃��C�A�E�g���Čv�Z����
	void RecalcCloseBtn();
	//	�r���[�ؑփ^�u���C�A�E�g���Čv�Z����
	void RecalcViewTabLayout();
	//	����{�^���𐶐�����
	void CreateCloseButton();
	//	�^�u�̃q�b�g�e�X�g
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

