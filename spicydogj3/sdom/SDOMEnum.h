#pragma once

#include <map>

/** 列挙値と名をマッピングするクラス
	(利用方法)
	enum {
		PICT_TYPE_JPG,
		PICT_TYPE_PNG,
		PICT_TYPE_GIF
	};
	ENUM_TYPE(PictType);
	ENUM_TYPE_ITEM(PICT_TYPE_JPG,"jpg")
	ENUM_TYPE_ITEM(PICT_TYPE_PNG,"png")
	ENUM_TYPE_ITEM(PICT_TYPE_PNG,"gif") */
class SDOMEnumType {
private:
	std::map<int,CString> m_i2s;
	std::map<CString,int> m_s2i;
public:
	/** 指定した列挙子値を持つ列挙子があるかどうか */
	BOOL Exists(int value) const;
	/** 指定した列挙子名を持つ列挙子があるかどうか */
	BOOL Exists(LPCTSTR name) const;
	/** 列挙子名を列挙子値に変更 */
	int Name2Value(LPCTSTR name) const;
	/** 列挙子値を列挙子名に変更 */
	LPCTSTR Value2Name(int value) const;
private:
	friend class SDOMEnumTypeItem;
	/** 列挙子を定義する */
	void Define(int value,LPCTSTR name);
};

/** SDOMEnumTypeの各項目を定義する為のヘルパークラス */
class SDOMEnumTypeItem {
public:
	/** @param type  列挙型名(C++の型・変数名命名規則にしたがっていること)
	    @param value 列挙子値
		@param name  列挙子名 */
	SDOMEnumTypeItem(const SDOMEnumType& type,int value,LPCTSTR name);
};

#define __ENUM_TYPE_CONCATENATE_DIRECT( s1, s2 ) s1##s2
#define __ENUM_TYPE_CONCATENATE( s1, s2 ) __ENUM_TYPE_CONCATENATE_DIRECT( s1, s2 )

#define ENUM_TYPE(type) static const SDOMEnumType type
#define ENUM_TYPE_ITEM(type,value,name)  static const SDOMEnumTypeItem __ENUM_TYPE_CONCATENATE(type,__LINE__)(type,value,name)
