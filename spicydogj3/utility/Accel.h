#pragma once

#include <afxtempl.h>

/// アクセレレータテーブルサイズを取得
int GetAcceleratorTableSize(HACCEL haccel);

/// アクセレレータハンドルラッパークラス
class Accel {
private:
	HACCEL m_haccel;
public:
	Accel();
	~Accel();
	BOOL Attach(HACCEL);
	HACCEL Detach();
	BOOL DeleteAccel();
	HACCEL GetAccel() const;
}; // class Accel

/**
	アクセレレータテーブルをマージする
*/
class MultiAccel {
private:
	/// マージ済アクセラレータ
	Accel m_accelMerged;
	/// マージする必要があるかどうか
	BOOL m_bToMerge;
	/// マージマップ
	CArray<HACCEL,HACCEL&> m_accelMap;
public:
	MultiAccel();
	/// アクセル設定
	void SetAccel(int nIndex,HACCEL);
	/// マージ済アクセレレータを作成する
	HACCEL GetMergedAccel();
private:
	/// マージしたアクセラレータハンドルを作成する
	void CreateAccel();
}; // class AccelMerge


/**
	アクセレレータオーナー
*/
class AccelOwner {
private:
	HACCEL m_haccel;
public:
	AccelOwner();
	virtual ~AccelOwner();
	virtual HACCEL GetDefaultAccelerator();
	void SetAccelerators(UINT);
	void SetAccelerators(HACCEL);
}; // class AccelOwner


