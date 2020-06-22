/**

    @todo   デジタル時計の上書き描画
    @todo   メール受信イベントイベント
    @todo   電話受信イベント
    @todo   スケジュールイベント
    @todo   
*/

#include "configure.h"

#if defined(JPHONE_F) || defined(JPHONE_G)
    #define JPHONE_NORESIDENT
#elif defined(JPHONE_FM) || defined(JPHONE_GM)
    #define JPHONE_RESIDENT
#else
    #error invalid configuration.
#endif

/// プラットフォーム判別
#if   defined(JPHONE_F) || defined(JPHONE_FM)
    #define JPHONE_PLATFORM_F
#elif defined(JPHONE_G) || defined(JPHONE_GM)
    #define JPHONE_PLATFORM_G
#else
    #error invalid configuration.
#endif

/* デバッグコード */
#ifdef DEBUG
    #define DUMP(s) SpicydogCanvas.dump(s)
    #define DUMP_IF(b,s) do { if (b) {DUMP(s);} } while (false)
    #define DISPLAY_DUMP(g) SpicydogCanvas.displayDump(g)
#else
    #define DUMP(s)
    #define DUMP_IF(b,s)
    #define DISPLAY_DUMP(g)
#endif

#include "import.h"
#include "sdc.H"
#include "debug/debug.h"
#include "env/color.h"
#include "env/key.h"
#include "env/device.h"
#include "env/dialog.h"
#include "env/sounds.h"

#define DelayJumpActParam            DoubleIntParam
#define DelayJumpActParamDelay       N1
#define DelayJumpActParamFrameToJump N2

#define RndJumpActParam              DoubleIntParam
#define RndJumpActParamCase          N1
#define RndJumpActParamFrameToJump   N2

public class SpicydogApp  extends MIDlet {
    /**
    *   startAppの2回目以降の再開呼出を検出する
    *   true  
    *   false 初めての startAppの呼び出しである
    */
    private static boolean IS_RESUMED;
    
    /**
    *   クリティカルな初期化を行う。
    *   TelephonyListener/MailListener関連で必要な
    *   リソースの初期化を行う。
    */
    public SpicydogApp() {
        SpicydogCanvas.initPlayer(this);
    }

    /**
    *   @see MIDlet.startApp
    */
    public void startApp() {
        synchronized(this) {
            if (IS_RESUMED) {
                DUMP("startApp resumed");
                return;
            }
            IS_RESUMED = true;
            DUMP("startApp start");
        }
        SpicydogCanvas.play();
    }

    public void pauseApp() {
#if defined(JPHONE_PLATFORM_F) && defined(JPHONE_RESIDENT)
        SpicydogCanvas.stopSystemSound();
#endif
    }
    
    public void destroyApp(boolean unconditional) {
#if defined(JPHONE_PLATFORM_F) && defined(JPHONE_RESIDENT)
        SpicydogCanvas.stopSystemSound();
#endif
    }
    
    #include "SpicydogCanvas.h"

    /** 整数値パラメータを2つ持つアクションの共通構造体 */
    private static class DoubleIntParam {
        protected final int N1;
        protected final int N2;
        DoubleIntParam(int n1,int n2) {
            N1 = n1;
            N2 = n2;
        }
    }
}
