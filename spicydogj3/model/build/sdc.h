#pragma once

/** ピクチャキャラクタ */
#define CHARID_PICT   1
/** サウンドキャラクタ */
#define CHARID_SOUND  2
/** ボタンキャラクタ */
#define CHARID_BUTTON 3
/** テキストキャラクタ */
#define CHARID_TEXT   4

/** ジャンプ アクションID */
#define ACTIONID_JUMP                  1
/** 再生アクション アクションID */
#define ACTIONID_PLAY                  2 
/** 終了アクション アクションID */
#define ACTIONID_QUIT                  3
/** 一時停止アクション アクションID */
#define ACTIONID_STOP                  4
/** 音声再生停止 アクションID */
#define ACTIONID_SOUND_STOP            5
/** 音声再生開始 アクションID */
#define ACTIONID_SOUND_PLAY            6
/** バックライト設定 アクションID */
#define ACTIONID_SET_BACKLIGHT         7
/** バイブレーション設定アクションID */
#define ACTIONID_SET_VIBRATION         8
/** ソフトキー設定アクションID */
#define ACTIONID_SET_SOFTKEY           9
/** 背景色設定アクションID */
#define ACTIONID_SET_BACKGROUND_COLOR 10
/** 遅延ジャンプのアクションID */
#define ACTIONID_DELAY_JUMP           11
/** ランダムジャンプのアクションID */
#define ACTIONID_RANDOM_JUMP          12

/** 新規インスタンス生成 */
#define INSTID_NEW_INSTANCE    0x01
/** インスタンス削除 */
#define INSTID_DEL_INSTANCE    0x02
/** アクション実行 */
#define INSTID_DO_ACTION       0x04
/** フレーム表示 */
#define INSTID_SHOW_FRAME      0x00
/** 終了 */
#define INSTID_END             0xFF
