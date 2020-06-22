#pragma once

#include "SDOMReader.h"

namespace xercesc_2_2 {
class DOMDocument;
class DOMNode;
class DOMElement;
};

class CFile;
class MFCUnserializer;
class SDOMBuffer;

using namespace xercesc_2_2;

class Ver3SDOMReader : public SDOMReader {
private:
	CFile*           m_file;
	MFCUnserializer* m_unserializer;
	DOMDocument*     m_doc;

public:
	virtual BOOL CanRead(CFile*);
	virtual DOMDocument* Read(CFile*);

private:
	void readHeader();
	void skipOrgFonts();
	DOMElement* readAppliConf();
	DOMElement* readCharList();
	DOMElement* readChar();
	DOMElement* readBtnChar(int object);
	DOMElement* readTextChar(int object);
	DOMElement* readSndChar(int object);
	DOMElement* readShapeChar(int object);
	DOMElement* readPictChar(int object);
	void readPictGif2Png(DOMElement*);
	void readPictJpeg(DOMElement*);
	void readPictPaththrough(DOMElement*);
	DOMElement* readMainStage(int);
	DOMElement* readSubStages();
	DOMElement* readSubStage(int);
	void readStage(DOMElement*,int);
	DOMElement* readLayer();
	DOMElement* readKeyFrame();
	DOMElement* readKeyFrameLabel(int);
	DOMElement* readPlacing();
	DOMElement* readColoring();
	DOMElement* readAnimating();
	DOMElement* readActions(BOOL);
	DOMElement* readLabel();
	
	DOMElement* readPalette();
	DOMElement* readBuffer(LPCWSTR,SDOMBuffer* buffer);
	void readTypeSafeObject(LPCTSTR);

	void appendString  (DOMNode*,LPCWSTR,LPCWSTR);
	void appendString  (DOMNode*,LPCWSTR,LPCSTR);
	void appendColor   (DOMNode*,LPCWSTR,COLORREF);
	void appendInt     (DOMNode*,LPCWSTR,int);
	void appendBoolean (DOMNode*,LPCWSTR,BOOL);
	void appendNullData(DOMNode*,LPCWSTR);
	void appendPoint   (DOMNode*,LPCWSTR,POINT);
	void appendSize    (DOMNode*,LPCWSTR,SIZE);
	void appendDouble  (DOMNode*,LPCWSTR,double);
	void insertString  (DOMNode*,LPCWSTR,LPCSTR);
	void appendIDREF   (DOMNode*,LPCWSTR,DOMNode*);
	void setAttID      (DOMElement*,int);


	void skipBuffer();
	void skipRGB();
	void skipFPOINT();
};
