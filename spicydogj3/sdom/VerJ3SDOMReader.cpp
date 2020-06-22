/** $Id: VerJ3SDOMReader.cpp,v 1.5 2003/05/26 09:12:02 m_yamada Exp $ */

#include "stdafx.h"
#include "VerJ3SDOMReader.h"
#include <memory>
#include <AFXTEMPL.H>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include "SDOMException.h"
#include "sdomn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* XSD_PATH = "xsd\\spicydogj3.xsd";

MemBufInputSource* newMemBufInputSource(CFile* file);

class ErrorReporter : public ErrorHandler {
public:
    ErrorReporter();
    ~ErrorReporter();

    virtual void warning(const SAXParseException& toCatch);
    virtual void error(const SAXParseException& toCatch);
    virtual void fatalError(const SAXParseException& toCatch);
    virtual void resetErrors();

private:
    BOOL getSawErrors() const;
    BOOL    fSawErrors;
};

BOOL VerJ3SDOMReader::CanRead(CFile* file) {
	ASSERT(AfxIsValidAddress(file,sizeof CFile));
	return true;
}

DOMDocument* VerJ3SDOMReader::Read(CFile* file) {
	ASSERT(AfxIsValidAddress(file,sizeof CFile));
    BOOL errorsOccured = false;
    try {
		std::auto_ptr<MemBufInputSource> input;
		std::auto_ptr<XercesDOMParser> parser;
		std::auto_ptr<ErrorReporter> reporter;

		input    = std::auto_ptr<MemBufInputSource>(newMemBufInputSource(file));
		parser   = std::auto_ptr<XercesDOMParser>(new XercesDOMParser);
		reporter = std::auto_ptr<ErrorReporter>(new ErrorReporter);

		parser->setDoNamespaces(true);
		parser->setDoSchema(true);
		parser->setErrorHandler(reporter.get());
		/*
		parser->setValidationScheme(XercesDOMParser::Val_Always);
		parser->setValidationSchemaFullChecking(true);
		parser->setDoSchema(true);
		parser->setExternalSchemaLocation(XSD_PATH);
		parser->setExternalNoNamespaceSchemaLocation(XSD_PATH);
		parser->setDoNamespaces(true);
		;*/

        parser->parse(*input);
		const DOMDocument* document = parser->getDocument();
		ASSERT(document != NULL);
		const DOMElement*  element  = document != NULL ? document->getDocumentElement() : NULL;
		ASSERT(element != NULL);
		const LPCWSTR      version  = element  != NULL ? element->getAttribute(SDBV3JKEY_VERSION) : NULL;
		TRACE("Spicydog XML Document Version = %S\n",version);
		if (wcscmp(version,SDBV3J_VERSION) != 0) {
			SDOMException::throwException(SDOMException::CAUSE_UNKNOWN_VERSION);
		}
		return parser->adoptDocument();

    } catch (const XMLException& e) {
		TRACE("An error occurred during parsing\nMessage=%s\n",e.getMessage());
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);

	} catch (const DOMException& e) {
		TRACE("A DOM error occurred during parsing\nCode=%d\n",e.code);
        SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);

	} catch (CFileException* p) {
		throw p;

    } catch (...) {
        TRACE("An unknown error occurred during parsing\n");
        SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);

    }
	ASSERT(FALSE);
	return NULL;
}

	MemBufInputSource* newMemBufInputSource(CFile* file) {

		DWORD len = file->GetLength();
		XMLByte* buffer = new XMLByte[len];
		if (len != file->Read(buffer,len)) {
			AfxThrowFileException(CFileException::endOfFile);
		}
		return new MemBufInputSource(buffer,len,file->GetFilePath(),true);
	}


	ErrorReporter::ErrorReporter() : fSawErrors(false) {
	}

	ErrorReporter::~ErrorReporter() {
	}

	inline BOOL ErrorReporter::getSawErrors() const {
		return fSawErrors;
	}

	void ErrorReporter::warning(const SAXParseException&) {
	}

	void ErrorReporter::error(const SAXParseException& toCatch) {
		fSawErrors = true;
		TRACE("Error at file %S,%d,%d,%S\n",
			  toCatch.getSystemId(),
			  toCatch.getLineNumber(),
			  toCatch.getColumnNumber(),
			  toCatch.getMessage()
			);
	}

	void ErrorReporter::fatalError(const SAXParseException& toCatch) {
		fSawErrors = true;

		TRACE("Fatal Error at file %S,%d,%d,%S\n"
			 ,toCatch.getSystemId()
			 ,toCatch.getLineNumber()
			 ,toCatch.getColumnNumber()
			 ,toCatch.getMessage());
	}

	void ErrorReporter::resetErrors() {
		// No-op in this case
	}
