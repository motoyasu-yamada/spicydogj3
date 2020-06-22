#pragma once

#include "utility/MemoryUtility.h"

class SDOMBuffer : public GlobalAllocator {
private:
	HGLOBAL m_hg;

public:
	SDOMBuffer();
	SDOMBuffer(DWORD dwSize);
	SDOMBuffer(const SDOMBuffer&);
	SDOMBuffer& operator = (const SDOMBuffer&);
	virtual ~SDOMBuffer();

	CString bin2hex() const;
	void hex2bin(LPCTSTR hex);

private:
	void CopyFrom(const SDOMBuffer&);
};
