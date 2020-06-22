#include "stdafx.h"
#include "SMTP.h"
#include "MailMessage.h"
#include "utility/StringUtility.h"

#if defined(_DEBUG)

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

class smtpTest : public CppUnit::TestFixture {
private:
CPPUNIT_TEST_SUITE(smtpTest);
	CPPUNIT_TEST(testSend);
CPPUNIT_TEST_SUITE_END();
public:
	void testSend();
private:
	void send(LPCTSTR server,LPCTSTR tos,LPCTSTR from,LPCTSTR subject,LPCTSTR body);
};

CPPUNIT_TEST_SUITE_REGISTRATION(smtpTest);

void smtpTest::testSend() {
	send("mx5.spicysoft.com","t_hirano@spicysoft.com,m_yamada@spicysoft.com","princo@princo.org","test","body");
	send("mx5.spicysoft.com","m_yamada@spicysoft.com","princo@princo.org","“ú–{Œê",".\r\n");
}

void smtpTest::send(LPCTSTR server,LPCTSTR tos,LPCTSTR from,LPCTSTR subject,LPCTSTR body) {

	CMailMessage msg;
	CSMTP smtp(server);

	msg.m_sFrom    = from;
	msg.m_sSubject = subject;
	msg.m_sBody    = body;
	CStringArray tolist;
	Split(tos,',',tolist);
	for (int n = 0;n < tolist.GetSize();n++) {
		msg.AddMultipleRecipients(tolist.ElementAt(n),CMailMessage::BCC );
	}

	CPPUNIT_ASSERT(smtp.Connect());
	CPPUNIT_ASSERT(smtp.SendMessage(&msg));
	CPPUNIT_ASSERT(smtp.Disconnect());
}

#endif