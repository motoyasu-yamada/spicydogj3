#include "stdafx.h"
#include "resource.h"
#include "act.h"

#include "sdom/sdomn.h"
#include "sdom/SDOMArchive.h"
#include <afxtempl.h>
#include <algorithm>
#include <vector>
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

std::auto_ptr<CDefaultActs> CDefaultActs::s_pacts;

Act::Act()
:	m_nCount(0)
{
	Init();
}

Act::Act(const Act& src) {
	CopyFrom(src);
}

Act& Act::operator = (const Act& src) {
	CopyFrom(src);
	return *this;
}

void Act::CopyFrom(const Act& src) {
	m_nCount = src.m_nCount;
	CopyMemory(&m_Entries,src.m_Entries,sizeof m_Entries);
}

void Act::Init()
{
	m_nCount = 0;
	ZeroMemory(&m_Entries,sizeof m_Entries);
}

BOOL Act::Find(COLORREF cr) const
{
	return FindIndex(cr) != -1;
}

int Act::FindIndex(COLORREF cr) const
{
	for (int n = 0;n < m_nCount;n++)
	{
		const PALETTEENTRY& pe = m_Entries[n];
		COLORREF cr2 = RGB(pe.peRed,pe.peGreen,pe.peBlue);
		if (cr2 == cr)
			return n;
	}
	return -1;
}

BOOL Act::Load(CFile& file)
{
	VERIFY(&file);
	try
	{
		int nFileLen = file.GetLength();
		if (nFileLen % 3 != 0)
			return FALSE;

		int nMaxCount =  nFileLen / 3;
		if (256 < nMaxCount)
		{
			ASSERT(FALSE);
			nMaxCount = 256;
		}
		for (m_nCount = 0 ;m_nCount < nMaxCount;m_nCount++)
		{
			ASSERT(file.GetPosition() < file.GetLength());
			BYTE r;
			BYTE g;
			BYTE b;
			VERIFY(file.Read(&r,sizeof r) == sizeof r);
			VERIFY(file.Read(&g,sizeof g) == sizeof g);
			VERIFY(file.Read(&b,sizeof b) == sizeof b);

			COLORREF cr = RGB(r,g,b);
			//TRACE("[%03d]=#%06X\n",m_nCount,cr);
			if (Find(cr))
				break;
			
			PALETTEENTRY& pe = m_Entries[m_nCount];
			pe.peRed = r;
			pe.peBlue = b;
			pe.peGreen = g;
			pe.peFlags = 0;
		}
		ASSERT(m_nCount <= 256);
		return TRUE;
	}
	catch(CException* pe)
	{
		DEBUG_ONLY(pe->ReportError());
		ASSERT(FALSE);
		pe->Delete();
		return FALSE;
	}
}

BOOL Act::LoadResource(HMODULE hm,UINT n)
{
	Init();

	HRSRC hs = ::FindResource(hm,MAKEINTRESOURCE(n),_T("ACT"));
	if (hs == NULL) {
		TRACE("GetLastError() %d \n",GetLastError());
		return FALSE;
	}

	DWORD dwSize = ::SizeofResource(hm,hs);
	ASSERT(0 < dwSize);
	HGLOBAL hg = ::LoadResource(hm,hs);
	if (hg == NULL)
		return FALSE;

	LPVOID pv = ::LockResource(hg);
	ASSERT(AfxIsValidAddress(pv,dwSize,FALSE));
	if (pv == NULL)
		return FALSE;
	
	CMemFile file((LPBYTE)pv,dwSize,0);
	return Load(file);
}

BOOL Act::LoadFile(LPCTSTR ptsz)
{
	Init();

	CFile file;
	if (!file.Open(ptsz,CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	return Load(file);
}

void Act::CreateFromPalette(HPALETTE h,int nCount)
{
	ASSERT(0 < nCount && nCount <= 256);
	m_nCount = ::GetPaletteEntries(h,0,nCount,m_Entries);
}

struct COLORUSE
{
	int m_nUseCount;
	PALETTEENTRY m_pe;
	BOOL operator < (const COLORUSE&) const;
};

BOOL COLORUSE::operator < (const COLORUSE& rhs) const
{
	return rhs.m_nUseCount < m_nUseCount;
}

HPALETTE Act::CreatePalette() const {
	CArray<BYTE,BYTE&> buf;
	buf.SetSize(sizeof(LOGPALETTE)+(256-1) * sizeof(PALETTEENTRY));
	LOGPALETTE& lpal = (LOGPALETTE&)(*buf.GetData());         
	lpal.palVersion = 0x300;
	lpal.palNumEntries = 256;
	for (int n = 0;n < 256;n++) {
		PALETTEENTRY& pe = lpal.palPalEntry[n];
		pe = m_Entries[n];
	}
	
	HPALETTE hpal = ::CreatePalette(&lpal);
	if (hpal == NULL)
		AfxThrowResourceException();
	return hpal;
}

#if !defined(ACT_NOUSE_LEADTOOL)
void Act::SorByBitmap(const LBitmapBase& _bmp,BOOL bTrans,COLORREF crTrans)
{
	CWaitCursor wc;

	LBitmapBase& bmp = const_cast<LBitmapBase&>(_bmp);
	ASSERT(bmp.GetBitsPerPixel() == 24);
	ASSERT(bmp.GetColorOrder() == ORDER_BGR);

	CPalette pal;
	pal.Attach(CreatePalette());
	std::vector<COLORUSE> cus;
	cus.resize(256);
	for (int n = 0;n < 256;n++)
	{
		COLORUSE& cu = cus[n];
		cu.m_nUseCount = 0;
		cu.m_pe = m_Entries[n];
	}
	bmp.Access();
	
	int nRows = bmp.GetHeight();
	int nCols = bmp.GetWidth();
	int nBytesPerLine = bmp.GetBytesPerLine();
	for (int nRow = 0;nRow < nRows;nRow++)
	{
		LBuffer buffer;
		bmp.GetRow(&buffer,nRow) ;
		if(buffer.GetSize())
		{
			BYTE* pLineBuff=(BYTE*)buffer.Lock();
			ASSERT(AfxIsValidAddress(pLineBuff,nBytesPerLine));
			for(int nCol = 0;nCol < nCols;nCol++)
			{
				BYTE bB = *pLineBuff++;
				BYTE bG = *pLineBuff++;
 				BYTE bR = *pLineBuff++;
				COLORREF cr = RGB(bR,bG,bB);
				int nPalIndex = pal.GetNearestPaletteIndex(cr);
				COLORUSE& cu = cus[nPalIndex];
				cu.m_nUseCount ++;
			}
			buffer.Unlock();
		}
	}
	bmp.Release();

	std::vector<COLORUSE>::iterator iBegin = cus.begin();
	if (bTrans)
	{
		ASSERT(0x000000 <= crTrans && crTrans <= 0xFFFFFF);
		int nPalIndex = pal.GetNearestPaletteIndex(crTrans);
		ASSERT(0 <= nPalIndex && nPalIndex < 256);
		COLORUSE& cu = cus[nPalIndex];
		cu.m_nUseCount = INT_MAX;
	}

	std::sort(iBegin,cus.end());
	{
		ASSERT(m_nCount <= cus.size());
		for (int n = 0;n < 256;n++)
		{
			COLORUSE& cu = cus[n];
			ASSERT(AfxIsValidAddress(&cu,sizeof cu));
			ASSERT(AfxIsValidAddress(&m_Entries[n],sizeof PALETTEENTRY));
			m_Entries[n] = cu.m_pe;
		}	
	}
}
#endif

void Act::SetPaletteEntry(PALETTEENTRY& pe,COLORREF cr)
{
	ASSERT(AfxIsValidAddress(&pe,sizeof pe));
	pe.peRed = GetRValue(cr);
	pe.peGreen = GetGValue(cr);
	pe.peBlue = GetBValue(cr);
	pe.peFlags  = 0;
}

#if !defined(ACT_NOUSE_LEADTOOL)
void Act::CreateFromBitmap(const LBitmapBase& _bmp,BOOL b,COLORREF cr) {
	CWaitCursor wc;
	
//#define USE_CQL
#ifdef USE_CQL
	VERIFY(cqlCreatePalette(_bmp,m_Entries,256,b,cr));
	m_nCount = 256;
#else
	LBitmapBase& bmp = const_cast<LBitmapBase&>(_bmp);
	ASSERT(bmp.GetBitsPerPixel() == 24);
	ASSERT(bmp.GetColorOrder() == ORDER_BGR);

	std::map<COLORREF,int> colorcount;
	{
		bmp.Access();
		
		int nRows = bmp.GetHeight();
		int nCols = bmp.GetWidth();
		int nBytesPerLine = bmp.GetBytesPerLine();
		for (int nRow = 0;nRow < nRows;nRow++)
		{
			LBuffer buffer;
			bmp.GetRow(&buffer,nRow) ;
			if(buffer.GetSize())
			{
				BYTE* pLineBuff=(BYTE*)buffer.Lock();
				ASSERT(AfxIsValidAddress(pLineBuff,nBytesPerLine));
				for(int nCol = 0;nCol < nCols;nCol++)
				{
					BYTE bB = (*pLineBuff++) & 0xF0;
					BYTE bG = (*pLineBuff++) & 0xF0;
 					BYTE bR = (*pLineBuff++) & 0xF0;
					COLORREF cr = RGB(bR,bG,bB);
					std::map<COLORREF,int>::iterator i = colorcount.find(cr);
					if (i == colorcount.end())
						colorcount.insert(std::map<COLORREF,int>::value_type(cr,0));
					else
						i->second = i->second + 1;
				}
				buffer.Unlock();
			}
		}
		bmp.Release();
	}
	
	std::list<COLORUSE> coloruse;
	{
		std::map<COLORREF,int>::const_iterator i;
		for (i = colorcount.begin();i != colorcount.end();i++)
		{
			COLORUSE cu;
			SetPaletteEntry(cu.m_pe,i->first); 
			cu.m_nUseCount = i->second;
			coloruse.push_back(cu);
		}
		coloruse.sort();
	}

	{
		m_nCount = coloruse.size();
		if (256 < m_nCount)
			m_nCount = 256;	

		std::list<COLORUSE>::const_iterator i = coloruse.begin();
		for (int n = 0;n < m_nCount;n++)
		{
			const COLORUSE& cu = *i++;
			PALETTEENTRY& pe = m_Entries[n];
			pe = cu.m_pe;
		}
	}
#endif
}
#endif

void Act::Empty() {
	Init();
}

int Act::GetCount() const
{
	ASSERT(0 <= m_nCount && m_nCount <= 256);
	return m_nCount;
}

PALETTEENTRY& Act::operator [] (int n)
{
	ASSERT(0 <= n && n < m_nCount);
	return m_Entries[n];
}

const PALETTEENTRY& Act::operator [] (int n) const
{
	ASSERT(0 <= n && n < m_nCount);
	return m_Entries[n];
}

COLORREF Act::GetColor(int n) const
{
	ASSERT(0 <= n && n < m_nCount);
	const PALETTEENTRY& pe = m_Entries[n];
	return RGB(pe.peRed,pe.peGreen,pe.peBlue);
}

ACTTYPE CAST_ACTTYPE(int n) {
	if (n == ACTTYPE_GRAY ||
		n == ACTTYPE_IMODE   ||
		n == ACTTYPE_MAC     ||
		n == ACTTYPE_MONO    ||
		n == ACTTYPE_RGB     ||
		n == ACTTYPE_WEB     ||
		n == ACTTYPE_WIN     ||
		n == ACTTYPE_CUSTOM) {
		return (ACTTYPE)n;
	}
	return ACTTYPE_IMODE;
}

void Act::Serialize(SDOMArchive* ar) {
	if (ar->IsLoading()) {
		int n = 0;
		while (ar->HasNextElement()) {
			PALETTEENTRY& entry = m_Entries[n++];
			COLORREF cr;
			ar->SerializeCOLORREF(WSDOMN_PALETTE_ENTRY,cr);
			entry.peRed   = GetRValue(cr);
			entry.peGreen = GetGValue(cr);
			entry.peBlue  = GetBValue(cr);
		}
		m_nCount = n;

	} else {
		ASSERT(ar->IsStoring());
		for (int n = 0;n < m_nCount;n++) {
			const PALETTEENTRY& entry = m_Entries[n++];
			COLORREF cr;
			cr = RGB(entry.peRed,entry.peGreen,entry.peBlue);
			ar->SerializeCOLORREF(WSDOMN_PALETTE_ENTRY,cr);
		}
	}
}

void Act::AssertValid() {
	ASSERT(0 <= m_nCount && m_nCount < 256);
}

CDefaultActs::CDefaultActs() {
	Load(ACTTYPE_GRAY,IDACT_GRAY);
	Load(ACTTYPE_IMODE,IDACT_IMODE);
	Load(ACTTYPE_MAC,IDACT_MAC);
	Load(ACTTYPE_MONO,IDACT_MONO);
	Load(ACTTYPE_RGB,IDACT_RGB);
	Load(ACTTYPE_WEB,IDACT_WEB);
	Load(ACTTYPE_WIN,IDACT_WIN);
}

void CDefaultActs::Load(ACTTYPE at,UINT res) {
	Act* p = new Act;
	VERIFY(p->LoadResource(::AfxGetResourceHandle(),res));
	ASSERT(0 < p->GetCount());
	m_acts.insert(ACTS::value_type(at,ACTPTR(p)));
	ASSERT(0 < m_acts[at]->GetCount());
	CString string;
	VERIFY(string.LoadString(res));
	m_actnames[at] = string;
}

const CDefaultActs* CDefaultActs::GetDefaultActs() {
	if (s_pacts.get() == NULL)
		s_pacts = std::auto_ptr<CDefaultActs>(new CDefaultActs);

	return s_pacts.get();
}

const Act* CDefaultActs::GetAct(ACTTYPE at) const {
	ASSERT(0 <= at && at < ACTTYPE_CUSTOM);
	ACTS::const_iterator i = m_acts.find(at);
	ASSERT(i != m_acts.end());
	return (i->second).get();
}

CString CDefaultActs::GetActName(ACTTYPE at) const
{
	ASSERT(0 <= at && at < ACTTYPE_CUSTOM);
	ACTNAMES::const_iterator i = m_actnames.find(at);
	ASSERT(i != m_actnames.end());
	return i->second;
}

#if !defined(ACT_NOUSE_LEADTOOL)
BOOL LoadBitmapPictInfo(LPCTSTR ptszPath,PICTINFO& pi) {
	FILEINFO fi;
	ZeroMemory(&fi,sizeof fi);
	LFile file;
	file.SetFileName(const_cast<LPTSTR>(ptszPath));
	if (file.GetInfo(&fi) != SUCCESS)
		return FALSE;

	LBitmap bmp;
	file.SetBitmap(&bmp);
	file.SetFileName(const_cast<LPTSTR>(ptszPath));
	if (file.LoadBitmap(fi.BitsPerPixel,ORDER_BGR) != SUCCESS)
	{
		LSettings s;
		s.DisplayErrorList(AfxGetMainWnd()->GetSafeHwnd());
		return FALSE;
	}

	return LoadBitmapPictInfo(bmp,pi,fi);
}

BOOL LoadBitmapPictInfo(LBuffer& buffer,PICTINFO& pi) {
	FILEINFO fi;
	ZeroMemory(&fi,sizeof fi);
	LMemoryFile file;
	if (file.GetInfo(buffer,&fi) != SUCCESS)
		return FALSE;

	LBitmap bmp;
	file.SetBitmap(&bmp);
	if (file.LoadBitmap(buffer,fi.BitsPerPixel,ORDER_BGR) != SUCCESS)
	{
		LSettings s;
		s.DisplayErrorList(AfxGetMainWnd()->GetSafeHwnd());
		return FALSE;
	}

	return LoadBitmapPictInfo(bmp,pi,fi);
}

/**
	画像ファイル情報をファイルから取得
*/
BOOL LoadBitmapPictInfo(LBitmap& bmp,PICTINFO& pi,const FILEINFO& fi) {
	pi.bitsPerPixel = fi.BitsPerPixel;

	BITMAPHANDLE* pbmph = bmp.GetHandle();
	ASSERT(AfxIsValidAddress(pbmph,sizeof BITMAPHANDLE));
	if (pi.bitsPerPixel <= 8 && pbmph->hPalette) {
		pi.act.CreateFromPalette(pbmph->hPalette,pbmph->nColors);
		pi.actType = ACTTYPE_CUSTOM;
	} else {
		pi.bitsPerPixel = 8;
		const Act* pAct = CDefaultActs::GetDefaultActs()->GetAct(ACTTYPE_IMODE);
		ASSERT(AfxIsValidAddress(pAct,sizeof Act));
		pi.act = *pAct;
		pi.actType = ACTTYPE_IMODE;
	}

	if (pbmph->Flags.Transparency) {
		pi.transparency= TRUE;
		if (fi.BitsPerPixel <= 8) {
			int nTrans = pbmph->Transparency-0x1000000;
			PALETTEENTRY& pe = pi.act[nTrans];
			pi.transparentColor = RGB(pe.peRed,pe.peGreen,pe.peBlue);
		} else {
			pi.transparentColor = pbmph->Transparency;
		}
	} else {
		pi.transparency = FALSE;
		pi.transparentColor = 0;
	}
	ASSERT(0 <= pi.transparentColor && pi.transparentColor <= 0xFFFFFF);

	return TRUE;
}
#endif
