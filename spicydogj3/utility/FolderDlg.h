#pragma once

#include <shlobj.h>

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE	0x0040
#endif


class FolderDlg
{
private:	
	CString m_strTitle;		// �L���v�V����
	CString m_strRoot;		// ���[�g�t�H���_
	CString m_strSelected;	// �I�����ꂽ�t�H���_
	
	BROWSEINFO	m_bi;
	HWND m_hwndMe;			// �������g
	HWND m_hwndParent;		// �e�E�C���h�E
	BOOL m_bDoneModal;		// DoModal���Ăяo���ꂽ���ǂ���

//	�C���^�[�t�F�[�X
public:
	FolderDlg(HWND hParent,LPCTSTR ptszTitle,LPCTSTR ptszRoot = NULL,UINT uiFlag = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE);

	// ���[�U�Ƀt�H���_��I��������
	virtual int DoModal();
	//	���[�U���I�������t�H���_�[���擾����
	CString GetFolderPath() const;
	// �I�����ꂽ�t�H���_�Ɋ֘A�}�����Ă���V�X�e���C���[�W���X�g�ւ̃C���f�b�N�X
	int GetFolderImage() const;
	// �_�C�A���O�X�^�C���̐ݒ�/�擾
	//          BIF_BROWSEFORCOMPUTER    �R���s���[�^�̂ݑI���\
	//          BIF_BROWSEFORPRINTER     �v�����^�[�̂ݑI���\
	//          BIF_DONTGOBELOWDOMAIN    �l�b�g���[�N�t�H���_�͑I��s�\
	//          BIF_RETURNFSANCESTORS    �t�@�C���V�X�e���A���Z�X�^�[�̂ݑI���\
	//          BIF_RETURNONLYFSDIRS     �t�@�C���V�X�e���f�B���N�g���̂ݑI���\
	//          BIF_STATUSTEXT           �_�C�A���O�{�b�N�X�ɃX�e�[�^�X�G���A���܂�
	UINT GetStyle() const;
	void SetStyle(UINT ulFlags);


// �p���N���X����̑���
protected:
	void EnableOK		(BOOL bEnable);
	void SetSelection	(LPCITEMIDLIST pidl);
	void SetSelection	(LPCTSTR);
	void SetStatusText	(LPCTSTR);

//	�C���v�������g
public:
	virtual ~FolderDlg();
protected:
	// BROWSEFOLDER�̃��b�Z�[�W����
	virtual void OnInit();
	virtual void OnSelChanged(LPCITEMIDLIST pidl);

	// �R�[���o�b�N����
private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
};
