#pragma once

#include "SDOMReader.h"


namespace xercesc_2_2 {
class DOMDocument;
};
class CFile;
using namespace xercesc_2_2;

class VerJ3SDOMReader : public SDOMReader {
public:
	virtual BOOL CanRead(CFile* p);
	virtual DOMDocument* Read(CFile* p);
};
