#pragma once

#include "model/build/Buildable.h"
#include "sdom/olm.h"
#include "utility/Bezier.h"

class COleDataSource;
class SDOMArchive;
class AffineMatrix;
class Coloring;

/** プレビュー描画情報 */
class CharPreview {
public:
	CDC*     dc;
	CRect    rect;
	COLORREF bkgndColor;
};

/** キャラクタ型情報 */
class CharType {
public:
	CharType(
		CRuntimeClass* guiClass,
		LPCTSTR charTypeName,
		UINT    displayName,
		LPCWSTR xmlNodeName,
		int  iconIndex,
		BOOL guiModal,
		BOOL canButtonChar,
		BOOL canSoundChar,
		BOOL canAttachLayer,
		BOOL canModifyColoring,
		BOOL canModifyPlacing
	);
	CRuntimeClass* guiClass;
	CString charTypeName;
	UINT    displayName;
	LPCWSTR xmlNodeName;
	int  iconIndex;
	BOOL guiModal;
	BOOL canButtonChar;
	BOOL canSoundChar;
	BOOL canAttachLayer;
	BOOL canModifyColoring;
	BOOL canModifyPlacing;
};

/** キャラクタ */
class Char 
:	public COLMObject,
	public Buildable {
private:
	CString m_name;			

protected:
	/** コンストラクタ */
	Char();
	/** 複製コンストラクタ */
	Char(const Char&);
public:
	virtual ~Char();
	/** キャラクタ名の取得 */
	CString GetName() const;
	/** キャラクタ名の設定 */
	void SetName(LPCTSTR ptsz);
	/** 複製する */
	virtual Char* Clone() const = 0;
	/** シリアライズ */
	virtual void Serialize(SDOMArchive*) = 0;
	/** ビルドする */
	virtual void Build(Builder*) const = 0;
	/** キャラクタ型情報 */
	virtual const CharType& GetCharType() const = 0;
	/** プレビュー描画 */
	virtual void DrawPreview(const CharPreview*) const = 0; 
	/** 描画 */
	virtual void Draw(CDC* pDC,const AffineMatrix*,const Coloring*) const = 0;
	/** キャラクタ型名からキャラクタインスタンスを生成する */
	static SDOMSerializable* factory(LPCWSTR);
	/** キャラクタインスタンスからキャラクタ型名を取得 */
	static LPCWSTR namer(SDOMSerializable*);
	/** 外接矩形を取得する - 必ずオーバーライドすること */
	virtual void GetRect(CRect&) const = 0;
	/**	キャラクタに外接するベジェ曲線を取得
		オーバーライドしない場合は自動的にGetRectから計算される */
	virtual void GetBezier(CBEZIER&) const;
	virtual CString GetInfo() const;
	virtual CString GetLongInfo() const;
};





