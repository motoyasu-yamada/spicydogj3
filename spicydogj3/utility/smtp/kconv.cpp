//
// kconv.cc    kanji conversion helper
//
// 				Author Tatsuo Baba
//
#include "stdafx.h"
#include <string.h>
#include "kconv.h"
#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// JIS ESCAPE sequence
static char TOJIS[] = "\033$B";  // 1983
static char TOJIS2[] = "\033$@"; // 1978
static char TOASCII[] = "\033(J";// ROMA 
static char TOASCII2[] = "\033(B";// ASCII
static char TOHANKANA[] = "\033(I";// HANKAKU KANA
// JIS 7bit mode  escape sequence for hankaku kana
#define SO 0x0e
#define SI 0x0f
static unsigned int jis2sjis(unsigned int jis)
{    unsigned int hib, lob;    
    hib = (jis >> 8) & 0xff;    lob = jis & 0xff;
    lob += (hib & 1) ? 0x1f : 0x7d;    if (lob >= 0x7f) lob++;
    hib = ((hib - 0x21) >> 1) + 0x81;    if (hib > 0x9f) hib += 0x40;
    return (hib << 8) | lob;
}
static
unsigned char sjis_tab[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 1x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 2x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 3x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 4x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 5x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 6x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 7x */
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 8x */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 9x */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* Ax */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* Bx */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* Cx */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* Dx */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* Ex */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,	/* Fx */
};
int iskanji(int c) {
	return sjis_tab[c & 0xff];
}
int han2zen(unsigned int *zenkaku, unsigned char *str)
{
    static unsigned int z[64] = {
        0x2121,0x2123,0x2156,0x2157,0x2122,0x2126,0x2572,0x2521,
        0x2523,0x2525,0x2527,0x2529,0x2563,0x2565,0x2567,0x2543,
        0x213c,0x2522,0x2524,0x2526,0x2528,0x252a,0x252b,0x252d,
        0x252f,0x2531,0x2533,0x2535,0x2537,0x2539,0x253b,0x253d,
        0x253f,0x2541,0x2544,0x2546,0x2548,0x254a,0x254b,0x254c,
        0x254d,0x254e,0x254f,0x2552,0x2555,0x2558,0x255b,0x255e,
        0x255f,0x2560,0x2561,0x2562,0x2564,0x2566,0x2568,0x2569,
        0x256a,0x256b,0x256c,0x256d,0x256f,0x2573,0x212b,0x212c};
    typedef struct {
        unsigned char han;
        unsigned int zen;
    } TBL;
    static TBL daku[] = {
        {0xb3,0x2574},{0xb6,0x252c},{0xb7,0x252e},{0xb8,0x2530},
        {0xb9,0x2532},{0xba,0x2534},{0xbb,0x2536},{0xbc,0x2538},
        {0xbd,0x253a},{0xbe,0x253c},{0xbf,0x253e},{0xc0,0x2540},
        {0xc1,0x2542},{0xc2,0x2545},{0xc3,0x2547},{0xc4,0x2549},
        {0xca,0x2550},{0xcb,0x2553},{0xcc,0x2556},{0xcd,0x2559},
        {0xce,0x255c},{0,0}};
    static TBL handaku[] = {
        {0xca,0x2551},{0xcb,0x2554},{0xcc,0x2557},{0xcd,0x255a},
        {0xce,0x255d},{0,0}};
    int i;
    if (*str < 0xa0 || *str > 0xdf) return 0;
    if (*(str+1) == 0xde) {           /* ‘÷‰¹•„ */
        for (i = 0; daku[i].zen != 0; i++)
            if (*str == daku[i].han) {
                *zenkaku = jis2sjis(daku[i].zen);
                return 2;
            }
    } else if (*(str+1) == 0xdf) {    /* ”¼‘÷‰¹•„ */
        for (i = 0; handaku[i].zen != 0; i++)
            if (*str == handaku[i].han) {
                *zenkaku = jis2sjis(handaku[i].zen);
                return 2;
            }
    }
    *zenkaku = jis2sjis(z[*str - 0xa0]);
    return 1;
}
static unsigned int hankaku2zen(int hankaku)
{
    static unsigned int z[64] = {
        0x2121,0x2123,0x2156,0x2157,0x2122,0x2126,0x2572,0x2521,
        0x2523,0x2525,0x2527,0x2529,0x2563,0x2565,0x2567,0x2543,
        0x213c,0x2522,0x2524,0x2526,0x2528,0x252a,0x252b,0x252d,
        0x252f,0x2531,0x2533,0x2535,0x2537,0x2539,0x253b,0x253d,
        0x253f,0x2541,0x2544,0x2546,0x2548,0x254a,0x254b,0x254c,
        0x254d,0x254e,0x254f,0x2552,0x2555,0x2558,0x255b,0x255e,
        0x255f,0x2560,0x2561,0x2562,0x2564,0x2566,0x2568,0x2569,
        0x256a,0x256b,0x256c,0x256d,0x256f,0x2573,0x212b,0x212c };
    if (hankaku < 0xa0 || hankaku > 0xdf) return 0;
    return z[hankaku - 0xa0];
}
static unsigned int euc2jis(unsigned int euc)
{
    unsigned int jis;
    if ((euc & 0xff00) == 0x8e00)
        jis = hankaku2zen(euc & 0xff);
    else jis = euc & ~0x8080;
    return jis;
}
BYTE *Beuc2jis(BYTE *data,int len,int *outlen)
{
	BYTE *tp = new BYTE[len*3 +10];
	if (tp == NULL) {
		*outlen = 0; 
		return NULL;
	}
	int unsigned jis = 0;
	int j = 0;
	int dbcs = 0;
	int hankana = 0;
	BYTE ch;
	for (int i= 0;i < len;i++) {
		ch = data[i];
		tp[j++] = ch;
		if (ch < 128) {		//  012345
			if (dbcs == 0 && hankana == 0)	// 
				continue;
			memcpy(tp+j-1,TOASCII,3);
			tp[j+2] = ch;
			j += 3;
			dbcs = hankana = 0;
			continue;
		}
		j--;
		if (ch == 0x8e) {	// hankaku kana ?
			if (dbcs || hankana == 0) {
				dbcs = 0; hankana = 1; 
				memcpy(tp+j,TOHANKANA,3);
				j += 3;
			}
			tp[j++] = data[++i];
			continue;
		}
		if (dbcs == 0 || hankana) {
			dbcs = 1; hankana = 0;
			memcpy(tp+j,TOJIS,3);
			j += 3;
		}
		jis = euc2jis( (ch << 8) | data[++i]); 
		tp[j++] = jis >> 8;
		tp[j++] = jis;
	}
	if (dbcs || hankana) {
		memcpy(tp+j,TOASCII,3);
		j += 3;
	}
	tp[j] = 0;
	*outlen = j;
	return tp;
}
BYTE *Bjis2sjis(BYTE *data,int len,int *outlen)
{
	BYTE *tp = new BYTE[len +1];
	if (tp == NULL) {
		*outlen = 0; 
		return NULL;
	}
	int dest = 0;
	int dbcs  = 0;
	int hankana = 0;
	for (int pos = 0;pos < len;pos++) {
		tp[dest++] = data[pos];
		if (data[pos] != '\033') {
			if (dbcs == 0 || hankana)
				continue;
			dest--;
			// convert 1 jis to sjis
			int sjis = jis2sjis((data[pos] << 8) | (data[pos+1] & 0x00ff));
			if (sjis == 0) {	// 1999.04.03  fixed
				if (data[pos+1] == '\033')  // escape found 
					continue;
				sjis = 33185;  // ¡
			}
			tp[dest++] = sjis >> 8;
			tp[dest++] = sjis;
			pos++;
			continue;
		}
		// escape found
		dest--;
		if (memcmp(data+pos,TOJIS,3) == 0 || memcmp(data+pos,TOJIS2,3) == 0) {
			dbcs = 1; hankana = 0;
			pos += 2;
			continue;
		}
		if (memcmp(data+pos,TOASCII,3) == 0 || memcmp(data+pos,TOASCII2,3) == 0) {
			dbcs = hankana = 0;
			pos += 2;
			continue;
		}
		if (memcmp(data+pos,TOHANKANA,3) == 0) {
			dbcs = 0; hankana = 1;
			pos += 2;
			continue;
		}
	}
	tp[dest] = 0;
	*outlen = dest;
	return tp;
}
unsigned int sjis2jis(unsigned int sjis)
{
    unsigned int hib, lob;
    hib = (sjis >> 8) & 0xff;
    lob = sjis & 0xff;
    hib -= (hib <= 0x9f) ? 0x71 : 0xb1;
    hib = (hib << 1) + 1;
    if (lob > 0x7f) lob--;
    if (lob >= 0x9e) {
        lob -= 0x7d;
        hib++;
    } else lob -= 0x1f;
    return (hib << 8) | lob;
}
BYTE *Bsjis2jis(BYTE *data,int len,int *outlen)
{
	BYTE *tp = new BYTE[len*3 + 10];
	if (tp == NULL) {
		*outlen = 0; 
		return NULL;
	}
	int dbcs,j;
	dbcs = j = 0;
	BYTE ch1; 
	unsigned int zenkaku;
	int han = 0;
	int hankana = 0;
	for (int i = 0;i < len; i++) {
		ch1 = data[i];
		if (iskanji(ch1)) {  // SJIS kanji start ?
//			if(hankana) {
//				hankana = 0;
//				tp[j++] = SI;
//			}
			if(!dbcs || hankana) {
				dbcs = 1; hankana = 0;
				memcpy(tp+j,TOJIS,3);
				j += 3;
			}
			// convert 1 kanji to jis
			unsigned int  jis = sjis2jis((ch1 << 8) | data[++i]);
			if (jis == 0)
				jis = 8739;
			tp[j++] = jis >> 8;
			tp[j++] = jis;
			continue;
		}
		han = han2zen(&zenkaku, data+i);
		if (han == 0) {	// no hankaku
			if (dbcs || hankana) {  // kanji mode ?
				dbcs = hankana = 0;
				memcpy(tp+j,TOASCII,3);
				j += 3;
			}
//			if (hankana) {  // hankaku kana mode ?
//				hankana = 0;
//				tp[j++] = SI;
//			}
			if (ch1 > 127)	// not 7 bit
				ch1 = 32;
			tp[j++] = ch1;
			continue;
		}	// hankaku kana mode ?
		if (hankana == 0) {
			hankana = 1;
			memcpy(tp+j,TOHANKANA,3);
			j += 3;
//			tp[j++] = SO;
		}
		tp[j++] = ch1;
	}
	if (dbcs || hankana) {  // kanji or hankaku kana mode ?
		memcpy(tp+j,TOASCII,3);
		j += 3;
	}
//	else 
//		if (hankana)   // kana mode ?
//			tp[j++] = SI;
	tp[j] = 0;
	*outlen = j;
	return tp;
}
static unsigned int euc2sjis(unsigned int euc)
{
    unsigned int jis;
    unsigned int hib, lob;
    if ((euc & 0xff00) == 0x8e00)
        jis = hankaku2zen(euc & 0xff);
    else jis = euc & ~0x8080;
    hib = (jis >> 8) & 0xff;
    lob = jis & 0xff;
    lob += (hib & 1) ? 0x1f : 0x7d;
    if (lob >= 0x7f) lob++;
    hib = ((hib - 0x21) >> 1) + 0x81;
    if (hib > 0x9f) hib += 0x40;
    return (hib << 8) | lob;
}
BYTE *Beuc2sjis(BYTE *data,int len,int *outlen)
{
	BYTE *tp = new BYTE[len + 1];
	if (tp == NULL) {
		*outlen = 0; 
		return NULL;
	}
	int j = 0;
	BYTE ch1; 
	for (int i = 0;i < len; i++) {
		ch1 = data[i];
		if (ch1 > 127) {  // kanji start ?
			// convert 1 kanji to jis
			if (ch1 == 0x8e) { // hankaku kana
				tp[j++] = data[++i] ;
				continue;
			}
			unsigned int  sjis = euc2sjis((ch1 << 8) | data[++i]);
			if (sjis == 0)
				sjis = 33185;
			tp[j++] = sjis >> 8;
			tp[j++] = sjis;
			continue;
		}
		tp[j++] = ch1;
	}
	tp[j] = 0;
	*outlen = j;
	return tp;
}
unsigned int jis2euc(unsigned int jis)
{
    return jis | 0x8080;
}
unsigned int sjis2euc(unsigned int sjis)
{
	return jis2euc(sjis2jis(sjis));    
	// 1999.12.04 Fix ‰žAŽžA‹ž‚È‚Ç‚ª ‚¨‚©‚µ‚¢
	/*
	unsigned int hib, lob;
    hib = (sjis >> 8) & 0xff;
    lob = sjis & 0xff;
    hib -= (hib <= 0x9f) ? 0x71 : 0xb1;
    hib = (hib << 1) + 1;
    if (lob >= 0x9e) {
        lob -= 0x7e;
        hib++;
    } else if (lob > 0x7f) lob -= 0x20;
    else lob -= 0x1f;
    hib |= 0x80;
    lob |= 0x80;
    return (hib << 8) | lob;
*/
}
BYTE *Bjis2euc(BYTE *data,int len,int *outlen)
{
	BYTE *tp = new BYTE[len +1];
	if (tp == NULL) {
		*outlen = 0; 
		return NULL;
	}
	int dest = 0;
	int dbcs  = 0;
	int hankana = 0;
	for (int pos = 0;pos < len;pos++) {
		tp[dest++] = data[pos];
		if (data[pos] != '\033') {
			if (dbcs == 0 && hankana ==0)
				continue;
			dest--;
			if (hankana) {
				tp[dest++] = 0x8e;
				tp[dest++] = data[pos];
				continue;
			}
			// convert 1 jis to euc
			int euc = jis2euc((data[pos] << 8) | (data[pos+1] & 0x00ff));
			if (euc == 0) {	// 1999.04.03  fixed
				if (data[pos+1] == '\033')  // escape found 
					continue;
				euc = sjis2euc(33185);  // ¡
			}
			tp[dest++] = euc >> 8;
			tp[dest++] = euc;
			pos++;
			continue;
		}
		// escape found
		dest--;
		if (memcmp(data+pos,TOJIS,3) == 0 || memcmp(data+pos,TOJIS2,3) == 0) {
			dbcs = 1; hankana = 0;
			pos += 2;
			continue;
		}
		if (memcmp(data+pos,TOASCII,3) == 0 || memcmp(data+pos,TOASCII2,3) == 0) {
			dbcs = hankana = 0;
			pos += 2;
			continue;
		}
		if (memcmp(data+pos,TOHANKANA,3) == 0) {
			dbcs = 0; hankana = 1;
			pos += 2;
			continue;
		}
	}
	tp[dest] = 0;
	*outlen = dest;
	return tp;
}
BYTE *Bsjis2euc(BYTE *data,int len,int *outlen)
{
	BYTE *tp = new BYTE[len*2 +1];
	if (tp == NULL) {
		*outlen = 0; 
		return NULL;
	}
	int j = 0;
	BYTE ch1;
	for (int i = 0;i < len; i++) {
		ch1 = data[i];
		if (iskanji(ch1)) {  // kanji start ?
			// convert 1 kanji to euc
			unsigned int  euc = sjis2euc((ch1 << 8) | data[++i]);
			if (euc == 0)
				euc = sjis2euc(33185);
			tp[j++] = euc >> 8;
			tp[j++] = euc;
			continue;
		}
		unsigned int jis = hankaku2zen(ch1);
		if (jis) {
			tp[j++] = 0x8e;
			tp[j++] = ch1;
			continue;
/*			unsigned int  euc = jis2euc(jis); // zenkaku kana
			if (euc == 0)
				euc = sjis2euc(33185);
			tp[j++] = euc >> 8;
			tp[j++] = euc;
			continue;
*/
		}
		tp[j++] = ch1;
	}
	tp[j] = 0;
	*outlen = j;
	return tp;
}
// 0 : Unknown  (input only)
// 1 : SJIS
// 2 : EUC
// 3 : JIS
// 4 : UNICODE
// check kanji code type
int Bktype(BYTE* szData,int len)
{
//	int unitest;
//	IsTextUnicode(szData,len,&unitest);
//	if (unitest & IS_TEXT_UNICODE_UNICODE_MASK)
//		return UNICODE_UCS2;
	if (strchr((const char*)szData,'\033'))
		return JIS_CODE;
	int euc = 0;
	int sjis = 0;
	int unicode = 0;
	//$match_sjis = '([\x81-\x9f\xe0-\xfc][\x40-\x7e\x80-\xfc])+';
	//$match_euc  = '([\xa1-\xfe]{2})+';
	for (int i = 0;i<len;i++) {
		BYTE ch = szData[i];
		if (ch < 128) {
			if (ch == 0)
				unicode++;
			continue;
		}
		BYTE ch2 = szData[++i];
		if ((ch <= 0x9f || ch >= 0xe0)  &&
			(ch2 < 0xa1 || ch2 <= 0xfc) ) {
			sjis++; continue;
		}
		euc++;
	}
	if (unicode > 1)
		return UNICODE_UCS2;
	if (euc == 0 && sjis == 0)
		return UNKNOWN_CODE;
	if (euc > sjis)
		return EUC_CODE;
	return SJIS_CODE;
}
// kanji code converter
BYTE *Bkconv(BYTE* szData, int len, int intype, int outtype,int *outlen)
{
	*outlen = len;		// assume same len
	if (len < 2) {
		BYTE *tp = new BYTE[len+1];
		if (tp == NULL) {
			*outlen = 0; 
			return NULL;
		}
		memcpy(tp,szData,len);
		tp[len] = 0;
		return tp;
	}
	if (intype == UNKNOWN_CODE) {
		intype = Bktype(szData,len);
		if (intype == UNKNOWN_CODE)  // still unknown or no kanji
			intype = SJIS_CODE;		// assume sjis
	}
	// no translate 
	if (intype == outtype) {
		BYTE *tp = new BYTE[len+1];
		if (tp == NULL) {
			*outlen = 0; 
			return NULL;
		}
		memcpy(tp,szData,len);
		tp[len] = 0;
		return tp;
	}
	// now ready to convert
	BYTE *datax;
	datax = szData;
	// 1999.06.18 Support UTF8
	switch ( intype ) {
	case EUC_CODE:
		switch ( outtype ) {
		case SJIS_CODE:
			return Beuc2sjis(datax,len,outlen);
		case JIS_CODE:
			return Beuc2jis(datax,len,outlen);
		}
		break;
	case JIS_CODE:
		switch ( outtype ) {
		case SJIS_CODE:
			return Bjis2sjis(datax,len,outlen);
		case EUC_CODE:
			return Bjis2euc(datax,len,outlen);
		}
		break;
	case SJIS_CODE:
		switch ( outtype ) {
		case JIS_CODE:
			return Bsjis2jis(datax,len,outlen);
		case EUC_CODE:
			return Bsjis2euc(datax,len,outlen);
		}
		break;
	}
	// invalid type
	BYTE *tp = new BYTE[len+1];
	if (tp == NULL) {
		*outlen = 0; 
		return NULL;
	}
	memcpy(tp,datax,len);
	tp[len] = 0;
	return tp;
}

char *KConv::Conv(char *data, int len,int intype, int outtype)
{
	if (m_pData) {
		delete [] m_pData;
		m_pData = 0;
	}
	m_pData = (char*)Bkconv((BYTE*)data,len,intype,outtype, &m_nSize);
	m_nType = outtype;
	return m_pData;
}
