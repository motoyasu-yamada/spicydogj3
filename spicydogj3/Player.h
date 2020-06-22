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
	//	先頭のフレームに移動
	void FirstFrame();
	//	次のフレームに移動
	void NextFrame();
	//	前のフレームに移動
	void PrevFrame();
	//	最後のフレームに移動
	void LastFrame();
	//	再生フレームをスキップ
	void SetPlayFrame(int n);
	//	現在の再生フレームを取得
	int GetPlayFrame() const;
	//	再生最大フレーム数を求める
	int GetLastFrame() const;
	//	現在再生中かどうか
	BOOL IsPlaying() const;
	//	再生開始
	void Play();
	//	再生停止
	void Stop();
	//	編集開始のために再生を停止する
	void PrepareEdit();
};