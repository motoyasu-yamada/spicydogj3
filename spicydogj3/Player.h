#pragma once

#include "sdom/OLM.H"

class SpicydogDoc;

class Player {
private:
	SpicydogDoc* m_document;
	int          m_frame;
	BOOL         m_playing;

public:
	Player(SpicydogDoc*);
	//	�擪�̃t���[���Ɉړ�
	void FirstFrame();
	//	���̃t���[���Ɉړ�
	void NextFrame();
	//	�O�̃t���[���Ɉړ�
	void PrevFrame();
	//	�Ō�̃t���[���Ɉړ�
	void LastFrame();
	//	�Đ��t���[�����X�L�b�v
	void SetPlayFrame(int n);
	//	���݂̍Đ��t���[�����擾
	int GetPlayFrame() const;
	//	�Đ��ő�t���[���������߂�
	int GetLastFrame() const;
	//	���ݍĐ������ǂ���
	BOOL IsPlaying() const;
	//	�Đ��J�n
	void Play();
	//	�Đ���~
	void Stop();
	//	�ҏW�J�n�̂��߂ɍĐ����~����
	void PrepareEdit();
};