#include "stdafx.h"
#include "resource.h"
#include "Stage.h"

#include "cmd/CmdSet.h"
#include "cmd/stage/MovLayerCmd.h"
#include "cmd/stage/InsLayerCmd.h"
#include "cmd/stage/AttachLayerCharCmd.h"
#include "Layer.h"
#include "model/build/Builder.h"
#include "model/char/CharList.h"
#include "model/stage/KeyFrame.h"
#include "model/char/char.h"
#include "model/AffineMatrix.h"
#include "model/action/ActionList.h"
#include "sdom/sdomn.h"
#include "SpicydogDoc.h"
#include "Player.h"
#include "utility/MemoryUtility.h"
#include "cmd/stage/InsKeyFrameCmd.h"
#include "model/AffineMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Stage::Stage()
:	m_nSelFrame(-1),
	m_nSelFrameEnd(-1)
{
}

Stage::~Stage() {
}

void Stage::AssertValid() const {
	COLMObject::AssertValid();
	m_layers.AssertValid();

	ASSERT((m_nSelFrame == -1 && m_nSelFrameEnd == -1) ||
		   (0 <= m_nSelFrame && m_nSelFrame <= m_nSelFrameEnd));
}

void Stage::Serialize(SDOMArchive* ar) {
	ar->SerializeStatic(WSDOMN_LAYERS,&m_layers);
}

void Stage::GetSel(int& nStart,int& nEnd) const {
	nStart = m_nSelFrame;
	nEnd = m_nSelFrameEnd;

	ASSERT((nStart == -1 && nEnd == -1) ||
		   (0 <= nStart && nStart <= nEnd));
}

void Stage::SetSel(int nStart,int nEnd) {
	ASSERT((nStart == -1 && nEnd == -1) ||
		   (0 <= nStart && nStart <= nEnd));
	
	m_nSelFrame = nStart;
	m_nSelFrameEnd = nEnd;
}

void Stage::AddSel(int n) {
	int nStart;
	int nEnd;
	GetSel(nStart,nEnd);
	if (nStart == -1)
	{
		ASSERT(nEnd == -1);
		nStart = nEnd = n;
	}
	else
	{
		ASSERT(nEnd != -1);
		nStart = min(nStart,n);
		nEnd = max(nEnd,n);
	}
	SetSel(nStart,nEnd);
}

int Stage::GetSelFrame() const {
	ASSERT_VALID(this);
	return m_nSelFrame;
}

void Stage::SelectFrame(int nSel) {
	m_nSelFrame = nSel;
	m_nSelFrameEnd = nSel;
	ASSERT_VALID(this);
}

void Stage::UnselectFrame() {
	m_nSelFrame = -1;
	m_nSelFrameEnd = -1;
}

BOOL Stage::IsFrameSel() const {
	int nSelLayer = GetSelLayer();
	return !(nSelLayer < 0 || m_nSelFrame < 0);
}

int Stage::GetFrameCount() const {
	int nFrameCount = 0;
	for (int n = 0;n < m_layers.GetCount();n++) {
		const Layer* layer = m_layers.GetObject(n);
		int nFrameCount2 = layer->GetFrameCount();
		if (nFrameCount < nFrameCount2)
			nFrameCount = nFrameCount2;
	}
	return nFrameCount;
}

int Stage::GetLayerCount() const
{
	return m_layers.GetCount();
}

const Layer* Stage::GetLayer(int n) const
{
	ASSERT(0 <= n && n < GetLayerCount());

	return m_layers.GetObject(n);
}

Layer* Stage::GetLayer(int n)
{
	ASSERT(0 <= n && n < GetLayerCount());

	return m_layers.GetObject(n);
}

void Stage::ShowAll() {
	for (int n = 0;n < m_layers.GetCount();n++) {
		Layer* layer = m_layers.GetObject(n);
		layer->SetVisible(TRUE);
	}	
}

void Stage::LockOtherLayers(int nUnlock) {
	for (int n = 0;n < m_layers.GetCount();n++) {
		Layer* layer = m_layers.GetObject(n);
		layer->SetLock(n != nUnlock);
	}
}

void Stage::HideOtherLayers(int nVisible) {
	for (int n = 0;n < m_layers.GetCount();n++) {
		Layer* layer = m_layers.GetObject(n);
		layer->SetVisible(nVisible == n);
	}
}

int Stage::GetSelLayer() const
{
	ASSERT_VALID(this);

	for (int n = 0;n < m_layers.GetCount();n++)
	{
		const Layer* layer = m_layers.GetObject(n);
		if (layer->IsSelected())
			return n;
	}
	return -1;
}

void Stage::SelectLayer(int nSel)
{
	ASSERT_VALID(this);
	ASSERT(0 <= nSel);// && nSel < GetLayerCount());
	
	UnselectLayer();
	GetLayer(nSel)->Select();
}

void Stage::UnselectLayer() {
	ASSERT_VALID(this);

	for (int n = 0;n < m_layers.GetCount();n++) {
		Layer* layer = m_layers.GetObject(n);
		layer->Unselect();
	}
}

	CString getLabelDisplayName(const Stage* stage) {
		CString format;
		format.Format(IDS_STAGE_LABEL_NAME,stage->GetStageName());
		return format;
	}

void Stage::GetLabelList(LABEL_NAME_LIST& list) const {
	list.push_back(LABEL_NAME(this,getLabelDisplayName(this)));

	for (int n = 0;n < m_layers.GetCount();n++) {
		const Layer* layer = m_layers.GetObject(n);
		ASSERT_VALID(layer);
		layer->GetLabelList(this,list);
	}
}

void Stage::Build(Builder* builder) const {
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));

	builder->EnterStage(GetFrameCount());
	Label::BuildLabel(builder,0);

	int frameCount = GetFrameCount();
	int layerCount = GetLayerCount();
	for (int layerPos = 0;layerPos < layerCount;layerPos++) {
		const Layer* layer;
		layer = GetLayer(layerPos);
		switch (builder->GetBuildStep()) {
		case Builder::BUILDSTEP_FIRST:
			builder->UseChar(layer->GetAttachedChar());
			break;
		case Builder::BUILDSTEP_LAST:
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}

	for (int framePos = 0;framePos < frameCount;framePos ++) {
		for (int layerPos = 0;layerPos < layerCount;layerPos++) {
			int depth;
			const Layer* layer;
			layer = GetLayer(layerPos);
			ASSERT_VALID(layer);
			depth = layerCount - layerPos - 1;
			ASSERT(0 <= depth && depth < layerCount);

			if (layer->GetFrameCount() < framePos) {
				continue;
			}

			switch (builder->GetBuildStep()) {
			case Builder::BUILDSTEP_FIRST:
				BuildFrameFirst(builder,layer,framePos);
				break;
			case Builder::BUILDSTEP_LAST:
				BuildFrameLast(builder,layer,depth,framePos);
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		switch (builder->GetBuildStep()) {
		case Builder::BUILDSTEP_FIRST:
			break;
		case Builder::BUILDSTEP_LAST:
			builder->WriteInstruction(INSTID_SHOW_FRAME);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
}

void Stage::BuildFrameFirst(Builder* builder,const Layer* layer,int framePos) const {
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));
	ASSERT_VALID(layer);
	ASSERT(0 <= framePos && framePos <= layer->GetFrameCount());

	const KeyFrame* keyFrame =layer->GetKeyFrame(framePos);
	if (keyFrame != NULL) {
		ASSERT(framePos == keyFrame->GetFramePos());
		if (keyFrame->HasLabel()) {
			keyFrame->BuildKeyFrameLabel(builder);
		}
	}

	const KeyFrame* begin = layer->GetBeginKeyFrame(framePos);
	if (begin != NULL && begin->GetFramePos() == framePos && begin->HasAction()) {
		builder->Build(const_cast<KeyFrame*>(begin)->GetActionList());
	}
}

void Stage::BuildFrameLast(Builder* builder,const Layer* layer,int depth,int framePos) const {
	int frameCount = layer->GetFrameCount();
	if (frameCount < framePos) {
		return;
	}
	if (frameCount == framePos) {
		builder->WriteInstruction(INSTID_DEL_INSTANCE);
		builder->WriteDepth(depth);
		return;
	}
	if (!layer->GetVisible()) {
		return;
	}

	//	–¢ƒtƒŒ[ƒ€ˆ—
	const KeyFrame* begin = layer->GetBeginKeyFrame(framePos);
	const KeyFrame* end   = layer->GetEndKeyFrame(framePos);
	if (begin == NULL) {
		return;
	}
	
	if (layer->GetAttachedChar() != NULL) {
		KeyFrameState beginState;
		begin->GetState(&beginState);

		if (begin->IsEmpty()) {
			if (begin->GetFramePos() == framePos) {
				builder->WriteInstruction(INSTID_DEL_INSTANCE);
				builder->WriteDepth(depth);
			}
			return;
		}
		
		BOOL writePlacing = false;
		Placing   placing;
		Coloring  coloring;

		if (end != NULL && !end->IsEmpty() && beginState.animating.animating) {
			KeyFrameState endState;
			end->GetState(&endState);

			int n1 = framePos - begin->GetFramePos();
			int n2 = end->GetFramePos() - begin->GetFramePos();
			int easing = beginState.animating.easing;

			placing.Interpolate(&beginState.placing,&endState.placing,n1,n2,easing);
			coloring.Interpolate(&beginState.coloring,&endState.coloring,n1,n2,easing);

			writePlacing = true;

		} else if (begin->GetFramePos() == framePos) {
			placing = beginState.placing;
			coloring= beginState.coloring;
			writePlacing = true;
		}

		if (writePlacing) {
			AffineMatrix matrix;
			placing.Translate(&matrix);

			builder->WriteInstruction(INSTID_NEW_INSTANCE);
			builder->WriteDepth      (depth);	
			builder->WriteCharRef    (layer->GetAttachedChar());
			builder->WriteMatrix     (&matrix);
			builder->WriteColoring   (&coloring);
		}
	}

	if (begin->GetFramePos() == framePos && begin->HasAction()) {
		builder->WriteInstruction(INSTID_DO_ACTION);
		builder->Build(const_cast<KeyFrame*>(begin)->GetActionList());
	}
}

TOLMOwners<Layer,false>& Stage::GetOwners() {
	ASSERT_VALID(&m_layers);
	return m_layers;
}

int Stage::HitFrameCharLayer(int pos,CPoint ptLP) const {
	const int num = m_layers.GetCount();
	for (int n = 0;n < num;n++) {
		const Layer* layer = m_layers.GetObject(n);
		if (layer->GetLock())
			continue;

		CBEZIER bezier;
		layer->GetBezier(pos,bezier);
		if (PtInBezier(bezier,ptLP))
			return n;
	}
	return -1;
}

int Stage::HitFrameCharLayer(int pos,CRect rcLP) const {
	const int num = m_layers.GetCount();
	for (int n = 0;n < num;n++) {
		const Layer* layer = m_layers.GetObject(n);
		if (layer->GetLock())
			continue;

		CBEZIER bezier;
		layer->GetBezier(pos,bezier);
		if (RectInBezier(bezier,rcLP))
			return n;
	}
	return -1;
}

void Stage::Draw(int pos,CDC* pDC) const {
	const int num = m_layers.GetCount();
	for (int n = 0;n < num;n++) {
		const Layer* layer = m_layers.GetObject(num - n - 1);
		layer->Draw(pos,pDC);
	}
}

struct CFS_LAYERS {
	DWORD        processID;
	SpicydogDoc* document;
	int          layerIndex;
};

BOOL Stage::SubmitOle(COleDataSource* pSrc,BOOL bDnD,SpicydogDoc* pDoc,int nSel) {
	ASSERT_VALID(pDoc);
	ASSERT_VALID(pSrc);
	ASSERT(0 <= nSel && nSel < GetLayerCount());
	if (nSel < 0) {
		return FALSE;
	}
	if (bDnD) {
		pSrc->CacheGlobalData(CF_LAYERS,Data2Ole_CF_LAYER(pDoc,nSel));
	}
	return TRUE;
}

BOOL Stage::AcceptOle(COleDataObject* pObj,DROPEFFECT de,SpicydogDoc* document,OLE_ACCEPT_PARAM& param) {
	document->BeginEdit();
	if (!param.m_bDnD) {
		return FALSE;
	}
	if (pObj->IsDataAvailable(CF_LAYERS)) {
		if (de == DROPEFFECT_MOVE && AcceptOleMove(pObj,document,param)) {
			 return TRUE;
		}
	}
	if (pObj->IsDataAvailable(CF_CHARS)) {
		if (AcceptOleChar(pObj,document,param))
			return TRUE;
	}
	return FALSE;
}

BOOL Stage::AcceptOleChar(COleDataObject* pObj,SpicydogDoc* document,OLE_ACCEPT_PARAM& param) {
	if (pObj == NULL || document == NULL) {
		ASSERT(FALSE);
		return FALSE; 
	}
	ASSERT_VALID(document);

	if (!pObj->IsDataAvailable(CF_CHARS)) {
		ASSERT(FALSE);
		return FALSE;
	}

	HGLOBAL hg = pObj->GetGlobalData(CF_CHARS);
	if (hg == NULL) {
		ASSERT(FALSE);
		return FALSE; 
	}

	TGlobalAllocatorTypedPtr<CFS_CHARS> cfs(hg);
	if (!AfxIsValidAddress(cfs,sizeof CFS_CHARS)) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (cfs->processID != GetCurrentProcessId()) {
		return FALSE;
	}
	if (!AfxIsValidAddress(cfs->document,sizeof SpicydogDoc)) {
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT_VALID(cfs->document);
	
	if (document != cfs->document) {
		return FALSE;
	}

	if (param.m_Method == OLE_ACCEPT_PARAM::methodIns) {
		return AcceptOleChar_Ins(document,cfs,param);
	} else {
		return AcceptOleChar_Ovr(document,cfs,param);
	}
}

BOOL Stage::AcceptOleChar_Ins(SpicydogDoc* document,CFS_CHARS* cfs,OLE_ACCEPT_PARAM& param) {
	CmdSet* pcmd = new CmdSet(IDCMD_INS_CHAR);

	BOOL bPlaced = FALSE;
	int charCount = cfs->charCount;
	ASSERT(0 < charCount);

	int nInsLayer = param.m_nLayerPos;
	if (nInsLayer < 0) {
		ASSERT(FALSE);
		return FALSE;
	}

	int nInsFrame = param.m_nFramePos;
	int nFrameLen = nInsFrame + param.m_nFrameLen;
	if (nInsFrame == -1 || nFrameLen <= nInsFrame) {
		ASSERT(FALSE);
		return FALSE;
	}

	for (int n = 0;n < charCount;n++) {
		Char* pChar = cfs->chars[n];
		if (pChar == NULL || !AfxIsValidAddress(pChar,sizeof Char))
			{ ASSERT(FALSE);continue; }
		ASSERT_VALID(pChar);

		if (!pChar->GetCharType().canAttachLayer)
			continue;

		Layer* layer = new Layer;
		layer->SetName(pChar->GetName());
		layer->AttachChar(pChar);
		
		KeyFrameKeepers k;
		KeyFrameOwners& o = layer->GetKeyFrameOwners();
		o.Expand(nFrameLen);

		KeyFrame* key = layer->GenerateKeyFrame(nInsFrame,FALSE);
		if (key == NULL) {
			ASSERT(FALSE);
			continue;
		}
		KeyFrameState state;
		key->GetState(&state);
		state.placing.x = param.m_ptDropPos.x;
		state.placing.y = param.m_ptDropPos.y;
		key->SetState(&state);
		k.OwnNew(key);
		k.TransferTo(o,nInsFrame,1);

		ASSERT_VALID(layer);
		pcmd->AddCommand(new InsLayerCmd(this,nInsLayer,layer));
		bPlaced = TRUE;
	}

	if (bPlaced) {
		document->DoCmd(pcmd);
		SelectLayer(nInsLayer);
		if (param.m_nFramePos != -1) {
			document->player->SetPlayFrame(param.m_nFramePos);
			SelectFrame(param.m_nFramePos);
		}
		document->UpdateAll(NULL);
	} else {
		delete pcmd;
	}
	return TRUE;
}

BOOL Stage::AcceptOleChar_Ovr(SpicydogDoc* document,CFS_CHARS* cfs,OLE_ACCEPT_PARAM& param) {
	if (document == NULL || cfs == NULL)
		{ ASSERT(FALSE);return FALSE;}

	int charCount = cfs->charCount;
	if (!(0 < charCount))
		{ ASSERT(FALSE);return FALSE;}

	Char* pChar = cfs->chars[0];
	if (pChar == NULL || !AfxIsValidAddress(pChar,sizeof Char)) {
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT_VALID(pChar);

	if (!pChar->GetCharType().canAttachLayer)
		return FALSE;

	int nSel = param.m_nLayerPos;
	if (nSel < 0) {
		ASSERT(FALSE);
		return FALSE;
	}

	Layer* layer = GetLayer(nSel);
	ASSERT_VALID(layer);

	CmdSet* pcmd = new CmdSet(IDCMD_INS_CHAR);
	pcmd->AddCommand(new AttachLayerCharCmd(layer,pChar));
	if (param.m_nFramePos != -1) {
		KeyFrame* key = layer->GenerateKeyFrame(param.m_nFramePos,FALSE);
		ASSERT_VALID(key);
		KeyFrameState state;
		key->GetState(&state);
		state.placing.x = param.m_ptDropPos.x;
		state.placing.y = param.m_ptDropPos.x;
		key->SetState(&state);
		pcmd->AddCommand(new InsKeyFrameCmd(layer,param.m_nFramePos,key));

	}

	document->DoCmd(pcmd);
	SelectLayer(nSel);
	if (param.m_nFramePos != -1) {
		document->player->SetPlayFrame(param.m_nFramePos);
		SelectFrame(param.m_nFramePos);
	}
	document->UpdateAll(NULL);

	return TRUE;
}

BOOL Stage::AcceptOleMove(COleDataObject* pObj,SpicydogDoc* document,OLE_ACCEPT_PARAM& param) {
	ASSERT_VALID(document);
	ASSERT(pObj != NULL);
	int nPos = param.m_nLayerPos;

	HGLOBAL hg = pObj->GetGlobalData(CF_LAYERS);
	ASSERT(hg);

	TGlobalAllocatorTypedPtr<CFS_LAYERS> cfg(hg);
	if (!AfxIsValidAddress(cfg,sizeof CFS_LAYERS)) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (cfg->processID != GetCurrentProcessId()) {
		return FALSE;
	}

	if (!AfxIsValidAddress(cfg->document,sizeof SpicydogDoc))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT_VALID(cfg->document);
	ASSERT_VALID(document);
	if (document != cfg->document) {
		return FALSE;
	}

	int nSrcPos = cfg->layerIndex;
	if (nPos < 0 || GetLayerCount() <= nPos ||
		nSrcPos < 0 || GetLayerCount() <= nSrcPos) {
		return FALSE;
	}

	if (nPos != nSrcPos) {
		document->DoCmd(new MovLayerCmd(this,nPos,nSrcPos));
	}
	document->UpdateAll(NULL);
	return TRUE;
}

HGLOBAL Stage::Data2Ole_CF_LAYER(SpicydogDoc* document,int nSel) const {
	GlobalAllocator a(sizeof CFS_LAYERS,GMEM_FIXED | GMEM_SHARE);
	TGlobalAllocatorTypedPtr<CFS_LAYERS> cf(a);
	ASSERT(AfxIsValidAddress(cf,sizeof(CFS_LAYERS)));

	cf->processID  = GetCurrentProcessId();
	cf->document   = document;
	cf->layerIndex = nSel;

	return a.Detach();
}
