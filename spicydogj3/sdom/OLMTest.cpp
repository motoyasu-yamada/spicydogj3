//	$Id: OLMTest.cpp,v 1.4 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"

#if defined(_DEBUG)

#include "sdom/olm.h"
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


class OLMTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(OLMTest);
		CPPUNIT_TEST(testOLMOwner);
		CPPUNIT_TEST(testOLMOwners);
	CPPUNIT_TEST_SUITE_END();
public:
    OLMTest() {}
	void setUp() {}

	void testOLMOwner();
    void testOLMOwners();
};
CPPUNIT_TEST_SUITE_REGISTRATION(OLMTest);


class OLMTestObject : public COLMObject {
public:
	static BOOL deleted;
	virtual void Serialize(SDOMArchive*) {}
	virtual ~OLMTestObject() {
		CPPUNIT_ASSERT(!deleted);
		deleted = true;
	}
	static SDOMSerializable* factory(LPCWSTR name) { return NULL; }
	static LPCWSTR namer(SDOMSerializable*) { return NULL; }
};
BOOL OLMTestObject::deleted = false;

/** OLMOwner �̃��C�t�T�C�N���̓���e�X�g */
void OLMTest::testOLMOwner() {
	{
		//  �󐶐�
		OLMTestObject::deleted = false;
		TOLMOwner<OLMTestObject> owner;
		CPPUNIT_ASSERT(owner.GetObject() == NULL);
		CPPUNIT_ASSERT(!OLMTestObject::deleted);

		// ������
		owner.OwnNew(new OLMTestObject);
		CPPUNIT_ASSERT(!OLMTestObject::deleted);
		CPPUNIT_ASSERT(AfxIsValidAddress(owner.GetObject(),sizeof OLMTestObject));
	
		// �ړ�
		TOLMOwner<OLMTestObject> owner2;
		owner2.InheritedFrom(&owner);
		CPPUNIT_ASSERT(!OLMTestObject::deleted);

		//	�폜
		CPPUNIT_ASSERT(!OLMTestObject::deleted);
	}
	CPPUNIT_ASSERT(OLMTestObject::deleted);
}

/** OLMOwners�̃��C�t�T�C�N���̃e�X�g */
void OLMTest::testOLMOwners() {
	OLMTestObject::deleted = false;
	{
		TOLMOwners<OLMTestObject,true> owners;
		COLMOwner& owner = *owners.m_owners.insert(owners.m_owners.end());
		owner.OwnNew(new OLMTestObject);
		CPPUNIT_ASSERT(!OLMTestObject::deleted);
	}
	CPPUNIT_ASSERT(OLMTestObject::deleted);

}

#endif
