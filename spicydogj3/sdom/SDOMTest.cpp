//	$Id: SDOMTest.cpp,v 1.7 2003/07/30 12:10:46 m_yamada Exp $
#include "stdafx.h"

#if defined(_DEBUG)

#include "sdom/Ver3SDOMReader.h"
#include "sdom/VerJ3SDOMReader.h"
#include "sdom/SDOMException.h"
#include "sdom/SDOMWriter.h"
#include "sdom/SDOMBuffer.h"
#include "model/Appli.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <atlconv.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace xercesc_2_2;

/**  @testcase 旧ファイルの読込テスト
		旧ファイルをSDOM形式で読み込む。
		SDOM形式を新ファイル形式で出力。
		出力したファイルをSDOM形式で読み込む。
		というファイル読込テストを行う。 */
class SDOMTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SDOMTest);
		CPPUNIT_TEST(testSDOMBuffer);
		CPPUNIT_TEST(testSerialize);
		CPPUNIT_TEST(testImportSerializeWriteRead);
		CPPUNIT_TEST(testSDOMArchiveHexBin);
		CPPUNIT_TEST(testSlashes);
	CPPUNIT_TEST_SUITE_END();
public:
    SDOMTest();
	void setUp();
    void testImportSerializeWriteRead();
	void testSerialize();
	void testSDOMBuffer();
	void testSDOMArchiveHexBin();
	void testSlashes();

protected:
	CString m_tempDir;
	void testVer3File(LPCTSTR);
	static void setupMemFile(CMemFile& mem,LPCTSTR ptsz);
	static void testSlash(LPCTSTR test);
};
CPPUNIT_TEST_SUITE_REGISTRATION(SDOMTest);

SDOMTest::SDOMTest() {
}

void SDOMTest::testSlashes() {
	testSlash(NULL);
	testSlash("");
	testSlash("\r\t\n\\");
	testSlash("表\\現\\宇\\宙");
}

void SDOMTest::testSlash(LPCTSTR slash) {
	CPPUNIT_ASSERT(strcmp(slash,SDOMArchive::stripSlashes(SDOMArchive::addSlashes(slash))) == 0);
	CPPUNIT_ASSERT(strcmp(slash,SDOMArchive::addSlashes(SDOMArchive::stripSlashes(slash))) == 0);
}

void SDOMTest::setUp() {
	CPPUNIT_ASSERT(GetTempPath(MAX_PATH,m_tempDir.GetBuffer(MAX_PATH)) != 0);
	m_tempDir.ReleaseBuffer();
}

void SDOMTest::setupMemFile(CMemFile& mem,LPCTSTR ptsz) {
	mem.Write(ptsz,strlen(ptsz));
	mem.SeekToBegin();
}

void SDOMTest::testSDOMArchiveHexBin() {
	CString hexL = SDOMArchive::bin2hex("A0- ",4);
	CPPUNIT_ASSERT(hexL == "41302D20");

	LPCTSTR test = "00FF8008F88F00A00A";
	CPPUNIT_ASSERT(SDOMArchive::hexLength(test) == 9);

	BYTE pointer[256];
	SDOMArchive::hex2bin(test,pointer);
	BYTE test2[] = {0x00,0xff,0x80,0x08,0xf8,0x8f,0x00,0xa0,0x0a};
	CPPUNIT_ASSERT(memcmp(pointer,test2,9) == 0);	
}

void SDOMTest::testSDOMBuffer() {
	CString hex = "00FF8008F88F00A00A";
	SDOMBuffer buffer;
	buffer.hex2bin(hex);
	CPPUNIT_ASSERT(buffer.bin2hex() == hex);
}

class StaticSerilizeTest;
class StaticSerilizeTest : public SDOMSerializable {
public:
	std::auto_ptr<StaticSerilizeTest> child1;
	std::auto_ptr<StaticSerilizeTest> child2;
	std::auto_ptr<StaticSerilizeTest> child3;
	BOOL leaf;
	CString name;
	StaticSerilizeTest(int depth,CString _name) {
		name = _name;
		if (depth == 2) {
			leaf = true;
		} else if (depth < 2) {
			child1 = std::auto_ptr<StaticSerilizeTest>(new StaticSerilizeTest(depth+1,name+"-1"));
			child2 = std::auto_ptr<StaticSerilizeTest>(new StaticSerilizeTest(depth+1,name+"-2"));
			child3 = std::auto_ptr<StaticSerilizeTest>(new StaticSerilizeTest(depth+1,name+"-3"));
			leaf = false;
		} else {
			CPPUNIT_ASSERT(FALSE);
		}
	}

	virtual void Serialize(SDOMArchive* archive) {
		USES_CONVERSION;

		ASSERT(AfxIsValidAddress(archive,sizeof SDOMArchive));
		if (!leaf) {
			archive->SerializeStatic(A2W(child1->name),child1.get());
			archive->SerializeStatic(A2W(child2->name),child2.get());
			archive->SerializeStatic(A2W(child3->name),child3.get());
		} else {
			CString valueString = "\\\\表示\\\\</node>";
			int value723 = 723;
			int value0   = 0;
			int value_1  = -1;
			BOOL valueTrue  = true;
			BOOL valueFalse = false;
			COLORREF valueFF00BB = RGB(0xFF,0x00,0xBB);

			archive->SerializeString(L"leaf-1",valueString);
			CPPUNIT_ASSERT(valueString == "\\表示\\</node>");

			archive->SerializeINT(L"leaf-2",value723);
			CPPUNIT_ASSERT_EQUAL(value723,723);

			archive->SerializeINT(L"leaf-3",value0);
			CPPUNIT_ASSERT_EQUAL(value0,0);
			
			archive->SerializeINT(L"leaf-4",value_1);
			CPPUNIT_ASSERT_EQUAL(value_1,-1);

			archive->SerializeBOOL(L"leaf-5",valueTrue);
			CPPUNIT_ASSERT_EQUAL(valueTrue,TRUE);

			archive->SerializeBOOL(L"leaf-6",valueFalse);
			CPPUNIT_ASSERT_EQUAL(valueFalse,FALSE);

			archive->SerializeCOLORREF(L"leaf-7",valueFF00BB);
			CPPUNIT_ASSERT_EQUAL(valueFF00BB,RGB(0xFF,0x00,0xBB));
			
		}
	}
};

void SDOMTest::testSerialize() {
	typedef std::auto_ptr<DOMDocument> DOCUMENTP;
	typedef std::auto_ptr<SDOMReader> READERP;
	DOCUMENTP document;
	READERP reader;

	CString leaf = "<leaf-1>\\\\表示\\\\&lt;/node&gt;</leaf-1>"
		           "<leaf-2>723</leaf-2>"
				   "<leaf-3>0</leaf-3>"
				   "<leaf-4>-1</leaf-4>"
				   "<leaf-5>true</leaf-5>"
				   "<leaf-6>false</leaf-6>"
				   "<leaf-7>#FF00BB</leaf-7>";
	CString xml = CString("<?xml version=\"1.0\" encoding=\"shift_jis\" standalone=\"no\" ?>") +
						"<spicydog version=\"spicydogj3\">" + 
							"<spicydog-1>" + 
								   "<spicydog-1-1>" + leaf + "</spicydog-1-1>" +
		                           "<spicydog-1-2>" + leaf + "</spicydog-1-2>" +
								   "<spicydog-1-3>" + leaf + "</spicydog-1-3>" +
							"</spicydog-1>"
							"<spicydog-2>" + 
								   "<spicydog-2-1>" + leaf + "</spicydog-2-1>" +
		                           "<spicydog-2-2>" + leaf + "</spicydog-2-2>" +
								   "<spicydog-2-3>" + leaf + "</spicydog-2-3>" +
							"</spicydog-2>"
							"<spicydog-3>" + 
								   "<spicydog-3-1>" + leaf + "</spicydog-3-1>" +
		                           "<spicydog-3-2>" + leaf + "</spicydog-3-2>" +
								   "<spicydog-3-3>" + leaf + "</spicydog-3-3>" +
							"</spicydog-3>"
						"</spicydog>";

	CMemFile mem;
	setupMemFile(mem,xml);	
	reader = READERP(new VerJ3SDOMReader);
	document = DOCUMENTP(reader->Read(&mem));
	
	StaticSerilizeTest serializable(0,"spicydog");

	SDOMArchive loading;
	loading.SerializeLoad(document.get(),L"spicydog",&serializable);

	SDOMArchive storing;
	document = DOCUMENTP(storing.SerializeStore(L"spicydog",&serializable));
}

void SDOMTest::testImportSerializeWriteRead() {
	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pwd);
	TRACE("pwd=%s\n",pwd);

	CFileFind find;
	BOOL b = find.FindFile("tests/sdb/*.sdb");
	while (b) {
		b = find.FindNextFile();
		try {
			testVer3File(find.GetFilePath());
			continue;

		} catch(SDOMException* e) {
			DEBUG_ONLY(e->ReportError());
			ASSERT(FALSE);
			TRACE("corrupted file %s \n",find.GetFileName());
			throw;
		} catch(std::exception& e) {
			TRACE("%s \n",e.what());
			ASSERT(FALSE);
			throw;

		} catch(XMLException& e) {
			TRACE("%s(%d):%S\n",e.getSrcFile(),e.getSrcLine(),e.getMessage());
			ASSERT(FALSE);
			throw;

		} catch(...) {
			TRACE("corrupted file %s \n",find.GetFileName());
			ASSERT(FALSE);
			throw;
		}
	}
}

void SerializeStatic(SDOMArchive* archive,Appli* appli) {
	try {
		ASSERT(wcscmp(WSDOMN_SPICYDOG,L"spicydog") == 0);
		archive->SerializeStatic(WSDOMN_SPICYDOG,appli);

	} catch(CException* pe) {
		CString error;
		pe->GetErrorMessage(error.GetBuffer(1024),1024);
		error.ReleaseBuffer();
		TRACE("%s\n",error);
		ASSERT(FALSE);

	} catch(std::exception& e) {
		TRACE("%s \n",e.what());
		ASSERT(FALSE);

	} catch(XMLException& e) {
		TRACE("%s(%d):%S\n",e.getSrcFile(),e.getSrcLine(),e.getMessage());
		ASSERT(FALSE);

	} catch(DOMException& e) {
		TRACE("%S(%d)\n",e.msg,e.code);
		ASSERT(FALSE);

	} catch(...) {
		ASSERT(FALSE);
	}
}

void SDOMTest::testVer3File(LPCTSTR path) {
	TRACE("testVer3File: %s\n",path);
	ASSERT(CString(path).Find("車") == -1);

	typedef std::auto_ptr<DOMDocument> DOCUMENTP;
	typedef std::auto_ptr<SDOMReader> READERP;
	typedef std::auto_ptr<SDOMWriter> WRITERP;
	DOCUMENTP document;
	READERP reader3;
	READERP readerJ;
	WRITERP writerJ;
	
	reader3 = READERP(new Ver3SDOMReader);
	readerJ = READERP(new VerJ3SDOMReader);
	writerJ = WRITERP(new SDOMWriter);

	CString temp;
	CPPUNIT_ASSERT(GetTempFileName(m_tempDir,"SDT",0,temp.GetBuffer(MAX_PATH)) != 0);
	temp.ReleaseBuffer();

	std::auto_ptr<Appli> appli(new Appli);
	TRACE("Ver3 reading...\n");
	{
		CFile input3 (path,CFile::modeRead);
		document = DOCUMENTP(reader3->Read(&input3));
		input3.Close();
		SDOMArchive archive;
		archive.SerializeLoad(document.get(),WSDOMN_SPICYDOG,appli.get());
	}
	
	TRACE("Ver3J writing...\n");
	{
		SDOMArchive archive;
		document = DOCUMENTP(archive.SerializeStore(WSDOMN_SPICYDOG,appli.get()));
		CFile outputJ(temp,CFile::modeWrite | CFile::modeCreate);
		writerJ->Write(&outputJ,document.get());
		outputJ.Close();
		document = DOCUMENTP(NULL);
	}
	
	TRACE("deleting...\n");
	appli = std::auto_ptr<Appli>(new Appli);

	TRACE("Ver3J reading...\n");
	{
		CFile inputJ (temp,CFile::modeRead);
		DOMDocument* d = readerJ->Read(&inputJ);
		document = DOCUMENTP(d);
		inputJ.Close();
		SDOMArchive archive;
		archive.SerializeLoad(document.get(),WSDOMN_SPICYDOG,appli.get());
	}
	
	TRACE("done. \n");
}


#endif