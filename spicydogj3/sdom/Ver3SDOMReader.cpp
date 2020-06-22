//	$Id: Ver3SDOMReader.cpp,v 1.10 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"

#include "Ver3SDOMReader.h"
#include "MFCUnserializer.h"
#include <math.h>
#include <memory>
#include <ltwrappr.h>
#include <atlconv.h>
#include <afxtempl.h>
#include <xercesc/dom/DOM.hpp>
#include "xml/xml.h"
#include "sdomn.h"
#include "SDOMArchive.h"
#include "SDOMException.h"
#include "SDOMBuffer.h"
#include "DOMImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Ver3SDOMReaderImpl {
	enum ACTTYPE {
		ACTTYPE_IMODE = 0,
		ACTTYPE_WEB = 1,
		ACTTYPE_WIN = 2,
		ACTTYPE_MAC = 3,
		ACTTYPE_GRAY = 4,
		ACTTYPE_MONO = 5,
		ACTTYPE_RGB = 6,
		ACTTYPE_CUSTOM = 7
	}; // enum ACTTYPE

	enum PICTOPTTYPE {
		PICTOPTTYPE_PATHTHROUGH = 0,
		PICTOPTTYPE_PNG         = 1,
		PICTOPTTYPE_JPG         = 2
	}; // enum PICTOPTTYPE

	const int KEY_0 = 0x00;
	const int KEY_1 = 0x01;
	const int KEY_2 = 0x02;
	const int KEY_3 = 0x03;
	const int KEY_4 = 0x04;
	const int KEY_5 = 0x05;
	const int KEY_6 = 0x06;
	const int KEY_7 = 0x07;
	const int KEY_8 = 0x08;
	const int KEY_9 = 0x09;
	const int KEY_ASTER = 0x0a;
	const int KEY_POUND = 0x0b;
	const int KEY_LEFT  = 0x10;
	const int KEY_UP    = 0x11;
	const int KEY_RIGHT = 0x12;
	const int KEY_DOWN  = 0x13;
	const int KEY_SELECT= 0x14;
	const int KEY_SOFT1 = 0x15;
	const int KEY_SOFT2 = 0x16;
	const int KEY_IAPP   = 0x18;
	const int KEY_UPPER_LEFT  = 0x1a;  
	const int KEY_UPPER_RIGHT = 0x1b;  
	const int KEY_LOWER_RIGHT = 0x1c;  
	const int KEY_LOWER_LEFT  = 0x1d;  
	const int KEY_PAGE_UP     = 0x1e;  
	const int KEY_PAGE_DOWN   = 0x1f;

	const DWORD SIGNATURE_SERIALIZE_CONTEXT = 'SDBH';
	const int SIZE_OF_SERIALIZE_CONTEXT = 256;
	struct _SERIALIZE_CONTEXT {
		DWORD m_dwSignature;
		DWORD m_dwContextId;
		WORD  m_wWriterId;
		DWORD m_wWriterMajVer;
		DWORD m_wWriterMinVer;
	};
	struct SERIALIZE_CONTEXT : public _SERIALIZE_CONTEXT {
		BYTE RESERVED[SIZE_OF_SERIALIZE_CONTEXT - sizeof(_SERIALIZE_CONTEXT)];
	};
	const DWORD SC_FILE_SDB_V3      = 0x00000010;

	BOOL cast_bool(BOOL b);
	BOOL IsVer3(SERIALIZE_CONTEXT& header);

	BOOL IsLeadJpeg(int n);

	const LPCTSTR TNAME_CSTOP_SOUND_ACTION         = "CStopSoundAction";
	const LPCTSTR TNAME_CPLAY_MOVIE_ACTION         = "CPlayMovieAction";
	const LPCTSTR TNAME_CSTOP_MOVIE_ACTION         = "CStopMovieAction";
	const LPCTSTR TNAME_CQUIT_ACTION               = "CQuitAction";
	const LPCTSTR TNAME_CDEACTIVATE_ACTION         = "CDeactivateAction";
	const LPCTSTR TNAME_CSLEEP_ACTION              = "CSleepAction";
	const LPCTSTR TNAME_CRESET_WAKEUP_ACTION       = "CResetWakeupAction";
	const LPCTSTR TNAME_CENABLE_CLOCK_TICK_ACTION  = "EnableClockTickAction";
	const LPCTSTR TNAME_CDISABLE_CLOCK_TICK_ACTION = "DisableClockTickAction";
	const LPCTSTR TNAME_CGOTO_FRAME_ACTION         = "CGotoFrameAction";
	const LPCTSTR TNAME_CPLAY_SOUND_ACTION         = "CPlaySoundAction";
	const LPCTSTR TNAME_CSET_SOFTKEY_ACTION        = "CSetSoftKeyAction";
	const LPCTSTR TNAME_CSET_VIBRATION_ACTION      = "CSetVibrationAction";
	const LPCTSTR TNAME_CSET_BKLIGHT_ACTION        = "CSetBklightAction";
	const LPCTSTR TNAME_CSET_BKCLR_ACTION          = "CSetBkClrAction";
	const LPCTSTR TNAME_CGOTO_MOVIE_ACTION         = "CGotoMovieAction";
	const LPCTSTR TNAME_CLAUNCH_BROWSER_ACTION     = "CLaunchBrowserAction";
	const LPCTSTR TNAME_CCALL_TELPHONE_ACTION      = "CCallTelPhoneAction";
	const LPCTSTR TNAME_CSET_WAKEUP_ACTION         = "CSetWakeupAction";
	const LPCTSTR TNAME_CDELAY_GOTO_FRAME_ACTION   = "CDelayGotoFrameAction";
	const LPCTSTR TNAME_CRANDOM_GOTO_FRAME_ACTION  = "CRandomGotoFrameAction";
	const LPCTSTR TNAME_CLAUNCH_TYPE_GOTO_FRAME_ACTION = "CLaunchTypeGotoFrameAction";
	const LPCTSTR TNAME_CMOVIE           = "CMovie";
	const LPCTSTR TNAME_CORG_FONT        = "COrgFont";
	const LPCTSTR TNAME_CDICTIONARY      = "CDictionary";
	const LPCTSTR TNAME_CBTN_CHAR        = "CBtnChar";
	const LPCTSTR TNAME_CTEXT_CHAR       = "CTextChar";
	const LPCTSTR TNAME_CSHAPE_CHAR      = "CShapeChar";
	const LPCTSTR TNAME_CSOUND_CHAR      = "CSoundChar";
	const LPCTSTR TNAME_CPICT_CHAR       = "CPictChar";
	const LPCTSTR TNAME_CSHAPE_PANE      = "CShapePane";
	const LPCTSTR TNAME_CACTION_LIST     = "CActionList";
	const LPCTSTR TNAME_CKEY_FRAME_LABEL = "CKeyFrameLabel";
	const LPCTSTR TNAME_CTIMELINE        = "CTimeline";
	const LPCTSTR TNAME_CSUB_STAGE       = "CSubStage";
	const LPCTSTR TNAME_CTIMELINE_LAYER  = "CTimelineLayer";
	const LPCTSTR TNAME_CKEY_FRAME       = "CKeyFrame";
};

#define LT_DISPLAY_ERROR_LIST LBase::DisplayErrorList(NULL,TRUE)

using namespace Ver3SDOMReaderImpl;

DOMDocument* Ver3SDOMReader::Read(CFile* file) {
	ASSERT(AfxIsValidAddress(file,sizeof CFile));
	DOMNodeP<DOMDocument> doc;
	DOMElement* rootElem;

	MFCUnserializer unserializer(*file);
	m_file = file;
	m_unserializer = &unserializer;

	DOMImplementation* impl = DOMImpl::getImpl();
	m_doc    = impl->createDocument(NULL,WSDOMN_SPICYDOG,NULL);
	doc      = DOMNodeP<DOMDocument>(m_doc);
	rootElem = doc->getDocumentElement();

		readHeader();
		rootElem->appendChild(readAppliConf());
		skipOrgFonts();
		rootElem->appendChild(readCharList());
		rootElem->appendChild(readMainStage(0));
		rootElem->appendChild(readSubStages());

	return doc.release();
}

BOOL Ver3SDOMReader::CanRead(CFile* file) {
	ASSERT(AfxIsValidAddress(file,sizeof CFile));
	SERIALIZE_CONTEXT header;
	UINT read = file->Read(&header,SIZE_OF_SERIALIZE_CONTEXT);
	if (read != SIZE_OF_SERIALIZE_CONTEXT) {
		file->SeekToBegin();
		return FALSE;
	}
	file->SeekToBegin();
	return IsVer3(header);
}

void Ver3SDOMReader::readHeader() {
	SERIALIZE_CONTEXT header;
	m_unserializer->ReadBuffer((BYTE*)&header,SIZE_OF_SERIALIZE_CONTEXT);
	if (!IsVer3(header)) {
		SDOMException::throwException(SDOMException::CAUSE_UNKNOWN_VERSION);
	}
	readTypeSafeObject(TNAME_CMOVIE);
}

DOMElement* Ver3SDOMReader::readAppliConf() {
	CString     strAppName;
	CString     strPlatformKey;
	CStringList lstrDevices;
	BOOL        bP503Bug;
	BOOL        bAutoBoot;
	int         nAutoBootHour;
	CString     strLaunchByMail;
	CString     strLaunchByBrowser;
	BOOL        bDebugTrace;
	SIZE        szStageSize;
	int         nSetDrawArea;
	SIZE        szDrawArea;
	COLORREF    crBkgnd;
	COLORREF    crBkframe;
	int         nFrameRate;
	BOOL        bLoopPlay;

	m_unserializer->ReadValue(strPlatformKey);
	m_unserializer->ReadValue(strAppName);
	m_unserializer->ReadValue(nSetDrawArea);
	m_unserializer->ReadValue(szStageSize);
	m_unserializer->ReadValue(szDrawArea);
	m_unserializer->ReadValue(lstrDevices);
	m_unserializer->ReadValue(bP503Bug);
	m_unserializer->ReadValue(bAutoBoot);
	m_unserializer->ReadValue(nAutoBootHour);
	m_unserializer->ReadValue(strLaunchByMail);
	m_unserializer->ReadValue(strLaunchByBrowser);
	m_unserializer->ReadValue(bDebugTrace);
	m_unserializer->ReadValue(crBkgnd);
	m_unserializer->ReadValue(crBkframe);
	m_unserializer->ReadValue(nFrameRate);
	m_unserializer->ReadValue(bLoopPlay);

	DOMNodeP<DOMElement> appli_conf(m_doc->createElement(WSDOMN_APPLI_CONF));
	appendString  (appli_conf.get(),WSDOMN_TITLE,           strAppName);
	appendString  (appli_conf.get(),WSDOMN_APPLI_TYPE,      ASDOMV_JPHONEF);
	appendSize    (appli_conf.get(),WSDOMN_STAGE_SIZE,      szStageSize);
	appendColor   (appli_conf.get(),WSDOMN_BACKGROUND_COLOR,crBkgnd);
	appendColor   (appli_conf.get(),WSDOMN_BORDER_COLOR,    crBkframe);
	appendInt     (appli_conf.get(),WSDOMN_FRAME_RATE,      nFrameRate);
	appendBoolean (appli_conf.get(),WSDOMN_LOOP_PLAY,       cast_bool(bLoopPlay));
	appendString  (appli_conf.get(),WSDOMN_SCREEN_CLOCK_TYPE,WSDOMV_CLOCK_NONE);
	appendString  (appli_conf.get(),WSDOMN_VENDOR,          "");
	appendNullData(appli_conf.get(),WSDOMN_ICON);
	appendString  (appli_conf.get(),WSDOMN_COMMENT,"");

	return appli_conf.release();
}

void Ver3SDOMReader::readTypeSafeObject(LPCSTR type) {
	ASSERT(AfxIsValidString(type));

	int object;
	CString className;
	m_unserializer->ReadObject(className,object);
	if (className != type) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}

void Ver3SDOMReader::skipOrgFonts() {
	int fonts;
	m_unserializer->ReadValue(fonts);
	for (int n = 0;n < fonts;n++) {
		readTypeSafeObject(TNAME_CORG_FONT);

		CString name;
		int startPos;
		int endPost;

		m_unserializer->ReadValue(name);
		m_unserializer->ReadValue(startPos);
		m_unserializer->ReadValue(endPost);
		skipBuffer();
	}
}

DOMElement* Ver3SDOMReader::readCharList() {
	readTypeSafeObject(TNAME_CDICTIONARY);

	DOMNodeP<DOMElement> dictionary(m_doc->createElement(WSDOMN_CHAR_LIST));

	int characters;
	m_unserializer->ReadValue(characters);
	for (int n = 0;n < characters;n++) {
		DOMNode* c = readChar();
		if (c != NULL) {
			dictionary->appendChild(c);
		}
	}

	return dictionary.release();
}

DOMElement* Ver3SDOMReader::readChar() {
	CString className;
	int object;
	if (!m_unserializer->ReadObject(className,object)) {
		return (DOMElement*)m_unserializer->GetObject(object);
	}
	//TRACE("className=%s\n",className);
	CString charName;
	m_unserializer->ReadValue(charName);
	//TRACE("charName=%s\n",charName);

	DOMNodeP<DOMElement> c(NULL);
	if (className == TNAME_CBTN_CHAR) {
		c = DOMNodeP<DOMElement>(readBtnChar(object));

	} else if (className == TNAME_CTEXT_CHAR) {
		c = DOMNodeP<DOMElement>(readTextChar(object));

	} else if (className == TNAME_CSHAPE_CHAR) {
		c = DOMNodeP<DOMElement>(readShapeChar(object));
		
	} else if (className == TNAME_CSOUND_CHAR) {
		c = DOMNodeP<DOMElement>(readSndChar(object));

	} else if (className == TNAME_CPICT_CHAR) {
		c = DOMNodeP<DOMElement>(readPictChar(object));

	} else {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	if (c.get() != NULL) {
		insertString(c.get(),WSDOMN_NAME,charName);
		setAttID(c.get(),(int)c.get());
	}
	
	return c.release();
}

	BOOL testBit(DWORD bitmap,int bit) {
		DWORD mask = 1;
		mask <<= bit;
		return (bitmap & mask) != 0;
	}

DOMElement* Ver3SDOMReader::readBtnChar(int object) {
	DWORD keyMap;
	DWORD eventMap;	
	DOMNodeP<DOMElement> normalState;
	DOMNodeP<DOMElement> activeState;
	DOMNodeP<DOMElement> actions;
	
	DOMNodeP<DOMElement> btn(m_doc->createElement(WSDOMN_BTN_CHAR));
	m_unserializer->RegistObject(object,btn.get());

	normalState = DOMNodeP<DOMElement>(readChar());
	activeState = DOMNodeP<DOMElement>(readChar());
	m_unserializer->ReadValue(keyMap);
	m_unserializer->ReadValue(eventMap);
	actions = DOMNodeP<DOMElement>(readActions(TRUE));

	appendIDREF(btn.get(),WSDOMN_NORMAL_STATE_CHAR,normalState.get());
	appendIDREF(btn.get(),WSDOMN_ACTIVE_STATE_CHAR,activeState.get());
	
	DOMNodeP<DOMElement> keyEvent(m_doc->createElement(WSDOMN_KEY_EVENT));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_0,testBit(keyMap,KEY_0));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_1,testBit(keyMap,KEY_1));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_2,testBit(keyMap,KEY_2));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_3,testBit(keyMap,KEY_3));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_4,testBit(keyMap,KEY_4));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_5,testBit(keyMap,KEY_5));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_6,testBit(keyMap,KEY_6));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_7,testBit(keyMap,KEY_7));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_8,testBit(keyMap,KEY_8));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_9,testBit(keyMap,KEY_9));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_ASTER, testBit(keyMap,KEY_ASTER));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_POUND, testBit(keyMap,KEY_POUND));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_LEFT,  testBit(keyMap,KEY_LEFT));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_UP,    testBit(keyMap,KEY_UP));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_RIGHT, testBit(keyMap,KEY_RIGHT));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_DOWN,  testBit(keyMap,KEY_DOWN));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_SELECT,testBit(keyMap,KEY_SELECT));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_SOFT_1, testBit(keyMap,KEY_SOFT1));
		appendBoolean(keyEvent.get(),WSDOMN_KEY_SOFT_2, testBit(keyMap,KEY_SOFT2));
	btn->appendChild(keyEvent.release());

	DOMNodeP<DOMElement> mail(m_doc->createElement(WSDOMN_JPHONE_MAIL_EVENT));
		appendBoolean(mail.get(),WSDOMN_USE_EVENT,     false);
		appendBoolean(mail.get(),WSDOMN_STANDARD_MAIL, false);
		appendBoolean(mail.get(),WSDOMN_GREETING_MAIL, false);
		appendBoolean(mail.get(),WSDOMN_RELAY_MAIL,    false);
		appendBoolean(mail.get(),WSDOMN_AUTO_MAIL,     false);
		appendBoolean(mail.get(),WSDOMN_EMERGENCY_MAIL,false);
		appendBoolean(mail.get(),WSDOMN_SUPER_MAIL,    false);
		appendBoolean(mail.get(),WSDOMN_SKY_MAIL,      false);
	btn->appendChild(mail.release());

	DOMNodeP<DOMElement> schedule(m_doc->createElement(WSDOMN_JPHONE_SCHEDULE_EVENT));
		appendBoolean(schedule.get(),WSDOMN_USE_EVENT,false);
		appendBoolean(schedule.get(),WSDOMN_RECEIVE_ALL,false);
		appendString (schedule.get(),WSDOMN_CRITERIA,"");
	btn->appendChild(schedule.release());

	DOMNodeP<DOMElement> tel(m_doc->createElement(WSDOMN_JPHONE_TEL_EVENT));
		appendBoolean(tel.get(),WSDOMN_USE_EVENT,false);
		appendBoolean(tel.get(),WSDOMN_RECEIVE_ALL,false);
		appendString (tel.get(),WSDOMN_CRITERIA,"");
	btn->appendChild(tel.release());

	btn->appendChild(actions.release());
	
	normalState.release();
	activeState.release();
	return btn.release();
}

DOMElement* Ver3SDOMReader::readTextChar(int object) {
	CString strText;
	int DUMMY;
	BOOL bAutoResize;
	COLORREF crForeColor;
	int nWidth;
	int nHeight;
	int nFontType;
	int nFontFace;
	int nFontStyle;
	int nFontSize;
	CString orgFontClass;
	int orgFontObject;
	DOMNodeP<DOMElement> text(m_doc->createElement(WSDOMN_TEXT_CHAR));
	m_unserializer->RegistObject(object,text.get());

	m_unserializer->ReadValue(strText);
	m_unserializer->ReadValue(DUMMY);
	m_unserializer->ReadValue(DUMMY);	
	m_unserializer->ReadValue(nWidth);
	m_unserializer->ReadValue(nHeight);
	m_unserializer->ReadValue(nFontType);
	m_unserializer->ReadValue(nFontFace);
	m_unserializer->ReadValue(nFontStyle);
	m_unserializer->ReadValue(nFontSize);
	m_unserializer->ReadValue(crForeColor);
	m_unserializer->ReadObject(orgFontClass,orgFontObject);
	m_unserializer->ReadValue(bAutoResize);

	appendString (text.get(),WSDOMN_TEXT,       strText);
	appendBoolean(text.get(),WSDOMN_AUTO_RESIZE,cast_bool(bAutoResize));
	appendColor  (text.get(),WSDOMN_TEXT_COLOR, crForeColor);
	appendInt    (text.get(),WSDOMN_WIDTH,      nWidth);
	appendInt    (text.get(),WSDOMN_HEIGHT,     nHeight);
	switch(nFontFace) {
	case 0:
		appendString(text.get(),WSDOMN_FONT_FACE,WSDOMV_SYSTEM);
		break;
	case 1:
		appendString(text.get(),WSDOMN_FONT_FACE,WSDOMV_MONOSPACE);
		break;
	case 2:
		appendString(text.get(),WSDOMN_FONT_FACE,WSDOMV_PROPORTIONAL);
		break;
	default:
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	switch(nFontSize) {
	case 0:
		appendString(text.get(),WSDOMN_FONT_SIZE,WSDOMV_SMALL);
		break;
	case 1:
		appendString(text.get(),WSDOMN_FONT_SIZE,WSDOMV_MEDIUM);
		break;
	case 2:
		appendString(text.get(),WSDOMN_FONT_SIZE,WSDOMV_LARGE);
		break;
	default:
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	appendBoolean(text.get(),WSDOMN_FONT_STYLE_BOLD,     false);
	appendBoolean(text.get(),WSDOMN_FONT_STYLE_ITALIC,   false);
	appendBoolean(text.get(),WSDOMN_FONT_STYLE_UNDERLINE,false);

	return text.release();
}

DOMElement* Ver3SDOMReader::readShapeChar(int object) {
	m_unserializer->RegistObject(object,NULL);

	int panes;
	m_unserializer->ReadValue(panes);

	while (0 < panes--) {
		CString paneClassName;
		int object;
		m_unserializer->ReadObject(paneClassName,object);
		ASSERT(paneClassName == TNAME_CSHAPE_PANE);

		BOOL m_bHasFillColor;
		BOOL m_bHasFrameColor;
		BOOL m_bPathClosed;
		int m_nInterp;

		m_unserializer->ReadValue(m_bHasFillColor);
		skipRGB();
		m_unserializer->ReadValue(m_bHasFrameColor);
		skipRGB();
		m_unserializer->ReadValue(m_bPathClosed);
		m_unserializer->ReadValue(m_nInterp);

		int points;
		m_unserializer->ReadValue(points);
		//TRACE("points=%d\n",points);
		while (0 < points--) {
			int type;

			skipFPOINT();
			m_unserializer->ReadValue(type);
			skipFPOINT();
			skipFPOINT();
		}
	}

	int RESERVED;
	BOOL m_bBmpLoad;
	POINT pt;
	SIZE  sz;
	m_unserializer->ReadValue(m_bBmpLoad);
	m_unserializer->ReadValue(pt);
	m_unserializer->ReadValue(sz);
	m_unserializer->ReadValue(RESERVED);
	m_unserializer->ReadValue(RESERVED);
	m_unserializer->ReadValue(RESERVED);
	m_unserializer->ReadValue(RESERVED);

	return NULL;
}

DOMElement* Ver3SDOMReader::readSndChar(int object) {
	DOMNodeP<DOMElement> snd(m_doc->createElement(WSDOMN_SOUND_CHAR));
	m_unserializer->RegistObject(object,snd.get());
	return NULL;
	//snd->appendChild(readBuffer(WSDOMN_DATA,NULL));
	//return snd.release();
}

DOMElement* Ver3SDOMReader::readBuffer(LPCWSTR name,SDOMBuffer* buffer) {
	USES_CONVERSION;

	UINT length;
	m_unserializer->ReadValue(length);

	CString data;
	if (length != 0) {
		std::auto_ptr<SDOMBuffer> bufferP;
		if (buffer == NULL) {
			buffer = new SDOMBuffer;
			bufferP = std::auto_ptr<SDOMBuffer>(buffer);
		}
		
		buffer->Alloc(length);
		GlobalAllocatorPtr src(*buffer);
		m_unserializer->ReadBuffer((LPBYTE)(LPVOID)src,length);
		data = buffer->bin2hex();
	}

	DOMElement* n = m_doc->createElement(name);
    DOMText* v = m_doc->createTextNode(A2W(data));
	n->appendChild(v);

	return n;
}

LPCWSTR name_act_type(int type) {
	LPCWSTR name;
	switch(type) {
	case ACTTYPE_IMODE:
		name = WSDOMV_IMODE_PALETTE;
		break;
	case ACTTYPE_WEB:
		name = WSDOMV_WEB_PALETTE;
		break;
	case ACTTYPE_WIN:
		name = WSDOMV_WIN_PALETTE;
		break;
	case ACTTYPE_MAC:
		name = WSDOMV_MAC_PALETTE;
		break;
	case ACTTYPE_GRAY:
		name = WSDOMV_GRAY_PALETTE;
		break;
	case ACTTYPE_MONO:
		name = WSDOMV_MONO_PALETTE;
		break;
	case ACTTYPE_RGB:
		name = WSDOMV_RGB_PALETTE;
		break;
	case ACTTYPE_CUSTOM:
		name = WSDOMV_CUSTOM_PALETTE;
		break;
	default:
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	return name;
}

void Ver3SDOMReader::readPictPaththrough(DOMElement* pictChar) {
	LBuffer     buffer;
	FILEINFO    info;
	LMemoryFile file;
	LBitmapBase bitmap;
	DOMNodeP<DOMElement> data;
	DOMNodeP<DOMElement> quantize;
	DOMNodeP<DOMElement> palette;
	DOMNodeP<DOMElement> png;

	SDOMBuffer src;
	data = DOMNodeP<DOMElement>(readBuffer(WSDOMN_DATA,&src));
	GlobalAllocatorPtr ptr(src);
	buffer.Copy(ptr,src.GetSize());
	ptr.Unlock();

	ZeroMemory(&info,sizeof info);
	if (file.GetInfo(buffer,&info) != SUCCESS) {
		LT_DISPLAY_ERROR_LIST;
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	if (IsLeadJpeg(info.Format)) {
		DOMNodeP<DOMElement> pathThrough;
		pathThrough = DOMNodeP<DOMElement>(m_doc->createElement(WSDOMN_PATH_THROUGH));
			pathThrough->appendChild(data.release());
		pictChar->appendChild(pathThrough.release());
		return;
	}
	if (info.Format != FILE_GIF) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	file.SetBitmap(&bitmap);
	if (file.LoadBitmap(buffer,24,ORDER_BGR) != SUCCESS) {
		LT_DISPLAY_ERROR_LIST;
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	//TRACE("path-through gif->png\n");

	int paletteType;
	int colors;
	BOOL useDither;
	BOOL useTransparency;
	COLORREF transparentColor;
	int bitsPerPixel;

	paletteType = ACTTYPE_CUSTOM;
	bitsPerPixel = info.BitsPerPixel;
	colors = pow(2,bitsPerPixel);
	useDither = false;
	
	// パレット読込
	BITMAPHANDLE* pbmp = bitmap.GetHandle();
	ASSERT(AfxIsValidAddress(pbmp,sizeof BITMAPHANDLE));
	HPALETTE hPalette = pbmp->hPalette;
	if (8 <  bitsPerPixel) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	palette = DOMNodeP<DOMElement>(m_doc->createElement(WSDOMN_PALETTE));
	PALETTEENTRY entries[256];
	if (hPalette == NULL) {
		L_GetFixedPalette((LPRGBQUAD)entries,256);
	} else {
		int paletteEntries = GetPaletteEntries(hPalette,0,0,NULL);
		GetPaletteEntries(hPalette,0,paletteEntries,entries);
		for (int n = 0;n < paletteEntries;n++) {
			PALETTEENTRY e = entries[n];
			appendColor(palette.get(),WSDOMN_PALETTE_ENTRY,RGB(e.peRed,e.peGreen,e.peBlue));
		}
	}
	
	// 透明色
	if (pbmp->Flags.Transparency) {
		useTransparency = true;
		if (pbmp->Transparency <= 0x1000000) {
			const PALETTEENTRY& pe = entries[pbmp->Transparency - 0x1000000];
			transparentColor = RGB(pe.peRed,pe.peGreen,pe.peBlue);
		} else {
			transparentColor = pbmp->Transparency;
		}
	} else {
		useTransparency = false;
		transparentColor = 0;
	}
	ASSERT(0 <= transparentColor && transparentColor <= 0xFFFFFF);
	
	// DIB画像化
	LBuffer bufferDIB;
	LMemoryFile fileSave;
	file.SetBitmap(&bitmap);
	if (file.SaveBitmap(&bufferDIB,FILE_BMP,24,0,NULL) != SUCCESS) {
		LT_DISPLAY_ERROR_LIST;
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	quantize = DOMNodeP<DOMElement>(m_doc->createElement(WSDOMN_QUANTIZE_PNG));
	LPCVOID pointer = bufferDIB.Lock();
	UINT length = bufferDIB.GetSize();
	try {
		CString hex = SDOMArchive::bin2hex(pointer,length);
		appendString(quantize.get(),WSDOMN_BITMAP,hex);
		buffer.Unlock();
	} catch(...) {
		throw;
	}
	appendString(quantize.get(),WSDOMN_PALETTE_TYPE,name_act_type(paletteType));
	quantize->appendChild(palette.release());
	appendInt    (quantize.get(),WSDOMN_COLORS,           colors);
	appendBoolean(quantize.get(),WSDOMN_USE_DITHER,       cast_bool(useDither));
	appendBoolean(quantize.get(),WSDOMN_USE_TRANSPARENCY, cast_bool(useTransparency));
	appendColor  (quantize.get(),WSDOMN_TRANSPARENT_COLOR,transparentColor);
	pictChar->appendChild(quantize.release());
}

void Ver3SDOMReader::readPictJpeg(DOMElement* pictChar) {
	DOMNodeP<DOMElement> quantize;
	DOMNodeP<DOMElement> bitmap;
	int compress;

	bitmap  = DOMNodeP<DOMElement>(readBuffer(WSDOMN_BITMAP,NULL));
	m_unserializer->ReadValue(compress);
	readPalette()->release();

	quantize = DOMNodeP<DOMElement>(m_doc->createElement(WSDOMN_QUANTIZE_JPG));
		quantize->appendChild(bitmap.release());
		appendInt(quantize.get(),WSDOMN_COMPRESS,compress);
	pictChar->appendChild(quantize.release());
}

void Ver3SDOMReader::readPictGif2Png(DOMElement* pictChar) {
	DOMNodeP<DOMElement> quantize;
	DOMNodeP<DOMElement> bitmap;
	DOMNodeP<DOMElement> palette;
	int paletteType;
	int colors;
	BOOL useDither;
	BOOL useTransparency;
	COLORREF  transparentColor;

	bitmap = DOMNodeP<DOMElement>(readBuffer(WSDOMN_BITMAP,NULL));
	m_unserializer->ReadValue(useTransparency);
	m_unserializer->ReadValue(transparentColor);
	m_unserializer->ReadValue(colors);
	m_unserializer->ReadValue(paletteType);
	m_unserializer->ReadValue(useDither);
	palette = DOMNodeP<DOMElement>(readPalette());

	quantize = DOMNodeP<DOMElement>(m_doc->createElement(WSDOMN_QUANTIZE_PNG));
		quantize->appendChild(bitmap.release());
		appendString(quantize.get(),WSDOMN_PALETTE_TYPE,name_act_type(paletteType));
		quantize->appendChild(palette.release());
		appendInt    (quantize.get(),WSDOMN_COLORS,           colors);
		appendBoolean(quantize.get(),WSDOMN_USE_DITHER,       cast_bool(useDither));
		appendBoolean(quantize.get(),WSDOMN_USE_TRANSPARENCY, cast_bool(useTransparency));
		appendColor  (quantize.get(),WSDOMN_TRANSPARENT_COLOR,transparentColor);
	pictChar->appendChild(quantize.release());
}

DOMElement* Ver3SDOMReader::readPictChar(int object) {
	CPoint centerPoint;
	int quntizeType;
	DOMNodeP<DOMElement> pictChar;

	m_unserializer->ReadValue(centerPoint);
	m_unserializer->ReadValue(quntizeType);

	pictChar = DOMNodeP<DOMElement>(m_doc->createElement(WSDOMN_PICT_CHAR));
	m_unserializer->RegistObject(object,pictChar.get());

	if (quntizeType == PICTOPTTYPE_PATHTHROUGH) {
		//TRACE("path-throught\n");
		readPictPaththrough(pictChar.get());

	} else if (quntizeType == PICTOPTTYPE_JPG) {
		//TRACE("jpeg\n");
		readPictJpeg(pictChar.get());

	} else if (quntizeType == PICTOPTTYPE_PNG) {
		//TRACE("gif\n");
		readPictGif2Png(pictChar.get());

	} else {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	
	appendPoint(pictChar.get(),WSDOMN_CENTER_POINT,centerPoint);
	return pictChar.release();
}

DOMElement* Ver3SDOMReader::readPalette() {
	DOMElement* palette = m_doc->createElement(WSDOMN_PALETTE);

	int count;
	m_unserializer->ReadValue(count);

	CArray<COLORREF,COLORREF&> colors;
	colors.SetSize(256);
	for (int n1 = 0;n1 < 256;n1++) {
		BYTE r,g,b;
		m_unserializer->ReadValue(r);
		m_unserializer->ReadValue(g);
		m_unserializer->ReadValue(b);
		colors[n1] = RGB(r,g,b);
	}
	
	for (int n2 = 0;n2 < count;n2++) {
		COLORREF cr = colors[n2];
		appendColor(palette,WSDOMN_PALETTE_ENTRY,cr);
	}
	return palette;
}


DOMElement* Ver3SDOMReader::readActions(BOOL isObject) {
	if (isObject) {
		readTypeSafeObject(TNAME_CACTION_LIST);
	}

	DOMElement* actions = m_doc->createElement(WSDOMN_ACTION_LIST);
	int count;
	m_unserializer->ReadValue(count);
	for(int n = 0;n < count;n++) {
		CString className;
		CString actionName;
		
		int object;
		m_unserializer->ReadObject(className,object);
		m_unserializer->ReadValue(actionName);
		
		//TRACE("%s\n",className);
		DOMElement* a;
		if        (className == TNAME_CSTOP_SOUND_ACTION)        {
			a = m_doc->createElement(WSDOMN_SOUND_STOP_ACTION);

		} else if (className == TNAME_CPLAY_MOVIE_ACTION)        {
			a = m_doc->createElement(WSDOMN_PLAY_ACTION);

		} else if (className == TNAME_CSTOP_MOVIE_ACTION)        {
			a = m_doc->createElement(WSDOMN_STOP_ACTION);

		} else if (className == TNAME_CQUIT_ACTION)             {
			a = m_doc->createElement(WSDOMN_QUIT_ACTION);

		} else if (className == TNAME_CDEACTIVATE_ACTION)       {
			a = NULL;

		} else if (className == TNAME_CSLEEP_ACTION)            {
			a = NULL;

		} else if (className == TNAME_CRESET_WAKEUP_ACTION)      {
			a = NULL;

		} else if (className == TNAME_CENABLE_CLOCK_TICK_ACTION) {
			a = NULL;

		} else if (className == TNAME_CDISABLE_CLOCK_TICK_ACTION) {
			a = NULL;

		} else if (className == TNAME_CGOTO_FRAME_ACTION) {
			DOMElement*l = readLabel();
			a = m_doc->createElement(WSDOMN_JUMP_ACTION);
			appendIDREF(a,WSDOMN_JUMPTO,l);

		} else if (className == TNAME_CPLAY_SOUND_ACTION) {
			DOMElement*c = readChar();

			a = m_doc->createElement(WSDOMN_SOUND_PLAY_ACTION);
			appendIDREF(a,WSDOMN_SOUND,c);

		} else if (className == TNAME_CSET_SOFTKEY_ACTION) {
			int     which;
			CString label;
			m_unserializer->ReadValue(which);
			m_unserializer->ReadValue(label);

			a = m_doc->createElement(WSDOMN_SET_SOFTKEY_ACTION);
			appendString(a,WSDOMN_WHICH,which == 0 ? ASDOMV_LEFT : ASDOMV_RIGHT);
			appendString(a,WSDOMN_LABEL,  label);
		
		} else if (className == TNAME_CSET_VIBRATION_ACTION) {
			int type;
			m_unserializer->ReadValue(type);

			a = m_doc->createElement(WSDOMN_SET_VIBRATION_ACTION);
			appendBoolean(a,WSDOMN_ENABLED,type == 1);

		} else if (className == TNAME_CSET_BKLIGHT_ACTION) {
			int type;
			m_unserializer->ReadValue(type);

			a = m_doc->createElement(WSDOMN_SET_BACKLIGHT_ACTION);
			appendBoolean(a,WSDOMN_ENABLED,type == 1);

		} else if (className == TNAME_CSET_BKCLR_ACTION) {
			COLORREF cr;
			m_unserializer->ReadValue(cr);

			a = m_doc->createElement(WSDOMN_SET_BACKGROUND_COLOR_ACTION);
			appendColor(a,WSDOMN_COLOR,cr);

		} else if (className == TNAME_CGOTO_MOVIE_ACTION) {
			CString url;
			m_unserializer->ReadValue(url);
			a = NULL;

		} else if (className == TNAME_CLAUNCH_BROWSER_ACTION) {
			CString url;
			m_unserializer->ReadValue(url);
			a = NULL;

		} else if (className == TNAME_CCALL_TELPHONE_ACTION) {
			CString phone;
			m_unserializer->ReadValue(phone);
			a = NULL;

		} else if (className == TNAME_CSET_WAKEUP_ACTION) {
			int time;
			m_unserializer->ReadValue(time);
			a = NULL;

		} else if (className == TNAME_CDELAY_GOTO_FRAME_ACTION) {
			int delay;
			m_unserializer->ReadValue(delay);
			DOMElement*l = readLabel();
			
			a = m_doc->createElement(WSDOMN_DELAY_JUMP_ACTION);
			appendIDREF(a,WSDOMN_JUMPTO,l);

		} else if (className == TNAME_CRANDOM_GOTO_FRAME_ACTION) {
			a = m_doc->createElement(WSDOMN_RANDOM_JUMP_ACTION);
		
			DOMNodeP<DOMNode> cases;
			cases = DOMNodeP<DOMNode>(m_doc->createElement(WSDOMN_CASES));

			int count;
			m_unserializer->ReadValue(count);
			for (int n = 0;n < count;n++) {
				int probability;
				DOMNodeP<DOMElement> label;

				m_unserializer->ReadValue(probability);
				label = DOMNodeP<DOMElement>(readLabel());
				
				DOMNodeP<DOMElement> case1;
				case1 = DOMNodeP<DOMElement>(m_doc->createElement(WSDOMN_CASE));
				appendInt(case1.get(),WSDOMN_PROBABILITY,probability);
				appendIDREF(case1.get(),WSDOMN_JUMPTO,label.release());
				cases->appendChild(case1.release());
			}
			a->appendChild(cases.release());


		} else if (className == TNAME_CLAUNCH_TYPE_GOTO_FRAME_ACTION) {
			int type;
			m_unserializer->ReadValue(type);

			DOMElement*l = readLabel();
			a = NULL;

		} else {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
		if (a != NULL) {
			actions->appendChild(a);
		} else {
		}
	}
	return actions;
}

DOMElement* Ver3SDOMReader::readLabel() {
	CString className;
	int object;
	if (!m_unserializer->ReadObject(className,object)) {
		return (DOMElement*)m_unserializer->GetObject(object);
	}
	if (object == 0) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	DOMNodeP<DOMElement> label;
	if (className == TNAME_CKEY_FRAME_LABEL) {
		label = DOMNodeP<DOMElement>(readKeyFrameLabel(object));

	} else if (className == TNAME_CTIMELINE) {
		label = DOMNodeP<DOMElement>(readMainStage(object));

	} else if (className == TNAME_CSUB_STAGE) {
		label = DOMNodeP<DOMElement>(readSubStage(object));

	} else {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

	return label.release();
}

DOMElement* Ver3SDOMReader::readKeyFrameLabel(int object) {
	if (object == 0) {
		CString className;
		if (!m_unserializer->ReadObject(className,object)) {
			return (DOMElement*)m_unserializer->GetObject(object);
		}
		if (className != TNAME_CKEY_FRAME_LABEL) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
	}

	CString name;
	CString comment;
	m_unserializer->ReadValue(name);
	m_unserializer->ReadValue(comment);

	DOMNodeP<DOMElement> label(m_doc->createElement(L"dummy"));
	appendString(label.get(),WSDOMN_LABEL_NAME,   name);
	appendString(label.get(),WSDOMN_LABEL_COMMENT,comment);
	ASSERT(0 < object);
	m_unserializer->RegistObject(object,label.get());
	return label.release();
}

DOMElement* Ver3SDOMReader::readMainStage(int object) {
	if (object == 0) {
		CString className;
		if (!m_unserializer->ReadObject(className,object)) {
			return (DOMElement*)m_unserializer->GetObject(object);
		}
		if (className != TNAME_CTIMELINE) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
	}
	
	DOMNodeP<DOMElement> stage(m_doc->createElement(WSDOMN_MAIN_STAGE));
	readStage(stage.get(),object);

	return stage.release();
}

DOMElement* Ver3SDOMReader::readSubStages() {
	DOMNodeP<DOMElement> stages(m_doc->createElement(WSDOMN_SUB_STAGES));
	
	int count;
	m_unserializer->ReadValue(count);
	for (int n = 0;n < count;n++) {
		DOMNodeP<DOMElement> stage(readSubStage(0));
		stages->appendChild(stage.release());
	}

	return stages.get();
}

DOMElement* Ver3SDOMReader::readSubStage(int object) {
	ASSERT(0 <= object);
	if (object == 0) {
		CString className;
		if (!m_unserializer->ReadObject(className,object)) {
			return (DOMElement*)m_unserializer->GetObject(object);
		}
		if (className != TNAME_CSUB_STAGE) {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
	}

	DOMNodeP<DOMElement> stage(m_doc->createElement(WSDOMN_SUB_STAGE));

	readStage(stage.get(),object);
	CString name;
	m_unserializer->ReadValue(name);
	insertString(stage.get(),WSDOMN_NAME,name);

	return stage.release();
}

void Ver3SDOMReader::readStage(DOMElement* stage,int object) {
	ASSERT(AfxIsValidAddress(stage,sizeof DOMElement,TRUE));
	
	DOMNodeP<DOMElement> layers(m_doc->createElement(WSDOMN_LAYERS));
	int count;
	m_unserializer->ReadValue(count);
	for (int n = 0;n < count;n++) {
		DOMNodeP<DOMElement> layer(readLayer());
		layers->appendChild(layer.release());
	}

	ASSERT(0 < object);
	setAttID(stage,object);
	m_unserializer->RegistObject(object,stage);
	stage->appendChild(layers.release());
}

DOMElement* Ver3SDOMReader::readLayer() {
	readTypeSafeObject(TNAME_CTIMELINE_LAYER);

	DOMNodeP<DOMElement> layer(m_doc->createElement(WSDOMN_LAYER));

	CString     name;
	DOMNodeP<DOMElement> chr;
	BOOL visible;
	BOOL locked;
	int  frame_count;
	m_unserializer->ReadValue(name);
	m_unserializer->ReadValue(visible);
	m_unserializer->ReadValue(locked);
	chr = DOMNodeP<DOMElement>(readChar());
	m_unserializer->ReadValue(frame_count);

	appendString (layer.get(),WSDOMN_NAME,   name);
	appendBoolean(layer.get(),WSDOMN_VISIBLE,cast_bool(!visible));
	appendBoolean(layer.get(),WSDOMN_LOCKED, cast_bool(locked));
	appendIDREF  (layer.get(),WSDOMN_CHAR,chr.get());

	DOMNodeP<DOMElement> keyframes(m_doc->createElement(WSDOMN_KEY_FRAME_LIST));
	appendInt(keyframes.get(),WSDOMN_FRAME_LENGTH,frame_count);
	int count;
	m_unserializer->ReadValue(count);
	for (int n = 0;n < count;n++){
		int pos;
		BOOL empty;
		m_unserializer->ReadValue(pos);

		readTypeSafeObject(TNAME_CKEY_FRAME);	
		m_unserializer->ReadValue(empty);

		DOMNodeP<DOMElement> placing  (readPlacing());
		DOMNodeP<DOMElement> coloring (readColoring());
		DOMNodeP<DOMElement> animating(readAnimating());
		DOMNodeP<DOMElement> actions  (readActions(FALSE));
		DOMNodeP<DOMElement> label    (readLabel());

		DOMNodeP<DOMElement> keyframe(m_doc->createElement(WSDOMN_KEY_FRAME));
		appendInt    (keyframe.get(),WSDOMN_POS,  pos);
		appendBoolean(keyframe.get(),WSDOMN_EMPTY,cast_bool(empty));
		keyframe->appendChild(placing.release());
		keyframe->appendChild(coloring.release());
		keyframe->appendChild(animating.release());
		if (label.get() != NULL) {
			DOMElement* element = (DOMElement*)label->getFirstChild();
			CString name = ((DOMText*)element->getFirstChild())->getNodeValue();
			element = (DOMElement*)element->getNextSibling();
			CString comment = ((DOMText*)element->getFirstChild())->getNodeValue();

			appendBoolean(keyframe.get(),WSDOMN_USE_LABEL,true);
			appendString(keyframe.get(), WSDOMN_LABEL_NAME,name);
			appendString(keyframe.get(), WSDOMN_LABEL_COMMENT,comment);
			setAttID(keyframe.get(),(int)label.get());

		} else {
			appendBoolean(keyframe.get(),WSDOMN_USE_LABEL,false);
			appendString(keyframe.get(), WSDOMN_LABEL_NAME,"");
			appendString(keyframe.get(), WSDOMN_LABEL_COMMENT,"");

			setAttID(keyframe.get(),(int)keyframe.get());
		}
		keyframe->appendChild(actions.release());

		keyframes->appendChild(keyframe.release());
	}

	layer->appendChild(keyframes.release());
	chr.release();
	return layer.release();
}


DOMElement* Ver3SDOMReader::readPlacing() {
	short x,y;
	double cx,cy;
	double rotate;

	m_unserializer->ReadValue(x);
	m_unserializer->ReadValue(y);
	m_unserializer->ReadValue(cx);
	m_unserializer->ReadValue(cy);
	m_unserializer->ReadValue(rotate);

	DOMNodeP<DOMElement> placing(m_doc->createElement(WSDOMN_PLACING));
	appendInt(placing.get(),WSDOMN_X,x);
	appendInt(placing.get(),WSDOMN_Y,y);
	appendDouble(placing.get(),WSDOMN_CX,x);
	appendDouble(placing.get(),WSDOMN_CY,y);
	appendDouble(placing.get(),WSDOMN_RADIAN,x);

	return placing.release();
}

DOMElement* Ver3SDOMReader::readColoring() {
	short  addR,addG,addB;
	double mulR,mulG,mulB;
	int edit;

	m_unserializer->ReadValue(mulR);
	m_unserializer->ReadValue(mulG);
	m_unserializer->ReadValue(mulB);
	m_unserializer->ReadValue(addR);
	m_unserializer->ReadValue(addG);
	m_unserializer->ReadValue(addB);
	m_unserializer->ReadValue(edit);
	
	DOMNodeP<DOMElement> coloring(m_doc->createElement(WSDOMN_COLORING));
	appendInt(coloring.get(),WSDOMN_ADD_RED,  addR);
	appendInt(coloring.get(),WSDOMN_ADD_GREEN,addG);
	appendInt(coloring.get(),WSDOMN_ADD_BLUE, addB);

	appendDouble(coloring.get(),WSDOMN_MUL_RED,  mulR);
	appendDouble(coloring.get(),WSDOMN_MUL_GREEN,mulG);
	appendDouble(coloring.get(),WSDOMN_MUL_BLUE, mulB);
	return coloring.release();
}

DOMElement* Ver3SDOMReader::readAnimating() {
	BOOL bAnimate;
	BOOL sizing;
	int rotateType;
	int rotateCount;
	int easing;
	m_unserializer->ReadValue(bAnimate);
	m_unserializer->ReadValue(sizing);
	m_unserializer->ReadValue(rotateType);
	m_unserializer->ReadValue(rotateCount);
	m_unserializer->ReadValue(easing);

	DOMNodeP<DOMElement> animating(m_doc->createElement(WSDOMN_ANIMATING));
	appendBoolean(animating.get(),WSDOMN_USE_ANIMATION,cast_bool(bAnimate));
	appendBoolean(animating.get(),WSDOMN_EASING,   cast_bool(easing));
	return animating.release();
}

void Ver3SDOMReader::appendString(DOMNode* parent,LPCWSTR name,LPCWSTR text) {
	ASSERT(AfxIsValidAddress(parent,sizeof DOMNode));
	ASSERT(AfxIsValidString(name));
	ASSERT(AfxIsValidString(text));

	DOMNodeP<DOMNode> child(m_doc->createElement(name));
	DOMNodeP<DOMText> value(m_doc->createTextNode(text));
	child->appendChild(value.release());
	parent->appendChild(child.release());
}

void Ver3SDOMReader::appendString(DOMNode* parent,LPCWSTR name,LPCSTR text) {
	USES_CONVERSION;
	appendString(parent,name,A2W(text));
}

void Ver3SDOMReader::appendColor(DOMNode* n,LPCWSTR name,COLORREF cr) {
	CString str;
	str.Format("#%06x",cr);
	appendString(n,name,str);
}

void Ver3SDOMReader::appendSize(DOMNode* n,LPCWSTR name,SIZE sz) {
	DOMNode* p = m_doc->createElement(name);
	n->appendChild(p);
	appendInt(p,WSDOMN_CX,sz.cx);
	appendInt(p,WSDOMN_CY,sz.cy);
}

void Ver3SDOMReader::appendPoint   (DOMNode* n,LPCWSTR name,POINT pt) {
	DOMNode* p = m_doc->createElement(name);
	n->appendChild(p);
	appendInt(p,WSDOMN_X,pt.x);
	appendInt(p,WSDOMN_Y,pt.y);
}

void Ver3SDOMReader::appendInt(DOMNode* n,LPCWSTR name,int i) {
	CString str;
	str.Format("%d",i);
	appendString(n,name,str);
}

void Ver3SDOMReader::appendDouble(DOMNode* n,LPCWSTR name,double d) {
	CString format;
	format.Format("%f",d);
	appendString(n,name,format);
}

void Ver3SDOMReader::appendBoolean(DOMNode* n,LPCWSTR name,BOOL b) {
	appendString(n,name,b ? ASDOMV_TRUE: ASDOMV_FALSE);
}

void Ver3SDOMReader::appendNullData(DOMNode* n,LPCWSTR name) {
	appendString(n,name,"");
}

void Ver3SDOMReader::skipBuffer() {
	INT n;
	m_unserializer->ReadValue(n);
	m_unserializer->Skip(n);
}

void Ver3SDOMReader::appendIDREF(DOMNode* object,LPCWSTR name,DOMNode* ref) {
	ASSERT(AfxIsValidAddress(object,sizeof DOMNode));
	ASSERT(AfxIsValidString(name));
	USES_CONVERSION;
	int idref;
	if (ref == NULL) {
		DOMNodeP<DOMElement> child(m_doc->createElement(name));
		child->setAttribute(SDOMArchive::NILL_NAME,SDOMArchive::NILL_VALUE);
		object->appendChild(child.release());
	} else {
		idref = (int)ref;
		ASSERT(AfxIsValidAddress(ref,sizeof DOMNode));
		ASSERT(0 < idref);
		appendString(object,name,A2W(SDOMArchive::encodeID(idref)));
	}
}

void Ver3SDOMReader::setAttID(DOMElement* element,int id) {
	ASSERT(AfxIsValidAddress(element,sizeof DOMElement));
	ASSERT(0 < id);
	USES_CONVERSION;
	element->setAttribute(WSDOMN_ID,A2W(SDOMArchive::encodeID(id)));
}

void Ver3SDOMReader::skipRGB() {
	BYTE skip;
	m_unserializer->ReadValue(skip);
	m_unserializer->ReadValue(skip);
	m_unserializer->ReadValue(skip);
}

void Ver3SDOMReader::skipFPOINT() {
	double skip;
	m_unserializer->ReadValue(skip);
	m_unserializer->ReadValue(skip);
}

void Ver3SDOMReader::insertString(DOMNode* parent,LPCWSTR key,LPCSTR text) {
	USES_CONVERSION;
		
	DOMNodeP<DOMNode> name (m_doc->createElement(key));
	DOMNodeP<DOMText> value(m_doc->createTextNode(A2W(text)));
	name->appendChild(value.release());
	parent->insertBefore(name.release(),parent->getFirstChild());
}

namespace Ver3SDOMReaderImpl {

	BOOL cast_bool(BOOL b) {
		if (b == FALSE) {
			ASSERT(!b);
			return false;
		} else {
			ASSERT(b);
			return TRUE;
		}
	}

	BOOL IsVer3(SERIALIZE_CONTEXT& header) {
		ASSERT(AfxIsValidAddress(&header,SIZE_OF_SERIALIZE_CONTEXT));
		if (header.m_dwSignature != 'SDBH') {
			//TRACE("Invalid signature %X \n",header.m_dwSignature);
			return false;
		}
		if (header.m_dwContextId != SC_FILE_SDB_V3) {
			//TRACE("Old version %d \n",header.m_dwContextId);
			return false;
		}
		return true;
	}

	BYTE hex(int n) {
		ASSERT(0x0 <= n && n <= 0xF);
		if (n < 10) {
			return '0' + n;
		} else {
			return 'A' + n - 10;
		}
	}

	CString buffer2string(LPCVOID buffer,UINT length) {
		CString string;
		LPBYTE src = (LPBYTE)buffer;
		LPBYTE dst  =(LPBYTE)string.GetBuffer(length * 2);
		for (int n = 0;n < length;n++) {
			ASSERT(AfxIsValidAddress(src,1,FALSE));
			ASSERT(AfxIsValidAddress(dst,2,TRUE));
			int byte = *src++;
			*dst++ = hex(byte >> 4);
			*dst++ = hex(byte & 0xF);
		}
		string.ReleaseBuffer();
		return string;
	}

	BOOL IsLeadJpeg(int n) {
		return n == FILE_JFIF ||
			   n == FILE_LEAD1JFIF ||
			   n == FILE_LEAD2JFIF;
	}

};
