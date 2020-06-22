#pragma once

#include "Char.h"
#include "model/build/Buildable.h"
#include "sdom/SDOMSerializable.h"

class Char;
class Builder;
class SDOMArchive;
class SpicydogDoc;

class CharList
:	public CObject,
	public Buildable,
	public SDOMSerializable {
	friend class InsCharCmd;
	friend class DelCharCmd;
	friend class DupCharCmd;
private:
	typedef TOLMOwners<Char,true> CHAROWNERS;
	CHAROWNERS m_charList;

public:
	CharList();

	virtual void Serialize(SDOMArchive* ar);
	virtual void Build(Builder*) const;
	virtual void AssertValid() const;

	Char* GetChar(int n);
	const Char* GetChar(int n) const;
	int GetCharCount() const;

	/** キャラクタに対応するキャラクタIDを取得
		キャラクタIDは0からではなく、1から採番される。
		@param		bAllowNull	NULLキャラクタを受け入れるかどうか
								このフラグがFALSEの場合にNULLキャラクタを渡されると内部例外発生
		@return		0... NULLキャラクタ 1～有効なキャラクタID
		@exception	<Internal>	ディクショナリに含まれないキャラクタID */
	int GetCharID(const Char*,BOOL bAllowNull = FALSE) const;
	/** 存在するキャラクタかどうか */
	BOOL ExistChar(const Char*) const;

	BOOL OleIsSupported(COleDataObject*) const;
	HGLOBAL Data2Ole_CF_CHARS(SpicydogDoc* document,const std::list<Char*>&) const;
	BOOL AcceptOle(SpicydogDoc*,COleDataObject* pOdo,BOOL bDnD,DROPEFFECT de,CPoint pt);
	Char* Ole2Data_CF_TEXT(HGLOBAL,COleDataObject*) const;
	Char* Ole2Data_CF_DIB(HGLOBAL,COleDataObject*) const;
	Char* Ole2Data_CF_BITMAP(HGLOBAL,COleDataObject*) const;
	Char* Ole2Data_CF_HDROP(HGLOBAL,COleDataObject*) const;
}; 

#pragma warning(disable : 4200)
struct CFS_CHARS {
	DWORD        processID;
	SpicydogDoc* document;
	int          charCount;
	Char*        chars[];
};