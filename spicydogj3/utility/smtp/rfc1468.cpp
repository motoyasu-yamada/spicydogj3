#include "stdafx.h"
#include "rfc1468.h"
#include "kconv.h"

static const LPCTSTR CRLF    = "\r\n";
static const LPCTSTR TOJIS   = "\033$B";
static const LPCTSTR TOASCII = "\033(B";
static const size_t  TOJIS_LENGTH   = 3;
static const size_t  TOASCII_LENGTH = 3;
static const LPCSTR  MIME           = "=?ISO-2022-JP?B?";

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CRFC1468::EncodeBody(LPCSTR in) {
	CString out;
	convertEncoding(in,out);
	return out;
}

CString CRFC1468::EncodeSubject(LPCSTR in) {
	CString str;
	CString jis;
	CString sub;

	int len = convertEncoding(in,jis);
	if (len == 0) {
		return "";
	}

	subjis(jis,36,sub);
	str  += MIME;
	str  += m_base64.Encode(sub,strlen(sub));
	str  += "?=";

	for(;;) {
		int len = subjis(jis,42,sub);
		if (len == 0) {
			break;
		}
		ASSERT(0 < len);

		str += CRLF;
		str += " ";
		str += MIME;
		str += m_base64.Encode(sub,strlen(sub));
		str += "?=";
	}
	return str;
}

CString CRFC1468::escapeSingleDot(LPCTSTR in) {
	ASSERT(_tcscmp("\015\012","\r\n") == 0);

	CString out;
	UINT length = _tcslen(in);
	while (*in != '\0') {
		if (_tcsncmp(in,"\n.",2) == 0) {
			out += "\n..";
			continue;
		}
		out += *in;
	}
	return out;
}

CString CRFC1468::_copyjis(LPCTSTR& src,size_t len,size_t& rest) {
	ASSERT(AfxIsValidString(src,len));
	ASSERT(len <= rest);
	CString out;
	out    = CString(src,len);
	src   += len;
	rest  -= len;
	ASSERT(0 <= rest);
	return out;
}
CString CRFC1468::_addjis(LPCTSTR add,size_t& rest) {
	ASSERT(AfxIsValidString(add));
	ASSERT(strlen(add) <= rest);
	rest += strlen(add);
	ASSERT(0 <= rest);
	return add;
}

BOOL CRFC1468::subjis(CString& src,size_t required,CString& out) {
	ASSERT(AfxIsValidString(src,required));
	out.Empty();
	
	size_t rest  = required;
	LPCSTR begin = src;
	LPCSTR temp  = begin;
	BOOL   dbcs  = FALSE;
	while (*temp != '\0' && rest != 0) {
		ASSERT(0 <= rest && rest <= required);
		if (memcmp(temp,TOJIS,TOJIS_LENGTH) == 0) {
			if (rest < 8) {
				break;
			}
			ASSERT(!dbcs);
			dbcs = TRUE;
			out   += _copyjis(temp,TOJIS_LENGTH,rest);
			out   += _copyjis(temp,2,rest);
			continue;
		}
		if (memcmp(temp,TOASCII,TOASCII_LENGTH) == 0) {
			if (rest < 3) {
				break;
			}
			ASSERT(dbcs);
			dbcs = FALSE;
			out   += _copyjis(temp,TOASCII_LENGTH,rest);
			continue;
		}
		if (dbcs) {  // jis mode ?
			if (rest < 5) {	// enough room ?
				break;
			}
			out += _copyjis(temp,2,rest);
		} else {
			out += _copyjis(temp,1,rest);
		}
	}
	ASSERT(begin <= temp);
	size_t done = temp - begin;
	if (done != 0) {
		src = src.Mid(done);
	}
	if (dbcs) {
		out += _addjis(TOASCII,rest);
		src = TOJIS + src;
	}
	ASSERT(0 <= rest && rest <= required);
	return done != 0;
}

int CRFC1468::convertEncoding(LPCSTR str,CString& out) {
	out.Empty();
	int dbcs,kctr;
	dbcs = kctr = 0;
	register unsigned char ch1; 
	unsigned int zenkaku;
	int han;
	for (register int i = 0;; i++) {
		ch1 = *(str+i);
		if (ch1 == 0)
			break;
		if (iskanji(ch1)) {  // SJIS kanji start ?
			if(!dbcs) {
				dbcs = 1;
				out += TOJIS;
			    //out->Add(TOJIS,3);    // put Esc sequence
			}
			// convert 1 kanji to jis
			unsigned short sjis = (ch1 << 8) | (*(str + ++i) & 0x00ff);
			unsigned short jis = sjis2jis(sjis);
			if (jis == 0) {
				jis = 8739;
			}
			out += static_cast<CHAR>((jis >> 8) & 0xFF);
			out += static_cast<CHAR>( jis       & 0xFF);
			kctr++;
			continue;
		}
		han = han2zen(&zenkaku, (unsigned char*)str + i);
		if (han == 0) {	// no hankaku
			if (dbcs) {  // kanji mode ?
				dbcs = 0;
			    //out->Add(TOASCII,3);    // put Esc sequence
				out += TOASCII;
			}
			if (ch1 > 127) {	// not 7 bit
				ch1 = 32;
			}
			//out->AddByte(ch1);
			out += static_cast<CHAR>(ch1);
			continue;
		}
		if(!dbcs) {
			dbcs = 1;
		    //out->Add(TOJIS,3);    // put Esc sequence
			out += TOJIS;
		}
		out += static_cast<CHAR>((zenkaku >> 8) & 0xFF);
		out += static_cast<CHAR>( zenkaku       & 0xFF);
		kctr++;
		i = i + han -1;		//先頭2文字が濁音・半濁音カナの組合せ?
	}
	if (dbcs) {  // kanji mode ?
		out += TOASCII;
	    //out->Add(TOASCII,3);    // put Esc sequence
	}
	
	return kctr;
}
