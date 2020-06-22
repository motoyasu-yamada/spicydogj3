#pragma once

#include <list>
#include <memory>

/**
	�h�L�������g�ύX���X�i�[
*/
class DocumentListener
{
public:
	virtual void OnDocumentActivated(CDocument*) {}
	virtual void OnDocumentUpdated(CDocument*,LPARAM,CObject*) {}
	virtual void OnNotify(DWORD dwNotifyCode,CDocument*,LPARAM,LPVOID pv) {}
	virtual ~DocumentListener() {}
};

/**
	�h�L�������g�ύX�ʒm�N���X
*/
class DocumentNotifier
{
private:
	typedef std::list<DocumentListener*> DOCUMENTLISTENERS;
//	�f�[�^�����o
private:
	//	�V���O���g���I�u�W�F�N�g
	static std::auto_ptr<DocumentNotifier> s_pNotifier;
	//	�A�N�e�B�u�h�L�������g
	CDocument*	m_pDocument;
	//	���X�i
	DOCUMENTLISTENERS m_Listeners;

//	�C���^�[�t�F�[�X
protected:
	//	�R���X�g���N�^
	DocumentNotifier();
public:
	/**
		�h�L�������g�����ł������Ƃ�ʒm
		OnCloseDocument�ƃf�X�g���N�^����Ăяo�����Ƃ��\
		@param pDocument ���ł��悤�Ƃ��Ă���h�L�������g
	*/
	void NotifyDocumentExpired(CDocument* pDocument);
	//	�V���O���g���m�e�B�t�@�C�A�[���擾
	static DocumentNotifier* GetNotifier();
	//	���݃A�N�e�B�u�ȃh�L�������g���擾
	CDocument* GetActiveDocument() const;
	//	�h�L�������g�C�x���g���X�i��o�^
	void AddListener(DocumentListener*);
	//	�h�L�������g�C�x���g���X�i���폜
	void RemoveListener(DocumentListener*);
	//	�h�L�������g���A�N�e�B�u�ɂȂ������Ƃ�ʒm
	void ActivateDocument(CDocument*);
	//	�h�L�������g���X�V���ꂽ���Ƃ�ʒm
	void UpdateDocument(CDocument*,LPARAM = 0,CObject* = NULL);
	//	�h�L�������g�C�x���g��ʒm����
	void Notify(DWORD dwNotifyCode,CDocument*,LPARAM = 0,LPVOID pv = NULL);
	void ForceActivateDocument(CDocument*);
};

