#pragma once

class SDOMArchive;

/** ACT_NOUSE_LEADTOOL	- 定義されているとLeadTool関連のコードを削除する */
#if !defined(ACT_NOUSE_LEADTOOL)
#include <ltwrappr.h>
#endif
#include "sdom/SDOMSerializable.h"
#include <memory>
#include <map>

/**	カラーテーブル */
class Act
:	public CObject,
	public SDOMSerializable {
private:
	int m_nCount;
	PALETTEENTRY m_Entries[256];

public:
	Act();
	Act(const Act&);
	Act& operator = (const Act&);
	virtual void AssertValid();
	int FindIndex(COLORREF) const;

	BOOL LoadFile(LPCTSTR);
	BOOL LoadResource(HMODULE,UINT);
	void CreateFromPalette(HPALETTE,int);
#if !defined(ACT_NOUSE_LEADTOOL)
	void CreateFromBitmap(const LBitmapBase&,BOOL,COLORREF);
#endif

	void Empty();
	int GetCount() const;
	PALETTEENTRY& operator [] (int);
	const PALETTEENTRY& operator[] (int) const;
	COLORREF GetColor(int n) const; 
	HPALETTE CreatePalette() const;

#if !defined(ACT_NOUSE_LEADTOOL)
	void SorByBitmap(const LBitmapBase& _bmp,BOOL bTrans = FALSE,COLORREF crTrans = 0xFFFFFFFF);
#endif
	static void SetPaletteEntry(PALETTEENTRY&,COLORREF);

private:
	virtual void Serialize(SDOMArchive*);
	void CopyFrom(const Act&);
	void Init();
	BOOL Load(CFile&);
	BOOL Find(COLORREF) const;
};


/** 既定カラーテーブル種別 */
enum ACTTYPE {
	ACTTYPE_IMODE = 0,
	ACTTYPE_WEB = 1,
	ACTTYPE_WIN = 2,
	ACTTYPE_MAC = 3,
	ACTTYPE_GRAY = 4,
	ACTTYPE_MONO = 5,
	ACTTYPE_RGB = 6,
	ACTTYPE_CUSTOM = 7
}; // enum ACTTYPE
ACTTYPE CAST_ACTTYPE(int n);

/** 既定カラーテーブル一覧 */
class CDefaultActs {
private:
	typedef std::auto_ptr<Act> ACTPTR;
	typedef std::map<ACTTYPE,ACTPTR> ACTS;
	typedef std::map<ACTTYPE,CString> ACTNAMES;
private:
	static std::auto_ptr<CDefaultActs> s_pacts;
	ACTS m_acts;
	ACTNAMES m_actnames;

public:
	static const CDefaultActs* GetDefaultActs();
	const Act* GetAct(ACTTYPE) const;
	CString GetActName(ACTTYPE) const;

private:
	CDefaultActs();
	void Load(ACTTYPE,UINT);
}; // CDefaultActs

/**
	画像情報
*/
struct PICTINFO {
	int      bitsPerPixel;
	BOOL     transparency;
	COLORREF transparentColor;
	ACTTYPE  actType;
	Act     act;
};

#if !defined(ACT_NOUSE_LEADTOOL)
BOOL LoadBitmapPictInfo(LBitmap& bmp,PICTINFO& pi,const FILEINFO& fi);
BOOL LoadBitmapPictInfo(LBuffer& buffer,PICTINFO& pi);
#endif

BOOL LoadBitmapPictInfo(LPCTSTR ptszPath,PICTINFO& pi);
