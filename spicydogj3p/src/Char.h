/** 画像キャラクタ
    
    @author     m.yamada
    @version    v 2.1.0-2001/08/13 */
private static class Char {
    private Char    INST_CLASSTYPE;
    protected int   CLASSTYPE;
    private String  PS0;
    private String  PS1;
    private int     PI0;
    private int     PI1;
    private int     PI2;
    private int     PI3;
    private int     PI4;
    private int     PI5;
    private int     PI6;
    private boolean PB0;
    private boolean PB1;
    private Image   PIG0;
    
    /* 基底処理 */
    public Char(int n) {
        CLASSTYPE = n;
    }
    
    public Char newInst(AffineMatrix matrix,Coloring coloring) throws Exception {
        switch(CLASSTYPE) {
        case CHARID_PICT:
            return newPict(matrix,coloring);
        case CHARID_TEXT:
            return newText(matrix,coloring);
        case CHARID_BUTTON:
            return newBtn(matrix,coloring);
        }
        ASSERT(false);
        return null;
    }
    
    public void deleteInst() {
        switch(CLASSTYPE) {
        case CHARID_BUTTON:
            deleteBtn();
            return;
        case CHARID_PICT:
            deletePict();
            return;
        case CHARID_TEXT:
            return;
        }
        ASSERT(false);
    }
    
    public void paintInst(Graphics g) {
        switch(CLASSTYPE) {
        case CHARID_PICT:
            paintPict(g);
            return;
        case CHARID_TEXT:
            paintText(g);
            return;
        case CHARID_BUTTON:
            paintBtn(g);
            return;
        }
        ASSERT(false);
    }
    
    public void defineClass() throws Exception {
        TRACE("defineClass(" + CLASSTYPE + ") at" + SpicydogCanvas.POS_READING);
        
        switch(CLASSTYPE) {
        case CHARID_PICT:
            definePict();
            return;
        case CHARID_SOUND:
            defineSnd();
            return;
        case CHARID_TEXT:
            defineText();
            return;
        case CHARID_BUTTON:
            defineBtn();
            return;
        }
        ASSERT(false);
    }
    
    /* BTN : ボタンキャラクタ */
    #define          BTNCLASS_NORMAL_STATE  PI0
    #define          BTNCLASS_ACTIVE_STATE  PI1
    #define          BTNCLASS_KEYMAP        PI2
    #define          BTNCLASS_MAILEVENT     PI3
    #define          BTNCLASS_SCHEDULEEVENT PB0
    #define          BTNCLASS_TELEVENT      PB1
    #define          BTNCLASS_SCHEDULEEVENT_CRITERIA PS0
    #define          BTNCLASS_TELEVENT_CRITERIA      PS1
    #define          BTNINST_ACTIVE         PB0

    private int[]    BTNCLASS_ACTIONS;
    private Object[] BTNCLASS_ACTIONPARAMS;
    
    private Char         BTNINST_NORMAL_STATE;
    private Char         BTNINST_ACTIVE_STATE;
    
    private void defineBtn() {
        BTNCLASS_NORMAL_STATE  = SpicydogCanvas.readUI16();
        BTNCLASS_ACTIVE_STATE  = SpicydogCanvas.readUI16();
        BTNCLASS_KEYMAP        = SpicydogCanvas.readUI32();
        BTNCLASS_MAILEVENT     = SpicydogCanvas.readUI8();
        BTNCLASS_SCHEDULEEVENT = SpicydogCanvas.readUI8() == 1;
        if (BTNCLASS_SCHEDULEEVENT) {
            BTNCLASS_SCHEDULEEVENT_CRITERIA = SpicydogCanvas.readString();
        }
        BTNCLASS_TELEVENT      = SpicydogCanvas.readUI8() == 1;
        if (BTNCLASS_TELEVENT) {
            BTNCLASS_TELEVENT_CRITERIA = SpicydogCanvas.readString();
        }
        TRACE("NormalState:" + BTNCLASS_NORMAL_STATE);
        TRACE("ActiveState:" + BTNCLASS_ACTIVE_STATE);
        TRACE("KeyMap     :" + BTNCLASS_KEYMAP);
        TRACE("MailEvent  :" + BTNCLASS_MAILEVENT);
        TRACE("Schedule   :" + BTNCLASS_SCHEDULEEVENT + "," + BTNCLASS_SCHEDULEEVENT_CRITERIA);
        TRACE("TelEvent   :" + BTNCLASS_TELEVENT      + "," + BTNCLASS_TELEVENT_CRITERIA);

        SpicydogCanvas.readActionList();
        BTNCLASS_ACTIONS      = SpicydogCanvas.ACTIONS;
        BTNCLASS_ACTIONPARAMS = SpicydogCanvas.ACTIONPARAMS;
    }

    private void paintBtn(Graphics g) {
        if (BTNINST_ACTIVE) {
            paintActiveBtn(g);
        } else {
            if (BTNINST_NORMAL_STATE != null) {
                BTNINST_NORMAL_STATE.paintInst(g);
            }
        }
    }
    
    private void paintActiveBtn(Graphics g) {
        if (BTNINST_ACTIVE_STATE != null) {
            BTNINST_ACTIVE_STATE.paintInst(g);
        }
    }
    
    private Char newBtn(AffineMatrix matrix,Coloring coloring) throws Exception {
        Char inst = new Char(CHARID_BUTTON);
        inst.INST_CLASSTYPE = this;

        if (BTNCLASS_NORMAL_STATE != 0) {
            inst.BTNINST_NORMAL_STATE = SpicydogCanvas.CHARS[BTNCLASS_NORMAL_STATE].newInst(matrix,coloring);
        }
        
        if (BTNCLASS_ACTIVE_STATE != 0) {
            inst.BTNINST_ACTIVE_STATE = SpicydogCanvas.CHARS[BTNCLASS_ACTIVE_STATE].newInst(matrix,coloring);
        }
        return inst;
    }

    private void deleteBtn() {
        if (BTNINST_NORMAL_STATE  != null) {
            BTNINST_NORMAL_STATE.deleteInst();
        }
        if (BTNINST_ACTIVE_STATE != null) {
            BTNINST_ACTIVE_STATE.deleteInst();
        }
    }
    
    /**
        検索条件(criteria)でカンマ区切りで指定したキーワードが
        text文字列に含まれるかどうか判定。
        @param  criteria
        @param  text
        @return true  - 
                false - 
    */
    private static boolean matches(String criteria,String text) {
        if (criteria == null || 
            criteria.length() == 0) {
            return true;
        }

        int start = 0;
        int find;
        do {
            find = criteria.indexOf(',',start);
            String c1;
            if (find == -1) {
                c1 = criteria.substring(start);
            } else {
                c1 = criteria.substring(start,find);
                start = find + 1;
            }
            if (text.indexOf(c1) != -1) {
                return true;
            }
        } while (find != -1);
        
        return false;
    }
    
    /**
        このボタンがイベントに判定されたかどうかポーリングする
        @param  keystate 現在のキー状態
        @return true  -  ボタンがアクティブになった
                false -  アクティブにはならない
    */
    public boolean pollingEvent(int keystate) {
        ASSERT(CLASSTYPE == CHARID_BUTTON);
        Char classtype = INST_CLASSTYPE;
        CHECK: {
            if ((classtype.BTNCLASS_KEYMAP &  keystate) != 0) {
                break CHECK;
            }

            if (classtype.BTNCLASS_MAILEVENT != 0 && 
                SpicydogCanvas.MAIL_EVENT) {
                DUMP("# Mail Event #");
                int mask = 1 << (SpicydogCanvas.MAIL_EVENT_DETAIL -1);
                if ((mask & classtype.BTNCLASS_MAILEVENT) != 0) {
                    break CHECK;
                }
            }
            if (classtype.BTNCLASS_SCHEDULEEVENT &&
                SpicydogCanvas.SCHEDULED_EVENT   &&
                matches(BTNCLASS_SCHEDULEEVENT_CRITERIA,
                        SpicydogCanvas.SCHEDULED_EVENT_COMMENT)) {
                DUMP("# Schedule Event #");
                break CHECK;
            }
            if (classtype.BTNCLASS_TELEVENT    && 
                SpicydogCanvas.TELEPHONY_EVENT &&
                    (matches(BTNCLASS_TELEVENT_CRITERIA,
                             SpicydogCanvas.TELEPHONY_EVENT_NAME) ||
                     matches(BTNCLASS_TELEVENT_CRITERIA,
                             SpicydogCanvas.TELEPHONY_EVENT_NUMBER))) {
                DUMP("# Tel Event #");
                break CHECK;
            }
            return false;
        }
        
        BTNINST_ACTIVE = true;
        redraw();
        BTNINST_ACTIVE = false;
        
        SpicydogCanvas.ACTIONS      = INST_CLASSTYPE.BTNCLASS_ACTIONS;
        SpicydogCanvas.ACTIONPARAMS = INST_CLASSTYPE.BTNCLASS_ACTIONPARAMS;
        SpicydogCanvas.waitKeypadClear();
        SpicydogCanvas.runActionList();
        return true;
    }

    /* SND サウンドクラス */
    public  SOUND_DATA SNDCLASS_MSOUND;
    
    private void defineSnd() {
        int length = SpicydogCanvas.readUI16();
        int pos    = SpicydogCanvas.POS_READING;
        SpicydogCanvas.POS_READING += length;
        
        byte[] buffer = new byte[length];
        System.arraycopy(SpicydogCanvas.BUFFER,pos,buffer,0,length);
        TRACE("" + (char)buffer[0] + "," + (char)buffer[1] + "," + (char)buffer[2] + (char)buffer[3]);

        try {
            SNDCLASS_MSOUND = new SOUND_DATA(buffer);
        } catch(Throwable t) {
            DEBUG_ONLY(t.printStackTrace());
            ASSERT(false);
            Alert alert = new Alert("未対応のﾌｫｰﾏｯﾄ"); 
            alert.setTimeout(Alert.FOREVER); 
            Display.getDisplay(APP).setCurrent(alert,THIS);
            long limit;
            limit = System.currentTimeMillis() + 5000;
            while (System.currentTimeMillis() < limit) {
            }
            APP.notifyDestroyed();
        }
    }

    /* TEXT : テキストキャラクタ */
    #define TEXTCLASS_STRING        PS0
    #define TEXTCLASS_WIDTH     PI0
    #define TEXTCLASS_HEIGHT    PI1
    #define TEXTCLASS_FCOLOR    PI2
    #define TEXTCLASS_FONTFACE  PI3
    #define TEXTCLASS_FONTSIZE  PI4
    #define TEXTCLASS_FONTSTYLE PI5

    #define TEXTINST_FCOLOR     PI0
    #define TEXTINST_LEFT       PI1
    #define TEXTINST_TOP        PI2
    
    /** テキストキャラクタ読み込み　*/
    private void defineText() throws Exception {
        TEXTCLASS_STRING = SpicydogCanvas.readString();
        int[] xy = SpicydogCanvas.readXY();
        TEXTCLASS_WIDTH  = xy[0];
        TEXTCLASS_HEIGHT = xy[1];
        TEXTCLASS_FCOLOR = SpicydogCanvas.readRGB();
        TEXTCLASS_FONTFACE   = SpicydogCanvas.readUI8();
        TEXTCLASS_FONTSIZE   = SpicydogCanvas.readUI8();
        TEXTCLASS_FONTSTYLE  = SpicydogCanvas.readUI8();
        
        TRACE("Text:"   + TEXTCLASS_STRING);
        TRACE("Width:"  + TEXTCLASS_WIDTH);
        TRACE("Height:" + TEXTCLASS_HEIGHT);
        TRACE("FColor:" + TEXTCLASS_FCOLOR);
        TRACE("Face:"   + TEXTCLASS_FONTFACE);
        TRACE("Size:"   + TEXTCLASS_FONTSIZE);
        TRACE("Style:"  + TEXTCLASS_FONTSTYLE);
        
    }

    private Char newText(AffineMatrix matrix,Coloring coloring) {
        Char inst = new Char(CLASSTYPE);
        inst.INST_CLASSTYPE = this;
        
        inst.TEXTINST_FCOLOR = SpicydogCanvas.realizeColor(coloring,TEXTCLASS_FCOLOR);
        inst.TEXTINST_LEFT   = SpicydogCanvas.operatorX(matrix,0,0);
        inst.TEXTINST_TOP    = SpicydogCanvas.operatorY(matrix,0,0);
        return inst;
    }

    private void paintText(Graphics g) {
        ASSERT(g != null);
        String text = INST_CLASSTYPE.TEXTCLASS_STRING;
        ASSERT(text != null);
        int length  = text.length();
        char[] ac = new char[length];
        text.getChars(0,length,ac,0);

        ASSERT(INST_CLASSTYPE.TEXTCLASS_FONTFACE == Font.FACE_MONOSPACE    ||
               INST_CLASSTYPE.TEXTCLASS_FONTFACE == Font.FACE_PROPORTIONAL ||
               INST_CLASSTYPE.TEXTCLASS_FONTFACE == Font.FACE_SYSTEM);
        TRACE("TEXTCLASS_FONTSIZE" + INST_CLASSTYPE.TEXTCLASS_FONTSIZE);
        ASSERT(INST_CLASSTYPE.TEXTCLASS_FONTSIZE == Font.SIZE_LARGE  ||
               INST_CLASSTYPE.TEXTCLASS_FONTSIZE == Font.SIZE_MEDIUM ||
               INST_CLASSTYPE.TEXTCLASS_FONTSIZE == Font.SIZE_SMALL);
        ASSERT((INST_CLASSTYPE.TEXTCLASS_FONTSTYLE &
               (~(Font.STYLE_BOLD | Font.STYLE_ITALIC | Font.STYLE_UNDERLINED)))
               == 0);
        Font font = Font.getFont(
                    INST_CLASSTYPE.TEXTCLASS_FONTFACE,
                    INST_CLASSTYPE.TEXTCLASS_FONTSTYLE,
                    INST_CLASSTYPE.TEXTCLASS_FONTSIZE);
        g.setFont(font);
        g.setColor(TEXTINST_FCOLOR);

        int fontHeight = font.getHeight();
        int start      = 0;
        int y          = TEXTINST_TOP + fontHeight;
        int w          = INST_CLASSTYPE.TEXTCLASS_WIDTH;
        ASSERT(0 < w);

        DEBUG_ONLY(dumpByteArray(text.getBytes()));
        while (0 < length) {
            int breakPos = getLineBreak(font,text,start,length,w);
            int crlfPos  = text.indexOf("\r\n",start);
            boolean bCrLfBreak = crlfPos != -1 && crlfPos < breakPos;
            if (bCrLfBreak) {
                TRACE("CRLF");
                breakPos = crlfPos;
            }
            
            int lineLen = (breakPos - start);
            ASSERT(0 < lineLen && lineLen <= (text.length() - start));
            TRACE("->" + new String(ac,start,lineLen));

            g.drawChars(ac,start,lineLen,TEXTINST_LEFT,y,Graphics.LEFT | Graphics.BOTTOM);
            length -= lineLen;
            if (bCrLfBreak) {
                length -= 2;
                start = breakPos + 2;
            } else {
                start = breakPos;
            }
            ASSERT(length == 0 || 0 <= start   && start   < text.length());
            y += fontHeight;
        }
    }

    private static int getLineBreak(Font font,String str,int start,int length,int width) {
        int pos = start;
        int currentWidth = 0;
        for (int n = 0;n < length;n++) {
            char c = str.charAt(pos);
            currentWidth += font.charWidth(c);
            if (width < currentWidth) {
                break;//return pos;
            }
            pos++;
        }
        TRACE("getLineBreak=" + pos);
        return pos;
    }

    /* PICT 画像キャラクタ */
    #define PICTCLASS_CENTERX  PI0
    #define PICTCLASS_CENTERY  PI1
    #define PICTCLASS_USECOUNT PI2
    #define PICTCLASS_IMAGE    PIG0
    #define PICTCLASS_OFFSET   PI5
    #define PICTCLASS_LENGTH   PI6

    #define PICTINST_LEFT      PI1
    #define PICTINST_TOP       PI0
    #define PICTINST_CREATED   PB0

    private void definePict() throws Exception {
        int[] xy = SpicydogCanvas.readXY();
        PICTCLASS_CENTERX = xy[0];
        PICTCLASS_CENTERY = xy[1];
        PICTCLASS_LENGTH  = SpicydogCanvas.readUI16();
        PICTCLASS_OFFSET  = SpicydogCanvas.POS_READING;
        SpicydogCanvas.POS_READING += PICTCLASS_LENGTH;
        
        TRACE("CenterX:" + PICTCLASS_CENTERX);
        TRACE("CenterY:" + PICTCLASS_CENTERY);
        TRACE("Length:"  + PICTCLASS_LENGTH);
        TRACE("Offset:"  + PICTCLASS_OFFSET);

        PICTINST_CREATED   = false;
    }
    
    /** 
        ピクチャクラスからインスタンスを生成します。
        ただしリソースのロード等の高負荷作業は、
        実際にインスタンスがリソースを必要とされるまで
        遅延される可能性があります。
        
        (注意)同一クラスに対して非スレッドセーフです。
        @see PICTCLASS_USECOUNT このクラスのインスタンス数をカウントしています
        @see newInst
        @see deletePict
        @see createPict
        @see destroyPict
    */
    private Char newPict(AffineMatrix matrix,Coloring coloring) throws Exception {
        Char inst = new Char(CLASSTYPE);
        inst.INST_CLASSTYPE = this;
        inst.PICTINST_LEFT = SpicydogCanvas.operatorX(matrix,0,0) - PICTCLASS_CENTERX;
        inst.PICTINST_TOP  = SpicydogCanvas.operatorY(matrix,0,0) - PICTCLASS_CENTERY;
        PICTCLASS_USECOUNT++;
        return inst;
    }
    
    /** 
        インスタンスを削除
        (注意)同一クラスに対して非スレッドセーフ
        
        @see PICTCLASS_USECOUNT クラスのインスタンス数をカウントしています
                                deletePictメソッドでデクリメントされ、
                                0になると、リソースが破棄されます。
    */
    private void deletePict() {
        ASSERT(INST_CLASSTYPE != null);
        if (--INST_CLASSTYPE.PICTCLASS_USECOUNT == 0) {
            if (INST_CLASSTYPE.PICTINST_CREATED) {
                INST_CLASSTYPE.PICTINST_CREATED = false;
                ASSERT(INST_CLASSTYPE.PICTCLASS_IMAGE  != null);
                destoryPict();
            }
        }
    }
    
    /**
        クラスの同一インスタンスが共通するリソースを破棄します
        (注意)同一クラスに対して非スレッドセーフ
        @see Char.deleteInst 
    */
    private void destoryPict() {
        Char cls = INST_CLASSTYPE;
        cls.PICTCLASS_IMAGE  = null;
        TRACE("PICT:DESTROY #" + cls);
        System.gc();
    }
    
    /**
        同一クラスのインスタンスが共有するリソースを実際にロードします
        @see Char.paintPict */
    private Image createPict() {
        Char c = INST_CLASSTYPE;
        ASSERT(c.PICTCLASS_IMAGE  == null);
        try {
            ASSERT(!c.PICTINST_CREATED);
            ASSERT(SpicydogCanvas.BUFFER != null);
            ASSERT((c.PICTCLASS_OFFSET + c.PICTCLASS_LENGTH) <= SpicydogCanvas.BUFFER.length);
            TRACE("PICT:CREATE #" + c + ":" + SpicydogCanvas.BUFFER + "," + c.PICTCLASS_OFFSET + "," + c.PICTCLASS_LENGTH);
            c.PICTCLASS_IMAGE = Image.createImage(
                                    SpicydogCanvas.BUFFER,
                                    c.PICTCLASS_OFFSET,
                                    c.PICTCLASS_LENGTH);
            c.PICTINST_CREATED = true;
        } catch(Throwable e) {
            EXCEPTION(e);
        }
        return c.PICTCLASS_IMAGE;
    }
    
    /** ピクチャキャラクタを描画する 
        @see Char.createPict */
    public void paintPict(Graphics g) {
        ASSERT(INST_CLASSTYPE.PICTCLASS_USECOUNT != 0);
        Image i = INST_CLASSTYPE.PICTCLASS_IMAGE;
        if (i == null) {
            i = createPict();
        }

        if (i != null) {
            g.drawImage(i,PICTINST_LEFT,PICTINST_TOP,Graphics.TOP | Graphics.LEFT);
        }
    }

} // class Char

