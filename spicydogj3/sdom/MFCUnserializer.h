#pragma once

#pragma warning(disable : 4786)
#include <map>

namespace xercesc_2_2 {
class DOMNode;
};

class MFCUnserializer {
private:
	CFile& m_file;

	int m_index;
	std::map<int,CString>  m_classes;
	std::map<int,BOOL>     m_objects;
	std::map<int,xercesc_2_2::DOMNode*> m_nodes;

public:
	MFCUnserializer(CFile&);

	void ReadValue(int&);
	void ReadValue(short&);
	void ReadValue(double&);
	void ReadValue(BYTE&);
	void ReadValue(WORD&);
	void ReadValue(DWORD&);
	void ReadValue(UINT&);
	void ReadValue(CString&);
	void ReadValue(SIZE&);
	void ReadValue(POINT&);
	void ReadValue(CStringList&);

	void Skip(int);
	void ReadBuffer(BYTE*,int len);

	int  ReadCount();
	BOOL ReadObject(CString& className,int& object);
	
	xercesc_2_2::DOMNode* GetObject(int) const;
	void RegistObject(int,xercesc_2_2::DOMNode*);

private:
	int  getReadStringLength();
	BOOL alreadyLoadObject(int);
	void loadClass(CString& className);
	void registObject(CString className,int& object);
	void getClass(int index,CString& className);
};