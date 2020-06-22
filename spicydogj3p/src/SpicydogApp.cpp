/**

    @todo   �f�W�^�����v�̏㏑���`��
    @todo   ���[����M�C�x���g�C�x���g
    @todo   �d�b��M�C�x���g
    @todo   �X�P�W���[���C�x���g
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

/// �v���b�g�t�H�[������
#if   defined(JPHONE_F) || defined(JPHONE_FM)
    #define JPHONE_PLATFORM_F
#elif defined(JPHONE_G) || defined(JPHONE_GM)
    #define JPHONE_PLATFORM_G
#else
    #error invalid configuration.
#endif

/* �f�o�b�O�R�[�h */
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
    *   startApp��2��ڈȍ~�̍ĊJ�ďo�����o����
    *   true  
    *   false ���߂Ă� startApp�̌Ăяo���ł���
    */
    private static boolean IS_RESUMED;
    
    /**
    *   �N���e�B�J���ȏ��������s���B
    *   TelephonyListener/MailListener�֘A�ŕK�v��
    *   ���\�[�X�̏��������s���B
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

    /** �����l�p�����[�^��2���A�N�V�����̋��ʍ\���� */
    private static class DoubleIntParam {
        protected final int N1;
        protected final int N2;
        DoubleIntParam(int n1,int n2) {
            N1 = n1;
            N2 = n2;
        }
    }
}
