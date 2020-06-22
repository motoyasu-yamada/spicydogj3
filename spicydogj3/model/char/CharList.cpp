#include "stdafx.h"
#include "resource.h"
#include "CharList.h"
#include "cmd/CmdSet.h"
#include "cmd/char/InsCharCmd.h"
#include "model/char/PictChar.h"
#include "model/char/SndChar.h"
#include "model/char/TextChar.h"
#include "model/build/Builder.h"
#include "SpicydogDoc.h"
#include "utility/FileUtility.h"
#include "utility/MemoryUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CharList::CharList() {
}

void CharList::AssertValid()  const {
	CObject::AssertValid();
	ASSERT_VALID(&m_charList);
}

void CharList::Serialize(SDOMArchive* p) {
	m_charList.Serialize(p);
}

int CharList::GetCharCount() const {
	return m_charList.GetCount();
}

const Char* CharList::GetChar(int n) const {
	return m_charList.GetObject(n);
}

Char* CharList::GetChar(int n) {
	return m_charList.GetObject(n);
}

BOOL CharList::ExistChar(const Char* pChar) const {
	return std::find(m_charList.Begin(),m_charList.End(),pChar) != m_charList.End();
}


int CharList::GetCharID(const Char* pFind,BOOL bAllowNull) const {
	if (bAllowNull && pFind == NULL) {
		return 0;
	}

	VERIFY(ExistChar(pFind));

	int nID = 1;
	CHAROWNERS::ConstItr i = m_charList.Begin();
	while (i != m_charList.End()) {
		const Char* p = CHAROWNERS::I2P(i);
		if (p == pFind) {
			return nID;
		}
		i++;
		nID++;
	}
	ASSERT(FALSE);
	return 0;
}

void CharList::Build(Builder* builder) const {
	ASSERT(AfxIsValidAddress(this,sizeof CharList));
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	int charListPos;
	if (builder->GetBuildStep() == Builder::BUILDSTEP_LAST) {
		charListPos = builder->GetWritingPos();
		builder->WriteUI16(0);
	}

	int writtenChar = 0;
	CHAROWNERS::ConstItr i;
	for (i  = m_charList.Begin();
		 i != m_charList.End();
		 i++) {
		const Char* p = CHAROWNERS::I2P(i);

		if (builder->GetBuildStep() == Builder::BUILDSTEP_FIRST) {
			builder->Build(p);
		} else {
			if (0 < builder->GetCharUseCount(p)) {
				builder->Build(p);
				writtenChar++;
			}
		}
	}

	if (builder->GetBuildStep() == Builder::BUILDSTEP_LAST) {
		int savedPos = builder->SetWritingPos(charListPos);
		//TRACE("CharNum=%d\n",writtenChar);
		builder->WriteUI16(writtenChar);
		builder->SetWritingPos(savedPos);
	}
}

HGLOBAL CharList::Data2Ole_CF_CHARS(SpicydogDoc* document,const std::list<Char*>& chars) const {

	GlobalAllocator a(sizeof CFS_CHARS + sizeof(Char*) * chars.size(),
		                 GMEM_FIXED | GMEM_SHARE);
	{
		TGlobalAllocatorTypedPtr<CFS_CHARS> cf(a);
		ASSERT(AfxIsValidAddress(cf,sizeof(CFS_CHARS)));

		cf->processID = GetCurrentProcessId();
		cf->document  = document;
		cf->charCount = chars.size();
		
		std::list<Char*>::const_iterator i;
		int n = 0;
		for (i = chars.begin();i != chars.end();i++) {
			cf->chars[n++] = *i;
		}
	}

	return a.Detach();
}

BOOL CharList::OleIsSupported(COleDataObject* pobj) const {
	if (pobj->IsDataAvailable(CF_TEXT))
		return TRUE;
	if (pobj->IsDataAvailable(CF_DIB))
		return TRUE;
	if (pobj->IsDataAvailable(CF_BITMAP))
		return TRUE;
	if (pobj->IsDataAvailable(CF_HDROP))
		return TRUE;
	return FALSE;
}

BOOL CharList::AcceptOle(SpicydogDoc* document,COleDataObject* pObj,BOOL bDnD,DROPEFFECT de,CPoint pt) {
	
	if (bDnD && de != DROPEFFECT_COPY)
		return FALSE;

	Char* pDst = NULL;
	if (pObj->IsDataAvailable(CF_TEXT)) {
		pDst = Ole2Data_CF_TEXT(pObj->GetGlobalData(CF_TEXT),pObj);
	} else if (pObj->IsDataAvailable(CF_DIB)) {
		pDst = Ole2Data_CF_DIB(pObj->GetGlobalData(CF_DIB),pObj);
		pDst->SetName(document->charNamingRule.NewIncNumName(IDS_PASTE_PICT_CHAR_NAME_FORMAT));	
	} else if (pObj->IsDataAvailable(CF_BITMAP)) {
		pDst = Ole2Data_CF_BITMAP(pObj->GetGlobalData(CF_BITMAP),pObj);
		pDst->SetName(document->charNamingRule.NewIncNumName(IDS_PASTE_PICT_CHAR_NAME_FORMAT));	
	}
	// else if (pObj->IsDataAvailable(CF_HDROP)) {
	//	pDst = Ole2Data_CF_HDROP(pObj->GetGlobalData(CF_HDROP),pObj);
	//	pDst->SetName(document->charNamingRule.NewIncNumName(IDS_PASTE_CHAR_NAME_FORMAT));	
	//}

	if (pDst == NULL) {
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT_VALID(pDst);
	document->DoCmd(new InsCharCmd(this,-1,pDst));
	document->UpdateAll(NULL);			

	return TRUE;
}

Char* CharList::Ole2Data_CF_TEXT(HGLOBAL hg,COleDataObject*) const
{
	GlobalAllocatorPtr p(hg);
	LPCTSTR ptsz = (LPCTSTR)(LPVOID)(p);
	if (!AfxIsValidString(ptsz))
		return NULL;
	
	return new TextChar(ptsz);
}

DWORD GetColorCountFromBitmapInfoHeader (const BITMAPINFOHEADER& bmih) {
    //  With the BITMAPINFO format headers, the size of the palette
    //  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
    //  is dependent on the bits per pixel ( = 2 raised to the power of
    //  bits/pixel).
    if (bmih.biSize == sizeof(BITMAPINFOHEADER) &&
		bmih.biClrUsed != 0)
	   return bmih.biClrUsed;

    switch (bmih.biBitCount){
		case 1:
			return 2;
		case 4:
			return 16;
		case 8:
			return 256;
        case 16:
		case 24:
		case 32:
				return 0;
		default:
			ASSERT(FALSE);
			return 0;
    }
}

DWORD GetPaletteSizeFromBitmapInfoHeader(const BITMAPINFOHEADER& bmih) {
	DWORD dwNumColors = GetColorCountFromBitmapInfoHeader(bmih);
	_ASSERT(0 <= dwNumColors && dwNumColors <= 256);

    if (bmih.biSize != sizeof(BITMAPCOREHEADER)) {
        return dwNumColors * sizeof(RGBQUAD);

	} else
        return dwNumColors * sizeof(RGBTRIPLE);
}

HBITMAP CreateDIBSectionFromDIB(HANDLE hdibSrc) {
	if (hdibSrc == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	LPBITMAPINFO pbi = static_cast<LPBITMAPINFO>(::GlobalLock(hdibSrc));
	if (pbi == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	HDC hdc = NULL;
	HBITMAP hbmpDst = NULL;
	TRY
	{
		hdc = ::GetDC(NULL);
		if (hdc == NULL)
			AfxThrowMemoryException();
		
		LPVOID pv = NULL;
		hbmpDst = ::CreateDIBSection(hdc,pbi,DIB_RGB_COLORS,&pv,0,0);
		if (hbmpDst == NULL)
			AfxThrowMemoryException();
		
		CopyMemory(pv,
				   reinterpret_cast<LPBYTE>(pbi) + pbi->bmiHeader.biSize + GetPaletteSizeFromBitmapInfoHeader(pbi->bmiHeader),
				   pbi->bmiHeader.biSizeImage);
	}
	CATCH(CMemoryException,e)
	{
		ASSERT(FALSE);
		DEBUG_ONLY(e->ReportError());
		if (hbmpDst != NULL)
			::DeleteObject(hbmpDst);
	}
	END_CATCH

	if (hdc != NULL)
		::ReleaseDC(NULL,hdc);

	::GlobalUnlock(pbi);
	return hbmpDst;
}

Char* CharList::Ole2Data_CF_DIB(HGLOBAL hg,COleDataObject*) const {
	HBITMAP hbmp = CreateDIBSectionFromDIB(hg);
	if (hbmp == NULL) {
		ASSERT(FALSE);
		return NULL;
	}
	return PictChar::newPaste(hbmp,NULL);
}

Char* CharList::Ole2Data_CF_BITMAP(HGLOBAL hg,COleDataObject* pObj) const {
	HPALETTE hp = NULL;
	if (pObj->IsDataAvailable(CF_PALETTE)) {
		hp = (HPALETTE) pObj->GetGlobalData(CF_PALETTE);
	}
	return PictChar::newPaste((HBITMAP)hg,hp);
}

Char* CharList::Ole2Data_CF_HDROP(HGLOBAL h,COleDataObject*) const {
	HDROP hdrop = (HDROP)h;
	int nFiles  = DragQueryFile(hdrop,0xFFFFFFFF,NULL,0);
	for (int n = 0;n < nFiles;n++)
	{
		CString str;
		UINT nLen = DragQueryFile(hdrop,n,NULL,0) + (sizeof TCHAR) * 2;	
		DragQueryFile(hdrop,n,str.GetBuffer(nLen),nLen);
		str.ReleaseBuffer();

		CString strExt = GetFileExt(str);
		if (strExt.CompareNoCase("BMP") == 0 ||
			strExt.CompareNoCase("GIF") == 0 ||
			strExt.CompareNoCase("PNG") == 0 ||
			strExt.CompareNoCase("JPG") == 0 ||
			strExt.CompareNoCase("JPEG")) {
			return PictChar::newImport(str);
		} else if (strExt.CompareNoCase("MLD") == 0) {
			CFile file(str,CFile::modeRead | CFile::shareDenyNone);
			GlobalAllocator alloc;
			alloc.Alloc(file.GetLength());
			GlobalAllocatorPtr ptr(alloc);
			file.Read(ptr,file.GetLength());
			ptr.Unlock();

			return new SndChar(alloc.Detach());
		}
	}
	return NULL;
}
