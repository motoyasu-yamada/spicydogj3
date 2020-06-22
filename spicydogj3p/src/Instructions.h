
    private static void chunkShowFrame() {
        final long waitFor = FRAME_START + MPF;
        for(;;) {
            redraw();
            if (DELAY_JUMP_QUEUE && DELAY_JUMP_MARK <= System.currentTimeMillis()) {
                jumpTo(DELAY_JUMP_GOTO_FRAME);
                DELAY_JUMP_QUEUE = false;
                break;
            }
            if (PLAYING && waitFor <= System.currentTimeMillis()) {
                break;
            }
            int keystate = getKeypadState();
#if defined(DEBUG)
                if (keystate == (1 << KEY_POUND) ) {
                    DUMP("*TEST-EVENT*");
                    MAIL_EVENT              = true;
                    MAIL_EVENT_DETAIL       = 127;
                    TELEPHONY_EVENT         = true;
                    SCHEDULED_EVENT_COMMENT = "comment";
                    SCHEDULED_EVENT         = true;
                    TELEPHONY_EVENT_NAME    = "name";
                    TELEPHONY_EVENT_NUMBER  = "number";
                }
#endif
            for (int n = 0;n <= MAX_INSTANCES;n++) {
                Char c = INSTANCES[n];
                if (c != null && c.CLASSTYPE == CHARID_BUTTON) {
                    if (c.pollingEvent(keystate)) {
#if defined(JPHONE_RESIDENT)
                        MAIL_EVENT      = false;
                        TELEPHONY_EVENT = false;
                        SCHEDULED_EVENT = false;
#endif
                        redraw();
                        break;
                    }
                }
            }
        }
#if defined(JPHONE_RESIDENT)
        if ((EVENT_RAISED + 2000) < System.currentTimeMillis()) {
            MAIL_EVENT      = false;
            TELEPHONY_EVENT = false;
            SCHEDULED_EVENT = false;
        }
#endif
        FRAME_CURRENT++;
        FRAME_START     = System.currentTimeMillis();
    }

    private static void deleteInst() {
        int instid = readUI8();
        ASSERT(instid <= 255);
        if (INSTANCES[instid] == null) {
            return;
        }
        INSTANCES[instid].deleteInst();
        INSTANCES[instid] = null;
    }

    private static void chunkNewInst() throws Exception {
        int instid = readUI8();
        int charid = readUI16();
        if (MAX_INSTANCES < instid) {
            MAX_INSTANCES = instid;
        }
        Char old = INSTANCES[instid];
        INSTANCES[instid] = CHARS[charid].newInst(newMATRIX(),newColoring());
        if (old != null) {
            old.deleteInst();
        }
    }

    private static void chunkDoAction() {
        readActionList();
        runActionList();
    }
