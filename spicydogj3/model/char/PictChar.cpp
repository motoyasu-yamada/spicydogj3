#include "stdafx.h"
#include "resource.h"
#include "PictChar.h"

#include <afxmt.h>
#include "sdom/SDOMArchive.h"
#include "sdom/SDOMException.h"
#include "model/build/Builder.h"
#include "model/AffineMatrix.h"
#include "model/type/FPoint.h"
#include "utility/ReenterLock.h"
#include "utility/FileUtility.h"
#include "utility/LeadToolUtility.h"
#include "view/PictCharView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if defined(_DEBUG)
#define L_DISPLAY_ERROR_LIST do {ASSERT(FALSE);LSettings s;s.DisplayErrorList(AfxGetMainWnd()->GetSafeHwnd());} while(FALSE)
#else
#define L_DISPLAY_ERROR_LIST ((void)(0))
#endif


namespace PictCharImpl {
	int GetColorBits(int nColors);
	BOOL is_between(int n,int min,int max);
	BOOL IsLeadJpeg(int n);
	void CopyGlobalAlloc2Buf(LBuffer& dst,const SDOMBuffer& src);
	BOOL ExistPath(LPCTSTR path);
	int NormalizeColorBitsPNG(int);
};
using namespace PictCharImpl;

PictData::PictData() {
}

PictData::PictData(const PictData& r) {
	CopyFrom(r);
}

PictData& PictData::operator = (const PictData& r) {
	CopyFrom(r);
	return *this;
}

void PictData::CopyFrom(const PictData& r) {
	if (m_bmp.Copy(const_cast<LBitmapBase&>(r.m_bmp)) != SUCCESS) {
		ASSERT(FALSE);
		AfxThrowMemoryException();
	}
	if (const_cast<LBuffer&>(r.m_data).GetSize() == 0) {
		m_data.Free();
		return;
	}
	if (m_data.Copy(const_cast<LBuffer*>(&r.m_data)) != SUCCESS) {
		TRACE("=>%d\n",GetLastError());
		ASSERT(FALSE);
		AfxThrowMemoryException();
	}
}

void PictQuantizeParam::Init() {
	m_quantize    = PICT_QUANTIZE_PATHTHROUGH;
	m_jpgCompress = 80;
	m_pngPaletteType  = ACTTYPE_IMODE;
	m_pngPalette      = *CDefaultActs::GetDefaultActs()->GetAct(ACTTYPE_IMODE);
	m_pngColors   = 256;
	m_pngUseDither        = 0;
	m_pngUseTransparency  = FALSE;
	m_pngTransparentColor = RGB(0x00,0x00,0x00);
}

PictQuantizeParam::PictQuantizeParam() {
	Init();
}

PictQuantizeParam::PictQuantizeParam(const PictQuantizeParam& r) {
	Init();
	CopyFrom(r);
}

PictQuantizeParam& PictQuantizeParam::operator = (const PictQuantizeParam& r) {
	Init();
	CopyFrom(r);
	return *this;
}

void PictQuantizeParam::CopyFrom(const PictQuantizeParam& r) {
	m_quantize = r.m_quantize;
	if (m_quantize == PICT_QUANTIZE_JPG) {
		m_jpgCompress = r.m_jpgCompress;
	}
	if (m_quantize == PICT_QUANTIZE_PNG) {
		m_pngPalette        = r.m_pngPalette;
		m_pngPaletteType    = r.m_pngPaletteType;
		m_pngColors         = r.m_pngColors;
		m_pngUseDither      = r.m_pngUseDither;
		m_pngTransparentColor = r.m_pngTransparentColor;
		m_pngUseTransparency  = r.m_pngUseTransparency;
	}
}

PictOptCache::PictOptCache() {
	Init();
}

void PictOptCache::Init() {
	m_bValid = FALSE;
	if (m_bmp.IsAllocated()) {
		m_bmp.Free();
	}
	if (m_data.IsValid()) {
		m_data.Free();
	}
	m_nFormat  = (PICTOPTOUTFORMAT)-1;
	m_nDataLen = -1;
	m_bTrans   = FALSE;
	m_crTrans  = RGB(0x00,0x00,0x00);
	m_sz       = CSize(0,0);
}


void PictData::AssertValid() const {
	LBitmapBase& bmp = const_cast<LBitmapBase&>(m_bmp);
	ASSERT(bmp.GetBitsPerPixel() == 24);
	ASSERT(bmp.GetColorOrder() == ORDER_BGR);
	ASSERT(is_between(bmp.GetWidth(), PICT_MIN_WIDTH,PICT_MAX_WIDTH));
	ASSERT(is_between(bmp.GetHeight(),PICT_MIN_HEIGHT,PICT_MAX_HEIGHT));
}


void PictQuantizeParam::AssertValid() const {
	switch(m_quantize) {
	case PICT_QUANTIZE_PNG:
		ASSERT(0 <= m_pngTransparentColor && m_pngTransparentColor <= 0xFFFFFF);
		ASSERT(2 <= m_pngColors && m_pngColors <= 256);	
		break;
	case PICT_QUANTIZE_JPG:
		ASSERT(0 <= m_jpgCompress && m_jpgCompress <= 100);
		break;
	case PICT_QUANTIZE_PATHTHROUGH:
		break;
	default:
		ASSERT(FALSE);
	}
}

void PictOptCache::AssertValid() const {
	ASSERT(m_bValid);
	LBitmapBase& bmp = const_cast<LBitmapBase&>(m_bmp);
	LBuffer&     dat = const_cast<LBuffer&>    (m_data);
	ASSERT(is_between(bmp.GetWidth(), PICT_MIN_WIDTH,PICT_MAX_WIDTH));
	ASSERT(is_between(bmp.GetHeight(),PICT_MIN_HEIGHT,PICT_MAX_HEIGHT));
	ASSERT(0 < dat.GetSize());
}

void PictChar::AssertValid() const {
	Char::AssertValid();
	/** @todo
	DEBUG_ONLY(m_data.AssertValid());
	DEBUG_ONLY(m_quantize.AssertValid());
	DEBUG_ONLY(m_cache.AssertValid());

	PictData&     dat   = const_cast<PictData&>(m_data);
	PictOptCache& cache = const_cast<PictOptCache&>(m_cache);

	CSize sz = GetBitmapSize();
	ASSERT(is_between(m_centerPoint.x,0,sz.cx)); 
	ASSERT(is_between(m_centerPoint.y,0,sz.cy));
	ASSERT(dat.m_bmp.GetWidth()  == cache.m_bmp.GetWidth());
	ASSERT(dat.m_bmp.GetHeight() == cache.m_bmp.GetHeight());

	switch (m_quantize.m_quantize){
	case PICT_QUANTIZE_PNG:
		ASSERT(cache.m_bmp.GetBitsPerPixel() == GetColorBits());
		break;
	case PICT_QUANTIZE_JPG:
		break;
	case PICT_QUANTIZE_PATHTHROUGH:
		ASSERT(dat.m_data.GetSize() == cache.m_data.GetSize());
		break;
	}
	*/
}


CharType PictChar::PICT_CHAR_TYPE(
	RUNTIME_CLASS(PictCharView),
	"PictChar",
	IDS_CHAR_DISPLAY_NAME_PICT,
	WSDOMN_PICT_CHAR,
	2,
	false,
	true,
	false,
	true,
	false,
	false);

PictChar::PictChar() {
	m_centerPoint = CPoint(0,0);
}

PictChar::PictChar(const PictChar& right) {
	m_centerPoint = right.m_centerPoint;
	m_quantize = right.m_quantize;
	m_data     = right.m_data;
	Optimize();
}

const CharType& PictChar::GetCharType() const {
	return PICT_CHAR_TYPE;
}

Char* PictChar::Clone() const {
	return new PictChar(*this);
}

PictChar* PictChar::newPictChar(const PictData& data,const PictQuantizeParam& param) {
	PictChar* pNew = new PictChar;
	pNew->m_data = data;
	pNew->m_quantize = param;
	pNew->m_centerPoint.x = pNew->m_data.m_bmp.GetWidth()  / 2;
	pNew->m_centerPoint.y = pNew->m_data.m_bmp.GetHeight() / 2;
	if (!pNew->Optimize()) {
		ASSERT(FALSE);
		delete pNew;
		return NULL;
	}
	return pNew;
}

PictChar* PictChar::newPaste(HBITMAP hBitmap,HPALETTE hPalette) {
	PictData data;
	CClientDC dc(CWnd::GetDesktopWindow());
	if (data.m_bmp.ConvertFromDDB(dc, hBitmap, hPalette) != SUCCESS) {
		ASSERT(FALSE);
		return NULL;
	}

	L_UINT32 DEFAULT_FLAGS = CRF_FIXEDPALETTE|CRF_FLOYDSTEINDITHERING;
	if (data.m_bmp.GetBitsPerPixel() != 24 &&
		data.m_bmp.ColorRes(24,CRF_BYTEORDERBGR|DEFAULT_FLAGS) != SUCCESS) {
		ASSERT(FALSE);
		return NULL;
	}

	if (!is_between(data.m_bmp.GetWidth(),PICT_MIN_WIDTH,PICT_MAX_WIDTH)) {
		ASSERT(FALSE);
		return NULL;
	}

	if (!is_between(data.m_bmp.GetHeight(),PICT_MIN_HEIGHT,PICT_MAX_HEIGHT)) {
		ASSERT(FALSE);
		return NULL;
	}

	PictQuantizeParam param;
	if (hPalette != NULL) {
		param.m_pngPaletteType = ACTTYPE_CUSTOM;
		param.m_pngColors = ::GetPaletteEntries(hPalette,0,0,NULL);
		param.m_pngPalette.CreateFromPalette(hPalette,param.m_pngColors);
	} else {
		param.m_pngPaletteType = ACTTYPE_IMODE;
		param.m_pngColors = 256;
		param.m_pngPalette = *CDefaultActs::GetDefaultActs()->GetAct(ACTTYPE_IMODE);
	}
	param.m_quantize = PICT_QUANTIZE_PNG;
	return newPictChar(data,param);
}

PictChar* PictChar::newEmpty(CSize sz,COLORREF cr) {
	if (!is_between(sz.cx,PICT_MIN_WIDTH,PICT_MAX_WIDTH)) {
		ASSERT(FALSE);
		return NULL;
	}

	if (!is_between(sz.cy,PICT_MIN_HEIGHT,PICT_MAX_HEIGHT)) {
		ASSERT(FALSE);
		return NULL;
	}

	PictData data;
	if (data.m_bmp.Create(sz.cx,sz.cy,24,ORDER_BGR) != SUCCESS) {
		ASSERT(FALSE);
		AfxThrowMemoryException();
	}

	if (data.m_bmp.Fill(cr) != SUCCESS) {
		ASSERT(FALSE);
		return NULL;
	}

	PictQuantizeParam param;
	param.m_quantize = PICT_QUANTIZE_PNG;
	return newPictChar(data,param);
}

PictChar* PictChar::newImportPathThrough(LPCTSTR ptszPath) {
	ASSERT(AfxIsValidString(ptszPath));
	ASSERT(ExistPath(ptszPath));

	PictData data;
	CFile file;
	
	if (!file.Open(ptszPath,CFile::modeRead | CFile::shareDenyNone)) {
		ASSERT(FALSE);
		return NULL;
	}

	UINT length = file.GetLength();
	data.m_data.Reallocate(length);
	
	LPVOID pointer = data.m_data.Lock();
	UINT read = file.Read(pointer,length);
	data.m_data.Unlock();
	if (read != length) {
		ASSERT(FALSE);
		return NULL;
	}

	file.Close();

	if (!PictChar::Data2Bmp(data.m_data,data.m_bmp)) {
		ASSERT(FALSE);
		return NULL;
	}

	PictQuantizeParam param;
	param.m_quantize = PICT_QUANTIZE_PATHTHROUGH;

	return newPictChar(data,param);
}

/**
	画像ファイルを「スパイシードッグ自動書き出し方式」として読み込み
	内部形式に変換する。
*/
PictChar* PictChar::newImport(LPCTSTR ptszPath) {
	ASSERT(AfxIsValidString(ptszPath));
	ASSERT(ExistPath(ptszPath));
	PictData data;
	
	if (!LoadBmp(ptszPath,data)) {
		ASSERT(FALSE);
		return NULL;
	}

	PictQuantizeParam param;
	param.m_quantize = PICT_QUANTIZE_PNG;
	return newPictChar(data,param);
}

BOOL PictChar::IsEditable() const {
	return m_quantize.m_quantize != PICT_QUANTIZE_PATHTHROUGH;
}

int PictChar::GetColorBits() const {
	ASSERT(m_quantize.m_quantize == PICT_QUANTIZE_PNG);
	return PictCharImpl::GetColorBits(m_quantize.m_pngColors);
}

void PictChar::SetCenter(CPoint pt) {
	m_centerPoint = pt;
	ASSERT_VALID(this);
}

void PictChar::SetPictData(const PictData& data) {
	ASSERT(IsEditable());
	m_data = data;
	m_centerPoint.x  = min(m_centerPoint.x,m_data.m_bmp.GetWidth());
	m_centerPoint.y  = min(m_centerPoint.y,m_data.m_bmp.GetHeight());
	Optimize();
	ASSERT_VALID(this);
}

void PictChar::SetPictOptParam(const PictQuantizeParam& param) {
	DEBUG_ONLY(param.AssertValid());
	m_quantize = param;
	Optimize();
	ASSERT_VALID(this);	
}

BOOL PictChar::ExportCache(LPCTSTR ptsz) const {
	try {
		ASSERT(AfxIsValidString(ptsz));
		ASSERT(this);
		LBitmapBase& bmp = const_cast<LBitmapBase&>(m_cache.m_bmp);

		LBuffer buffer;
		LMemoryFile fileMem;
		fileMem.SetBitmap(&bmp);

		if (fileMem.SaveBitmap(&buffer,FILE_PNG,GetColorBits(),0) == SUCCESS) {
			ASSERT(FALSE);
			return FALSE;
		}

		LPVOID p = ::GlobalLock(buffer.GetHandle());

		CFile file;
		VERIFY(file.Open(ptsz,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone));
		file.Write(p,buffer.GetSize());
		file.Close();
		
		::GlobalUnlock(p);

	} catch(CFileException* pe) {
		ASSERT(FALSE);
		pe->Delete();
		return FALSE;
	}
	return TRUE;
}


BOOL PictChar::LoadBmp(LPCTSTR ptszPath,PictData& data) {
	LFile file;
	file.SetBitmap(&data.m_bmp);
	file.SetFileName(const_cast<LPTSTR>(ptszPath));

	FILEINFO fi;
	ZeroMemory(&fi,sizeof fi);
	if(file.LoadBitmap(24,ORDER_BGR,NULL,&fi) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		return FALSE;
	}
	return TRUE;
}

BOOL OptimizePAT(PictOptCache& cache,const PictData& data) {
	cache.m_data.Copy(const_cast<LBuffer*>(&data.m_data));
	
	FILEINFO fi;
	ZeroMemory(&fi,sizeof fi);
	LMemoryFile file;
	if (file.GetInfo(cache.m_data,&fi) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		return FALSE;
	}

	PICTOPTOUTFORMAT of;
	if (IsLeadJpeg(fi.Format)) {
		of = PICTOPTOUTFORMAT_JPG;
	} else if (fi.Format == FILE_PNG) {
		of = PICTOPTOUTFORMAT_PNG;
	} else {
		ASSERT(FALSE);
		return FALSE;
	}

	file.SetBitmap(&cache.m_bmp);
	if (file.LoadBitmap(cache.m_data,24,ORDER_BGR) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		return FALSE;
	}
	ASSERT(cache.m_bmp.GetColorOrder() == ORDER_BGR);
/** @todo
	if (PICTOPTOUTFORMAT_JPG == of && cache.m_bmp.IsProgressive()) {
		ASSERT(FALSE);
		return FALSE;
	}
*/

	BITMAPHANDLE* pbmph = cache.m_bmp.GetHandle();
	ASSERT(AfxIsValidAddress(pbmph,sizeof BITMAPHANDLE));
	if (pbmph->Flags.Transparency) {
		cache.m_bTrans = TRUE;
		//if (fi.BitsPerPixel <= 8)  
		if (0xFFFFFF < pbmph->Transparency) {
			int nTrans = pbmph->Transparency-0x1000000;
			if (pbmph->hPalette == NULL) {
				ASSERT(FALSE);
				return FALSE;
			}
			if (pbmph->nColors <= nTrans) {
				ASSERT(FALSE);
				return FALSE;
			}

			PALETTEENTRY pe;
			if (::GetPaletteEntries(pbmph->hPalette,nTrans,1,&pe) != 1) {
				ASSERT(FALSE);
				return FALSE;
			}
			cache.m_crTrans = RGB(pe.peRed,pe.peGreen,pe.peBlue);
		} else {
			cache.m_crTrans = pbmph->Transparency;
		}
	}

	cache.m_nFormat = of;
	cache.m_bValid  = TRUE;
	return TRUE;
}

BOOL OptimizeGIF(PictOptCache& cache,const PictData& data,const PictQuantizeParam& param) {
	ASSERT(param.m_quantize == PICT_QUANTIZE_PNG);
	LBitmapBase& bmp = const_cast<LBitmapBase&>(data.m_bmp);

	int typeAct = param.m_pngPaletteType;

	Act act = param.m_pngPalette;
	act.SorByBitmap(bmp,param.m_pngUseTransparency,param.m_pngTransparentColor);
	HPALETTE hpal = act.CreatePalette();
	if (hpal == NULL) {
		ASSERT(FALSE);
		return FALSE;
	}

	int nFlag = CRF_USERHPALETTE;
	if (param.m_pngUseDither) {
		nFlag |= CRF_NODITHERING;
	} else {
		nFlag |= CRF_FLOYDSTEINDITHERING;
	}

	int nColorBits = GetColorBits(param.m_pngColors);
	if (cache.m_bmp.ColorRes(bmp,nColorBits,nFlag,NULL,hpal,act.GetCount()) != SUCCESS) {
		::DeleteObject(hpal);
		return FALSE;
	}

	ASSERT(cache.m_bmp.GetBitsPerPixel() == nColorBits);
	cache.m_bmp.SetPalette(hpal);
	BITMAPHANDLE* pbmph = cache.m_bmp.GetHandle();
	ASSERT(AfxIsValidAddress(pbmph,sizeof BITMAPHANDLE));	
	
	pbmph->Transparency       = 0xFFFFFFFF;
	pbmph->Flags.Transparency = FALSE;
	if (param.m_pngUseTransparency) {
		int nIndex = ::GetNearestPaletteIndex(hpal,param.m_pngTransparentColor);
		if (nIndex != -1) {
			pbmph->Flags.Transparency = TRUE;
			pbmph->Transparency = 0x1000000 + nIndex;
		}
		else {
			ASSERT(FALSE);
			return FALSE;
		}
	}

	LMemoryFile fileMem;
	fileMem.SetBitmap(&cache.m_bmp);
	if (fileMem.SaveBitmap(&cache.m_data,FILE_PNG,NormalizeColorBitsPNG(nColorBits)) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		return FALSE;
	}
	
	cache.m_bTrans  = param.m_pngUseTransparency;
	if (cache.m_bTrans) {
		cache.m_crTrans = param.m_pngTransparentColor;
	}

	cache.m_nFormat = PICTOPTOUTFORMAT_PNG;
	cache.m_bValid  = TRUE;
	return TRUE;
}

BOOL OptimizeJPG(PictOptCache& cache,const PictData& data,const PictQuantizeParam& param) {
	ASSERT(param.m_quantize == PICT_QUANTIZE_JPG);
	LBitmapBase& bmp = const_cast<LBitmapBase&>(data.m_bmp);

	LMemoryFile fileMem;
	fileMem.SetBitmap(&bmp);

	const int MINQ = 2;
	const int MAXQ = 255;
	int nQ = (100 - param.m_jpgCompress) * (MAXQ - MINQ) / 100 + MINQ;
	ASSERT(MINQ <= nQ && nQ <= MAXQ);
	if (fileMem.SaveBitmap(&cache.m_data,FILE_JFIF,24,nQ) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		return FALSE;
	}

	fileMem.SetBitmap(&cache.m_bmp);
	if (fileMem.LoadBitmap(cache.m_data,24,ORDER_BGR) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		return FALSE;
	}

	cache.m_nFormat = PICTOPTOUTFORMAT_JPG;
	cache.m_bValid  = TRUE;
	return TRUE;
}


BOOL PictChar::Optimize(PictOptCache& cache,const PictData& data,const PictQuantizeParam& param) {
	if (cache.m_bmp.IsAllocated()) {
		cache.m_bmp.Free();
	}
	if (cache.m_data.IsValid()) {
		cache.m_data.Free();
	}

	cache.m_bValid    = FALSE;
	cache.m_nDataLen  = -1;
	cache.m_bTrans    = FALSE;
	
	//DEBUG_ONLY(data.AssertValid());
	DEBUG_ONLY(param.AssertValid());

	if (param.m_quantize == PICT_QUANTIZE_PATHTHROUGH && 
		!OptimizePAT(cache,data)) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (param.m_quantize == PICT_QUANTIZE_PNG && 
		!OptimizeGIF(cache,data,param)) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (param.m_quantize == PICT_QUANTIZE_JPG && 
		!OptimizeJPG(cache,data,param)) {
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(cache.m_bValid);

	cache.m_sz       = CSize(cache.m_bmp.GetWidth(),cache.m_bmp.GetHeight());
	cache.m_nDataLen = cache.m_data.GetSize();

	DEBUG_ONLY(param.AssertValid());
	DEBUG_ONLY(cache.AssertValid());

	return TRUE;
}

BOOL PictChar::Optimize() const {
	BOOL b = Optimize(m_cache,m_data,m_quantize);
	ASSERT_VALID(this);
	return b;
}

CPoint PictChar::GetCenter() const {
	ASSERT_VALID(this);
	return m_centerPoint;
}

const PictQuantizeParam& PictChar::GetPictOptParam() const {
	ASSERT_VALID(this);
	return m_quantize;
}

const PictData& PictChar::GetPictData() const {
	ASSERT_VALID(this);
	return m_data;
}


void PictChar::CreateAct(Act& act,const LBitmapBase& bmp,const PictQuantizeParam& param) {
	switch (param.m_pngPaletteType) {
	case ACTTYPE_IMODE:
	case ACTTYPE_WEB:
	case ACTTYPE_WIN:
	case ACTTYPE_MAC:
	case ACTTYPE_GRAY:
	case ACTTYPE_MONO:
	case ACTTYPE_RGB:
		act = *CDefaultActs::GetDefaultActs()->GetAct((ACTTYPE)param.m_pngPaletteType);
		break;
	case ACTTYPE_CUSTOM:
		act.CreateFromBitmap(bmp,param.m_pngUseTransparency,param.m_pngTransparentColor);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}


CSize PictChar::GetBitmapSize() const {
	LBitmapBase& bmp = const_cast<LBitmapBase&>(m_data.m_bmp);
	CSize sz(bmp.GetWidth(),bmp.GetHeight());
	ASSERT(is_between(sz.cx,PICT_MIN_WIDTH, PICT_MAX_WIDTH));
	ASSERT(is_between(sz.cy,PICT_MIN_HEIGHT,PICT_MAX_HEIGHT));
	return sz;
}

void PictChar::Serialize(SDOMArchive* ar) {
	Char::Serialize(ar);

	ar->EnterType(m_quantize.m_quantize,PictQuantizeType);

	switch(m_quantize.m_quantize) {
	case PICT_QUANTIZE_PATHTHROUGH:
		ar->SerializeBuffer(WSDOMN_DATA,     m_data.m_data);
		if (ar->IsLoading()) {
			PictChar::Data2Bmp(m_data.m_data,m_data.m_bmp);
		}
		break;
	case PICT_QUANTIZE_JPG:
		ar->SerializeBitmap(WSDOMN_BITMAP,m_data.m_bmp);
		ar->SerializeINT   (WSDOMN_COMPRESS,m_quantize.m_jpgCompress);
		break;
	case PICT_QUANTIZE_PNG:
		ar->SerializeBitmap(WSDOMN_BITMAP,m_data.m_bmp);
		ar->SerializeType  (WSDOMN_PALETTE_TYPE,m_quantize.m_pngPaletteType,PaletteType);
		ar->SerializeStatic(WSDOMN_PALETTE,     &m_quantize.m_pngPalette);
		ar->SerializeINT   (WSDOMN_COLORS,      m_quantize.m_pngColors);
		ar->SerializeBOOL  (WSDOMN_USE_DITHER,         m_quantize.m_pngUseDither);
		ar->SerializeBOOL  (WSDOMN_USE_TRANSPARENCY,   m_quantize.m_pngUseTransparency);
		ar->SerializeCOLORREF(WSDOMN_TRANSPARENT_COLOR,m_quantize.m_pngTransparentColor);
		break;
	default:
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		break;
	}
	ar->ExitType();

	ar->SerializePOINT(WSDOMN_CENTER_POINT,m_centerPoint);

	if (ar->IsLoading()) {
		VERIFY(Optimize());
	}
}

void PictChar::Build(Builder* builder) const {
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	BuildContext context(builder,GetName());
	//** @todo
	if (!m_cache.m_bValid) {
		ASSERT(FALSE);
		builder->NotifyErr(IDS_EN_PICT_INVALID_PICT);
		return;
	}

	LBuffer& data = const_cast<LBuffer&>(m_cache.m_data);
	CPoint center = GetCenter();
	CSize  size   = GetBitmapSize();
	int length    = m_cache.m_nDataLen;
	LPVOID pointer;
	ASSERT(length == data.GetSize());
	ASSERT(length <= 0xFFFF);

	switch(builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (!(is_between(size.cx,PICT_MIN_WIDTH, PICT_MAX_WIDTH ) &&
		      is_between(size.cy,PICT_MIN_HEIGHT,PICT_MAX_HEIGHT))) {
			ASSERT(FALSE);
			builder->NotifyErr(IDS_EN_PICT_INVALID_SIZE,size.cx,size.cy);
		}

		if (!(is_between(center.x,0,size.cx) && 
			  is_between(center.y,0,size.cy))) {
			ASSERT(FALSE);
			builder->NotifyErr(IDS_EN_PICT_INVALID_CENTER);
		}

		if (!is_between(length,PICT_DATA_MIN_LENGTH,PICT_DATA_MAX_LENGTH)) { 
			ASSERT(FALSE);
			builder->NotifyErr(IDS_EN_PICT_DATA_OVER_SIZE);
		}
		break;
	case Builder::BUILDSTEP_LAST:
		ASSERT(m_cache.m_bValid);
		pointer = GlobalLock(data.GetHandle());
		ASSERT(0 < length);
		ASSERT(AfxIsValidAddress(pointer,length,TRUE));
		builder->WriteChar(this,CHARID_PICT);	
		builder->WriteXY(center.x,center.y);
		builder->WriteBuffer(pointer,length);
		GlobalUnlock(data.GetHandle());
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}


BOOL PictChar::Data2Bmp(const LBuffer& buf,LBitmapBase& bmp) {
	FILEINFO fi;
	ZeroMemory(&fi,sizeof fi);
	LMemoryFile file;
	if (file.GetInfo(const_cast<LBuffer&>(buf),&fi) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		return FALSE;
	}

	switch(fi.Format) {
	case FILE_PNG:
	case FILE_JFIF:
	case FILE_LEAD1JFIF:
	case FILE_LEAD2JFIF:
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	};

	file.SetBitmap(&bmp);
	if (file.LoadBitmap(const_cast<LBuffer&>(buf),24,ORDER_BGR) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;	
		return FALSE;
	}
	ASSERT(bmp.GetColorOrder() == ORDER_BGR);
	return TRUE;
}

void PictChar::DrawPreview(const CharPreview* preview) const {
	const CRect& rc = preview->rect;
	preview->dc->FillSolidRect(rc,preview->bkgndColor);

	if (!m_cache.m_bValid) {
		ASSERT(FALSE);
		return;
	}

	CSize szBmp  = GetBitmapSize();
	CSize szDraw = szBmp;
	const int nRectW = rc.Width();
	const int nRectH = rc.Height();
	if (nRectW < szDraw.cx || nRectH < szDraw.cy) {
		if (nRectW * szBmp.cy > szBmp.cx * nRectH) {
			szDraw.cx = nRectH * szBmp.cx / szBmp.cy;
			szDraw.cy = nRectH;
		} else {
			szDraw.cx = nRectW;
			szDraw.cy = nRectW * szBmp.cy / szBmp.cx;
		}
	}

	LPaintTransparent(*preview->dc,
						rc.left + (nRectW -szDraw.cx)/2,
						rc.top  + (nRectH -szDraw.cy)/2,
						szDraw.cx,
						szDraw.cy,
						m_cache.m_bmp,
						0,
						0,
						szBmp.cx,
						szBmp.cy,
						m_cache.m_bTrans,
						m_cache.m_crTrans);
} 

void PictChar::Draw(CDC* pDC,const AffineMatrix* matrix,const Coloring* coloring) const {
	if (!m_cache.m_bValid) {
		ASSERT(FALSE);
		return;
	}

	/*
	AffineMatrix m;
	m.x = matrix->x;
	m.y = matrix->y;
	CSize  szBmp = GetBitmapSize();
	FPoint fptCent = m * GetCenter() - m * CPoint(0,0);
	fptCent.x = m.x - fptCent.x;
	fptCent.y = m.y - fptCent.y;
	CPoint ptDst = fptCent;
	LPaintTransparent(*pDC,
						ptDst.x,
						ptDst.y,
						szBmp.cx,
						szBmp.cy,
						m_cache.m_bmp,
						0,
						0,
						szBmp.cx,
						szBmp.cy,
						m_cache.m_bTrans,
						m_cache.m_crTrans);
	*/
	AffineMatrix m;
	m.x = matrix->x;
	m.y = matrix->y;
	CSize  szBmp   = GetBitmapSize();
	FPoint fptCent = m * GetCenter() - m * CPoint(0,0);
	fptCent.x = m.x - fptCent.x;
	fptCent.y = m.y - fptCent.y;
	CPoint ptDst = fptCent;
	LPaintTransparent(*pDC,
						ptDst.x,
						ptDst.y,
						szBmp.cx,
						szBmp.cy,
						m_cache.m_bmp,
						0,
						0,
						szBmp.cx,
						szBmp.cy,
						m_cache.m_bTrans,
						m_cache.m_crTrans);
}

void PictChar::DrawEditview(CDC* pDC) const{
	const LBitmapBase& cbmp = m_quantize.m_quantize == PICT_QUANTIZE_PATHTHROUGH ?
		                      m_cache.m_bmp : 
	                          m_data.m_bmp;
	LBitmapBase& bmp = const_cast<LBitmapBase&>(cbmp);

	CRect rc(0,0,bmp.GetWidth(),bmp.GetHeight());
	bmp.SetSrcRect(&rc);
	bmp.SetClipSrcRect(NULL);
	bmp.SetDstRect(&rc);
	bmp.SetClipDstRect(NULL);

	LPaint paintDC(&bmp,*pDC);
	paintDC.PaintDC();
}

CString PictChar::GetInfo() const {
	CString type;
	if (m_quantize.m_quantize == PICT_QUANTIZE_PATHTHROUGH) {
		if (m_cache.m_nFormat == PICTOPTOUTFORMAT_JPG) {
			type.LoadString(IDS_PICT_TYPE_PTJPG);
		} else if (m_cache.m_nFormat == PICTOPTOUTFORMAT_PNG) {
			type.LoadString(IDS_PICT_TYPE_PTPNG);
		} else {
			type = _T("-");
			ASSERT(FALSE);
		}
	} else if (m_quantize.m_quantize == PICT_QUANTIZE_PNG) {
		type.LoadString(IDS_PICT_TYPE_PNG);
	} else if (m_quantize.m_quantize == PICT_QUANTIZE_JPG) {
		type.LoadString(IDS_PICT_TYPE_JPG);
	} else {
		type = _T("-");
		ASSERT(FALSE);
	}

	CSize sz = GetBitmapSize();
	CString size;
	size.Format("(%d x %d)",sz.cx,sz.cy);

	return type + " " + size;
}

BOOL FindExecutable(LPCTSTR ptszFile,CString& strExePath) {
	BOOL bFindExe = 32 < (int) FindExecutable(ptszFile,NULL,strExePath.GetBuffer(MAX_PATH*10));
	strExePath.ReleaseBuffer();

	if (!bFindExe) {
		TRACE("GetLastError = %d\n",GetLastError());
		AfxMessageBox(IDS_EN_CAN_NOT_GET_EDIT_EXE_PATH,MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (strExePath.IsEmpty() || !ExistPath(strExePath)) {
		AfxMessageBox(IDS_EN_EMPTY_EDIT_EXE_PATH,MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;
}

/**
*	外部編集ツールで画像を編集する
*
*	@param	data	画像データ
*	@return	成功(true),失敗(false)
*/
BOOL PictChar::EditByExe(PictData& data) {
	CString tempDir  = CreateTempDirectory("bmp");
	CString tempFile = CatPath(tempDir,"temp.bmp");
	LPCTSTR ptszTemp = tempFile;
	TRACE("PictChar::EditByExe=%s\n",ptszTemp);
	ASSERT(!ExistPath(ptszTemp));

	BOOL result = FALSE;
	LFile file;
	CString str;
	LPCTSTR ptszCmdLine;
	CString strExePath;
	STARTUPINFO si = {sizeof si};

	file.SetBitmap(&data.m_bmp);
	file.SetFileName(const_cast<LPTSTR>(ptszTemp));
	if (file.SaveBitmap(FILE_BMP) != SUCCESS) {
		L_DISPLAY_ERROR_LIST;
		goto exit;
	}

	if (!FindExecutable(ptszTemp,strExePath)) {
		ASSERT(FALSE);
		goto exit;
	}

	::GetStartupInfo(&si);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi,sizeof pi);
	
	
	str.Format(_T("\"%s\" \"%s\""),strExePath,ptszTemp);
	ptszCmdLine = str;
	TRACE("%s",ptszCmdLine);
	if (!::CreateProcess(NULL,const_cast<LPTSTR>(ptszCmdLine),NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) {
		TRACE("GetLastError = %d\n",GetLastError());
		AfxMessageBox(IDS_EN_INVALID_EDIT_EXE_PATH,MB_OK | MB_ICONEXCLAMATION);
		goto exit;
	}
	VERIFY(::CloseHandle(pi.hThread));
	for(;;) {
		MSG msg;

		if (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE) &&
			GetMessage(&msg, NULL, WM_PAINT, WM_PAINT)) {
			::DispatchMessage(&msg);
		}

		if (::WaitForSingleObject(pi.hProcess,0) != WAIT_TIMEOUT) {
			break;
		}
	}
	VERIFY(::CloseHandle(pi.hProcess));

	if (!LoadBmp(ptszTemp,data)) {
		ASSERT(FALSE);
		goto exit;
	}
	result = TRUE;
exit:
	VERIFY(::DeleteFile(tempFile));
	VERIFY(::DeleteFile(tempDir));
	return result;
}

namespace PictCharImpl {

	int GetColorBits(int nColors) {
		ASSERT(1 < nColors && nColors <= 256);
		if (nColors == 2)
			return 1;
		else if ( 2 < nColors && nColors <=  4)
			return 2;
		else if ( 4 < nColors && nColors <=  8)
			return 3;
		else if ( 8 < nColors && nColors <= 16)
			return 4;
		else if (16 < nColors && nColors <= 32)
			return 5;
		else if (32 < nColors && nColors <= 64)
			return 6;
		else if (64 < nColors && nColors <= 128)
			return 7;
		else {
			ASSERT(128 < nColors && nColors <= 256);
			return 8;
		}
	}
	
	BOOL is_between(int n,int min,int max) {
		return min <= n && n <= max;
	}

	BOOL IsLeadJpeg(int n) {
		return n == FILE_JFIF ||
			   n == FILE_LEAD1JFIF ||
			   n == FILE_LEAD2JFIF;
	}

	BOOL ExistPath(LPCTSTR path) {
		ASSERT(AfxIsValidString(path));
		if (path == NULL) {
			ASSERT(FALSE);
			return FALSE;
		}

		// 0xFFFFFFFF == Invalid
		return (::GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}

	int NormalizeColorBitsPNG(int bits) {
		switch(bits) {
		case 1:
			return 1;
		case 2:
		case 3:
		case 4:
			return 4;
		case 5:
		case 6:
		case 7:
		case 8:
			return 8;
		default:
			ASSERT(FALSE);
			return 8;
		}
	}
}

void PictChar::GetRect(CRect& rc) const {
	rc = CRect(-GetCenter(),GetBitmapSize());
}

