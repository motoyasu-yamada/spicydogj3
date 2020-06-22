#pragma once

/**
	メソッドへの再入禁止制御を行うクラス
	メソッドを抜けた時に自動的に再入禁止を解除
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