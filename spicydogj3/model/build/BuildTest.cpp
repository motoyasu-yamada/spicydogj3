#include "stdafx.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

#include "model/Appli.h"
#include "model/build/Builder.h"
#include "model/build/BuildOutput.h"
#include "sdom/Ver3SDOMReader.h"
#include "sdom/SDOMException.h"
#include "utility/FileUtility.h"
#include "utility/CmdExec.h"
#include <xercesc/dom/DOM.hpp>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class BuildTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(BuildTest);
  CPPUNIT_TEST(testCastDoubleToFI25);
  CPPUNIT_TEST(testSetWritingPos);
  CPPUNIT_TEST(testBuildFiles);
  CPPUNIT_TEST(testSubBits);
  CPPUNIT_TEST_SUITE_END();
private:

public:
	BuildTest() {}
	virtual ~BuildTest() {}
	virtual void setUp() {}

	virtual void tearDown() {}
	
	void testCastDoubleToFI25();
	void testSetWritingPos();
	void testBuildFiles();
	void testSubBits();
};


void BuildTest::testSubBits() {
	CPPUNIT_ASSERT(Builder::SubBits(0x00000000,0,32) == 0);
	CPPUNIT_ASSERT(Builder::SubBits(0xFFFFFFFF,0,32) == 0xFFFFFFFF);
	CPPUNIT_ASSERT(Builder::SubBits(0x80000000,31,1) == 1);
	CPPUNIT_ASSERT(Builder::SubBits(0x80000000,0,31) == 0);
	CPPUNIT_ASSERT(Builder::SubBits(0x00000001,0,1 ) == 1);
	CPPUNIT_ASSERT(Builder::SubBits(0x00000001,1,31) == 0);
}

void BuildTest::testCastDoubleToFI25() {
	int integer,decimal;

	Builder::CastDoubleToFI25(0,integer,decimal);
	CPPUNIT_ASSERT(integer == 0 && decimal == 0);

	Builder::CastDoubleToFI25(-1,integer,decimal);
	CPPUNIT_ASSERT(integer == -1 && decimal == 0);

	Builder::CastDoubleToFI25(1,integer,decimal);
	CPPUNIT_ASSERT(integer == 1 && decimal == 0);

	Builder::CastDoubleToFI25(255,integer,decimal);
	CPPUNIT_ASSERT(integer == 255 && decimal == 0);

	Builder::CastDoubleToFI25(256,integer,decimal);
	CPPUNIT_ASSERT(integer == 256 && decimal == 0);

	Builder::CastDoubleToFI25(-256,integer,decimal);
	CPPUNIT_ASSERT(integer == -256 && decimal == 0);

	Builder::CastDoubleToFI25(-257,integer,decimal);
	CPPUNIT_ASSERT(integer == -257 && decimal == 0);

	Builder::CastDoubleToFI25(32767,integer,decimal);
	CPPUNIT_ASSERT(integer == 32767 && decimal == 0);

	Builder::CastDoubleToFI25(-32768,integer,decimal);
	CPPUNIT_ASSERT(integer == -32768 && decimal == 0);

	Builder::CastDoubleToFI25(1.0625,integer,decimal);
	CPPUNIT_ASSERT(integer == 1 && decimal == 16);

	Builder::CastDoubleToFI25(-1.0625,integer,decimal);
	CPPUNIT_ASSERT(integer == -2 && decimal == 240);

	Builder::CastDoubleToFI25(0.9999,integer,decimal);
	CPPUNIT_ASSERT(integer == 0 && decimal == 255);

	Builder::CastDoubleToFI25(-0.9999,integer,decimal);
	CPPUNIT_ASSERT(integer == -1 && decimal == 1);
}

class BuildTestExecListener : public CCmdExecListener {
	virtual void OnOutput(LPCTSTR ptszMsg) {
		//TRACE(ptszMsg);
	}

	virtual void OnTerm(BOOL b) {
		TRACE("*** DONE ***\n");
	}
};

BuildTestExecListener listener;
CCmdExec exec;

void BuildTest::testBuildFiles() {
	typedef std::auto_ptr<DOMDocument> DOCUMENTP;
	typedef std::auto_ptr<SDOMReader> READERP;
	DOCUMENTP document;
	READERP reader3;
	reader3 = READERP(new Ver3SDOMReader);

	CFileFind find;
	BOOL b = find.FindFile("tests\\sdb\\*.sdb");
	while(b) {
		try {
			b = find.FindNextFile();
			CString path = find.GetFilePath();
			CFile input3 (path,CFile::modeRead);
			document = DOCUMENTP(reader3->Read(&input3));
			input3.Close();

			SDOMArchive archive;
			std::auto_ptr<Appli> appli(new Appli);
			archive.SerializeLoad(document.get(),WSDOMN_SPICYDOG,appli.get());

			CString dir = CreateTempDirectory("SBT");

			BuildOutput output;
			Builder builder(appli.get(),&output);
			builder.Build();
			builder.GenerateJARFile(dir + "\\test.jar");	
			builder.GenerateADFFile(dir + "\\test.jad","test.jar");
			builder.GenerateHTMFile(dir + "\\test.htm","test.jad");
		} catch(CException* e) {
			e->ReportError();
			ASSERT(FALSE);
			throw;
		} catch(...) {
			TRACE(find.GetFilePath());
			ASSERT(FALSE);
			throw;
		}
	};
}
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\jbmidp.jar

void BuildTest::testSetWritingPos() {
	BuildOutput output;
	Appli appli;
	Builder builder(&appli,&output);
	builder.m_step = Builder::BUILDSTEP_LAST;

	builder.SetWritingPos(0);
	builder.WriteUI8(0);
	CPPUNIT_ASSERT(builder.GetWritingPos() == 1);

	builder.SetWritingPos(0);
	builder.WriteUI16(0);
	CPPUNIT_ASSERT(builder.GetWritingPos() == 2);

	builder.SetWritingPos(0);
	builder.WriteUI32(0);
	CPPUNIT_ASSERT(builder.GetWritingPos() == 4);

	builder.SetWritingPos(0);
	builder.WriteXY(0,0);
	CPPUNIT_ASSERT(builder.GetWritingPos() == 3);

	builder.SetWritingPos(0);
	builder.WriteFI24(0);
	CPPUNIT_ASSERT(builder.GetWritingPos() == 3);

	builder.SetWritingPos(0);
	builder.WriteColor(RGB(0x00,0x00,0x00));
	CPPUNIT_ASSERT(builder.GetWritingPos() == 3);

	builder.SetWritingPos(0);
	builder.WriteBuffer("ABC",3);
	CPPUNIT_ASSERT(builder.GetWritingPos() == 5);

	builder.SetWritingPos(0);
	builder.WriteSJIS("ABC");
	CPPUNIT_ASSERT(builder.GetWritingPos() == 5);	
}

CPPUNIT_TEST_SUITE_REGISTRATION(BuildTest);
