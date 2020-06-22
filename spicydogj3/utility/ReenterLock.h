#pragma once

/**
	���\�b�h�ւ̍ē��֎~������s���N���X
	���\�b�h�𔲂������Ɏ����I�ɍē��֎~������
*/
class ReenterLock {
public:
	ReenterLock(BOOL& bLockFlag);
	~ReenterLock();
	BOOL Lock();
	void Unlock();
	static void InitReenterLock(BOOL& b);
private:
	BOOL& m_bLockFlag;
	BOOL m_bLocking;
};