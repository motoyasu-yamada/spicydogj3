#pragma once

/** ��`���� */
void MoveRectTo(CRect& rc,int x,int y);

/** �E�C���h�E�ړ� */
void MoveChildWindowToTop(CWnd& wnd,int nTop);
void MoveChildWindowToBottom(CWnd& wnd,int nBottom);
void MoveChildWindowToRight(CWnd& wnd,int nRight);
void MoveChildWindowToLeft(CWnd&,int);
void MoveChildWindowToLeftNext(CWnd& wnd,const CWnd& wndLeft);

/** �E�C���h�E�X���T�C�Y */
void StretchChildWindowToBottom(CWnd& wnd,int nBottom);
void StretchChildWindowToRight(CWnd& wnd,int nRight);
void StretchChildWindowWidth(CWnd& wnd,int nWidth);
void StretchChildWindowHeight(CWnd& wnd,int nHeight);

/**  �E�C���h�E�T�C�Y�擾 */
int GetWindowHeight(const CWnd&);
int	GetWindowWidth(const CWnd&);
int GetChildWindowLeft(const CWnd&);
int GetChildWindowTop(const CWnd&);
int GetChildWindowRight(const CWnd&);
int GetChildWindowBottom(const CWnd&);

/**  �e���W�n�ł̎q�E�C���h�E�ʒu */
void GetParentClientRect(const CWnd& wnd,CRect& rc);

/**  �|�b�v�A�b�v���j���[ */
void TrackDlgBtnPopupMenu(CDialog* pParent,UINT nItemID,UINT nMenuID,int nSubMenu = 0);
void TrackNewPopupMenu(POINT,CWnd*,UINT nID,int nSubMenu = 0);
void TrackNewPopupMenu(CWnd*,UINT nID,int nSubMenu = 0);

/** 	�_�C�A���O�̎q�R���g���[���́A�e�E�C���h�E���W�n�ł̋�`�����߂� */
void GetDlgItemRect(const CWnd* pParent,UINT nID,CRect& rc);
/** 	���ׂĂ̎q���E�C���h�E���g�p�s��(�W�F���)/�ɕύX */
void EnableDescendantWindows(CWnd* hWnd,BOOL b);
/** 	�I������Ă��閼�O�t���K���ɉ����ăR���g���[���̏��(�g�p��/�s��)���X�V */
void EnableDlgItem(CWnd* pParent,UINT nID,BOOL b);
/** 	�_�C�A���O�̃��\�[�X��̃T�C�Y */
SIZE GetDlgTemplateSize(UINT nDlgResID);
/**	�E�C���h�E�̎q���Ƀ��b�Z�[�W�𑗐M */
void SendMessageToDescendants(CWnd*,UINT,int = 0,WPARAM =0,LPARAM = 0,BOOL = FALSE);
void LoadWindowPlacement(CWnd* pWnd,LPCTSTR ptszSec,LPCTSTR ptszEntry);
void SaveWindowPlacement(CWnd* pWnd,LPCTSTR ptszSec,LPCTSTR ptszEntry);
CString WindowPlacementToString(const WINDOWPLACEMENT& w);
BOOL StringToWindowPlacement(LPCTSTR ptsz,WINDOWPLACEMENT& w);
void SetWindowPlacement(CWnd* pWnd,const WINDOWPLACEMENT& w);
void GetWindowPlacement(CWnd* pWnd,WINDOWPLACEMENT& w);
BOOL LoadWindowPlacementFromReg(LPCTSTR ptszSec,LPCTSTR ptszEntry,WINDOWPLACEMENT& w);
void SaveWindowPlacementToReg  (LPCTSTR ptszSec,LPCTSTR ptszEntry,const WINDOWPLACEMENT& w);

/// �V�X�e��256�F�ȏ��(�J�X�^���p���b�g�����HiColor�ȏ�)�̃C���[�W���X�g���쐬
BOOL CreateImageListColor32(CImageList& il,UINT nResID,int cx,int cy,COLORREF crMask);

/** ���݃A�N�e�B�u�ȃr���[���擾 */
CView* GetActiveView();
/** �L�[�������ĉ�����Ă��邩�ǂ��� */
BOOL GetKeyStillDown(int nKey);
/** �A�C�R�����̃^�u��`�悷�� */
void DrawTab(CDC& dc,const CRect& rc,BOOL sel,LPCTSTR text,CImageList& imageList,
			 int iconIndex,int leftMargin);
/** �N���b�N���h���b�O���Ă悤�Ƃ��Ă��邩�ǂ������� */
BOOL IsToStartDrag(CWnd* pWnd,CPoint ptDown,BOOL bLeftOrRight);
/** ���X�g�̌��ݑI������Ă����̃C���f�b�N�X���擾���� */
int GetListCtrlCurSel(const CListCtrl& listCtrl);
/** �n���h���ԍ�����n���h���̋�`���擾���� */
void GetHandleRect(const CRect& rc,int nHandle,CRect& rcHandle,int nHandleSize,int nStyle);
/** \n�ŕ������ꂽ���b�Z�[�W�������؂�o��*/
CString GetMessageString(UINT nID);