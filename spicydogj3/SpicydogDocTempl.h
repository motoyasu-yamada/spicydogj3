#pragma once

/**
	�J�X�^���h�L�������g�e���v���[�g
	(�W���ƈقȂ�_)
		- �h�L�������g�����K�ɍ쐬(�V�K�A�Ǎ�)��Ƀt���[�����쐬
		- �h�L�������g�̐V�K�쐬���ɃA�v���ݒ�_�C�A���O���J��
*/
class SpicydogDocTempl : public CMultiDocTemplate {
public:
	SpicydogDocTempl(UINT nId,CRuntimeClass* pDocC,CRuntimeClass* pFrameC,CRuntimeClass* pViewC);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName,BOOL bMakeVisible);
};

