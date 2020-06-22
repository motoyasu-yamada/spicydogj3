#if define(DOJA)
#define APPLICATION_TERMINATE(a) a.terminate()
#define GET_KEY_PAD_STATE()      CANVAS.getKeypadState()

#elif defined(JPHONE)
#define APPLICATION_TERMINATE(a) a.notifyDestroyed()
#define GET_KEY_PAD_STATE()      DeviceControl.getDefaultDeviceControl().getDeviceState(DeviceControl.KEY_STATE)

#else 
#error unsupported.
#endif

//#define USE_LAUNCH_SDC

class  Screen extends Canvas {
	DEFINE_GPRAHICS;
	
	private static String APPNAME;
	// デバイス依存色
	private static int DDCOLOR_WHITE;
	private static int DDCOLOR_BLACK;

	// 画面属性
	private static int CANVAS_WIDTH;
	private static int CANVAS_HEIGHT;
	protected static int DRAW_WIDTH;
	protected static int DRAW_HEIGHT;

	// 既存オブジェクト
	protected static Screen CANVAS;
	protected static Graphics GRAPHICS;
#if defined(JPHONE)
	private static Image GRAPHICS_IMAGE;
#endif
	protected static SOUND_DATA   ACTIVE_SND;
	private   static SOUND_PLAYER ACTIVE_SND_PLAYER;
	static private spicydog A;

	// ドキュメント情報
	private static int MAX_CHAR;
	private static int MAX_INST;
	private static boolean LOOP_PLAY;
	private static int DEFAULT_FRAME_BG;
	private static int DEFAULT_FRAME_BD;
	private static int MPF;

	// 再生管理
	public static Char[] LIST_CLASS;
	private static Char[] LIST_INST;
	private static int FRAME_BG;
	private static long FRAME_START;   // 再生開始時間
	private static int FRAME_TO_JUMP;
	private static int FRAME_CURRENT;
	private static boolean JUMPING;
	private static boolean PLAYING;
#if defined(DOJA2M) || defined(DOJA2S)
	private static String  SDC_DOWNLOAD_TYPE;
	private static boolean LAUNCH_SDC_QUEUE;
	private static String  LAUNCH_SDC_URL;
#endif

#if defined(DOJA2M)
	protected static int MA_STATE;
	private static boolean AS_CONCIERGE;
#endif
#if defined(DOJA2M) || defined(DOJA2S)
	private static int LAUNCHTYPE;
#endif

	// チャンク読込
	private static int CHUNKTYPE;
	private static int CHUNKSIZE;
	private static int CHUNKBEGIN_POS;
	private static int CHUNK_END_POS;

	// ダブルID読込
	public static int F;
	public static int S;

	// 読込バッファー
	private static int BUFFER_SIZE;
	public static byte[] BUFFER;
	private static int POS_WRITTEN;
	public static int POS_READING;

	// ビットストリーム読込
	private static int READING_BYTE;
	private static int ALREADY_READ_BITS;
	protected static String URL_SCRATCHPAD_BUFFER;

#if defined(DOJA2M) || defined(DOJA2S)
	/* 遅延ジャンプパラメータ */
	private static boolean DELAY_JUMP_QUEUE;
	private static long    DELAY_JUMP_MARK;
	private static int     DELAY_JUMP_GOTO_FRAME;
#endif

	private static Random RANDOMIZER = new Random();

#if defined(DOJA2M) || defined(DOJA2S)
	private static Image WAIT_CURSOR;
	private static Image IMG_PROGRESS_BKG;
	private static Image IMG_PROGRESS_BAR;
#endif

#if defined(DEBUG) && defined(DOJA)
	#define DUMP_BUFFER_SIZE 16
	private static String[] DUMP_BUFFER = new String[DUMP_BUFFER_SIZE];
	private static int DUMP_BUFFER_POS = 0;

	protected static void dumpAllState() {
		TRACE("ct:" + CHUNKTYPE);
		TRACE("jm:" + JUMPING);
		TRACE("pl:" + PLAYING);
	
		int nNumberOfInst = 0;
		int nNumberOfPictInst = 0;
		for (int n = 0;n < LIST_INST.length;n++) {
			Char i = LIST_INST[n];
			if (i != null) {
				if (i.CLASSTYPE == CHUNKTYPE_DEFINEPICT) {
					nNumberOfPictInst++;
				}
				nNumberOfInst++;
			}
		}
		TRACE("ni:" + nNumberOfInst);
		TRACE("np:" + nNumberOfPictInst);
	
		GRAPHICS.setColor(DDCOLOR_BLACK);
		GRAPHICS.fillRect(0,0,CANVAS_WIDTH,CANVAS_HEIGHT);
	}

	protected static void dump(String str) {
		TRACE(str);
		DUMP_BUFFER[DUMP_BUFFER_POS] = str;
		DUMP_BUFFER_POS = (DUMP_BUFFER_POS + 1) % DUMP_BUFFER_SIZE;
	}

	private static void displayDump(Graphics g) {
		ASSERT(g != null);
	
		Font f = Font.getFont(Font.STYLE_PLAIN | Font.SIZE_SMALL | Font.FACE_SYSTEM);
		int h = f.getHeight();
		g.setFont(f);
		final int DUMP_BUFFER_VISIBLE_LINE = 120 / h;
		for (int n = 0;n < DUMP_BUFFER_VISIBLE_LINE;n++) {
			int l = (DUMP_BUFFER_POS - n + DUMP_BUFFER_SIZE) % DUMP_BUFFER_SIZE;
			ASSERT(0 <= l && l < DUMP_BUFFER_SIZE);
			String str = (0 <= l && l < DUMP_BUFFER_SIZE) ? DUMP_BUFFER[l] : null;
			if (str != null) {
				g.setColor(DDCOLOR_BLACK);
				g.drawString(str,1,n * h + 1);
				g.setColor(DDCOLOR_WHITE);
				g.drawString(str,0,n * h);
			}
		}
	}
#endif

	/* プレーヤーの総初期化 */
	protected static void initApp(spicydog a) throws Exception {
		A = a;
#if defined(DOJA2M) || defined(DOJA2S)
		LAUNCHTYPE = A.getLaunchType();
#endif
#if defined(DOJA2M)
		MA_STATE = (AS_CONCIERGE = (LAUNCHTYPE == MApplication.LAUNCHED_AS_CONCIERGE)) 
					? MA_STATE_DEACTIVE : MA_STATE_ACTIVE;
#endif

		CANVAS = new Screen();
		CANVAS_WIDTH  = CANVAS.getWidth();
		CANVAS_HEIGHT = CANVAS.getHeight();

#if defined(DOJA)
		GRAPHICS = CANVAS.getGraphics();
#elif defined(JPHONE)
		GRAPHICS_IMAGE = Image.createImage(CANVAS_WIDTH,CANVAS_HEIGHT);
		GRAPHICS = GRAPHICS_IMAGE.getGraphics();
#else
		#error
#endif

		ACTIVE_SND_PLAYER = GET_SOUND_PLAYER();
#if defiend(DOJA)
		PhoneSystem.setAttribute(PhoneSystem.DEV_BACKLIGHT,PhoneSystem.ATTR_BACKLIGHT_ON);
#endif
		DDCOLOR_WHITE   = GET_COLOR_OF_NAME(COLOR_WHITE);
		DDCOLOR_BLACK   = GET_COLOR_OF_NAME(COLOR_BLACK);
	
#if defined(DOJA2M) || defined(DOJA2S)
		MediaImage mi;
		(mi = MediaManager.getImage(URL_WAIT)).use();
		WAIT_CURSOR = mi.getImage();
		(mi = MediaManager.getImage(URL_PROG_BKG)).use();
		IMG_PROGRESS_BKG = mi.getImage();
		(mi = MediaManager.getImage(URL_PROG_BAR)).use();
		IMG_PROGRESS_BAR = mi.getImage();
#endif

#if defined(DOJA)
		Display.setCurrent(CANVAS);
#elif defined(JPHONE)
		Display.getDisplay(a).setCurrent(CANVAS);
#else
	#error
#endif
	}

	/**
		スタンダードプレーヤー再生インターフェース
	*/
	protected static void play(String strUrl,String strAppName) throws Exception {
		TRACE("url =" + strUrl);
		TRACE("name=" + strAppName);
		APPNAME        = strAppName;
		initPlayer();
		displaySplash();
#if defined(DOJA1) | defined(JPHONE)
		initDocument(strUrl);
		playDocument();
#elif defined(DOJA2S) | defined(DOJA2M)
		LAUNCH_SDC_URL = strUrl;
		TRACE(strUrl);
		do {
			initDocument(LAUNCH_SDC_URL);
			playDocument();
		} while(LAUNCH_SDC_QUEUE);
#else
	#error unsupported configuration.
#endif
	}
	
	DEFINE_SOFT_LABEL;
	
	/** プレーヤー再生開始初期化
		@return	void */
	private static void initPlayer() {
		INIT_SOFT_LABEL(CANVAS);

		GRAPHICS_SET_BACKGROUND(CANVAS,FRAME_BG = DDCOLOR_WHITE);
#if defined(DOJA2M) || defined(DOJA2S)
		DELAY_JUMP_QUEUE = false;
#endif
	}

	/**
		ロゴをスクロール表示
	*/
	private static void displayLogo(String strPictUrl) {
#if defined(DOJA1) | defined(JPHONE)
		GRAPHICS.setColor(DDCOLOR_WHITE);
		GRAPHICS.fillRect(0,0,CANVAS_WIDTH,CANVAS_HEIGHT);
#else
		GRAPHICS.setColor(DDCOLOR_WHITE);
		GRAPHICS.fillRect(0,0,CANVAS_WIDTH,CANVAS_HEIGHT);
	
		// アプリ名表示
		String strAppName = APPNAME;
		if (strAppName != null) {
			Font f = Font.getFont(Font.SIZE_MEDIUM | Font.FACE_SYSTEM | Font.STYLE_PLAIN);
			GRAPHICS.setFont(f);
			int sw = f.stringWidth(strAppName);
			int x = (CANVAS_WIDTH - sw) / 2;
			int y = 38;
			GRAPHICS.setColor(DDCOLOR_BLACK);
			GRAPHICS_DRAW_STRING(GRAPHICS,strAppName,x+1,y);
			GRAPHICS_DRAW_STRING(GRAPHICS,strAppName,x-1,y);
			GRAPHICS_DRAW_STRING(GRAPHICS,strAppName,x,y+1);
			GRAPHICS_DRAW_STRING(GRAPHICS,strAppName,x,y-1);
	
			GRAPHICS.setColor(DDCOLOR_WHITE);
			GRAPHICS_DRAW_STRING(GRAPHICS,strAppName,x,y);
		}

		try {
			// ロゴ表示
			MediaImage mi;
			(mi = MediaManager.getImage(strPictUrl)).use();
			Image im = mi.getImage();
				int x = (CANVAS_WIDTH - im.getWidth())  / 2;
				int y =  40;
				GRAPHICS_DRAW_IMAGE(GRAPHICS,im,x,y);
			//im.dispose();
			mi.unuse();
			mi.dispose();
		
		} catch(Exception e) {
		}
#endif
	}

	/**
		ダウンロード中プログレスバーの表示
	
		@param	strMsg 現在の処理内容
	*/
	protected static void displayProgress(String strMsg) {
		GRAPHICS_LOCK(GRAPHICS);
#if defined(DOJA1) | defined(JPHONE)
			#define PROGRESS_BAR_WIDTH  100
			#define PROGRESS_BAR_HEIGHT 10

			int x = (CANVAS_WIDTH - PROGRESS_BAR_WIDTH) / 2;
			int y = CANVAS_HEIGHT / 2 - PROGRESS_BAR_HEIGHT;

			GRAPHICS.setColor(DDCOLOR_WHITE);
			GRAPHICS.fillRect(x,y,PROGRESS_BAR_WIDTH,PROGRESS_BAR_HEIGHT);

			GRAPHICS.setColor(DDCOLOR_BLACK);
			int w = ((PROGRESS_BAR_WIDTH-2) * POS_WRITTEN) / BUFFER_SIZE;
			GRAPHICS.fillRect(x+1,y+1,w,PROGRESS_BAR_HEIGHT-2);

#else
			int pw = IMG_PROGRESS_BKG.getWidth();
			int px = (CANVAS_WIDTH - pw) / 2;
			int py = CANVAS_HEIGHT - 25;
			GRAPHICS.drawImage(IMG_PROGRESS_BKG,px,py);
			int pbw = ((pw -2)* POS_WRITTEN) / BUFFER_SIZE;
			GRAPHICS.drawImage(IMG_PROGRESS_BAR,px,py,1,1,pbw,IMG_PROGRESS_BAR.getHeight());

			Font f = Font.getFont(Font.SIZE_SMALL | Font.FACE_SYSTEM | Font.STYLE_PLAIN);
			int sw = f.stringWidth(strMsg);
			int sx = (CANVAS_WIDTH - sw) / 2;
			GRAPHICS.setFont(f);
			GRAPHICS.setColor(DDCOLOR_BLACK);
			GRAPHICS_DRAW_STRING(GRAPHICS,strMsg,sx+1,py-3);
#endif
		GRAPHICS_UNLOCK(GRAPHICS);
	}

#if defined(DOJA2M) | defined(DOJA2S)
	protected static void displayProgress() {
		displayProgress(SDC_DOWNLOAD_TYPE);
	}
#endif

#if defined(DOJA2S) | defined(DOJA2M)
	/**
		URLのプロトコル表記を判別して 適切な InputStream を開く
		@param	strUrl 開きたいURL
				http:// resource:// を指定した絶対URLのみ可能
		@exception	Connector#open
					Connector#openInputStream
					HttpConnection#setRequestMethod
					HttpConnection#connect
					HttpConnection#openInputStream
					が返すexceptionがそのままスローされる
	*/
	private static HttpConnection HTTP_CONNECTION = null;
	private static void cleanupHttpConnection() throws Exception {
		if (HTTP_CONNECTION != null) {
			HTTP_CONNECTION.close();
			HTTP_CONNECTION = null;
		}
	}
	private static InputStream openInputStream(String strUrl) throws Exception {
		if (strUrl.startsWith("r")) {
			return Connector.openInputStream(strUrl);
		} else {
			cleanupHttpConnection();
			HTTP_CONNECTION = (HttpConnection)Connector.open(strUrl, Connector.READ, true);
			HTTP_CONNECTION.setRequestMethod(HttpConnection.GET);
			HTTP_CONNECTION.connect();
			return HTTP_CONNECTION.openInputStream();
		}
	}

	/**
		ドキュメントデータをバッファへダウンロードする
		@param strUrl ドキュメントデータのURL
		@see	S#POS_WRITTEN
		@see	S#BUFFER
		@see	S#BUFFER_SIZE
	*/
	private static int readUI32(InputStream is) throws IOException {
		return (is.read() << 24) + (is.read() << 16) + (is.read() << 8) + is.read();
	}
#endif

#if defined(DOJA)
	private static void downloadDocument(String strBaseUrl,boolean bInstall) throws Exception {
#if defined(DOJA1)
		boolean bRetry;
		do {
			InputStream    is = null;
			HttpConnection cn = null;
			bRetry = false;
			try {
				POS_WRITTEN = 0;
				if (strBaseUrl.startsWith("r")) {
					is = Connector.openInputStream(strBaseUrl);
				} else {
					(cn = (HttpConnection)Connector.open(strBaseUrl, Connector.READ,true)).
					                                setRequestMethod(HttpConnection.GET);
					cn.connect();
					is = cn.openInputStream();
				}
				is.skip(8);
				BUFFER_SIZE = (is.read() << 8) + is.read();
				BUFFER = new byte[BUFFER_SIZE];
				is.skip(6);
			
				while (POS_WRITTEN < BUFFER_SIZE) {
					displayProgress(STR_SDC_DOWNLOAD_INSTALL);
					final int rest = BUFFER_SIZE - POS_WRITTEN;
					is.read(BUFFER,POS_WRITTEN,rest < HTTP_LOAD_SIZE ? rest : HTTP_LOAD_SIZE);
					POS_WRITTEN += HTTP_LOAD_SIZE;
				}
				SDC_LASTMODIFIED = System.currentTimeMillis();
				SDC_STATE        = SDC_STATE_INSTALLED;
			} catch(Exception e) {
				TRACE("*exc*" + e);
				if (showDialog(DIALOG_DIALOG_YESNO,STR_REDOWNLOAD,DIALOG_BUTTON_YES) != DIALOG_BUTTON_YES) 
					term(null);
				bRetry = true;
			}
			try {
				if (is != null) {
					is.close();
				}
				if (cn != null) {
					cn.close();
				} 
			} catch(Exception e) {
			}
		} while(bRetry);
#else
		String strUrl = strBaseUrl;
		InputStream is = null;
		POS_WRITTEN = 0;
		int nSavedStartPos = 0;
		for(;;) {
			try {
				nSavedStartPos    = POS_WRITTEN;
				is = openInputStream(strUrl);
				int nSign    = readUI32(is);
				int nFiles   = is.read();
				int nFileNo  = is.read();
				int nDataLen = readUI32(is);
				int nFileLen = (is.read() << 8) + is.read();
				is.skip(4);

				if (POS_WRITTEN == 0) {
					BUFFER = new byte[BUFFER_SIZE = nDataLen];
				}
		
				while (0 < nFileLen) {
					displayProgress();
					int nRead = (HTTP_LOAD_SIZE < nFileLen) ? HTTP_LOAD_SIZE : nFileLen;
					is.read(BUFFER,POS_WRITTEN,nRead);
					nFileLen -= nRead;
					POS_WRITTEN += nRead;
				}
		
				is.close();

				if (++nFileNo == nFiles) {
					SDC_STATE        = bInstall ? SDC_STATE_INSTALLED : SDC_STATE_REQUIRE_REPARE;
					SDC_LASTMODIFIED = System.currentTimeMillis();
					cleanupHttpConnection();
					return;
				}
				strUrl = strBaseUrl + "." + nFileNo;
			
			} catch(Exception e) {
				TRACE("*exc*" + e);
				if (is != null) {
					is.close();
				}
				if (showDialog(DIALOG_DIALOG_YESNO,STR_REDOWNLOAD,DIALOG_BUTTON_YES) != DIALOG_BUTTON_YES) 
					term(null);
				POS_WRITTEN = nSavedStartPos;
			}
		}
#endif
	}
#endif

#if defined(DOJA)
	/**
		メモリ上のドキュメントデータをスクラッチパッドに保存

		[スクラッチパッド構造]
		(アドレス)	(データ形式)	(データ名)	(説明)
		0x0000		UI32			DataSize	データサイズ
		0x0004		UI8[DataSize]	Data		実データ
	*/
	private static void saveDocumentToSP(boolean bOnlyState) throws Exception {
		OutputStream os = Connector.openOutputStream(URL_SPDOCUMENTCASH);
		DataOutputStream dos = new DataOutputStream(os);
		dos.writeByte(SDC_STATE);
		if (!bOnlyState) {
			dos.writeLong(SDC_LASTMODIFIED);
			dos.writeInt (BUFFER_SIZE);
			int nSkip = SIZE_OF_SCRATCHPAD_HEADER - 13;
			while (0 < nSkip--)
				dos.writeByte(0);
			dos.write(BUFFER,0,BUFFER_SIZE);
		}

		dos.close();
		os.close();
	}
#endif

#if defined(DOJA)
	/*
		スクラッチパッド上のドキュメントをメモリに読込
		@see	S.POS_WRITTEN
		@see	S.BUFFER
		@see	S.BUFFER_SIZE
		@see	S.SDC_LASTMODIFIED
		@see	S.SDC_STATE
	*/
	/// 最後のSDCファイルを更新した日
	private static long SDC_LASTMODIFIED;
	/// スクラッチパッド内のSDCファイルの状態(未インストール,インストール済,要修復,要更新)
	private static int  SDC_STATE;
	private static boolean loadDocumentFromSP() throws Exception {

#if defined(USE_LAUNCH_SDC)
		InputStream is = Connector.openInputStream(URL_SPDOCUMENTCASH);
		DataInputStream dis = new DataInputStream(is);
		SDC_STATE = dis.readByte();
		if (SDC_STATE == SDC_STATE_NOT_INSTALLED) {
			dis.close();
			is.close();
			return false;
		}
		SDC_LASTMODIFIED = dis.readLong();
		POS_WRITTEN = BUFFER_SIZE = dis.readInt();
		BUFFER = new byte[BUFFER_SIZE];
		dis.skipBytes(SIZE_OF_SCRATCHPAD_HEADER - 13);
		is.read(BUFFER,0,BUFFER_SIZE);
		dis.close();
		is.close();
		ASSERT(SDC_STATE == SDC_STATE_INSTALLED);
		return true;
#else
		InputStream is = Connector.openInputStream(URL_SPDOCUMENTCASH);
		DataInputStream dis = new DataInputStream(is);
		SDC_STATE = dis.readByte();
		if (SDC_STATE == SDC_STATE_NOT_INSTALLED ||
		    SDC_STATE == SDC_STATE_REQUIRE_REPARE) {
			SDC_DOWNLOAD_TYPE = (SDC_STATE == SDC_STATE_NOT_INSTALLED) ? 
			                     STR_SDC_DOWNLOAD_INSTALL : STR_SDC_DOWNLOAD_REPARE;
			dis.close();
			is.close();
			return false;
		}
		SDC_LASTMODIFIED = dis.readLong();
		BUFFER = new byte[POS_WRITTEN = BUFFER_SIZE = dis.readInt()];
		dis.skipBytes(SIZE_OF_SCRATCHPAD_HEADER - 13);
		dis.read(BUFFER,0,BUFFER_SIZE);
	
		dis.close();
		is.close();
		return SDC_STATE == SDC_STATE_INSTALLED;
#endif
	}
#endif

	/**
		ドキュメントを再生可能な状態に初期化する
		指定できるドキュメントのパスは
		[リソースファイル絶対パス]				- resource:///xxxxxx.sdc
		[http絶対パス]							- http://xxxxxx/yyyyyy.sdc
		[JAMファイルの親フォルダからの相対パス]	- yyyy.sdc
	
		@strUrl	ドキュメントのパス
		@IApplication.getSourceURL
	*/
	private static void initDocument(String strUrl) throws Exception {
		TRACE("initDocument:url=" + strUrl);
#if defined(DOJA)
		POS_READING = 0;
		POS_WRITTEN = 0;

		/* 相対パスを絶対パスに変換 */
		if (!strUrl.startsWith("http://") && !strUrl.startsWith("resource://")) {
			strUrl = A.getSourceURL() + strUrl;
		}
		#define DOWNLOAD_TO_INSTALL true
		#define DOWNLOAD_TO_JUMP    false
		#define SAVE_ONLY_STATE  true
		#define SAVE_ALL_DATA    false
#if defined(USE_LAUNCH_SDC)
		if (LAUNCH_SDC_QUEUE || !loadDocumentFromSP()) {
			downloadDocument(strUrl,LAUNCH_SDC_QUEUE ? DOWNLOAD_TO_JUMP : DOWNLOAD_TO_INSTALL);
			saveDocumentToSP(SAVE_ALL_DATA);
		}
#else
		if (!loadDocumentFromSP()) {
			downloadDocument(strUrl,DOWNLOAD_TO_INSTALL);
			saveDocumentToSP(SAVE_ALL_DATA);
		}
#endif

#elif defined(JPHONE)
		InputStream input = CANVAS.getClass().getResourceAsStream("sdp.sdc");
		DataInputStream dataInput = new DataInputStream(input);
		POS_WRITTEN = BUFFER_SIZE = dataInput.readInt();
		TRACE("BUFFER_SIZE=" + BUFFER_SIZE);
		BUFFER = new byte[BUFFER_SIZE];
		dataInput.skipBytes(SIZE_OF_SCRATCHPAD_HEADER - 13);
		dataInput.read(BUFFER,0,BUFFER_SIZE);
	
		dataInput.close();
		input.close();
		TRACE("initDocument.inited.");
		return;
#else
		#error

#endif

	}

	/*
		ドキュメントヘッダーを処理
	*/
	private static void parseDocumentHeader() throws Exception {
		if (readUI8() != 0x03)
			throw new Exception(STR_ERR_INVALID_SDC);

		DRAW_WIDTH = readUI8();
		DRAW_HEIGHT= readUI8();
		MPF = 1000 / readUI8();
		DEFAULT_FRAME_BG = readColorOfRGB();
		DEFAULT_FRAME_BD = readColorOfRGB();
		MAX_CHAR  = readUI16();
		MAX_INST = readUI16();
		LOOP_PLAY = readUI8() == 1;
		skip(17);

		LIST_CLASS	 = new Char[MAX_CHAR];
		LIST_INST    = new Char[MAX_INST];
	}

	/**
		ドキュメント再生
	*/
	private static void playDocument() throws Exception {
		TRACE("play");
	
		parseDocumentHeader();
	
		FRAME_BG       = DEFAULT_FRAME_BG;
		CHUNKBEGIN_POS = getPos();
		FRAME_START    = getTime();
		JUMPING = false;
		PLAYING = true;
		FRAME_CURRENT  = 0;
#if defined(USE_LAUNCH_SDC)
		LAUNCH_SDC_QUEUE = false;
#endif
		for (;;) {
#if defined(USE_LAUNCH_SDC)
			if (LAUNCH_SDC_QUEUE) {
				for (int n = 0;n < MAX_INST;n++) {
					deleteInst(n);
				}
				for (int n = 0;n < MAX_CHAR;n++) {
					LIST_CLASS[n] = null;
				}
				System.gc();
				return;
			}
#endif
			if (JUMPING && FRAME_TO_JUMP < FRAME_CURRENT) {
				FRAME_BG       = DEFAULT_FRAME_BG;
				FRAME_CURRENT  = 0;
				setPos(CHUNKBEGIN_POS);
			}
			loadChunkHeader();

			if (JUMPING) {
				if (CHUNKTYPE == CHUNKTYPE_SHOWFRAME) {
					TRACE("j#" + FRAME_CURRENT);
					FRAME_CURRENT++;
				}
				skip(CHUNKSIZE);
			
				if (FRAME_CURRENT == FRAME_TO_JUMP) {
					JUMPING = false;
					for (int n = 0;n < MAX_INST;n++) {
						deleteInst(n);
					}
				}
			} else {
				switch(CHUNKTYPE) {
				case CHUNKTYPE_END:
					if (LOOP_PLAY) {
						FRAME_TO_JUMP = 0;
						JUMPING = true;
					} else
						term(STR_SUCCESS_END);
					break;

				case CHUNKTYPE_SHOWFRAME:
					chunkShowFrame();
					break;

				case CHUNKTYPE_NEWINST:
					chunkNewInst();
					break;

				case CHUNKTYPE_DELETEINST:
					deleteInst(readUI16());
					break;

				case CHUNKTYPE_DEFINEPICT:
				case CHUNKTYPE_DEFINESHAPE:
				case CHUNKTYPE_DEFINEBTN:
				case CHUNKTYPE_DEFINETEXT:
				case CHUNKTYPE_DEFINESND:
#if defined(DOJA2S) || defined(DOJA2M)
				case CHUNKTYPE_DEFINEORGFONT:
#endif
					defineClass(CHUNKTYPE,CHUNKSIZE);
					break;
				case CHUNKTYPE_DOACTION:
					//redraw(); // 2002/07/31 m.yamada ちらつき防止のため削除
					chunkDoAction();
					break;
#if defined(DOJA2S) || defined(DOJA2M)
				default:
					TRACE("*c*" + CHUNKTYPE + "," + CHUNKSIZE);
					ASSERT(false);
					skip(CHUNKSIZE);
					break;
#endif
				} // switch
			} // else
		
			ASSERT(CHUNK_END_POS == getPos());
			System.gc();
		} // for
	}

	private static int readColorOfRGB() {
		return GET_COLOR_OF_RGB(readUI8(),readUI8(),readUI8());
	}

	private static void loadChunkHeader() {
#ifdef DEBUG
		final int nChunkStartPos = getPos();
#endif
		final int n1   = readUI8();
		final int nCHT = n1 >> 6;
		CHUNKTYPE = n1 & 0x3F;
	
		if (nCHT == 0)
			CHUNKSIZE = 0;
		else if (nCHT == 1)
			CHUNKSIZE = readUI8();
		else if (nCHT == 2)
			CHUNKSIZE = readUI16();
	
		ASSERT(nCHT <= 2);
		CHUNK_END_POS = getPos() + CHUNKSIZE;
	}

	protected static int CURRENT_KEYPAD_STATE;
#if defined(DOJA2M) || defined(DOJA2S)
	protected static int CURRENT_EVENT_STATE;
#if !defined(DOJA2M)
	protected static int STATE_FOLDING        = -1;
#endif
	protected static int STATE_MAIL           = -1;
	protected static int EVENT_QUEUE          = 0;

	private static void updateEventState() {
		CURRENT_EVENT_STATE = EVENT_QUEUE;
	
#if !defined(DOJA2M)
		int stateFolding = PhoneSystem.getAttribute(PhoneSystem.DEV_FOLDING);
		if (STATE_FOLDING != -1 && stateFolding != STATE_FOLDING) {
			switch(stateFolding){
			case PhoneSystem.ATTR_FOLDING_CLOSE:
				CURRENT_EVENT_STATE |= EVENT_FOLDING_CLOSE;
				TRACE("+FOLDING_CLOSE");
				break;
			case PhoneSystem.ATTR_FOLDING_OPEN:
				CURRENT_EVENT_STATE |= EVENT_FOLDING_OPEN;
				TRACE("+FOLDING_OPEN");
				break;
			}
		}
		STATE_FOLDING = stateFolding;
#endif

		int stateMail    = PhoneSystem.getAttribute(PhoneSystem.DEV_MAILBOX);
		if (STATE_MAIL != -1 && stateMail != STATE_MAIL) {
			switch(stateMail) {
			case PhoneSystem.ATTR_MAIL_AT_CENTER:
				CURRENT_EVENT_STATE |= EVENT_MAIL_AT_CENTER;
				TRACE("+MAIL_AT_CENTER");
				break;
			case PhoneSystem.ATTR_MAIL_RECEIVED:
				CURRENT_EVENT_STATE |= EVENT_MAIL_RECEIVED;
				TRACE("+MAIL_RECEIVED");
				break;
			case PhoneSystem.ATTR_MAIL_NONE:
				CURRENT_EVENT_STATE |= EVENT_MAIL_NONE;
				TRACE("+MAIL_NONE");
				break;
			}
		}
		STATE_MAIL = stateMail;
	
		EVENT_QUEUE = 0;
	}
#endif

	private static void chunkShowFrame() {
		TRACE("#" + FRAME_CURRENT);
	
		redraw();
		waitFrame(FRAME_START,MPF);
		do {
			
			CURRENT_KEYPAD_STATE = GET_KEY_PAD_STATE();

//			if ((CURRENT_KEYPAD_STATE & (1 << Display.KEY_SOFT2)) !=0) {
//				showDialog(DIALOG_DIALOG_INFO,
//				          "PLAY=" + PLAYING +",MASTATE=" + MA_STATE,
//						  DIALOG_BUTTON_OK);
//			}

			//DUMP_IF(CURRENT_KEYPAD_STATE != 0,"K>"+CURRENT_KEYPAD_STATE);
#if defined(DOJA2M) || defined(DOJA2S)
			updateEventState();
#endif

			for (int n = 0;n < MAX_INST;n++) {
				Char c = LIST_INST[n];
				if (c != null && c.CLASSTYPE == CHUNKTYPE_DEFINEBTN) {
					if (c.pollingEvent()) {
						redraw();
						break;
					}
				}
			} // if
#if defined(DEBUG)
			redraw();
#endif

#if defined(USE_LAUNCH_SDC)
			if (LAUNCH_SDC_QUEUE)
				return;
			if (DELAY_JUMP_QUEUE && DELAY_JUMP_MARK <= getTime()) {
				FRAME_TO_JUMP    = DELAY_JUMP_GOTO_FRAME;
				JUMPING          = true;
				DELAY_JUMP_QUEUE = false;
				break;
			}
#endif
		}
		while (!PLAYING);

		FRAME_CURRENT++;
		FRAME_START = getTime();
	}

	private static void defineClass(int nClassType,int nSize) throws Exception {
		int nClassId = readUI16();
		if (LIST_CLASS[nClassId] == null) {
			(LIST_CLASS[nClassId] = new Char(nClassType)).defineClass();;
		} else {
			skip(nSize - 2 /* sizeof UI16 */);
		}
	}

	private static void deleteInst(int nId) {
		if (LIST_INST[nId] == null)
			return;
		LIST_INST[nId].deleteInst();
		LIST_INST[nId] = null;
	}

	private static void chunkNewInst() throws Exception {
		readDBUI16();
		Char old = LIST_INST[S];
		LIST_INST[S] = LIST_CLASS[F].newInst(new MATRIX(),new CTFORM(getPos() < CHUNK_END_POS));
		if (old != null)
			old.deleteInst();
	}

	private static void redraw() {
		synchronized(CANVAS) {
			GRAPHICS_LOCK(GRAPHICS);

			GRAPHICS.setColor(FRAME_BG);
			GRAPHICS.fillRect(0,0,DRAW_WIDTH,DRAW_HEIGHT);

			for (int n = 0; n < MAX_INST; n++) {
				Char c = LIST_INST[n];
				if (c != null)
					c.paintInst(GRAPHICS);
			}
#if defined(DOJA2M) || defined(DOJA2S)
			if (JUMPING) {
				GRAPHICS.drawImage(WAIT_CURSOR,0,0);
			}
#endif
			GRAPHICS_SET_BACKGROUND(CANVAS,DEFAULT_FRAME_BD);
			int n = CANVAS_WIDTH-DRAW_WIDTH;
			if (0 < n) {
				GRAPHICS_CLEAR_RECT(GRAPHICS,DRAW_WIDTH,0,n,CANVAS_HEIGHT);
			}
			n = CANVAS_HEIGHT-DRAW_HEIGHT;
			if (0 < n) {
				GRAPHICS_CLEAR_RECT(GRAPHICS,0,DRAW_HEIGHT,DRAW_WIDTH,n);
			}
			GRAPHICS_UNLOCK(GRAPHICS);
#if defined(JPHONE)
			CANVAS.repaint();
#endif
		}
	}

	private static void chunkDoAction() {
		readActionList();
		runActionList();
	}

	/**
		メモリ上の再生バッファよりアクションコードを読み込む。
		グローバルな変数ACTIONS、ACTIONPARAMSにロードされる。
	*/
	protected static int[] ACTIONS;
	protected static Object[] ACTIONPARAMS;
	protected static void readActionList() {
		int nActions = readUI8();

		ACTIONS = new int[nActions];
		ACTIONPARAMS = new Object[nActions];
		for (int n = 0;n < nActions;n++) {
			int nID = readUI8();
			ACTIONS[n] = nID;
		
			Object ob = null;
			switch (nID) {
			case ACTIONID_GOTOFRAME:
			case ACTIONID_PLAYSOUND:
			case ACTIONID_SETBKLIGHT:
			case ACTIONID_SETVIBRATION:
				ob = new Integer(readUI16());
				break;
			case ACTIONID_SETWAKEUP:
				ob = new Integer(readUI32());
				break;
			case ACTIONID_SETBKCLR:
				ob = new Integer(readColorOfRGB());
				break;
			
			/* 文字引数型 アクション */
#if defined(DOJA2M) || defined(DOJA2S)
			case ACTIONID_GOTOMOVIE:
			case ACTIONID_LAUNCHBROWSER:
			case ACTIONID_CALLTELPHONE:
				ob = readString();
				break;
#endif
			case ACTIONID_SETSOFTKEY:
				ACTIONS[n]= (readUI8() == 0) ? ACTIONID_SETSOFTKEY1 : ACTIONID_SETSOFTKEY2;
				ob = readString();
				break;
#if defined(DOJA2M) || defined(DOJA2S)
			/* 特別引数型 アクション */
			case ACTIONID_DELAYGOTOFRAME:
				ob = new DelayJumpActParam     (readUI32(),readLabel());
				break;
			case ACTIONID_LAUNCHTYPEGOTOFRAME: 
				ob = new LaunchTypeJumpActParam(readUI8(), readLabel());
				break;
			case ACTIONID_RANDOMGOTOFRAME: {
				int nRnds = readUI8();
				RndJumpActParam[] rap = new RndJumpActParam[nRnds];
				for (int n2 = 0;n2 < nRnds;n2++) {
					rap[n2] = new RndJumpActParam(readUI8(),readLabel());
				}
				ob = rap;
			}
			break;
#endif
			/* 引数無アクション */
			case ACTIONID_PLAYMOVIE:
			case ACTIONID_STOPMOVIE:
			case ACTIONID_STOPSOUND:
			case ACTIONID_QUIT:
				break;
#if defined(DOJA2M) || defined(DOJA2S)
			case ACTIONID_DEACTIVATE:
			case ACTIONID_SLEEP:
			case ACTIONID_RESETWAKEUP:
			case ACTIONID_ENABLECLOCKTICK:
			case ACTIONID_DISABLECLOCKTICK:
#if defined(DOJA2S)
				TRACE("スタンドアロンで利用できないアクション " + nID);
#endif
				break;
#endif
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
	
		@see	ACTIONS
		@see	LAUNCH_SDC_QUEUE
		@see	LAUNCH_SDC_URL
	*/
	protected static void runActionList() {
		final int nActions = ACTIONS.length;
		for (int n = 0;n < nActions;n++) {
			final int nID = ACTIONS[n];
			final Object obParam = ACTIONPARAMS[n];
			final int nParam = (obParam instanceof Integer) ? ((Integer)obParam).intValue() : 0;
			switch (nID) {
			case ACTIONID_SETSOFTKEY1:
				SET_SOFT_LABEL(CANVAS,SOFT_KEY_1,(String)obParam);
				break;
			case ACTIONID_SETSOFTKEY2:
				SET_SOFT_LABEL(CANVAS,SOFT_KEY_2,(String)obParam);
				break;
			case ACTIONID_GOTOFRAME:
				TRACE("ACTIONID_GOTOFRAME");
				FRAME_TO_JUMP = nParam;
				JUMPING = true;
				break;
			case ACTIONID_PLAYSOUND:
				TRACE("ACTIONID_PLAYSOUND");
				stopSnd();
				if (nParam != 0) {
					ACTIVE_SND = LIST_CLASS[nParam].SNDCLASS_MSOUND;
					SOUND_PLAYER_SET(ACTIVE_SND_PLAYER,ACTIVE_SND);
					ACTIVE_SND_PLAYER.play();
				}
				break;
			case ACTIONID_PLAYMOVIE:
				TRACE("ACTIONID_PLAYMOVIE");
				PLAYING = true;
				break;
			case ACTIONID_STOPMOVIE:
				TRACE("ACTIONID_STOPMOVIE");
				PLAYING = false;
				break;
			case ACTIONID_STOPSOUND:
				TRACE("ACTIONID_STOPSOUND");
				stopSnd();
				break;
			case ACTIONID_QUIT:
				TRACE("ACTIONID_QUIT");
				term(null);
				break;
			case ACTIONID_SETBKCLR:
				TRACE("ACTIONID_SETBKCLR");
				FRAME_BG = nParam;
				break;
			case ACTIONID_SETVIBRATION:
				TRACE("ACTIONID_SETVIBRATION");
				if (nParam == 0) {
					SYSTEM_SET_VIBRATOR_OFF;
					break;
				}
				SYSTEM_SET_VIBRATOR_ON;
				break;
			case ACTIONID_SETBKLIGHT:
				TRACE("ACTIONID_SETBKLIGHT");
				if (nParam == 0) {
					SYSTEM_SET_BACKLIGH_OFF;
					break;
				}
				SYSTEM_SET_BACKLIGH_ON;
				break;
#if defined(DOJA2M) || defined(DOJA2S)
			case ACTIONID_GOTOMOVIE:
				TRACE("ACTIONID_GOTOMOVIE");
				LAUNCH_SDC_QUEUE = true;
				LAUNCH_SDC_URL   = (String)obParam;
				break;
			case ACTIONID_CALLTELPHONE:
				TRACE("ACTIONID_CALLTELPHONE");
				Phone.call((String)obParam);
				break;
			case ACTIONID_LAUNCHTYPEGOTOFRAME: {
				TRACE("ACTIONID_LAUNCHTYPEGOTOFRAME");
				LaunchTypeJumpActParam p = (LaunchTypeJumpActParam) obParam;
				if (LAUNCHTYPE == p.LaunchTypeJumpActParamLaunchType) {
					FRAME_TO_JUMP = p.LaunchTypeJumpActParamFrameToJump;
					JUMPING = true;
				}
			}
			break;
			case ACTIONID_DELAYGOTOFRAME: {
				TRACE("ACTIONID_DELAYGOTOFRAME");
				DelayJumpActParam p = (DelayJumpActParam) obParam;
				long n2 = p.DelayJumpActParamDelay;
				if (n2 == 0) {
					DELAY_JUMP_QUEUE = false;
					ASSERT(p.DelayJumpActParamFrameToJump == 0);
				} else {
					DELAY_JUMP_QUEUE      = true;
					DELAY_JUMP_MARK       = n2 + getTime();
					DELAY_JUMP_GOTO_FRAME = p.DelayJumpActParamFrameToJump;
				}
				break;
			}
		
			case ACTIONID_RANDOMGOTOFRAME: {
				TRACE("ACTIONID_RANDOMGOTOFRAME");
				RndJumpActParam[] p = (RndJumpActParam[]) obParam;
				int nRnd = RANDOMIZER.nextInt();
				nRnd = ((nRnd < 0) ? -nRnd : nRnd) % 256;
				int c = p.length;
				for (int nI = 0;nI < c;nI++) {
					RndJumpActParam p1 = p[nI];
					if (nRnd <= p1.RndJumpActParamCase) {
						FRAME_TO_JUMP = p1.RndJumpActParamFrameToJump;
						JUMPING = true;
						break;
					} // if
				} // for
				break;
			} // case ACTIONID_RANDOMGOTOFRAME:
#endif
#if defined(DOJA2S)
			case ACTIONID_LAUNCHBROWSER:
				TRACE("ACTIONID_LAUNCHBROWSER");
				String[] astr = new String[1];
				astr[0] = (String)obParam;
				A.launch(A.LAUNCH_BROWSER,astr);
				break;
#endif
#if defined(DOJA2M)
			case ACTIONID_DEACTIVATE:
				if (AS_CONCIERGE) {
					TRACE("ACTIONID_DEACTIVATE");
					waitKeypadClear();
					A.deactivate();
					MA_STATE = MA_STATE_DEACTIVE;
				} else
					TRACE("通常起動時にDEACTIVATEできない");
				break;
			case ACTIONID_SLEEP:
				if (AS_CONCIERGE && MA_STATE == MA_STATE_DEACTIVE) {
					TRACE("ACTIONID_SLEEP");
					A.sleep();
					MA_STATE = MA_STATE_SLEEP;
				} else {
					TRACE("通常起動時または非活性化状態でない時にDEACTIVATEできない");
				}
				break;
			case ACTIONID_SETWAKEUP:
				if (AS_CONCIERGE) {
					TRACE("ACTIONID_SETWAKEUP");
					A.setWakeupTimer(nParam);
				} else {
					TRACE("通常起動時にSetWakeupできない");
				}
				break;
			case ACTIONID_RESETWAKEUP:
				if (AS_CONCIERGE) {
					TRACE("ACTIONID_RESETWAKEUP");
					A.resetWakeupTimer();
				} else {
					TRACE("通常起動時にResetWakeupできない");
				}
				break;
			case ACTIONID_ENABLECLOCKTICK:
			case ACTIONID_DISABLECLOCKTICK:
				if (AS_CONCIERGE) {
					TRACE("ACTIONID_SETCLOCKTICK");
					A.setClockTick(nID == ACTIONID_ENABLECLOCKTICK);
				} else {
					TRACE("通常起動時にSetClockTickできない");
				}
				break;
#endif
			} // switch
		} // for
	}

	protected static void stopSnd() {
		if (ACTIVE_SND != null) {
			ACTIVE_SND_PLAYER.stop();
		}
		ACTIVE_SND = null;
	}

	public void paint(Graphics g) {
#if defined(DOJA)
		redraw();

#elif defined(JPHONE)
		g.drawImage(GRAPHICS_IMAGE,0,0,Graphics.TOP|Graphics.LEFT);

#else
		#error
#endif
	}

	private static void waitFrame(long nStart,long nWait) {
		final long nLast = nStart + nWait;
		while (getTime() < nLast)
			;
	}

	protected static int showDialog(int nType,String strText,int nDefaultButton) {
#if   defined(JPHONE)
		return 0;

#elif defined(DOJA)
#if   defined(DOJA2M)
		if (AS_CONCIERGE && MA_STATE != MA_STATE_ACTIVE) {
			return nDefaultButton;
		}
#endif // #if defined(DOJA2M)
		Dialog d = new Dialog(nType,STR_DIALOG_TITLE);
		d.setText(strText);
		return d.show();
#endif // #elif defined(DOJA)
	}

	protected static void waitKeypadClear() {
		while (GET_KEY_PAD_STATE() != 0) {
		}
	}

	protected static void term(String str) {
		if (str != null) {
			showDialog(DIALOG_DIALOG_INFO,str,DIALOG_BUTTON_OK);
			}
		APPLICATION_TERMINATE(A);
	}

	protected static  void setPos(int n) {
		POS_READING = n;
	}

	protected static  void skip(int n) {
		POS_READING  += n;
	}

	protected static int readSI8() {
		return BUFFER[POS_READING++];
	}

	protected static  int readUI8() {
		int n = readSI8();
		if (n < 0)
			return 256 + n;
		else
			return n;
	}

	protected static  int readSI16() {
		int n = readUI16();
		if (0x8000 <= n)
			return -(0x10000) + n;
		else
			return n;
	}

	protected static  int readUI16() {
		return (readUI8() << 8) + readUI8();
	}

	protected static  int readUI32() {
		return (readUI16() << 16) + readUI16();
	}

	protected static  void loadBuffer(int nLen) throws IOException {
		URL_SCRATCHPAD_BUFFER = "scratchpad:///0;pos=" + (SIZE_OF_SCRATCHPAD_HEADER + POS_READING);
		skip(nLen);
	}

	protected static  int readRGB() {
		return (readUI8() << 16) + (readUI8() << 8) + readUI8();
	}

	protected static String readString() {
		int nLen = readUI16();
		char ac[] = new char[nLen];
		int nStringLen= 0;
		while(0 < nLen)
		{
			int n0 = readUI8();
			char c;
			if (n0 < 0x80)	//	0xxx-xxxx
			{
				c = (char) n0;
				nLen--;
			}
			else 
			{
				//	2バイト文字列 : 110xx-xxxx
				int n1 = readUI8();
				//if ((n0 & 0xE0) == 0xC0)
				if (n0 < 0xE0)
				{
					c = (char)(((n0 & 0x1f) << 6) + (n1 & 0x3F));
					nLen-=2;
				}
				// 3バイト文字列 : 111xx-xxxx
				else // if ((n & 0xE0) == 0xE0)
				{
					int n2 = readUI8();
					c = (char)(((n0 & 0x0F)<< 12) + ((n1 & 0x3F) << 6) + (n2 & 0x3F));
					nLen-=3;
				}
			}
			ac[nStringLen++] = c;
		}
	
		return new String(ac,0,nStringLen);
	}

	/** ※要仕様再検証※ */
	protected static void readDBUI16() {
		int n1 = readUI8();

		// ショートDBID		- 1バイト
		if ((n1 & 0x80) == 0) {
			F  = (n1 >> 4) & 7;
			S = n1 & 0x0F;
	
		// ショートDBID		- 2バイト	01ff-ffff fsss-ssss	
		} else if ((n1 & 0x40) == 0) {
			int n2 = readUI8();
			F  = ((n1 & 0x3F) << 1)+ ((n2  >> 7) & 1);
			S = n2 & 0x7F;
		}
	}

	/** ※要仕様再検証※ */
	protected static  void beginBitStream() {
		ALREADY_READ_BITS = 0;
		READING_BYTE = readUI8();
	}

	/** ※要仕様再検証※ */
	//protected static  void endBitStream()
	//{
	//}

	/** ※要仕様再検証※ */
	protected static  int readBSUI(int nBits)
	{
		int nRead = 0;
		for(;;)
		{
			if (ALREADY_READ_BITS == 8)
			{
				READING_BYTE = readUI8();
				ALREADY_READ_BITS = 0;
			}

			int nCanReadBits = 8 - ALREADY_READ_BITS;
			if (nBits <= nCanReadBits)
			{
				//int nAdd = (READING_BYTE >> (nCanReadBits - nBits)) & ((1 << nBits) -1);
				//nRead += nAdd;
				nRead += (READING_BYTE >> (nCanReadBits - nBits)) & ((1 << nBits) -1);
			
				ALREADY_READ_BITS += nBits;
				break;
			}
			else
			{
				//int nAdd = READING_BYTE & ((1 << nCanReadBits)-1);
				//nBits -= nCanReadBits;
				//nRead += nAdd << nBits;
				nBits -= nCanReadBits;
				nRead += (READING_BYTE & ((1 << nCanReadBits)-1)) << nBits;
			
				ALREADY_READ_BITS = 8;
			}
		}

		return nRead;
	}
	
	/** ※要仕様再検証※ */
	protected static  int readBSSI(int nBits) {
		int nRead = readBSUI(nBits);
		int nLimit = 1 << (nBits - 1);
		if (nRead < nLimit)
			return nRead;
		else
			return nRead - (nLimit << 1);
	}
}

