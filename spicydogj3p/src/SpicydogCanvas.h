//  $Id: SpicydogCanvas.h,v 1.17 2003/09/04 14:38:17 m_yamada Exp $

private static class  SpicydogCanvas
    extends    Canvas 
    implements Runnable
#if defined(JPHONE_RESIDENT)
                ,TelephonyListener
                ,MailListener
                ,ScheduledAlarmListener
#if defined(JPHONE_PLATFORM_G)
                ,RingStateListener
#endif
#endif
    {
    private static SpicydogApp    APP;
    private static SpicydogCanvas THIS;
    
    // �f�o�C�X�ˑ��F
    private static int DDCOLOR_WHITE;
    private static int DDCOLOR_BLACK;

    // ��ʑ���
    private static int CANVAS_WIDTH;
    private static int CANVAS_HEIGHT;
    protected static int DRAW_WIDTH;
    protected static int DRAW_HEIGHT;

    protected static Graphics     GRAPHICS;
    protected static SOUND_DATA   ACTIVE_SND;
    private   static SOUND_PLAYER ACTIVE_SND_PLAYER;

    // �h�L�������g���
    private static int MAX_CHAR;
    private static boolean LOOP_PLAY;
    private static int DEFAULT_FRAME_BG;
    private static int DEFAULT_FRAME_BD;
    private static int MPF;

    /** �L�����N�^�E�}�b�v */
    private static Char[] CHARS;
    /** �C���X�^���X�E�}�b�v */
    private static Char[] INSTANCES;
    /** �ő�C���X�^���X�� */
    private static int MAX_INSTANCES;
    /** ���x�� */
    private static Hashtable LABELS;

    /** �Z�J���_���[�E�T�[�t�F�[�X (�_�u���o�b�t�@�����O�`��p) */
    private static Image GRAPHICS_IMAGE;
    /** �x���W�����v�ҋ@�����ǂ��� */
    private static boolean DELAY_JUMP_QUEUE;
    /** �x���W�����v�� */
    private static int DELAY_JUMP_GOTO_FRAME;
    /** �x���W�����v�ҋ@�J�E���g */
    private static long DELAY_JUMP_MARK;
    /** �L�[�p�b�h��� */
    protected static int CURRENT_KEYPAD_STATE;
    private static int FRAME_BG;
    private static long FRAME_START;   // �Đ��J�n����
    private static int FRAME_CURRENT;
    private static boolean PLAYING;
    /** �������� */
    private static Random RANDOMIZER = new Random();
    /** ���v�^�C�v*/
    private static int CLOCK_TYPE;
    /** �f�W�^�����v�`��t�H���g */
    private static Image CLOCK_FONT[];
    /** �f�W�^�����v�`��t�H���g�̐� */
    #define CLOCK_FONT_NUM 19
    /** �f�W�^�����v�̃f���~�^ : �̕`��t�H���g */
    #define CLOCK_FONT_SEP 10
    /** ���v�f�� ��*/
    #define CLOCK_FONT_MONTH 11
    /** ���v�f�� ��*/
    #define CLOCK_FONT_DAY   12
    /** ���v�f�� �j���f��*/
    #define CLOCK_FONT_WEEKDAY 13
    /** ���v��\�����Ȃ� */
    #define CLOCK_NONE          0
    /** �����Ȏ��v��\������ */
    #define DIGITAL_CLOCK_SMALL 1
    /** �傫�ȃf�W�^�����v��\������ */
    #define DIGITAL_CLOCK_LARGE 2
    /** ���v����ʂ̊Ԃ̃}�[�W�� */
    #define CLOCK_OUT_MARGIN 6
    /** ���v�̊e�摜�Ԃ̃}�[�W�� */
    #define CLOCK_IN_MARGIN  2

    protected static boolean SCHEDULED_EVENT;
    protected static String  SCHEDULED_EVENT_COMMENT;
    protected static boolean TELEPHONY_EVENT;
    protected static String  TELEPHONY_EVENT_NAME;
    protected static String  TELEPHONY_EVENT_NUMBER;
    protected static boolean MAIL_EVENT;
    protected static int     MAIL_EVENT_DETAIL;
    private static Command[] SOFT_LABEL = new Command[2]; 
    #define SOFT_KEY_1 (0)
    #define SOFT_KEY_2 (1)

    #include "io.h"
    #include "Coloring.h"
    #include "AffineMatrix.h"
    #include "Char.h"
    #include "Instructions.h"
    #include "Actions.h"

    /**
    *   �v���[���[�Đ��J�n������
    *       �N���e�B�J���ȏ��������s���B
    */
    private static void initPlayer(SpicydogApp app) {
        try {

            APP  = app;
            THIS = new SpicydogCanvas();

            CANVAS_WIDTH      = THIS.getWidth();
            CANVAS_HEIGHT     = THIS.getHeight();

            ACTIVE_SND_PLAYER = GET_SOUND_PLAYER();
            DDCOLOR_WHITE     = GET_COLOR_OF_NAME(COLOR_WHITE);
            DDCOLOR_BLACK     = GET_COLOR_OF_NAME(COLOR_BLACK);
            INSTANCES         = new Char[256];
            DELAY_JUMP_QUEUE   = false;
            initSoftLabel(THIS);
            Display.getDisplay(APP).setCurrent(THIS);
            GRAPHICS_IMAGE    = Image.createImage(CANVAS_WIDTH,CANVAS_HEIGHT);
            GRAPHICS          = GRAPHICS_IMAGE.getGraphics();

            ///  F�^�̏ꍇ�̓��[�����M�p�̉�����Ǎ���
#if defined(JPHONE_PLATFORM_F) && defined(JPHONE_RESIDENT)
            InputStream input = THIS.getClass().getResourceAsStream("system.spf");
            if (input == null) {
                throw new IllegalStateException();
            }
            byte[] buffer = new byte[1024];
            int length    = input.read(buffer,0,1024);
            input.close();
            byte[] phrase = new byte[length];
            System.arraycopy(buffer,0,phrase,0,length);
            SYSTEM_PHRASE = new Phrase(phrase);
            SYSTEM_PHRASE_TRACK = PhrasePlayer.getPlayer().getTrack();
            SYSTEM_PHRASE_TRACK.setPhrase(SYSTEM_PHRASE);
#endif

#if defined(JPHONE_RESIDENT)
            DeviceControl.setMailListener(THIS);
#if defined(JPHONE_PLATFORM_G)
            DeviceControl.setRingStateListener(THIS);
#endif
            DeviceControl.setScheduledAlarmListener(THIS);
            DeviceControl.setTelephonyListener(THIS);
            TRACE("set listener.");
#endif
        } catch(Throwable t) {
            DEBUG_ONLY(t.printStackTrace());
            Alert alert = new Alert("��߲���ޯ��",
                                    "���ع���ݏ��������ɒv���I�װ������" + t.toString(),
                                    null,
                                    AlertType.ERROR);
            alert.setTimeout(Alert.FOREVER);
            Display.getDisplay(app).setCurrent(alert,THIS);
            waitKeypadClear();
            app.destroyApp(false);
            app.notifyDestroyed();
        }
    }

    /**
    *   �X�^���_�[�h�v���[���[�Đ��C���^�[�t�F�[�X
    */
    protected static synchronized void play() {
        DUMP("SpicydogCanvas.play");
        new Thread(THIS).start();
    }

    /**
    *   �Đ����C���X���b�h
    */
    public void run() {
        try {
            loadDocument();
            header();
            defineChars();
            defineLabels();
            runInstructions();
        } catch(Throwable t) {
            Alert alert = new Alert("�Đ��װ",
                                    t.toString(),
                                    null,
                                    AlertType.ERROR);
            alert.setTimeout(Alert.FOREVER);
            Display.getDisplay(APP).setCurrent(alert,THIS);
            EXCEPTION(t);
        }
    }

    private static void loadDocument() throws Exception {
        InputStream input = THIS.getClass().getResourceAsStream("sdp.sdc");
        if (input == null) {
            throw new IllegalStateException();
        }
        DataInputStream dataInput = new DataInputStream(input);
        int length = dataInput.readInt();
        BUFFER = new byte[length];
        dataInput.read(BUFFER,4,length-4);
        input.close();
    }

    /* �h�L�������g������ */
    private static void header() throws Exception {
        /// �w�b�_
        skip(4);
        int[] xy = readXY();
        DRAW_WIDTH  = xy[0];
        DRAW_HEIGHT = xy[1];
        MPF              = 1000 / readUI8();
        DEFAULT_FRAME_BG = readColorOfRGB();
        DEFAULT_FRAME_BD = readColorOfRGB();
        LOOP_PLAY        = readUI8() != 0;
        CLOCK_TYPE       = readUI8();
        TRACE("DrawSize:" + DRAW_WIDTH + "," + DRAW_HEIGHT);
        TRACE("MPF:     " + MPF);
        TRACE("FrameBG :" + DEFAULT_FRAME_BG);
        TRACE("FrameBD :" + DEFAULT_FRAME_BD);
        TRACE("LoopPlay:" + LOOP_PLAY);
        TRACE("ClockType:"+ CLOCK_TYPE);
        initClock();
    }
    
    /**
        ���v�̃��\�[�X�̏��������s��
        �X�p�C�V�[�h�b�O�̎��v�ݒ�Ɉˑ�����̂Ńw�b�_�[����
        �ǂݍ��񂾌�ŌĂԂׂ�
    */
    private static void initClock() throws Exception {
        CLOCK_FONT = null;
        CLOCK_FONT = new Image[CLOCK_FONT_NUM];
        System.gc();

        String prefix = "/L";
        switch(CLOCK_TYPE) {
        case CLOCK_NONE:
            break;
        case DIGITAL_CLOCK_SMALL:
            prefix = "/S";
        case DIGITAL_CLOCK_LARGE:
            for (int n = 0;n < CLOCK_FONT_NUM;n++) {
                CLOCK_FONT[n] = Image.createImage(prefix + n + ".png");
            }
            break;
        }
    }

    private static void defineChars() throws Exception {
        /// �L�����N�^
        MAX_INSTANCES = 0;
        MAX_CHAR      = readUI16();
        TRACE("CharNum :" + MAX_CHAR);
        CHARS   = new Char[MAX_CHAR+1];
        for (int n = 0;n < MAX_CHAR;n++) {
            int charid   = readUI16();
            int chartype = readUI8();
            TRACE("Char:" + charid + "," + chartype + "#" + n);
            Char c = new Char(chartype);
            c.defineClass();
            CHARS[charid] = c;
        }
    }

    private static void defineLabels() {
        LABELS = new Hashtable();
        int labelCount = readUI16();
        TRACE("LabelNum :" + labelCount);
        for (int n = 0;n < labelCount;n++) {
            TRACE("label at " + POS_READING);
            int frame = readUI16();
            int pos   = readUI32();
            TRACE("#" +  n + "," + frame + "," + pos);
            LABELS.put(new Integer(frame),
                       new Integer(pos));
        }
    }

    /*
    *   �C���X�g���N�V�������s���[�v
    */
    private static void runInstructions() throws Exception {
        TRACE("----------------------------------------------------------------");
        TRACE("play at " + POS_READING);
        TRACE("----------------------------------------------------------------");
        
        FRAME_BG       = DEFAULT_FRAME_BG;
        FRAME_START    = System.currentTimeMillis();
        PLAYING        = true;
        FRAME_CURRENT  = 0;

        for (;;) {
            int inst = readUI8();
            //DUMP("#" + FRAME_CURRENT + " @" + POS_READING + ":" + inst);
            switch(inst) {
                case INSTID_NEW_INSTANCE:
                    chunkNewInst();
                    break;
                case INSTID_DEL_INSTANCE:
                    deleteInst();
                    break;
                case INSTID_SHOW_FRAME:
                    chunkShowFrame();
                    break;
                case INSTID_DO_ACTION:
                    chunkDoAction();
                    break;
                case INSTID_END:
                    TRACE("*** END ***");
                    if (LOOP_PLAY) {
                        jumpTo(0);
                    } else {
                        APP.notifyDestroyed();
                        return;
                    }
                    break;
                default:
                    TRACE("inst=" + inst);
                    ASSERT(false);
                    break;
            }
        }
    }
    
    private static void jumpTo(int frame) {
        for (int n = 0;n <= MAX_INSTANCES;n++ ){
            if (INSTANCES[n] == null) {
                continue;
            }
            INSTANCES[n].deleteInst();
            INSTANCES[n] = null;
        }
        MAX_INSTANCES = 0;
        
        ASSERT(LABELS.containsKey(new Integer(frame)));
        POS_READING   =((Integer)LABELS.get(new Integer(frame))).intValue();
        FRAME_CURRENT = frame;
        
        TRACE("jumpTo to " + frame + "=>" + POS_READING);
    }

    private static void redraw() {
        synchronized(THIS) {
            Graphics g = GRAPHICS;
            g.translate(0,0);
            g.setColor(DEFAULT_FRAME_BD);
            g.fillRect(0,0,CANVAS_WIDTH,CANVAS_HEIGHT);
            int x = (CANVAS_WIDTH  - DRAW_WIDTH)/2;
            int y = (CANVAS_HEIGHT - DRAW_HEIGHT)/2;
            g.setClip(x,y,DRAW_WIDTH,DRAW_HEIGHT);

            g.translate(x,y);
            g.setColor(FRAME_BG);
            g.fillRect(0,0,DRAW_WIDTH,DRAW_HEIGHT);

            for (int n = 0; n <= MAX_INSTANCES; n++) {
                Char c = INSTANCES[n];
                if (c != null) {
                    c.paintInst(g);
                }
            }
            
            //  ���v��`��
            drawClock();
            
            //  ���W�n�����ɖ߂�
            g.translate(-x,-y);

            // �f�o�b�O�p�̃_���v�R�����g����ʂɕ`��
            DISPLAY_DUMP(g);

            // �o�b�O�o�b�t�@�[���v���C�}���[�ɓ]������
            THIS.repaint();
        }
    }

    private static void drawClock() {
        if (CLOCK_TYPE == CLOCK_NONE) {
            return;
        }
        Graphics graphics = GRAPHICS;
        ASSERT(graphics != null);
        Calendar current  = Calendar.getInstance();
        ASSERT(current != null);
        Image[]  font     = CLOCK_FONT;
        ASSERT(font != null);
        ASSERT(font.length == CLOCK_FONT_NUM);
        ASSERT(font[0] != null);
        ASSERT(font[CLOCK_FONT_NUM-1] != null);
        int width         = font[0].getWidth() + CLOCK_IN_MARGIN;
        int x             = CLOCK_OUT_MARGIN;
        int height        = font[0].getHeight() + CLOCK_OUT_MARGIN;
        int y             = DRAW_HEIGHT - height * 2;

        int num;
        num = current.get(Calendar.MONTH) + 1;
        graphics.drawImage(font[num / 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        graphics.drawImage(font[num % 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        graphics.drawImage(font[CLOCK_FONT_MONTH],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        num = current.get(Calendar.DAY_OF_MONTH);
        graphics.drawImage(font[num / 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        graphics.drawImage(font[num % 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        graphics.drawImage(font[CLOCK_FONT_DAY],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        num = CLOCK_FONT_WEEKDAY + current.get(Calendar.DAY_OF_WEEK) - 1;
        graphics.drawImage(font[num],x,y,Graphics.LEFT | Graphics.TOP);
        
        x   = CLOCK_OUT_MARGIN;
        y  += height;
        num = current.get(Calendar.HOUR_OF_DAY);
        graphics.drawImage(font[num / 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        graphics.drawImage(font[num % 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        graphics.drawImage(font[CLOCK_FONT_SEP],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        num = current.get(Calendar.MINUTE);
        graphics.drawImage(font[num / 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
        graphics.drawImage(font[num % 10],x,y,Graphics.LEFT | Graphics.TOP);
        x += width;
    }

    /**
    *   �Đ����̉������~����
    */
    protected static void stopSnd() {
        TRACE("stopSnd");
        if (ACTIVE_SND != null) {
            TRACE("* stoped *");
            try {
                ACTIVE_SND_PLAYER.stop();
                ACTIVE_SND_PLAYER.removePhrase();
            } catch(Throwable t) {
                DEBUG_ONLY(t.printStackTrace());
            }
        }
        ACTIVE_SND = null;
    }

    /*
    *   �o�b�N�o�b�t�@�ɕ`�悳��Ă�����e����ʂɕ\������
    *   
    */
    public void paint(Graphics g) {
        synchronized(THIS) {
            if (GRAPHICS_IMAGE == null) {
                return;
            }
            g.drawImage(GRAPHICS_IMAGE,0,0,Graphics.TOP|Graphics.LEFT);
        }
    }

    private static int getKeypadState() {
        return DeviceControl.
                    getDefaultDeviceControl().
                    getDeviceState(DeviceControl.KEY_STATE);
    }
    
    protected static void waitKeypadClear() {
        while (getKeypadState() != 0) {
        }
    }

    /**
    *   �\�t�g�L�[�̏�����
    */
    private static void initSoftLabel(Canvas canvas) { 
        canvas.addCommand(SOFT_LABEL[0] = new Command("",Command.SCREEN,1)); 
        canvas.addCommand(SOFT_LABEL[1] = new Command("",Command.SCREEN,2)); 
    }

    /**
    *   �\�t�g�L�[�̃��x���ݒ�
    */
    private static void setSoftLabel(Canvas canvas,int which,String label) { 
        ASSERT(which == 0 || which == 1); 
        ASSERT(canvas != null); 
        ASSERT(label  != null); 
        TRACE("which = " + which); 
        ASSERT(SOFT_LABEL[which] != null); 
        canvas.removeCommand(SOFT_LABEL[0]); 
        canvas.removeCommand(SOFT_LABEL[1]);

        SOFT_LABEL[which] = new Command(label,Command.SCREEN,which == SOFT_KEY_1 ? 1 : 2); 
        canvas.addCommand(SOFT_LABEL[0]);
        canvas.addCommand(SOFT_LABEL[1]); 
    }

#if defined(JPHONE_RESIDENT)
    private static long EVENT_RAISED;

    /**
    *   ���M�����Đ�����
    */
#if defined(JPHONE_PLATFORM_F)
    /// ���[��/�d�b�̒��M��
    private static Phrase      SYSTEM_PHRASE;
    /// ���[��/�d�b�̒��M���Đ��g���b�N
    private static PhraseTrack SYSTEM_PHRASE_TRACK;

    /*
    *   ���M�������Đ�����
    */
    private static void playSystemSound(int call) {
        stopSystemSound();
        try {
            ASSERT(0 <= call && call <= 255);
            SYSTEM_PHRASE_TRACK.play(call);
        } catch(Throwable t) {
            DEBUG_ONLY(t.printStackTrace());
        }
    }

    /*
    *   ���M�������~����
    */
    private static void stopSystemSound() {
        try {
            SYSTEM_PHRASE_TRACK.stop();
        } catch(Throwable t) {
            DEBUG_ONLY(t.printStackTrace());
        }
    }

#endif

    /**
    *   �X�P�W���[���C�x���g����M����
    */
    public void notice(String comment) {
        DUMP("*schedule notice*");
        DUMP("comment="+comment);
        SCHEDULED_EVENT         = true;
        SCHEDULED_EVENT_COMMENT = comment;
        EVENT_RAISED            = System.currentTimeMillis();
        APP.pauseApp();
        APP.notifyPaused();
    }

    /**
    *   ���[�����M���ɁA
    *   ���M�C�x���g�t���O�𗧂ĂāA���M�����Đ�����B
    */
    public void received(String name,String address,int detail) {
        synchronized(THIS) {
            DUMP("*mail received*");
            DUMP("name="+name);
            DUMP("addr="+address);
            DUMP("deta="+detail);

            MAIL_EVENT        = true;
            MAIL_EVENT_DETAIL = detail;
            EVENT_RAISED      = System.currentTimeMillis();

#if defined(JPHONE_PLATFORM_F)
            if (detail != MailListener.RELAY) {
                playSystemSound(4);
            }
#endif
        }
    }
    

    /**
    *   �d�b���������Ă����ꍇ�ɌĂ΂��
    *   @see TelephonyListener.ring
    */
    public void ring(String name,String number) {
        DUMP("*tel ring*");
        DUMP("name=" + name);
        DUMP("number=" + number);
    
        TELEPHONY_EVENT = true;
        TELEPHONY_EVENT_NAME   = name;
        TELEPHONY_EVENT_NUMBER = number;
        EVENT_RAISED           = System.currentTimeMillis();
        APP.pauseApp();
        APP.notifyPaused();
    }

    /**
    *   �d�b���؂ꂽ�ꍇ�ɌĂ΂��B
    *   @see TelephonyListener.ignored
    */
    public void ignored() {
        DUMP("ignored");
    }

#if defined(JPHONE_PLATFORM_G)
    public void ringStarted() {
        DUMP("ringStarted");
    }
    public void ringStopped() {
        DUMP("ringStopped");
    }
#endif

#endif

#if defined(DEBUG)
    #define DUMP_BUFFER_SIZE 16
    private static String[] DUMP_BUFFER = new String[DUMP_BUFFER_SIZE];
    private static int DUMP_BUFFER_POS = 0;

    protected static void dump(String str) {
        TRACE(str);
        DUMP_BUFFER[DUMP_BUFFER_POS] = str;
        DUMP_BUFFER_POS = (DUMP_BUFFER_POS + 1) % DUMP_BUFFER_SIZE;
    }
    
    private static void displayDump(Graphics g) {
        ASSERT(g != null);
        
        Font f = Font.getFont(Font.FACE_SYSTEM,Font.STYLE_PLAIN,Font.SIZE_SMALL);
        int h = f.getHeight();
        g.setFont(f);
        final int DUMP_BUFFER_VISIBLE_LINE = 120 / h;
        for (int n = 0;n < DUMP_BUFFER_VISIBLE_LINE;n++) {
            int l = (DUMP_BUFFER_POS - n + DUMP_BUFFER_SIZE) % DUMP_BUFFER_SIZE;
            ASSERT(0 <= l && l < DUMP_BUFFER_SIZE);
            String str = (0 <= l && l < DUMP_BUFFER_SIZE) ? DUMP_BUFFER[l] : null;
            if (str != null) {
                g.setColor(DDCOLOR_BLACK);
                g.drawString(str,1,n * h + 1,Graphics.TOP|Graphics.LEFT);
                g.setColor(DDCOLOR_WHITE);
                g.drawString(str,0,n * h,Graphics.TOP|Graphics.LEFT);
            }
        }
    }
#endif

#if defined(DEBUG)
    /**
    *   �o�C�g�z����_���v����
    */
    private static void dumpByteArray(byte[] array) {
        int pos;
        pos = 0;
        while(pos < array.length) {
            String str;
            str = "";
            for (int n = 0;n < 16;n++) {
                if (array.length <= pos) {
                    break;
                }
                String hex;
                int    value;
                value = (int)array[pos++];
                if (value < 0) {
                    value += 256;
                }
                if (value < 0x20) {
                    str += "�";
                } else {
                    str += (char)value;
                }
                hex = Integer.toHexString(value);
                if (hex.length() == 1) {
                    hex = "0" + hex;
                }
                System.out.print(hex + " ");
            }
            System.out.println(" " + str);
        }
    }
#endif
}

