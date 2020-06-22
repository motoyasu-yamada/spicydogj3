#pragma once

#include "cmd/Cmd.h"
#include "model/char/PictChar.h"

/**
	ピクチャーのセンタリング設定の変更コマンド
*/
class PictSetCenterCmd : public Cmd {
private:
	PictChar* m_pPictChar;
	CPoint m_ptCenterNew;
	CPoint m_ptCenterOld;
public:
	PictSetCenterCmd(PictChar*,CPoint ptNew);
	virtual void Do();
	virtual void Undo();
};


/**
	書出最適化設定を変更
*/
class PictSetOptParamCmd : public Cmd {
private:
	PictChar* m_pPictChar;
	PictQuantizeParam m_paramOld;
	PictQuantizeParam m_paramNew;
public:
	/**
		- パススルー以外⇒パススルーへの設定変更は不可
	*/
	PictSetOptParamCmd(PictChar*,const PictQuantizeParam&);
	virtual void Do();
	virtual void Undo();
};


/**
	(対象操作)
	ドット描画,リサイズ,トリム,リロード,外部エディット,エフェクト
*/
class PictModDataCmd : public Cmd {
private:
	PictChar* m_pPictChar;
	CPoint m_ptCenterOld;
	PictData m_dataNew;
	PictData m_dataOld;
public:
	/**
		@param p    パススルー書出設定のピクチャは変更できない
		@param data 変更後のデータ
	*/
	PictModDataCmd(UINT resource,PictChar* p,const PictData& data);
	virtual void Do();
	virtual void Undo();
};