#pragma once

//////////////////////////////////////////////////////////////////////////
//	これは[SpicyDog]のソースコードです。
//	著作権 スパイシーソフト株式会社
//	このソースコードに関するすべての権利はスパイシーソフトが保有します。
//
//	This is a part of SpicyDog  2001.
//	Copyright (C) 2000 Spicysoft Corporation.
//	All rights reserved.
//////////////////////////////////////////////////////////////////////////

//	余分なセル
const int CELL_OF_EXCESS = 120;

//	表示位置
const CSize OFFSET_OF_LAYER_CHAR(3,3);
const CSize OFFSET_OF_LAYER_NAME(25,1);
const CSize ROFFSET_OF_LAYER_NAME(-45,1);
const CSize ROFFSET_OF_LAYER_EYE(-40,1);
const CSize ROFFSET_OF_LAYER_LCK(-20,1);
const CSize OFFSET_OF_END(2,2);
const CSize OFFSET_OF_KEY(2, 2);
const CSize OFFSET_OF_ACT(3, 7);
const CSize OFFSET_OF_LAB(3,12);

//	画面メトリックス
const int HEIGHT_OF_HDR = 18;
const CSize SIZE_OF_DRAG_RECT_BORDER(2,2);
const CSize SIZE_OF_LAYER_BMP(18,16);
const CSize SIZE_OF_CHAR_BMP(15,11);
const CSize SIZE_OF_CELL(9,18);
const CSize SIZE_OF_KEY(4,4);
const CSize SIZE_OF_ACT(4,4);
const CSize SIZE_OF_LAB(4,4);

//	レイヤーアイコンリソースインデックス
const int LAYERICON_EYE		= 0;
const int LAYERICON_LOCK	= 1;
const int LAYERICON_EDIT	= 2;
const int LAYERICON_FRAME	= 3;
const int LAYERICON_TIME	= 4;
const int LAYERICON_PLAYPOS	= 5;

//	フレームアイコンリソースインデックス
const int FRAMEICON_KEYFRAME			= 0;
const int FRAMEICON_EMPTYKEYFRAME		= 1;
const int FRAMEICON_KEYFRAME_ACT		= 2;
const int FRAMEICON_EMPTYKEYFRAME_ACT	= 3;
const int FRAMEICON_TWEEN				= 4;
const int FRAMEICON_BEGINTWEEN			= 5;
const int FRAMEICON_ENDTWEEN			= 6;
const int FRAMEICON_EMPTYTWEEN			= 7;
const int FRAMEICON_LABEL				= 8;


//	画面要素色
const COLORREF COLOR_OF_HILIGHT = GetSysColor(COLOR_3DHILIGHT);
const COLORREF COLOR_OF_SHADOW = GetSysColor(COLOR_3DDKSHADOW);
const COLORREF COLOR_OF_HDR = GetSysColor(COLOR_BTNFACE);
const COLORREF COLOR_OF_HDRTXT = GetSysColor(COLOR_WINDOWTEXT);

const COLORREF COLOR_OF_LAYER = GetSysColor(COLOR_BTNFACE);
const COLORREF COLOR_OF_LAYER_SEL = RGB(0x00,0x00,0x80);
const COLORREF COLOR_OF_LAYERTEXT = GetSysColor(COLOR_WINDOWTEXT);
const COLORREF COLOR_OF_LAYERTEXT_SEL = RGB(0xFF,0xFF,0xFF);

const COLORREF COLOR_OF_PLAY = RGB(0x00,0x00,0xFF);
const COLORREF COLOR_OF_KEY = RGB(0x00,0x00,0x00);

const COLORREF COLOR_OF_FRAME_END = RGB(0x80,0x00,0x00);

const COLORREF COLOR_OF_KEYE_FILL = RGB(0xFF,0xFF,0xFF);
const COLORREF COLOR_OF_KEYE_FRAM = RGB(0x00,0x00,0x00);

const COLORREF COLOR_OF_ACT = RGB(0xFF,0x00,0x00);
const COLORREF COLOR_OF_LAB = RGB(0xFF,0xDF,0x00);

const COLORREF COLOR_OF_GRID  = RGB(0x9C,0x9C,0x9C);
const COLORREF COLOR_OF_CELL_SEL = RGB(0x00,0x00,0x80);
const COLORREF COLOR_OF_CELL_FILL  = RGB(0xFF,0xFF,0xFF);
const COLORREF COLOR_OF_CELLX_FILL = RGB(0xCC,0xCC,0xCC);

const COLORREF COLOR_OF_CELLV_FILL = RGB(0xFF,0xFF,0xFF);
const COLORREF COLOR_OF_CELLT_FILL = RGB(0x7F,0x5F,0xFF);

const COLORREF COLOR_OF_CELLV_FRAME = RGB(0x00,0x00,0x00);
