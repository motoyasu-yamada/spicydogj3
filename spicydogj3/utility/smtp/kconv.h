#pragma once

#define UNKNOWN_CODE 0
#define SJIS_CODE 1
#define EUC_CODE  2
#define JIS_CODE  3
#define UNICODE_UCS2  4
#define UNICODE_UTF8  5

BYTE *Bsjis2jis(BYTE * data,int len,int *outlen);
BYTE *Bsjis2euc(BYTE * data,int len,int *outlen);
BYTE *Beuc2sjis(BYTE * data,int len,int *outlen);
BYTE *Beuc2jis(BYTE * data,int len,int *outlen);
BYTE *Bjis2euc(BYTE * data,int len,int *outlen);
BYTE *Bjis2sjis(BYTE * data,int len,int *outlen);
BYTE *Bsjis2unicode(BYTE * data,int len, int *outlen);
BYTE *Bunicode2sjis(BYTE * data,int len, int *outlen);
int han2zen(unsigned int *zenkaku, unsigned char *str);
unsigned int sjis2jis(unsigned int sjis);
int iskanji(int c);
int Bktype(BYTE *szData,int len);
BYTE *Bkconv(BYTE *szData, int len,int intype, int outtype,int *outlen);
class  KConv {
public:
	KConv(){m_nSize=0;m_pData= NULL;};
	~KConv(){if (m_pData) delete []m_pData;};
	char *Conv(char *data, int len,int intype, int outtype);
	void Detach(){m_pData = NULL;m_nSize=0;};
	int GetSize(){return m_nSize;};
	char *GetBuffer(){return m_pData;};
private:
	char *m_pData;	// buffer pointer
	int m_nSize;	// buffer size
	int m_nType;	// current type
};
