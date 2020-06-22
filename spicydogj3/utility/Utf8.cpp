#include "stdafx.h"
#include "Utf8.h"
#include <malloc.h>

#if defined(_DEBUG)
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString ConvertToUTF8(LPCTSTR input) {
	CString output;

	// SJIS -> UNICODE
	LPWSTR unicode;
#if defined(_UNICODE) || defined(_UNICODE) 
	int unicodeLength = wcslen(input)+1;
	unicode = (LPWSTR) ::_alloca((unicodeLength+1) * sizeof WCHAR);
	ZeroMemory(uncode,unicodeLength * sizeof WCHAR);
	wcscpy(unicode,input);
#else
	int unicodeLength = MultiByteToWideChar(CP_ACP,0,input,-1,0,0);
	unicode = (LPWSTR) ::_alloca((unicodeLength+1) * sizeof WCHAR);
	ZeroMemory(unicode,unicodeLength);
	VERIFY(MultiByteToWideChar(CP_ACP,0,input,-1,unicode,unicodeLength));
#endif
	ASSERT(AfxIsValidString(unicode));
	//TRACE("%S\n",unicode);


	// UNICODE -> UTF8
	#define WRITE_BUTE(byte) output += (CHAR)byte

	while (*unicode != 0) {
		ASSERT(AfxIsValidString(unicode));
		WCHAR wc = *unicode; 
		// 1 バイト
		if (0x01 <= wc && wc <= 0x7F) {
			WRITE_BUTE(wc);
		// 2 バイト
		} else if (wc == 0x0000 || (0x0080 <= wc && wc <= 0x07FF)) {
			WRITE_BUTE(0xC0 | ((wc >> 6) & 0x1F));
			WRITE_BUTE(0x80 | (wc & 0x3F));

		// 3 バイト
		} else {
			WRITE_BUTE(0xE0 | ((wc >> 12) & 0x0F));
			WRITE_BUTE(0x80 | ((wc >> 6 ) & 0x3F));
			WRITE_BUTE(0x80 | (wc & 0x3F));
		}
		unicode++;
	}
	WRITE_BUTE(0);
	ASSERT(AfxIsValidString(unicode));

	return output;
}

#if defined(_DEBUG)
class UTF8Test : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(UTF8Test);
  CPPUNIT_TEST(testConvertToUTF8);
  CPPUNIT_TEST_SUITE_END();
private:

public:
	UTF8Test() {}
	virtual ~UTF8Test() {}
	virtual void setUp() {}
	virtual void tearDown() {}
	
	void testConvertToUTF8() {
		CPPUNIT_ASSERT(ConvertToUTF8("ABCDEFG") == "ABCDEFG");
		CPPUNIT_ASSERT(ConvertToUTF8("表能") == "\xE8\xA1\xA8\xE8\x83\xBD");
		CPPUNIT_ASSERT(ConvertToUTF8("宇宙") == "\xE5\xAE\x87\xE5\xAE\x99");
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(UTF8Test);
#endif

