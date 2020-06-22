#pragma once

#include "Char.H"
#include "model/type/Act.h"
#include "sdom/SDOMEnum.h"
#include <ltwrappr.h>

typedef BOOL (CALLBACK *PICT_EFFECT_PROC)(BITMAPHANDLE*,int);

/** 画像のサイズとして許される縦・横サイズ */
const int PICT_MIN_WIDTH  = 1;
const int PICT_MAX_WIDTH  = 1024;
const int PICT_MIN_HEIGHT = 1;
const int PICT_MAX_HEIGHT = 1024;
/** 画像のデータサイズとして許される容量 */
const int PICT_DATA_MIN_LENGTH = 1;
const int PICT_DATA_MAX_LENGTH = 0xFFFF;

/** 画像データ */
class PictData {
	friend class PictChar;
public:
	/** DIBビットマップ画像 */
	LBitmapBase m_bmp;
	/** パススルーデータ画像 */
	LBuffer     m_data;
	/** デフォルト状態 */
	PictData();
	/** 複製 */
	PictData(const PictData&);
	PictData& operator = (const PictData&);

private:
	/** 正規性チェック */
	void AssertValid() const;
	/** 複製ヘルパー */
	void CopyFrom(const PictData&);
};


/** 画像最適化方式 */
enum PICT_QUANTIZE {
	PICT_QUANTIZE_PATHTHROUGH = 0,
	PICT_QUANTIZE_PNG         = 1,
	PICT_QUANTIZE_JPG         = 2
};
ENUM_TYPE(PictQuantizeType);
ENUM_TYPE_ITEM(PictQuantizeType,PICT_QUANTIZE_PATHTHROUGH,ASDOMN_PATH_THROUGH);
ENUM_TYPE_ITEM(PictQuantizeType,PICT_QUANTIZE_PNG,ASDOMN_QUANTIZE_PNG);
ENUM_TYPE_ITEM(PictQuantizeType,PICT_QUANTIZE_JPG,ASDOMN_QUANTIZE_JPG);


ENUM_TYPE(PaletteType);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_IMODE, ASDOMV_IMODE_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_WEB,   ASDOMV_WEB_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_WIN,   ASDOMV_WIN_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_MAC,   ASDOMV_MAC_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_GRAY,  ASDOMV_GRAY_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_MONO,  ASDOMV_MONO_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_RGB,   ASDOMV_RGB_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_CUSTOM,ASDOMV_CUSTOM_PALETTE);

/** 画像最適化パラメータ */
class PictQuantizeParam {
public:
	int      m_quantize;
	/** JPEG圧縮率 0%-100% */
	int      m_jpgCompress;
	/** パレット形式 */
	int      m_pngPaletteType;
	/** パレット実データ */
	Act      m_pngPalette;
	/** カラー数 */
	int      m_pngColors;
	/** ディザ処理をするかどうか？ */
	BOOL     m_pngUseDither;
	/** 透明色を利用するかどうか？ */
	BOOL     m_pngUseTransparency;
	/** 透明色 */
	COLORREF m_pngTransparentColor;

	PictQuantizeParam();
	PictQuantizeParam(const PictQuantizeParam&);
	PictQuantizeParam& operator = (const PictQuantizeParam&);
private:
	friend class PictChar;
	void Init();
	void AssertValid() const;
	void CopyFrom(const PictQuantizeParam&);
};


/** 画像最適化キャッシュ */
enum PICTOPTOUTFORMAT {
	PICTOPTOUTFORMAT_PNG = 0,
	PICTOPTOUTFORMAT_JPG = 1,
};
class PictOptCache {
public:
	BOOL             m_bValid;
	LBitmapBase      m_bmp;
	LBuffer          m_data;
	PICTOPTOUTFORMAT m_nFormat;
	int				 m_nDataLen;
	BOOL             m_bTrans;
	COLORREF         m_crTrans;
	CSize			 m_sz;
public:
	PictOptCache();
	/** ピクチャの最適化 */
	BOOL Optimize(const PictData&,const PictQuantizeParam&);
private:
	friend class PictChar;
	void Init();
	void AssertValid() const;
}; // struct PictOptCache


/**	ピクチャキャラクタ */
class PictChar : public Char {
	friend class PictModDataCmd;
	friend class PictSetOptParamCmd;
	friend class PictSetCenterCmd;
	friend class PictCharView;
private:
	/** ピクチャキャラクタ型情報*/
	static CharType PICT_CHAR_TYPE;
	/** 画像のアニメーション中心点 */
	CPoint               m_centerPoint;
	/** 画像のデータ     */
	PictData             m_data;
	/** 画像の最適化設定 */
	PictQuantizeParam    m_quantize;
	/** 画像の表示キャッシュ */
	mutable PictOptCache m_cache;
public:
	PictChar();
	/**	(カスタムコンストラクタ)
		空のピクチャキャラクタを生成する
		
		@param		sz 新規キャラクタのサイズ
					cx/cy 1 pixel ～ 65535 pixel 以内
		@param		cr 塗りつぶし色
		@return		NULL - 引数が不正な場合
		@exception	CMemoryException メモリをアロケートできなかった場合 */
	static PictChar* newEmpty(CSize sz,COLORREF cr);
	/**	(カスタムコンストラクタ)
		パススルー書出設定型のピクチャをインポートする
		
		@param		パススルーピクチャが存在するパス
		@return		NULL - 	ファイルが存在しない場合
							未対応のファイルの場合
		@exception	CMemoryException メモリをアロケートできなかった場合 */
	static PictChar* newImportPathThrough(LPCTSTR ptszPath);
	/**	(カスタムコンストラクタ)
		パススルー書出設定型のピクチャをインポートする
		
		@param		パススルーピクチャが存在するパス
		@return		NULL - 	ファイルが存在しない場合
							未対応のファイルの場合
							ファイルサイズが規定より大きかった場合
		@exception	CMemoryException メモリをアロケートできなかった場合 */
	static PictChar* newImport(LPCTSTR ptszPath);
	/**	(カスタムコンストラクタ)
		クリップボードの画像情報を元にピクチャキャラクタを生成する
		
		@param hbmp	Win ビットマップハンドル
		@param hpal	Win パレットハンドル
		@return		NULL - 	未対応のファイルの場合
							ファイルサイズが規定より大きかった場合
		@exception	CMemoryException メモリをアロケートできなかった場合 */
	static PictChar* newPaste(HBITMAP,HPALETTE);

	virtual Char* Clone() const;

	virtual void Serialize(SDOMArchive*);
	virtual void Build(Builder*) const;
	virtual void AssertValid() const;


	/// 最適化設定およびビットマップが編集可能かどうか
	BOOL IsEditable() const;
	///  編集用の未最適化描画
	void DrawEditview(CDC* pDC) const;
	/// 画像のセンタリング設定を取得
	CPoint GetCenter() const;
	/// 画像の書出化設定を取得
	const PictQuantizeParam& GetPictOptParam() const;
	/// 画像のビットマップデータの取得
	const PictData& GetPictData() const;
	/// 最適化済画像のエクスポート
	BOOL ExportCache(LPCTSTR) const;

	static void CreateAct(Act& act,const LBitmapBase& bmp,const PictQuantizeParam& param);
	static BOOL LoadBmp  (LPCTSTR,PictData&);
	static BOOL Optimize (PictOptCache&,const PictData&,const PictQuantizeParam&);

private:
	static PictChar* newPictChar(const PictData&,const PictQuantizeParam&);
	PictChar(const PictChar& rhs);
	virtual const CharType& GetCharType() const;
	virtual void DrawPreview(const CharPreview*) const;
	virtual void Draw(CDC* pDC,const AffineMatrix*,const Coloring*) const;
	virtual void GetRect(CRect& rc) const;
	void SetCenter(CPoint);
	void SetPictData(const PictData&);
	void SetPictOptParam(const PictQuantizeParam&);
	CSize  GetBitmapSize() const;
	BOOL Optimize() const;
	int GetColorBits() const;
	static BOOL Data2Bmp(const LBuffer&,LBitmapBase&);
	static BOOL EditByExe(PictData&);
	virtual CString GetInfo() const;
};





