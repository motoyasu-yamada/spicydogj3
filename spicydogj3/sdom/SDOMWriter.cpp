//	$Id: SDOMWriter.cpp,v 1.5 2003/06/01 04:37:04 m_yamada Exp $
#include "stdafx.h"
#include "SDOMWriter.h"

#include <memory>
#include "DOMImpl.h"
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include "sdomn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace xercesc_2_2;

void SDOMWriter::Write(CFile* file,DOMDocument* document) {
	ASSERT(AfxIsValidAddress(file,sizeof CFile));
	ASSERT(AfxIsValidAddress(document,sizeof DOMDocument));

	std::auto_ptr<DOMWriter> serializer;
	std::auto_ptr<MemBufFormatTarget> target;
	DOMImplementation *impl;

	impl = DOMImpl::getImpl();
	serializer = std::auto_ptr<DOMWriter>(impl->createDOMWriter());
	target = std::auto_ptr<MemBufFormatTarget>(new MemBufFormatTarget);
	
	document->getDocumentElement()->setAttribute(L"xmlns",    SDBV3J_XMLNS);
	document->getDocumentElement()->setAttribute(L"xmlns:xsi",SDBV3J_XMLNSXSI);
	document->getDocumentElement()->setAttribute(SDBV3JKEY_VERSION,SDBV3J_VERSION);
	//document->getDocumentElement()->setAttribute(L"xsi:schemaLocation",
	//											 L"spicydog C:\\doc\\cvswork\\spicydogj3\\spicydogj3\\xsd\\spicydogj3.xsd");

	serializer->setEncoding(SDBV3J_ENCODING);
	serializer->writeNode(target.get(),*document);

	file->Write(target->getRawBuffer(),target->getLen());
}

