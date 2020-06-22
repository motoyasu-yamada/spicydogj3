    /**
        メモリ上の再生バッファよりアクションコードを読み込む。
        グローバルな変数ACTIONS、ACTIONPARAMSにロードされる。
    */
    protected static int[] ACTIONS;
    protected static Object[] ACTIONPARAMS;
    protected static void readActionList() {
        int actionCount = readUI8();
        TRACE("Actions:" + actionCount);
        
        ACTIONS = new int[actionCount];
        ACTIONPARAMS = new Object[actionCount];
        for (int n = 0;n < actionCount;n++) {
            int nID = readUI8();
            TRACE("action(" + nID + ") #" + n);
            
            ACTIONS[n] = nID;
        
            Object ob = null;
            switch (nID) {
            case ACTIONID_JUMP:
            case ACTIONID_SOUND_PLAY:
                ob = new Integer(readUI16());
                break;
            case ACTIONID_SET_BACKLIGHT:
            case ACTIONID_SET_VIBRATION:
                ob = new Integer(readUI8());
                break;
            case ACTIONID_SET_BACKGROUND_COLOR:
                ob = new Integer(readColorOfRGB());
                break;
            case ACTIONID_SET_SOFTKEY:
                ACTIONS[n]= (readUI8() == 0) ? ACTIONID_SETSOFTKEY1 : ACTIONID_SETSOFTKEY2;
                ob = readString();
                break;
            case ACTIONID_DELAY_JUMP:
                ob = new DelayJumpActParam(readUI32(),readUI16());
                break;
            case ACTIONID_RANDOM_JUMP: {
                int nRnds = readUI8();
                RndJumpActParam[] rap = new RndJumpActParam[nRnds];
                for (int n2 = 0;n2 < nRnds;n2++) {
                    rap[n2] = new RndJumpActParam(readUI8(),readUI16());
                }
                ob = rap;
            }
            break;
            /* 引数無アクション */
            case ACTIONID_PLAY:
            case ACTIONID_STOP:
            case ACTIONID_QUIT:
            case ACTIONID_SOUND_STOP:
                break;
            default:
                TRACE("InvalidAcction " + nID);
                ASSERT(false);
                break;
            }
        
            ACTIONPARAMS[n] = ob;
        }
    }

    /**
        アクションを実行する
    
        @see    ACTIONS
        @see    LAUNCH_SDC_QUEUE
        @see    LAUNCH_SDC_URL
    */
    protected static void runActionList() {
        final int actionCount = ACTIONS.length;
        for (int n = 0;n < actionCount;n++) {
            final int nID = ACTIONS[n];
            final Object obParam = ACTIONPARAMS[n];
            final int nParam = (obParam instanceof Integer) ? ((Integer)obParam).intValue() : 0;
            switch (nID) {
            case ACTIONID_SETSOFTKEY1:
                setSoftLabel(THIS,SOFT_KEY_1,(String)obParam);
                break;
            case ACTIONID_SETSOFTKEY2:
                setSoftLabel(THIS,SOFT_KEY_2,(String)obParam);
                break;
            case ACTIONID_JUMP:
                TRACE("ACTIONID_JUMP");
                jumpTo(nParam);
                break;
            case ACTIONID_SOUND_PLAY:
                TRACE("ACTIONID_SOUND_SOUND");
                stopSnd();
                if (nParam != 0) {
                    try {
                        ACTIVE_SND = CHARS[nParam].SNDCLASS_MSOUND;
                        ASSERT(ACTIVE_SND != null);
                        SOUND_PLAYER_SET(ACTIVE_SND_PLAYER,ACTIVE_SND);
                        ACTIVE_SND_PLAYER.play();
                    } catch(Throwable t) {
                        DEBUG_ONLY(t.printStackTrace());
                    }
                }
                break;
            case ACTIONID_PLAY:
                TRACE("ACTIONID_PLAY");
                PLAYING = true;
                break;
            case ACTIONID_STOP:
                TRACE("ACTIONID_STOP");
                PLAYING = false;
                break;
            case ACTIONID_SOUND_STOP:
                TRACE("ACTIONID_SOUND_STOP");
                stopSnd();
                break;
            case ACTIONID_QUIT:
                TRACE("ACTIONID_QUIT");
                APP.notifyDestroyed();
                break;
            case ACTIONID_SET_BACKGROUND_COLOR:
                TRACE("ACTIONID_SET_BACKGROUND_COLOR");
                FRAME_BG = nParam;
                break;
            case ACTIONID_SET_VIBRATION:
                TRACE("ACTIONID_SET_VIBRATION");
                if (nParam == 0) {
                    SYSTEM_SET_VIBRATOR_OFF;
                    break;
                }
                SYSTEM_SET_VIBRATOR_ON;
                break;
            case ACTIONID_SET_BACKLIGHT:
                TRACE("ACTIONID_SET_BACKLIGHT");
                if (nParam == 0) {
                    SYSTEM_SET_BACKLIGH_OFF;
                    break;
                }
                SYSTEM_SET_BACKLIGH_ON;
                break;
            case ACTIONID_DELAY_JUMP: {
                TRACE("ACTIONID_DELAY_JUMP");
                DelayJumpActParam p = (DelayJumpActParam) obParam;
                long n2 = p.DelayJumpActParamDelay;
                if (n2 == 0) {
                    DELAY_JUMP_QUEUE = false;
                    ASSERT(p.DelayJumpActParamFrameToJump == 0);
                } else {
                    DELAY_JUMP_QUEUE      = true;
                    DELAY_JUMP_MARK       = n2 + System.currentTimeMillis();
                    DELAY_JUMP_GOTO_FRAME = p.DelayJumpActParamFrameToJump;
                }
                break;
            }
        
            case ACTIONID_RANDOM_JUMP: {
                TRACE("ACTIONID_RANDOM_JUMP");
                RndJumpActParam[] p = (RndJumpActParam[]) obParam;
                int nRnd = RANDOMIZER.nextInt();
                nRnd = ((nRnd < 0) ? -nRnd : nRnd) % 256;
                int c = p.length;
                for (int nI = 0;nI < c;nI++) {
                    RndJumpActParam p1 = p[nI];
                    if (nRnd <= p1.RndJumpActParamCase) {
                        jumpTo(p1.RndJumpActParamFrameToJump);
                        break;
                    } // if
                } // for
                break;
            } // case ACTIONID_RANDOMGOTOFRAME:
            } // switch
        } // for
    }


