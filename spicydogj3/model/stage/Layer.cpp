#include "stdafx.h"
#include "resource.h"
#include "Layer.h"

#include "KeyFrame.h"
#include "KeyFrameKeepers.h"
#include "model/AffineMatrix.h"
#include "model/char/Char.h"
#include "sdom/sdomn.h"
#include "sdom/SDOMException.h"
#include "Stage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LayerStatus::LayerStatus() {
	m_bLocked  = false;
	m_bVisible = true; 
}

Layer::Layer() {
}


SDOMSerializable* Layer::factory(LPCWSTR name) {
	if (wcscmp(name,WSDOMN_LAYER) != 0) {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	return new Layer;
}

LPCWSTR Layer::namer(SDOMSerializable* object) {
	ASSERT(AfxIsValidAddress(object,sizeof SDOMSerializable));
	if (object->getTypeName() != "Layer") {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
	return WSDOMN_LAYER;
}

Layer* Layer::Clone() const {
	Layer* dst = new Layer;

	dst->CopyFrom(*this);
	dst->m_status = m_status;
	dst->m_char.ReferTo(m_char);

	int nFrames = m_keyFrames.GetFrameCount();
	if (0 < nFrames) {
		dst->m_keyFrames.Expand(nFrames);

		KeyFrameKeepers ks;
		KeyFrameOwners::ConstItr i;
		for (i = m_keyFrames.Begin();i != m_keyFrames.End();i++)
		{
			const KeyFrame* keyFrame = * i;
			ks.OwnNew(keyFrame->Clone());
		}
		ks.TransferTo(dst->m_keyFrames,0,m_keyFrames.GetFrameCount());
	}

	ASSERT_VALID(dst);
	return dst;
}

Layer::~Layer()
{
}

void Layer::Serialize(SDOMArchive* ar) {
	ASSERT_VALID(this);

	ar->SerializeString(WSDOMN_NAME,   m_status.m_strName);
	ar->SerializeBOOL(WSDOMN_VISIBLE,m_status.m_bVisible);
	ar->SerializeBOOL(WSDOMN_LOCKED, m_status.m_bLocked);
	ar->SerializeReferer(WSDOMN_CHAR,&m_char);
	ar->SerializeStatic(WSDOMN_KEY_FRAME_LIST,&m_keyFrames);

	ASSERT_VALID(this);
}

void Layer::AssertValid() const {
	CObject::AssertValid();
	ASSERT(AfxIsValidString(m_status.m_strName));
	if (!m_char.IsNullObject()) {
		ASSERT_VALID(&m_char);
	}
}

CString Layer::GetName() const {
	return m_status.m_strName;
}

void Layer::SetName(const CString& str) {
	m_status.m_strName = str;
}

BOOL Layer::GetLock() const {
	return m_status.m_bLocked;
}

void Layer::SetLock(BOOL b) {
	m_status.m_bLocked = b;
}

BOOL Layer::GetVisible() const {
	return m_status.m_bVisible;
}

void Layer::SetVisible(BOOL b) {
	m_status.m_bVisible = b;
}

void Layer::AttachChar(Char* p) {
	if (p == m_char) {
		return;
	}
	m_char.ReferTo(p);
}

const Char* Layer::GetAttachedChar() const {
	ASSERT_VALID(this);
	return m_char;
}

Char* Layer::GetAttachedChar() {
	ASSERT_VALID(&m_char);
	return m_char;
}

int Layer::GetFrameCount() const {
	return m_keyFrames.GetFrameCount();
}

KeyFrameOwners& Layer::GetKeyFrameOwners() {
	return m_keyFrames;
}

KeyFrame* Layer::GetKeyFrame(int pos)
{
	return const_cast<KeyFrame*>(m_keyFrames.Find(pos,KeyFrameOwners::FT_EQUAL));
}

const KeyFrame* Layer::GetKeyFrame(int pos) const {
	return m_keyFrames.Find(pos,KeyFrameOwners::FT_EQUAL);
}

const KeyFrame* Layer::GetBeginKeyFrame(int pos) const
{
	return m_keyFrames.Find(pos,KeyFrameOwners::FT_BEGIN);
}

const KeyFrame* Layer::GetEndKeyFrame(int pos) const
{
	return m_keyFrames.Find(pos,KeyFrameOwners::FT_END);
}

void Layer::GetLabelList(const Stage* stage,LABEL_NAME_LIST& list) const {
	ASSERT_VALID(stage);
	ASSERT_VALID(this);

	KeyFrameOwners::ConstItr i = m_keyFrames.Begin();
	while (i != m_keyFrames.End()) {
		const KeyFrame* keyFrame = *i;
		ASSERT_VALID(keyFrame);
		if (keyFrame->HasLabel()) {
			KeyFrameLabel label;
			keyFrame->GetLabel(&label);
			CString name;
			name.Format(IDS_KEYFRAME_LABEL_NAME,
				        stage->GetStageName(),
						label.labelName,
						keyFrame->GetFramePos() + 1);

			list.push_back(LABEL_NAME(keyFrame,name));
		}
		i++;
	}
}

BOOL Layer::IsKeyFrameType(int nType) {
	return nType == FRAME_KEY || nType == FRAME_EMPTYKEY;
}

KeyFrame* Layer::GenerateKeyFrame(int pos,BOOL empty) const {
	const KeyFrame* begin = GetBeginKeyFrame(pos);
	const KeyFrame* end   = GetEndKeyFrame(pos);
	
	KeyFrame* keyFrame = new KeyFrame(pos,empty);
	if (begin == NULL) {
		ASSERT_VALID(keyFrame);
		return keyFrame;
	}

	KeyFrameState newState;

	KeyFrameState beginState;
	begin->GetState(&beginState);
	
	if (end != NULL && !begin->IsEmpty() && !end->IsEmpty()) {
	
		KeyFrameState endState;
		end->GetState(&endState);

		int n   = pos                - begin->GetFramePos();
		int len = end->GetFramePos() - begin->GetFramePos() ;
		ASSERT(0 < len);
		ASSERT(0 < n && n < len);

		newState.Interpolate(&beginState,&endState,n,len);
	}

	keyFrame->SetState(&newState);

	ASSERT_VALID(keyFrame);
	return keyFrame;
}

int Layer::GetFrameType(int pos) const {
	const KeyFrame* begin = GetBeginKeyFrame(pos);
	const KeyFrame* end   = GetEndKeyFrame(pos);

	if (begin == NULL) {
		return FRAME_NULL;
	}
	ASSERT_VALID(begin);

	if (pos == begin->GetFramePos()) {
		if (begin->IsEmpty()) {
			return FRAME_EMPTYKEY;
		} else {
			return FRAME_KEY;
		}
	}

	if (begin->IsEmpty()) {
		return FRAME_NULL;
	}
	
	KeyFrameState state;
	begin->GetState(&state);

	if (!state.animating.animating) {
		return FRAME_NULL;
	}

	if (end == NULL) {
		return FRAME_EMPTY_TWEEN;
	} else if (end->IsEmpty()) {
		return FRAME_EMPTY_TWEEN;
	} else { 
		if (pos == end->GetFramePos() -1) {
			return FRAME_END_TWEEN;
		} else if (begin->GetFramePos() + 1 == pos) {
			return FRAME_BEGIN_TWEEN;
		} else {
			return FRAME_TWEEN;
		}
	}
}

void Layer::Draw(int pos,CDC* pDC) const {
	AffineMatrix matrix;
	Coloring     coloring;
	if (!GetFrameInfo(pos,&matrix,&coloring,FALSE)) {
		return;
	}
	m_char->Draw(pDC,&matrix,&coloring);
}

void Layer::GetBezier(int pos,CBEZIER& bezier) const {
	if (GetAttachedChar() == NULL) {
		return;
	}
	AffineMatrix matrix;
	Coloring coloring;
	if (!GetFrameInfo(pos,&matrix,&coloring,FALSE)) {
		return;
	}
	if (!m_char->GetCharType().canModifyPlacing) {
		matrix.cx = matrix.cy = 1;
		matrix.rx = matrix.ry = 0;
	}
	m_char->GetBezier(bezier);
	MatrixByCBezier(matrix.cx,matrix.rx,matrix.x,
		            matrix.ry,matrix.cy,matrix.y,
					bezier);
}

BOOL Layer::GetFrameInfo(int pos,AffineMatrix* matrix,Coloring* coloring,BOOL bOnlyKey) const {
	ASSERT(0 <= pos);
	if (!GetVisible() || GetAttachedChar() == NULL) {
		return FALSE;
	}
	if (GetFrameCount() <= pos) {
		return FALSE;
	}
	const KeyFrame* begin = GetBeginKeyFrame(pos);
	const KeyFrame* end = GetEndKeyFrame(pos);
	if (begin == NULL) {
		return FALSE;
	}
	int nBeginPos = begin->GetFramePos();
	if (bOnlyKey && (nBeginPos != pos)) {
		return FALSE;
	}
	if (begin->IsEmpty()) {
		return FALSE;
	}

	KeyFrameState stateBegin;
	begin->GetState(&stateBegin);
	Placing placing;
	if (end != NULL && stateBegin.animating.animating && !end->IsEmpty()) {
		KeyFrameState stateEnd;
		end->GetState(&stateEnd);
		int n1 = pos - nBeginPos;
		int n2 = end->GetFramePos() - nBeginPos;
		int easing = stateBegin.animating.easing;
		placing.Interpolate  (&stateBegin.placing, &stateEnd.placing, n1,n2,easing);
		coloring->Interpolate(&stateBegin.coloring,&stateEnd.coloring,n1,n2,easing);
	} else {
		 placing  = stateBegin.placing;
		*coloring = stateBegin.coloring;
	}
	placing.Translate(matrix);
	return TRUE;
}
