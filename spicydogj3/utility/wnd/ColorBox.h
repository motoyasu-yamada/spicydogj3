#pragma once

/** ColorBox �E�B���h�E
	�F(�w�i�F��`��F��)��\������STATIC�G���A */
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
	�c�[���o�[�p�̃J���[�\�� & �I���{�b�N�X
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
	/**  �E�C���h�E���쐬 */
	BOOL Create(CWnd* pParent,CRect rc,UINT nID,UINT nIconBmpId,COLORREF crMask);
	/**  �F���擾 */
	COLORREF GetColor(void) const;
	/**  �F��ݒ� */
	void SetColor(COLORREF);
	/**  �{�^�������� */
	void PressButton();
	/**  �N���X�������� */
	static void clinit();
	/**  ON_UPDATE_COMMAND_UI�n���h�������̃w���p�֐� */
	static void OnUpdateCmdUI(CCmdUI* pCmdUI,COLORREF crSet);
	/**  ON_COMMAND�����ɂė��p -> Command�𓊂���ColorBoxEx�ɃA�N�Z�X���Č��ݐF���擾 */
	static COLORREF GetCurrentColor();
	/**  �z�u�p�̃_�~�[Static�Ɠ��ʒu��ColorBoxEx���쐬����*/
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
	�J���[�s�b�J�[�̃}�E�X�����ݏE���Ă���F��ʒm����
*/
class ColorPickerListener {
public:
	virtual void OnPickerMove(CPoint ptDesktop,COLORREF cr) = 0;
public:
	virtual ~ColorPickerListener() {};
};

/** ��ʏォ��F���s�b�N�A�b�v���� */
class ColorPicker {
private:
	ColorPickerListener* m_pListener;
	HCURSOR m_hcurPicker;
	CWnd* m_pParent;
	COLORREF m_cr;
public:
	/**	�R���X�g���N�^
		@param wnd   �F���s�b�N�A�b�v����E�C���h�E
		@paran curid �J�[�\�����\�[�XID
	*/
	ColorPicker(CWnd*,UINT curid);
	/**	�F�̃s�b�L���O���s�� �F���I�������܂Ń��[�_�� */
	BOOL DoPick();
	/**	�s�b�L���O�����F���擾���� */
	COLORREF GetColor() const;
	/**	���X�i�[��ݒ� */
 	void SetListener(ColorPickerListener*);
};

#define WM_CLRBOXEX_SELECT	WM_USER + 0x1001
const COLORREF NULLCOLOR = 0xFFFFFFFF;
const COLORREF NULLCOLOR_1 = RGB(0xFF,0xFF,0xFF);
const COLORREF NULLCOLOR_2 = RGB(0x80,0x80,0x80);
const int NULLCOLOR_PATTERN_SIZE = 6;
void FillNullRect (CDC*,LPCRECT);
void FillSolidRect(CDC*,LPCRECT,COLORREF);