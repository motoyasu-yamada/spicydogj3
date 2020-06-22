// $Id: clinit.h,v 1.1 2003/05/27 10:52:18 m_yamada Exp $
#pragma once
#if defined(_MT)
#include <afxmt.h>
#endif

/**
	クラス初期化ユーティリティ
*/
template <typename _CT> 
class TClassInitializer {
private:
	static BOOL s_bInited;
#if defined(_MT)
	static CCriticalSection s_cs;
#endif
public:
	TClassInitializer() {
		if (s_bInited) {
			return;
		}
#if defined(_MT)
		CSingleLock lock(&s_cs,TRUE);
		if (s_bInited) {
			return;
		}
#endif
		_CT::clinit();
		s_bInited = TRUE;
	}
}; // class TClassInitizlier
#if defined(_MT)
template <typename _CT> CCriticalSection TClassInitializer<_CT>::s_cs;
#endif
template <typename _CT> BOOL TClassInitializer<_CT>::s_bInited = FALSE;

#define DECLARE_CLINIT(classname) TClassInitializer<classname> __clinitializer;public: static void clinit()
