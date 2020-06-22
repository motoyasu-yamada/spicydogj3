#pragma once

#include "Char.h"
#include "sdom/SDOMBuffer.h"
#include <memory>

class SndChar : public Char {
private:
	static CharType SND_CHAR_TYPE;
	SDOMBuffer m_data;

public:
	SndChar();
	SndChar(const SndChar&);
	SndChar(HANDLE h);
	virtual ~SndChar();
	HANDLE ReplaceHandle(HANDLE);
	HANDLE GetHandle() const;

private:
	virtual Char* Clone() const;	
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual const CharType& GetCharType() const;
	virtual void DrawPreview(const CharPreview*) const;
	virtual void Draw(CDC*,const AffineMatrix*,const Coloring*) const;
	virtual void GetRect(class CRect&) const;
};
