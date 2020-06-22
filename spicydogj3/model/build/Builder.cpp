#include "stdafx.h"
#include "Builder.h"
#include "resource.h"

#include "Buildable.h"
#include "BuildOutput.h"
#include "utility/zip/InfoZip.h"
#include "utility/utf8.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "model/AffineMatrix.h"
#include "model/stage/Coloring.h"
#include "model/char/Char.h"
#include "sdom/sdomn.h"
#include "utility/FileUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Builder::Builder(Appli* appli,BuildOutput* output) {
	ASSERT(AfxIsValidAddress(appli,sizeof Appli));
	ASSERT(AfxIsValidAddress(output,sizeof BuildOutput));
	m_appli  = appli;
	m_output = output;
}

void Builder::ThrowException() {
}

void Builder::Build(const Buildable* buildable) {
	ASSERT(buildable != NULL);
	buildable->PreBuild(this);
	buildable->Build(this);
	buildable->PostBuild(this);
}


void Builder::Build() {
	ASSERT(AfxIsValidAddress(m_appli,sizeof Appli));
	ASSERT(AfxIsValidAddress(m_output,sizeof BuildOutput));
	Prepare();
	BuildFirst();
	if (m_output->GetErrCount() == 0) {
		BuildLast();
	}
	Post();
}

void Builder::Prepare() {
	LABEL_NAME_LIST list;
	m_appli->GetLabelList(list);
	LABEL_NAME_LIST::const_iterator copy;
	copy = list.begin();
	while (copy != list.end()) {
		m_labels.push_back(copy->first);
		copy++;
	}
	m_stageOffset = 0;
	m_output->BeginBuildOutput();
}

void Builder::Post() {
	int length    = -1;
	int maxlength = 0;
	CString path;
	CString type;

	try {
		path = CreateTempFile(NULL);
		GenerateJARFile(path);
		length  = GetFileLength(path);
		VERIFY(::DeleteFile(path));

	} catch(...) {
		VERIFY(::DeleteFile(path));
	}

	type = m_appli->appliConf->appliType;
	if (type == ASDOMV_JPHONEF|| 
		type == ASDOMV_JPHONEFM) {
		maxlength = 1024 * 50;

	} else 	if (type == ASDOMV_JPHONEG || 
		        type == ASDOMV_JPHONEGM) {
		maxlength = 1024 * 80;

	} else {
		ASSERT(FALSE);
		maxlength = -1;
	}

	m_output->EndBuildOutput(length,maxlength);
}

void Builder::BuildFirst() {
	m_step = BUILDSTEP_FIRST;
	m_stageFrames = 0;
	m_stageOffset = 0;
	Build(m_appli);
}

void Builder::BuildLast() {
	m_step        = BUILDSTEP_LAST;
	m_writingPos  = 0;
	m_frame       = 0;

	WriteUI32(0);

	Build(m_appli);

	SetWritingPos(0);
	WriteUI32(m_buffer.GetSize());
}

void Builder::BeginInstruction() {
	switch(GetBuildStep()) {
	case BUILDSTEP_FIRST:
		break;
	case BUILDSTEP_LAST:
		BuildLabelsPre();
		ASSERT(m_frame == 0);
		MarkFrame(m_frame);
		break;
	default:
		break;
	}
}
	
	

void Builder::EndInstruction() {
	switch(GetBuildStep()) {
	case BUILDSTEP_FIRST:
		break;
	case BUILDSTEP_LAST:
		BuildLabelsPost();
		break;
	default:
		break;
	}	
}

void Builder::WriteInstruction(int instructionID) {
	TRACE("Inst(%d) #%d at %d\n",instructionID,m_frame,GetWritingPos());
	if (instructionID == INSTID_SHOW_FRAME) {
		m_frame++;
		MarkFrame(m_frame);
	}

	WriteUI8(instructionID);
}

void Builder::MarkFrame(int frame) {

	ASSERT(GetBuildStep() == BUILDSTEP_LAST);
	ASSERT(0 <= frame && frame <= 0xFFFF);
	
	int previous = m_frames.size();
	int requiredFrames = frame + 1;
	if (previous != requiredFrames) {
		ASSERT(previous <= frame);
		m_frames.resize(requiredFrames);
		for (int n = previous;n < requiredFrames;n++) {
			m_frames[n] = -1;
		}
	}
	m_frames[frame] = GetWritingPos();
}

void Builder::BuildLabelsPre() {
	ASSERT(GetBuildStep() == BUILDSTEP_LAST);
	m_labelsPos = GetWritingPos();
	int labelCount = m_labels.size();
	WriteUI16(0);
	for (int n = 0;n < labelCount;n++) {
		WriteUI16(0);
		WriteUI32(0);
	}
}

void Builder::BuildLabelsPost() {
	ASSERT(GetBuildStep() == BUILDSTEP_LAST);
	LABELS::const_iterator i;
	int savedPos;

	savedPos = SetWritingPos(m_labelsPos);

	WriteUI16(m_labels.size());
	i = m_labels.begin();
	//TRACE("frames = %d\n",m_frames.size());
	while (i != m_labels.end()) {
		const Label* label = *i++;
		int frame  = label->GetLabelAbsolutePos();
		ASSERT(frame < m_frames.size());
		int offset = m_frames[frame];
		ASSERT(offset != -1);
		TRACE("label #%d @%d at %d\n",frame,offset,GetWritingPos());
		WriteUI16(frame);
		WriteUI32(offset);
	}

	SetWritingPos(savedPos);
}

CString Builder::GenerateADF(LPCTSTR jarName,UINT length) const {
	VALUES values;
	CString jarSize;

	GetValues(values);
	values["JarURL"]  = jarName;
	jarSize.Format("%d",length);
	values["JarSize"] = jarSize;

	CString adf  = ReplaceValues("sdp.jad",values);
	return adf;
}

void Builder::GenerateADFFile(LPCTSTR path,LPCTSTR jarName) const {
	UINT length = GetFileLength(CatPath(GetFileParent(path),jarName));
	CString adf = GenerateADF(jarName,length);
	WriteFile(path,ConvertToUTF8(adf));
}

void Builder::GenerateHTMFile(LPCTSTR path,LPCTSTR adfName) const {
	VALUES values;

	GetValues(values);
	values["ADFName"] = adfName;

	CString html = ReplaceValues("sdp.htm",values);
	WriteFile(path,html);
}

void Builder::GetValues(VALUES& values) const {
	ASSERT(AfxIsValidAddress(m_appli,sizeof Appli));
	ASSERT(AfxIsValidAddress(m_appli->appliConf.get(),sizeof AppliConf));

	const AppliConf* conf = m_appli->appliConf.get();

	values["Title"]       = conf->title;
	if (conf->vendor.GetLength() == 0) {
		values["Vendor"] = "-";
	} else {
		values["Vendor"] = conf->vendor;
	}
	if (conf->comment.GetLength() == 0) {
		values["Description"] = "";
	} else {
		values["Description"] = conf->comment;
	}
}

CString Builder::GetSourcePath(LPCTSTR fname) const {
	ASSERT(AfxIsValidString(fname));
	CString directory;
	CString path;
	BOOL    result;

	directory = GetAppFilePath("sdp\\" + m_appli->appliConf->appliType);

	result = (::GetFileAttributes(directory) & FILE_ATTRIBUTE_DIRECTORY) != 0;
	if (!result) {
		ThrowException();
	}
	
	path      = directory + "\\" + fname;
	result = ::GetFileAttributes(path) != -1;
	if (!result) {
		ThrowException();
	}

	return path;
}

CString Builder::ReplaceValues(LPCTSTR fname,const VALUES& values) const {
	ASSERT(AfxIsValidString(fname));
	VALUES::const_iterator i;
	CString text;

	text = ReadFile(GetSourcePath(fname));

	i = values.begin();
	while (i != values.end()) {
		//TRACE("%s,%s\n",i->first,i->second);
		text.Replace("${" + i->first + "}",i->second);
		i++;
	}

	return text;
}

void Builder::GenerateJARFile(LPCTSTR path) const {
	ASSERT(AfxIsValidString(path));
	ASSERT(AfxIsValidAddress(m_appli,sizeof Appli));
	BOOL    result;
	CString tempDir;
	CString compDir;
	UINT length;
	LPCVOID pointer;
	CString savedCurrent;

	result = ::CopyFile(GetSourcePath("sdp.jar"),path,FALSE);
	if (!result) {
		ThrowException();
	}
	
	// 一時ディレクトリ作成
	compDir = CreateTempDirectory("SDT");

	// アイコンファイルの保存
	LBuffer& icon = m_appli->appliConf->icon;
	length = icon.GetSize();
	if (length != 0) {
		CFile fileIcon;
		result = fileIcon.Open(compDir + "\\icon.png",
			                   CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
		if (!result) {
			ThrowException();
		}
		pointer = icon.Lock();
		fileIcon.Write(pointer,length);
		icon.Unlock();
		if (!result) {
			ThrowException();
		}
		fileIcon.Close();
	} else {
		result = ::CopyFile(GetSourcePath("icon.png"),compDir + "\\icon.png",FALSE);
		if (!result) {
			ThrowException();
		}
	}

	// データファイルの保存
	GenerateSDCFile(compDir + "\\sdp.sdc");

	// META-INFディレクトリの作成
	result = ::CreateDirectory(compDir + "\\META-INF",NULL) != 0;
	if (!result) {
		ThrowException();
	}

	// META-INFファイルの生成
	VALUES values;
	GetValues(values);
	CString manifest = ReplaceValues("MANIFEST.MF",values);
	WriteFile(compDir + "\\META-INF\\MANIFEST.MF",ConvertToUTF8(manifest));

	
	savedCurrent = GetCurrentDirectory();
	result = SetCurrentDirectory(compDir);
	if (!result) {
		ThrowException();
	}

	CInfoZip iz;
	VERIFY(iz.Initialize());
	char* paths[] = {
		"icon.png",
		"sdp.sdc",
		"META-INF\\MANIFEST.MF",
		};
	iz.AddFiles(path,paths,sizeof paths / sizeof paths[0]);
	VERIFY(iz.Finalize());

	result = SetCurrentDirectory(savedCurrent);
	if (!result) {
		ThrowException();
	}
}

void Builder::GenerateSDCFile(LPCTSTR path) const {
	CFile fileSDC;
	BOOL result = fileSDC.Open(path,
		                  CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
	if (!result) {
		ThrowException();
	}
	fileSDC.Write(m_buffer.GetData(),m_buffer.GetSize());
	fileSDC.Close();
}

int Builder::GetWritingPos() const {
	ASSERT(GetBuildStep() == BUILDSTEP_LAST);
	ASSERT(m_writingPos <= m_buffer.GetSize());
	return m_writingPos;
}

int Builder::SetWritingPos(int pos) {
	ASSERT(pos <= m_buffer.GetSize());
	int prev = GetWritingPos();
	m_writingPos = pos;
	return prev;
}

void Builder::WriteUI8(int value) {
	ASSERT(0 <= value && value <= 0xff);
	BYTE byte = value;
	int currentSize = m_buffer.GetSize();
	int needSize = m_writingPos + 1;
	if (currentSize < needSize) {
		m_buffer.SetSize(needSize);
	}
	m_buffer[m_writingPos++] = byte;
}

void Builder::WriteUI16(int value) {
	ASSERT(0 <= value && value <= 0xffff);
	WriteUI8((value >> 8) & 0xFF);
	WriteUI8(value        & 0xFF);
}

void Builder::WriteUI32(int value) {
	ASSERT(0 <= value && value <= 0xffffffff);
	WriteUI8((value >> 24) & 0xFF);
	WriteUI8((value >> 16) & 0xFF);
	WriteUI8((value >>  8) & 0xFF);
	WriteUI8(value         & 0xFF);
}

void Builder::WriteSI16(int value) {
	ASSERT(-32768 <= value && value <= 32767); 
	unsigned int ui = (unsigned int)value;
	ASSERT(0 <= ui && ui <= 0xffff);
	WriteUI16((int)ui);
}

void Builder::CastDoubleToFI25(double value,int& integer,int& decimal) {
	ASSERT(-32768 <= value && value <= 32767); 
	integer = value;
	decimal = (value - integer) * 256;
	if (decimal < 0) {
		decimal += 256;
		integer -= 1;
	}
	ASSERT(0 <= decimal && decimal <= 255);	
	ASSERT(-32768 <= integer && integer <= 32767); 
}

void Builder::WriteFI24(double value) {
	ASSERT(-32768 <= value && value <= 32767); 
	int integer,decimal;
	CastDoubleToFI25(value,integer,decimal);
	ASSERT(0 <= decimal && decimal <= 255);
	ASSERT(-32768 <= integer && integer <= 32767); 

	WriteSI16(integer);
	WriteUI8(decimal);
}

int Builder::SubBits(int bits,int offset,int length) {
	ASSERT(0 <= offset && offset < 32);
	ASSERT(0 <  length && length <= 32);
	
	const int mask = (static_cast<__int64>(1) << length) - 1;
	bits >>= offset;
	bits  &= mask;
	return bits;
}

void Builder::WriteXY(int x,int y) {
	ASSERT(-2048 <= x && x <= 2047);
	ASSERT(-2048 <= y && y <= 2047);
	
	int b1 = SubBits(x,4,8);
	int b2 =(SubBits(x,0,4) << 4) + SubBits(y,8,4);
	int b3 = SubBits(y,0,8);
	WriteUI8(b1);
	WriteUI8(b2);
	WriteUI8(b3);
}

void Builder::WriteColor(COLORREF cr) {
	ASSERT(0x000000 <= cr && cr <= 0xFFFFFF);
	WriteUI8(GetRValue(cr));
	WriteUI8(GetGValue(cr));
	WriteUI8(GetBValue(cr));
}

void Builder::WriteBuffer(LPCVOID pv,int length) {
	ASSERT(AfxIsValidAddress(pv,length,FALSE));
	WriteUI16(length);
	LPBYTE bytes = (LPBYTE)pv;
	for (int n = 0;n < length;n++) {
		WriteUI8(bytes[n]);
	}
}

void Builder::WriteSJIS(LPCTSTR string) {
	ASSERT(AfxIsValidString(string));
	int length = strlen(string);
	WriteBuffer(string,length);
}

void Builder::WriteChar(const Char* c,int type) {
	ASSERT_VALID(c);
	int id = GetCharID(c);
	//TRACE("WriteChar at %d (%d,%d)\n",GetWritingPos(),id,type);
	WriteUI16(id);
	WriteUI8(type);
}

void Builder::WriteCharRef(const Char* c) {
	int charid;
	if (c == NULL) {
		charid = 0;
	} else {
		ASSERT_VALID(c);
		charid = GetCharID(c);
	}
	WriteUI16(charid);
}

void Builder::WriteDepth(int depth) {
	WriteUI8(depth);
}

void Builder::WriteLabelRef(const Label* label) {
	WriteUI16(label->GetLabelAbsolutePos());
}

BOOL Builder::IsValidLabel(const Label* label) const {
	if (label == NULL) {
		return false;
	}
	return std::find(m_labels.begin(),m_labels.end(),label) != m_labels.end();
}

void Builder::WriteMatrix(const AffineMatrix* matrix) {
	ASSERT(AfxIsValidAddress(this,sizeof Builder));
	ASSERT(AfxIsValidAddress(matrix,sizeof AffineMatrix));
	WriteXY(matrix->x,matrix->y);
}

void Builder::WriteColoring(const Coloring* coloring) {
	ASSERT(AfxIsValidAddress(this,sizeof Builder));
	ASSERT(AfxIsValidAddress(coloring,sizeof Coloring));
	/*
	if (coloring->addRed   == 0 &&
		coloring->addGreen == 0 &&
		coloring->addBlue  == 0 &&
		coloring->mulRed   == 1 &&
		coloring->mulGreen == 1 &&
		coloring->mulBlue  == 1) {
		return;
	}
	*/
	WriteSI16(coloring->addRed);
	WriteSI16(coloring->addGreen);
	WriteSI16(coloring->addBlue);
	WriteFI24(coloring->mulRed);
	WriteFI24(coloring->mulGreen);
	WriteFI24(coloring->mulBlue);
}

void Builder::WriteAction(int actionID) {
	WriteUI8(actionID);
}

void Builder::EnterStage(int stageFrames) {
	//TRACE("*** EnterStage %d:%d\n",stageFrames,GetBuildStep());
	switch (GetBuildStep()) {
	case BUILDSTEP_FIRST:
		ASSERT(0 <= stageFrames);
		ASSERT(0 <= m_stageOffset);
		ASSERT(0 <= m_stageFrames);
		m_stageOffset += m_stageFrames;
		m_stageFrames  = stageFrames;
		break;
	case BUILDSTEP_LAST:
		m_stageOffset = -1;
		break;
	default:
		break;
	}
}

void Builder::NotifyErr (UINT nFormat,...) {
	va_list args;
	va_start(args,nFormat);

	CString format;
	format.LoadString(nFormat);
	CString string;
	string.FormatV(format,args);
	va_end(args);
	
	m_output->NotifyErr(GetCurrentContext(),string);
}

void Builder::NotifyWarn(UINT nFormat,...) {
	va_list args;
	va_start(args,nFormat);

	CString format;
	format.LoadString(nFormat);
	CString string;
	string.FormatV(format,args);
	va_end(args);
	
	m_output->NotifyWarn(GetCurrentContext(),string);
}

Builder::BUILDSTEP Builder::GetBuildStep() const {
	ASSERT(m_step == BUILDSTEP_FIRST ||
		   m_step == BUILDSTEP_LAST);
	return m_step;
}

int Builder::GetAbsoluteFramePos(int relativePos) const {
	return m_stageOffset + relativePos;
}

void Builder::UseChar(const Buildable* object) {
	ASSERT(GetBuildStep() == Builder::BUILDSTEP_FIRST);
	if (object == NULL) {
		return;
	}

	ASSERT(AfxIsValidAddress(object,sizeof Buildable));
	CHARUSES::iterator i = m_charUses.find(object);
	if (i == m_charUses.end()) {
		m_charUses.insert(CHARUSES::value_type(object,1));
	} else {
		i->second++;
	}
}

int Builder::GetCharID(const Buildable* object) const {
	ASSERT(GetBuildStep() == Builder::BUILDSTEP_LAST);
	ASSERT(AfxIsValidAddress(object,sizeof Buildable));
	CHARUSES::const_iterator i = m_charUses.find(object);
	if (i == m_charUses.end()) {
		ASSERT(FALSE);
	}
	int length = m_charUses.size();
	int id = std::distance(m_charUses.begin(),i) + 1;
	ASSERT(0 < id && id <= length);
	//TRACE("GetCharID=%d,%d\n",id,length);
	return id;
}

int Builder::GetCharUseCount(const Buildable* object) const {
	ASSERT(GetBuildStep() == Builder::BUILDSTEP_LAST);
	ASSERT(AfxIsValidAddress(object,sizeof Buildable));
	CHARUSES::const_iterator i = m_charUses.find(object);
	if (i == m_charUses.end()) {
		return 0;
	} else {
		return i->second;
	}
}

BuildContext::BuildContext(Builder* builder,UINT resid,...) {
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));
	m_builder = builder;

	va_list args;
	va_start(args,resid);

	CString format;
	format.LoadString(resid);
	CString formatV;
	formatV.FormatV(format,args);
	va_end(args);

	m_builder->EnterContext(formatV);
}

BuildContext::BuildContext(Builder* builder,LPCTSTR ptsz) {
	ASSERT(AfxIsValidString(ptsz));
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));
	m_builder = builder;
	m_builder->EnterContext(ptsz);
}

BuildContext::~BuildContext() {
	m_builder->ExitContext();
}

void Builder::EnterContext(LPCTSTR ptsz) {
	ASSERT(AfxIsValidString(ptsz));
	if (0 < m_contexts.size()) {
		m_contexts.push(GetCurrentContext() + _T(" / ") + ptsz);
	} else {
		m_contexts.push(ptsz);
	}
}

void Builder::ExitContext() {
	ASSERT(0 < m_contexts.size());
	m_contexts.pop();
}

CString Builder::GetCurrentContext() const {
	ASSERT(0 < m_contexts.size());
	return m_contexts.top();
}

