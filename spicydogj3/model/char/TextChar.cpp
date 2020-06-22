#include "stdafx.h"
#include "resource.h"
#include "TextChar.h"

#include "sdom/sdomn.h"
#include "sdom/SDOMEnum.h"
#include "model/build/Builder.h"
#include "model/AffineMatrix.h"
#include "model/stage/Coloring.h"
#include "utility/StringUtility.h"
#include "view/TextCharView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ENUM_TYPE(FontFaceType);
ENUM_TYPE_ITEM(FontFaceType,TC_FACE_SYSTEM,      ASDOMV_SYSTEM);
ENUM_TYPE_ITEM(FontFaceType,TC_FACE_MONOSPACE,   ASDOMV_MONOSPACE);
ENUM_TYPE_ITEM(FontFaceType,TC_FACE_PROPORTIONAL,ASDOMV_PROPORTIONAL);

ENUM_TYPE(FontSizeType);
ENUM_TYPE_ITEM(FontSizeType,TC_SIZE_SMALL, ASDOMV_SMALL);
ENUM_TYPE_ITEM(FontSizeType,TC_SIZE_MEDIUM,ASDOMV_MEDIUM);
ENUM_TYPE_ITEM(FontSizeType,TC_SIZE_LARGE, ASDOMV_LARGE);

const int MIN_TEXT_LENGTH = 1;
const int MAX_TEXT_LENGTH = 1024;
const int FONT_PIXEL_SIZE[] = {9,12,16};
const UINT FONT_FACE[] = {
	IDS_FONT_FACE_SYSTEM,
	IDS_FONT_FACE_MONOSPACE,
	IDS_FONT_FACE_PROPOTIONAL
};

int CreateFont(CFont&,int face,int size,BOOL bold,BOOL italic,BOOL underline);

class CCharDrawer {
public:
	/// 座標系は論理座標系
	void Draw(CDC* dc,LPCRECT prc,CString str);
	CSize CalcSize(CString str);
	virtual ~CCharDrawer() {}
protected:
	virtual int DrawChar(CDC* dc,int x,int y,CString str,int charsize) = 0;
	virtual int GetFontHeight() const = 0;
	virtual int GetFontWidth() const = 0;
}; // class CCharDrawer

class CNativeCharDrawer : public CCharDrawer{
private:
	CFont  m_font;
	CDC*   m_pDC;
	CFont* m_pFontSaved;
	int m_nFontSize;
public:
	CNativeCharDrawer (CDC* dc,int face,int size,BOOL bold,BOOL italic,BOOL underline);
	~CNativeCharDrawer();
	virtual int DrawChar(CDC* dc,int x,int y,CString str,int charsize);
	virtual int GetFontHeight() const;
	virtual int GetFontWidth() const;
};
void DrawHelper(CDC* dc,const TextCharState& state,int xLP,int yLP,COLORREF cr);

TextCharState::TextCharState() {
	text = "";
	autoResize = true;
	textColor  = RGB(0x00,00,00);
	width      = 12;
	height     = 12;
	fontFace = TC_FACE_SYSTEM;
	fontSize = TC_SIZE_MEDIUM;
	fontStyleBold      = false;
	fontStyleItalic    = false;
	fontStyleUnderline = false;
}

TextCharState& TextCharState::operator= (const TextCharState& state) {
	text       = state.text;
	autoResize = state.autoResize;
	width      = state.width;
	height	   = state.height;
	fontFace   = state.fontFace;
	fontSize   = state.fontSize;
	textColor  = state.textColor;
	fontStyleBold      = state.fontStyleBold;
	fontStyleItalic    = state.fontStyleItalic;
	fontStyleUnderline = state.fontStyleUnderline;
	return *this;
}

CharType TextChar::TEXT_CHAR_TYPE(
	RUNTIME_CLASS(TextCharView),
	"TextChar",
	IDS_CHAR_DISPLAY_NAME_TEXT,
	WSDOMN_TEXT_CHAR,
	3,
	true,
	true,
	false,
	true,
	true,
	false);

TextChar::TextChar() {
}

TextChar::TextChar(const TextChar& right) {
	m_state = right.m_state;
}

TextChar::TextChar(LPCTSTR ptsz) {
	ASSERT(AfxIsValidString(ptsz));
	m_state.text = ptsz;
	SetName(CreateNameByBody(ptsz));
	UpdateAutoSize(&m_state);
}

const CharType& TextChar::GetCharType() const {
	return TEXT_CHAR_TYPE;
}

TextChar::~TextChar() {
}

Char* TextChar::Clone() const {
	return new TextChar(*this);
}

void TextChar::SetState(const TextCharState& state) {
	m_state = state;
	ASSERT_VALID(this);
}

void TextChar::GetState(TextCharState& state) {
	ASSERT_VALID(this);
	state = m_state;
}

void TextChar::Serialize(SDOMArchive* ar) {
	Char::Serialize(ar);
	ar->SerializeString(WSDOMN_TEXT,m_state.text);
	ar->SerializeBOOL(WSDOMN_AUTO_RESIZE,m_state.autoResize);
	ar->SerializeCOLORREF(WSDOMN_TEXT_COLOR,m_state.textColor);
	ar->SerializeINT(WSDOMN_WIDTH,m_state.width);
	ar->SerializeINT(WSDOMN_HEIGHT,m_state.height);
	ar->SerializeType(WSDOMN_FONT_FACE,m_state.fontFace,FontFaceType);
	ar->SerializeType(WSDOMN_FONT_SIZE,m_state.fontSize,FontSizeType);

	ar->SerializeBOOL(WSDOMN_FONT_STYLE_BOLD,m_state.fontStyleBold);
	ar->SerializeBOOL(WSDOMN_FONT_STYLE_ITALIC,m_state.fontStyleItalic);
	ar->SerializeBOOL(WSDOMN_FONT_STYLE_UNDERLINE,m_state.fontStyleUnderline);
}

void TextChar::AssertValid() const {
	ASSERT(AfxIsValidString(m_state.text));
	ASSERT(1 <= m_state.width  && m_state.width  <= MAX_TEXT_WIDTH);
	ASSERT(1 <= m_state.height && m_state.height <= MAX_TEXT_HEIGHT);
	ASSERT(m_state.fontFace == TC_FACE_SYSTEM ||
		   m_state.fontFace == TC_FACE_MONOSPACE ||
		   m_state.fontFace == TC_FACE_PROPORTIONAL);
	ASSERT(m_state.fontSize == TC_SIZE_SMALL ||
		   m_state.fontSize == TC_SIZE_MEDIUM ||
		   m_state.fontSize == TC_SIZE_LARGE);
}

CString TextChar::CreateNameByBody(LPCTSTR _body) {
	ASSERT(AfxIsValidString(_body));
	CString body = _body;
	body.TrimLeft();
	body = body.Left(32);
	int nBreak = body.FindOneOf(_T("\t\r\n"));
	switch (nBreak) {
	case -1:
		if (body.IsEmpty())
			body.LoadString(IDS_TEXTCHAR_NAME_FORMAT);
		break;
	case 0:
		body.LoadString(IDS_TEXTCHAR_NAME_FORMAT);
		break;
	default:
		body = body.Left(nBreak);
		break;
	}
	return body;
}

int TextChar::GetFontStyle() const {
	int style = 0;
	if (m_state.fontStyleBold) {
		style |= 1;
	}
	if (m_state.fontStyleItalic) {
		style |= 2;
	}
	if (m_state.fontStyleUnderline) {
		style |= 4;
	}
	ASSERT(0 <= style && style <= 7);
	return style;
}

/** フォントフェースの内部コードをMIDPコードに変換する */
int ConvertMIDPFontFace(int face) {
	switch(face) {
	case TC_FACE_SYSTEM:
		return 0;
	case TC_FACE_MONOSPACE:
		return 32;
	case TC_FACE_PROPORTIONAL:
		return 64;
	default:
		ASSERT(FALSE);
		return 0;
	}
}

/** フォントサイズの内部コードをMIDPコードに変換する */
int ConvertMIDPFontSize(int size) {
	switch(size) {
	case TC_SIZE_SMALL:
		return 8;
	case TC_SIZE_MEDIUM:
		return 0;
	case TC_SIZE_LARGE:
		return 16;
	default:
		ASSERT(FALSE);
		return 0;
	}
}

void TextChar::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));
	BuildContext context(builder,GetName());

	CString text = m_state.text;
	int length = text.GetLength();

	switch (builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (length < MIN_TEXT_LENGTH || MAX_TEXT_LENGTH < length) {
			builder->NotifyErr(IDS_EN_INVALID_NULL_TEXT);
		}
		break;

	case Builder::BUILDSTEP_LAST:
		builder->WriteChar(this,CHARID_TEXT);
		builder->WriteSJIS(text);
		builder->WriteXY(m_state.width,m_state.height);
		builder->WriteColor(m_state.textColor);
		builder->WriteUI8(ConvertMIDPFontFace(m_state.fontFace));
		builder->WriteUI8(ConvertMIDPFontSize(m_state.fontSize));
		builder->WriteUI8(GetFontStyle());
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}


void TextChar::UpdateAutoSize(TextCharState* state) {
	typedef std::auto_ptr<CCharDrawer> PCHARDRAWER;
	PCHARDRAWER drawer;
	drawer = PCHARDRAWER(new CNativeCharDrawer(
								NULL,
								state->fontFace,
								state->fontSize,
								state->fontStyleBold,
								state->fontStyleItalic,
								state->fontStyleUnderline));
	CSize sz  = drawer->CalcSize(state->text);
	state->width  = sz.cx;
	state->height = sz.cy;
}

void TextChar::DrawPreview(const TextCharState* state,CDC* dc,CRect rcPreview,COLORREF crBkgnd) {
	CPen penDot(PS_DOT,0,RGB(0x00,0x00,0x80));
	CBrush brushBkgnd(crBkgnd);

	dc->FillRect(rcPreview,&brushBkgnd);
	int width  = state->width;
	int height = state->height;
	int x = rcPreview.left + (rcPreview.Width()  - width)  / 2;
	int y = rcPreview.top  + (rcPreview.Height() - height) / 2;
	
	int nSaved = dc->SaveDC();
		CBrush* brush = dc->SelectObject(&brushBkgnd);
		CPen*   pen   = dc->SelectObject(&penDot);
		ASSERT_VALID(brush);
		ASSERT_VALID(pen);
		dc->SetBkMode(TRANSPARENT);
		dc->Rectangle(x,y,x + width,y + height);
	dc->RestoreDC(nSaved);

	DrawHelper(dc,*state,x,y,state->textColor);
}

void TextChar::GetRect(CRect& rc) const {
	rc.left   = 0;
	rc.right  = m_state.width;
	rc.top    = 0;
	rc.bottom = m_state.height;
}

void TextChar::DrawPreview(const CharPreview* preview) const {
	DrawPreview(&m_state,preview->dc,preview->rect,preview->bkgndColor);	
}

void TextChar::Draw(CDC* dc,const AffineMatrix* matrix,const Coloring* coloring) const {
	DrawHelper(dc,m_state,matrix->x,matrix->y,(*coloring) * m_state.textColor);
}

void DrawHelper(CDC* dc,const TextCharState& state,int xLP,int yLP,COLORREF cr) {
	ASSERT_VALID(dc);
	int nSaved = dc->SaveDC();
	dc->SetTextColor(cr);
	dc->SetTextAlign(TA_LEFT);
	dc->SetBkMode(TRANSPARENT);

	typedef std::auto_ptr<CCharDrawer> PCHARDRAWER;
	PCHARDRAWER drawer;
	drawer = PCHARDRAWER(new CNativeCharDrawer(
				dc,
				state.fontFace,
				state.fontSize,
				state.fontStyleBold,
				state.fontStyleItalic,
				state.fontStyleUnderline));
	
	CRect rcLP(0,0,state.width,state.height);
	rcLP.OffsetRect(xLP,yLP);
	drawer->Draw(dc,rcLP,state.text);
	VERIFY(dc->RestoreDC(nSaved));
}

int CreateFont(CFont& font,int face,int size,BOOL bold,BOOL italic,BOOL underline) {
	LOGFONT lf;
	ZeroMemory(&lf,sizeof lf);
	lf.lfCharSet = DEFAULT_CHARSET;
	int fontHeight = 0;
	CString fontFace;
	
	ASSERT(face == TC_FACE_SYSTEM || face == TC_FACE_MONOSPACE || face == TC_FACE_PROPORTIONAL);
	ASSERT(size == TC_SIZE_SMALL  || size == TC_SIZE_MEDIUM    || size == TC_SIZE_LARGE);

	fontFace   = LoadString(FONT_FACE[face]);
	fontHeight = FONT_PIXEL_SIZE[size];

	if (bold) {
		lf.lfWeight = FW_BOLD;
	}
	if (italic) {
		lf.lfItalic = TRUE;
	}
	if (underline) {
		lf.lfUnderline = TRUE;
	}

	lstrcpyn(lf.lfFaceName,fontFace,_countof(lf.lfFaceName));
	lf.lfHeight = fontHeight;
	VERIFY(font.CreateFontIndirect(&lf));

	ASSERT(0 < fontHeight);
	return fontHeight;
}

CSize CCharDrawer::CalcSize(CString str) {
	const int len  = str.GetLength();
	const int heightFont  = GetFontHeight();
	const int widthFont   = GetFontWidth();

	int maxx = 0;
	int x = 0;
	int y = heightFont;
	int pos = 0;
	while (pos < len) {
		TCHAR tc = str.GetAt(pos);
		if (tc == _T('\r')) {
			ASSERT(str.GetAt(pos+1) == _T('\n'));
			pos += 2;
			maxx = max(maxx,x);
			x = 0;
			y += heightFont;
		} else {
			const int charsize = isleadbyte(tc) ? 2 : 1;
			x += widthFont * charsize;
			pos += charsize;
		}
	}
	maxx = max(maxx,x);
	return CSize(maxx,y);
}

void CCharDrawer::Draw(CDC* dc,LPCRECT prc,CString str) {
	const int len  = str.GetLength();
	const int left  = prc->left;
	const int right = prc->right;	
	const int heightFont  = GetFontHeight();

	int x = left;
	int y = prc->top;
	int pos = 0;
	BOOL bAutoBroken = FALSE;
	while (pos < len) {
		TCHAR tc = str.GetAt(pos);
		if (tc == _T('\r')) {
			ASSERT(str.GetAt(pos+1) == _T('\n'));
			pos += 2;
			if (bAutoBroken) {
				bAutoBroken = FALSE;
			} else {
				y += heightFont;
				x = left;
			}
		} else {
			const int charsize = _istleadbyte(tc) ? 2 : 1;
			x += DrawChar(dc,x,y,str.Mid(pos,charsize),charsize); 
			pos += charsize;
			if (right <= x) {
				y += heightFont;
				x = left;
				bAutoBroken = TRUE;
			} else {
				bAutoBroken = FALSE;
			}
		}
	}
}


CNativeCharDrawer::CNativeCharDrawer(
	CDC* dc,
	int face,
	int size,
	BOOL bold,
	BOOL italic,
	BOOL underline) 
:	m_pDC(dc),
	m_pFontSaved(NULL) {

	m_nFontSize = CreateFont(m_font,face,size,bold,italic,underline);
	if (m_pDC != NULL) {
		VERIFY((m_pFontSaved = m_pDC->SelectObject(&m_font)) != NULL);
	}
}

int CNativeCharDrawer::GetFontHeight() const {
	return m_nFontSize;
}

int CNativeCharDrawer::GetFontWidth() const {
	return m_nFontSize / 2;
}

CNativeCharDrawer::~CNativeCharDrawer() {
	if (m_pDC != NULL) {
		VERIFY(m_pDC->SelectObject(m_pFontSaved) != NULL);
	}
}

int CNativeCharDrawer::DrawChar(CDC* dc,int x,int y,CString str,int charsize) {
	ASSERT(charsize == 1 || charsize == 2);
	VERIFY(dc->TextOut(x,y,str,charsize));
	return m_nFontSize * charsize / 2;
}




