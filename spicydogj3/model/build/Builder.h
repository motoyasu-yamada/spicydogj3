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

/** �r���h�R���e�L�X�g */
class BuildContext {
private:
	Builder* m_builder;
public:
	BuildContext(Builder*,UINT,...);
	BuildContext(Builder*,LPCTSTR);
	~BuildContext();
};


/** ���s */
class Builder {
	friend class Buildable;
	friend class BuildContext;
	friend class BuildTest;
public:
	enum BUILDSTEP { BUILDSTEP_FIRST,BUILDSTEP_LAST,BUILDSTEP_ERR};

	Builder(Appli*,BuildOutput* output);
	/** �Đ��t�@�C���𐶐����� */
	void Build();
	/** ��������ADF�𐶐����� */
	CString GenerateADF(LPCTSTR jarName,UINT length) const;
	/** �w�肵���p�X��JAR�𐶐����� */
	void GenerateJARFile(LPCTSTR path) const;
	/** �w�肵���p�X��ADF�𐶐����� */
	void GenerateADFFile(LPCTSTR path,LPCTSTR jarName) const;
	/** �w�肵���p�X��HTM�𐶐����� */
	void GenerateHTMFile(LPCTSTR path,LPCTSTR jamName) const;
	/** SDC�t�@�C����ۑ����� */
	void GenerateSDCFile(LPCTSTR path) const;
	/** �q�I�u�W�F�N�g���r���h���� */
	void Build(const Buildable*);

	/** ������ 8�r�b�g�����̏o�� */
	void WriteUI8(int);
	/** ������16�r�b�g�����̏o�� */
	void WriteUI16(int);
	/** ������32�r�b�g�����l���o�� */
	void WriteUI32(int);
	/** �����t16�r�b�g�����̏o�� */
	void WriteSI16(int);
	/** �Œ菬���_�l�̏����o�� */
	void WriteFI24(double);
	/** �����t12�r�b�g�����̏o�� */
	void WriteXY(int x,int y);
	/** �F���̏o�� */
	void WriteColor(COLORREF);
	/** �o�C�i���f�[�^�̏o��*/
	void WriteBuffer(LPCVOID,int);
	/** ������f�[�^�̏o��*/
	void WriteSJIS(LPCTSTR);

	/** �L�����N�^�𐶐����� */
	void WriteChar(const Char*,int type);
	/** �L�����N�^�Q�Ƃ𐶐����� */
	void WriteCharRef(const Char*);
	/** ���C���[�ݒ�*/
	void WriteDepth(int depth);
	/** ���ӂ���s��������o��*/
	void WriteMatrix(const AffineMatrix*);
	/** �F���������o�� */
	void WriteColoring(const Coloring*);
	/** ���x���o�� */
	void WriteLabelRef(const Label*);
	/** ���x�������������ǂ��� */
	BOOL IsValidLabel(const Label*) const;

	/** �C���X�g���N�V�������o�� */
	void WriteInstruction(int instructionID);
	/** �A�N�V�������o�� */
	void WriteAction(int actionID);

	/** ���݂̐�ΎQ�ƈʒu���擾 */
	int GetAbsoluteFramePos(int relativePos) const;
	/** �T�u�X�e�[�W��` */
	void EnterStage(int stageFrames);

	/** �����s�\��Q��� */
	void NotifyErr (UINT,...);
	/** �������̌x����� */
	void NotifyWarn(UINT,...);

	/** ���݂̃r���h�X�e�b�v */
	BUILDSTEP GetBuildStep() const;
	/** ���݂̏o�͈ʒu */
	int GetWritingPos() const;
	/** ���݂̏����ʒu��ύX���� */
	int SetWritingPos(int);
	/** �N���X�̎Q�Ƃ��L�^ */
	void UseChar(const Buildable*);
	/** �N���XID���擾 */
	int GetCharID(const Buildable*) const;
	/** �N���X�̎��g�p�����擾 */
	int GetCharUseCount(const Buildable*) const;
	/** �C���X�g���N�V�����u���b�N���o�J�n */
	void BeginInstruction();
	/** �C���X�g���N�V�����u���b�N���o�I�� */
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
	/** ���x���u���b�N���o - �_�~�[�̃��x����`�u���b�N���o��*/
	void BuildLabelsPre();
	/** ���x���u���b�N���o - �_�~�[�ŏ����o�������x����`�u���b�N�̓��e���C������ */
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


