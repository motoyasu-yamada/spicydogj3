
#if defined(DOJA)
	#if defined(DOJA2M)
		#define APPLICATION	MApplication
	#else
		#define APPLICATION IApplication
	#endif
	#define APPLICATION_START() public void start()
#elif defined(JPHONE)
	#if !defined(CONCIERGE)
		#define APPLICATION MIDlet
	#else
		#define APPLICATION ResidentMIDlet
	#endif
	#define APPLICATION_START() public void startApp()
#else
	#error unsupported configuration.
#endif

public class spicydog extends APPLICATION {

	APPLICATION_START() {
		try {
			TRACE("Application Booted.");
			Screen.initApp(this);
			TRACE("Application Inited.");
			Screen.play(getArgs()[0],hex2String(getArgs()[1]));
		} catch(Exception e) {
			EXCEPTION(e);
		}
	}

#if defined(JPHONE)
	public void pauseApp() {
	}
	public void destroyApp(boolean unconditional) {
	}
	private String[] getArgs() {
		return new String[]{getAppProperty("sdc"),getAppProperty("title")};
	}
#else
	#error
#endif

#if defined(DOJA2M)
	public void resume() {
		if (Screen.MA_STATE == MA_STATE_SLEEP) {
			Screen.MA_STATE = MA_STATE_DEACTIVE;
		}
	}
#endif

#if defined(DOJA2M)
	public void processSystemEvent(int type,int param) {
		switch (type) {
		case MApplication.CLOCK_TICK_EVENT:
			Screen.EVENT_QUEUE |= EVENT_CLOCK;
			DUMP("+CLOCK");
			break;
		case MApplication.WAKEUP_TIMER_EVENT:
			Screen.EVENT_QUEUE |= EVENT_WAKEUP;
			DUMP("+WAKEUP");
			break;
		case MApplication.MODE_CHANGED_EVENT:
			Screen.waitKeypadClear();
			Screen.MA_STATE    = MA_STATE_ACTIVE;
			Screen.EVENT_QUEUE |= EVENT_ACTIVATED;
			DUMP("+ACTIVATED");
			return;
		case MApplication.FOLD_CHANGED_EVENT:
			if (param == 1) {
				DUMP("+FOLDING_OPEN");
				if (Screen.MA_STATE == MA_STATE_SLEEP) {
					Screen.MA_STATE = MA_STATE_DEACTIVE;
				}
				Screen.EVENT_QUEUE |= EVENT_FOLDING_OPEN;
			} else {
				DUMP("+FOLDING_CLOSE");
				Screen.EVENT_QUEUE |= EVENT_FOLDING_CLOSE;
			}
			return;
		}
		Screen.MA_STATE = MA_STATE_DEACTIVE;
	}
#endif

	/**
		UNICODE文字列を16進数エンコードしたものをデコードする
		@param strHex (例) F8Ab32 ビッグエンディアン
	*/
	private static String hex2String(String strHex) {
#if defined(DOJA2S) || defined(DOJA2M)
		final int len = strHex.length() / 4;
		char ac[] = new char[len];
		for (int n = 0;n < len;n++) {
			final int start = n * 4;
			ac[n] = (char)(Integer.valueOf(strHex.substring(start,start+4),16).intValue());
		}
		return new String(ac,0,len);
#else
		return null;
#endif
	}


}
