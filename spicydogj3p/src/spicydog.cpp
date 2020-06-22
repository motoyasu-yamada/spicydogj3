/** SpicyDog Player v3

	All rights reserved by Spicysoft Corporation.
	
	アプリケーション起動パラメータ
		1	SDC実行パス                            */

#include "configure.h"

#include "import.h"
#include "sdc.H"
#include "debug/debug.h"
#include "env/color.h"
#include "env/key.h"
#include "env/softkey.h"
#include "env/graphics.h"
#include "env/device.h"
#include "env/dialog.h"
#include "env/sounds.h"

/* ローカルSDCデータヘッダ */
#define SIZE_OF_SCRATCHPAD_HEADER	16

#define SDC_STATE_NOT_INSTALLED		0x00
#define SDC_STATE_REQUIRE_REPARE	0x01
#define SDC_STATE_INSTALLED			0x02
#define SDC_STATE_REQUIRE_UPDATE	0x03

#define URL_SPLASH "resource:///s"
#define URL_WAIT   "resource:///w"
#define URL_PROG_BKG "resource:///p0"
#define URL_PROG_BAR "resource:///p1"

#define URL_SPDOCUMENTCASH "scratchpad:///0;pos=0"

#if defined(DOJA1)
	#define HTTP_LOAD_SIZE 128
#else
	#define HTTP_LOAD_SIZE 1024
#endif

/* スキンセット */
#define STR_DIALOG_TITLE        "SpicyDog"

/* 文字ソース*/
#define STR_ERR_INVALID_SDC     "不正アプリ"
#define STR_REDOWNLOAD          "電波断再接続？"
#define STR_SUCCESS_END         "アプリの実行完了"

#define STR_SDC_DOWNLOAD_INSTALL "インストール中"
#define STR_SDC_DOWNLOAD_UPDATE  "更新中"
#define STR_SDC_DOWNLOAD_REPARE	 "再セットアップ中"

#if defined(DOJA2M)
	#define MA_STATE_ACTIVE    0
	#define MA_STATE_DEACTIVE  1
	#define MA_STATE_SLEEP     2
#endif

/* 関数エイリアス */
#define getTime    System.currentTimeMillis
#define readLabel  readUI16
#define getPos()   (POS_READING)
#define displaySplash() displayLogo(URL_SPLASH)
	
/*
	A
	
	@version	3.0
	@author		m.yamada */
#define EVENT_FOLDING_CLOSE		 1
#define EVENT_FOLDING_OPEN		 2
#define EVENT_MAIL_AT_CENTER	 4
#define EVENT_MAIL_RECEIVED		 8
#define EVENT_MAIL_NONE			16
#define EVENT_ACTIVATED			32
#define EVENT_CLOCK				64
#define EVENT_WAKEUP			128

#include "Application.h"
#include "Screen.h"
#include "Char.h"


#if defined(DOJA2M) || defined(DOJA2S)
/**
	整数値パラメータを2つ持つアクションの共通構造体
*/
class DoubleIntParam {
	protected final int N1;
	protected final int N2;
	DoubleIntParam(int n1,int n2) {
		N1 = n1;
		N2 = n2;
	}
}

#define LaunchTypeJumpActParam DoubleIntParam
#define LaunchTypeJumpActParamLaunchType   N1
#define LaunchTypeJumpActParamFrameToJump  N2

#define DelayJumpActParam DoubleIntParam
#define DelayJumpActParamDelay       N1
#define DelayJumpActParamFrameToJump N2

#define RndJumpActParam DoubleIntParam
#define RndJumpActParamCase          N1
#define RndJumpActParamFrameToJump   N2

#endif




/**
	色情報変形行列
*/
class CTFORM
{
	private int MulR;
	private int MulG;
	private int MulB;
	private int AddR;
	private int AddG;
	private int AddB;
	
	public CTFORM(boolean b) {
		MulR = MulG = MulB = 0x100;
		//AddR = AddG = AddB = 0x000;
		if (!b)
			return;

		Screen.beginBitStream();
			int nMulBits    = Screen.readBSUI(5);
			int nAddBits    = Screen.readBSUI(4);
		
			if (nMulBits != 0)
			{
				MulR = Screen.readBSSI(nMulBits);
				MulG = Screen.readBSSI(nMulBits);
				MulB = Screen.readBSSI(nMulBits);
			}

			if (nAddBits != 0) {
				AddR = Screen.readBSSI(nAddBits);
				AddG = Screen.readBSSI(nAddBits);
				AddB = Screen.readBSSI(nAddBits);
			}
		//Screen.endBitStream();
	}
	
	/**
		色変形行列を元に色を変換しRGBからデバイス依存色に変換する。
		
		@param		ctform		色変形行列
		@param		nSrc		RGB形ｮ色 0x00rrggbb
		@return		変形後のデバイス依存形ｮの色情報
	*/
	public int realizeColor(int nSrc) {
		if (nSrc == -1)
			return -1;
		return GET_COLOR_OF_RGB(
						saturate(((nSrc >> 16) & 0xFF) * MulR / 0x100 + AddR,0,255),
						saturate(((nSrc >>  8) & 0xFF) * MulG / 0x100 + AddG,0,255),
						saturate(( nSrc        & 0xFF) * MulB / 0x100 + AddB,0,255));
	}
	
	/**
		指定範囲内[nMin,nMax]で値を飽和させる。

		@param
		@param	nMin	nMin未満の値は全てnMinに飽和する
		@param	nMax	nMax以上の値は全てnMaxに飽和させる
	*/
	private static int saturate(int n,int nMin,int nMax) {
		if (n < nMin)
			return nMin;
		else if (nMax < n)
			return nMax;
		return n;
	}
	
}

/* 形状情報変形行列 */
class MATRIX {
	private int ScaleX;
	private int ScaleY;
	private int RotateX;
	private int RotateY;
	private int TranslateX;
	private int TranslateY;
	
	public MATRIX() {
		Screen.beginBitStream();
			int nBits;
			if (Screen.readBSUI(1) == 1) {
				ScaleX = Screen.readBSSI(nBits = Screen.readBSUI(5));
				ScaleY = Screen.readBSSI(nBits);
			} else {
				ScaleX = ScaleY = 0x100;
			}
			if (Screen.readBSUI(1) == 1) {
				RotateX = Screen.readBSSI(nBits = Screen.readBSUI(5));
				RotateY = Screen.readBSSI(nBits);
			}
			TranslateX = Screen.readBSSI(nBits = Screen.readBSUI(5));
			TranslateY = Screen.readBSSI(nBits);

		//Screen.endBitStream();
	}

	public int operatorX(int x,int y) {
		return (x * ScaleX + y * RotateX) / 0x100 + TranslateX + Screen.DRAW_WIDTH /2;
	}
	
	public int operatorY(int x,int y) {
		return (y * ScaleY + x * RotateY) / 0x100 + TranslateY + Screen.DRAW_HEIGHT/2; 
	}
}

