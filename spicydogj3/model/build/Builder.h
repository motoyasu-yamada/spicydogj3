#pragma once

#include <memory>
#include <afxtempl.h>
#include <stack>
#include <map>
#include <vector>
#include <list>
#include "sdc.h"

class Appli;
class AppliConf;
class Buildable;
class Builder;
class BuildOutput;
class Label;
class Char;
class AffineMatrix;
class Coloring;

/** ビルドコンテキスト */
class BuildContext {
private:
	Builder* m_builder;
public:
	BuildContext(Builder*,UINT,...);
	BuildContext(Builder*,LPCTSTR);
	~BuildContext();
};


/** 発行 */
class Builder {
	friend class Buildable;
	friend class BuildContext;
	friend class BuildTest;
public:
	enum BUILDSTEP { BUILDSTEP_FIRST,BUILDSTEP_LAST,BUILDSTEP_ERR};

	Builder(Appli*,BuildOutput* output);
	/** 再生ファイルを生成する */
	void Build();
	/** メモリにADFを生成する */
	CString GenerateADF(LPCTSTR jarName,UINT length) const;
	/** 指定したパスにJARを生成する */
	void GenerateJARFile(LPCTSTR path) const;
	/** 指定したパスにADFを生成する */
	void GenerateADFFile(LPCTSTR path,LPCTSTR jarName) const;
	/** 指定したパスにHTMを生成する */
	void GenerateHTMFile(LPCTSTR path,LPCTSTR jamName) const;
	/** SDCファイルを保存する */
	void GenerateSDCFile(LPCTSTR path) const;
	/** 子オブジェクトをビルドする */
	void Build(const Buildable*);

	/** 無符号 8ビット整数の出力 */
	void WriteUI8(int);
	/** 無符号16ビット整数の出力 */
	void WriteUI16(int);
	/** 無符号32ビット整数値を出力 */
	void WriteUI32(int);
	/** 符号付16ビット整数の出力 */
	void WriteSI16(int);
	/** 固定小数点値の書き出し */
	void WriteFI24(double);
	/** 符号付12ビット整数の出力 */
	void WriteXY(int x,int y);
	/** 色情報の出力 */
	void WriteColor(COLORREF);
	/** バイナリデータの出力*/
	void WriteBuffer(LPCVOID,int);
	/** 文字列データの出力*/
	void WriteSJIS(LPCTSTR);

	/** キャラクタを生成する */
	void WriteChar(const Char*,int type);
	/** キャラクタ参照を生成する */
	void WriteCharRef(const Char*);
	/** レイヤー設定*/
	void WriteDepth(int depth);
	/** あふぃん行列を書き出す*/
	void WriteMatrix(const AffineMatrix*);
	/** 色情報を書き出す */
	void WriteColoring(const Coloring*);
	/** ラベル出力 */
	void WriteLabelRef(const Label*);
	/** ラベルが正しいかどうか */
	BOOL IsValidLabel(const Label*) const;

	/** インストラクションを出力 */
	void WriteInstruction(int instructionID);
	/** アクションを出力 */
	void WriteAction(int actionID);

	/** 現在の絶対参照位置を取得 */
	int GetAbsoluteFramePos(int relativePos) const;
	/** サブステージ定義 */
	void EnterStage(int stageFrames);

	/** 生成不能障害を報告 */
	void NotifyErr (UINT,...);
	/** 生成時の警告を報告 */
	void NotifyWarn(UINT,...);

	/** 現在のビルドステップ */
	BUILDSTEP GetBuildStep() const;
	/** 現在の出力位置 */
	int GetWritingPos() const;
	/** 現在の書込位置を変更する */
	int SetWritingPos(int);
	/** クラスの参照を記録 */
	void UseChar(const Buildable*);
	/** クラスIDを取得 */
	int GetCharID(const Buildable*) const;
	/** クラスの実使用数を取得 */
	int GetCharUseCount(const Buildable*) const;
	/** インストラクションブロック書出開始 */
	void BeginInstruction();
	/** インストラクションブロック書出終了 */
	void EndInstruction();

private:
	typedef std::stack<CString>            CONTEXTS;
	typedef std::map<const Buildable*,int> CHARUSES;
	typedef CArray<BYTE,BYTE&>      BUFFER;
	typedef std::list<const Label*> LABELS;
	typedef std::map<CString,CString> VALUES;
	typedef std::vector<int> FRAMES;

	const Appli*               m_appli;
	BuildOutput*               m_output;
	BUILDSTEP m_step;
	LABELS    m_labels;
	CHARUSES  m_charUses;
	BUFFER    m_buffer;
	CONTEXTS  m_contexts;
	FRAMES    m_frames;
	int m_frame;
	int m_writingPos;
	int m_stageOffset;
	int m_stageFrames;
	int m_labelsPos;
	
	static int SubBits(int,int,int);
	/** ラベルブロック書出 - ダミーのラベル定義ブロックを出力*/
	void BuildLabelsPre();
	/** ラベルブロック書出 - ダミーで書き出したラベル定義ブロックの内容を修正する */
	void BuildLabelsPost();
	void MarkFrame(int frame);
	void Prepare();
	void Post();
	void BuildLast();
	void BuildFirst();
	void EnterContext(LPCTSTR);
	void ExitContext();
	CString GetCurrentContext() const;
	void GetValues(VALUES&) const;
	CString GetSourcePath(LPCTSTR fname) const;
	CString ReplaceValues(LPCTSTR fname,const VALUES& values) const;
	static void CastDoubleToFI25(double,int&,int&);
	static void ThrowException();
};


