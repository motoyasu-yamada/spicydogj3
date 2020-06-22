//	$Id: SMAFPlayerListener.h,v 1.1 2003/06/01 04:37:15 m_yamada Exp $
#pragma once

/** SMAF再生イベントリスナ */
class SMAFPlayerListener {
public:
	/** 曲の終端で自動終了した */
	virtual void OnAutoStop() = 0;
};
