#pragma once

#include "utility/EventPublisher.H"

class ViewSwitchInfo {
public:
	CView*  m_pView;
	BOOL    m_bClosable;
	UINT    m_nIcon;
	CString m_strDefaultCaption;

public:
	//	�R���X�g���N�^
	ViewSwitchInfo();
	ViewSwitchInfo(CView*,LPCTSTR ptszName,BOOL,UINT);
	//	�R�s�[�R���X�g���N�^
	ViewSwitchInfo(const ViewSwitchInfo&);
	//	������Z�q
	ViewSwitchInfo& operator = (const ViewSwitchInfo&);

	//	�r���[�̃|�C���^���擾
	CView* GetView() const;
	//	�r���[�̃L���v�V�����̐ݒ�
	CString GetCaption() const;
	//	���邱�Ƃ��\�ȃr���[���ǂ���
	BOOL IsClosable() const;
	//	�r���[�������邩�ǂ����ݒ�
	void SetClosable(BOOL);
	//	�r���[�̃A�C�R�������擾
	UINT GetIcon() const;

//	�C���v�������g
private:
	void CopyFrom(const ViewSwitchInfo&);
};

//	�r���[�X�C�b�`�̃f�[�^�\���ύX���X�i�[
class ViewSwitchListener
{
//	�C���^�[�t�F�[�X
public:
	virtual void OnRemoveView(int n,CView*) {}
	virtual void OnActiveViewChanged(int nNew,int nOld) {}
//	�C���v�������g
public:
	//	���z�f�X�g���N�^
	virtual ~ViewSwitchListener() {}
};

//	�r���[�X�C�b�`�̃f�[�^�\���Ǘ�
class ViewSwitch : public CEventPublisher<ViewSwitch,ViewSwitchListener>
{
private:
	typedef std::list<ViewSwitchInfo> VIEWS;
	VIEWS m_Views;
	int   m_nActive;
	
	int m_nRemoved;
	CView* m_pRemovedView;
	int m_nNewActive;
	int m_nOldActive;

public:
	//	�R���X�g���N�^
	ViewSwitch();
	//	�X�C�b�`�ɓo�^����Ă���r���[��
	int GetViewCount() const;
	//	�X�C�b�`�ɓo�^����Ă���r���[�̏����擾����
	void GetViewInfo(int,ViewSwitchInfo&) const;
	//	�X�C�b�`�ɓo�^����Ă���r���[�̏���ύX����
	void SetViewInfo(int,const ViewSwitchInfo&);
	//	�J�����g�r���[�̕ύX
	void ActivateView(int);
	//	�J�����g�r���[�̎擾
	int GetActiveView() const;
	//	�r���[�̐V�K�ǉ�
	int AppendView(CView*,LPCTSTR,BOOL,UINT);
	//	�r���[�̍폜
	void RemoveView(int);

private:
	//	�C�x���g���s
	void FireRemoveView(int,CView*);
	void EnumRemoveView(ViewSwitch*,ViewSwitchListener*);
	void FireActiveViewChanged(int,int);
	void EnumActiveViewChanged(ViewSwitch*,ViewSwitchListener*);
};
