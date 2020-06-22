#pragma once

#include "Label.h"
#include "model/build/Buildable.h"
#include "sdom/olm.h"
#include "Layer.h"

class SpicydogDoc;
struct CFS_CHARS;

class Stage :
	public Buildable,
	public Label {
	friend class MovLayerCmd;
	friend class InsLayerCmd;
	friend class DelLayerCmd;
	friend class StageView;
	friend class TimelineDlg;
private:
	TOLMOwners<Layer,false> m_layers;
	int m_nSelFrame;
	int m_nSelFrameEnd;

public:
	Stage();
	virtual ~Stage();
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual void Build(Builder*) const;
	virtual CString GetStageName() const = 0;
	void Draw(int nFrame,CDC*) const;
	int GetLayerCount() const;
	const Layer* GetLayer(int n) const;
	Layer* GetLayer(int n);
	int GetFrameCount() const;
	int GetSelLayer() const;	
	void SelectLayer(int);
	void UnselectLayer();
	BOOL IsFrameSel() const;
	int GetSelFrame() const;
	void SelectFrame(int);
	void UnselectFrame();
	void GetSel(int& nStart,int& nEnd) const;
	void SetSel(int nStart,int nEnd);
	void AddSel(int n);	
	void ShowAll();
	void LockOtherLayers(int);
	void HideOtherLayers(int);
	int HitFrameCharLayer(int nFrame,CPoint ptLP) const;
	int HitFrameCharLayer(int nFrame,CRect rcLP) const;
	void GetLabelList(LABEL_NAME_LIST&) const;
	BOOL SubmitOle(COleDataSource*,BOOL bDnD,SpicydogDoc*,int nSel);
private:
	void BuildFrameFirst(Builder*,const Layer* layer,int framePos) const;
	void BuildFrameLast (Builder*,const Layer* layer,int depth,int framePos) const;
	TOLMOwners<Layer,false>& GetOwners();

	struct CFS_SDB2_INCHARS;
	struct OLE_ACCEPT_PARAM {
		BOOL m_bDnD;
		enum { methodIns,methodOver} m_Method;
		int m_nLayerPos;
		int m_nFramePos;
		int m_nFrameLen;
		CPoint m_ptDropPos;
	};
	BOOL AcceptOle(COleDataObject*,DROPEFFECT de,SpicydogDoc*,OLE_ACCEPT_PARAM& param); 
	HGLOBAL Data2Ole_CF_LAYER(SpicydogDoc*,int) const; 
	BOOL AcceptOleMove(COleDataObject*,SpicydogDoc*,OLE_ACCEPT_PARAM&); 
	BOOL AcceptOleChar(COleDataObject*,SpicydogDoc* pDoc,OLE_ACCEPT_PARAM&);
	BOOL AcceptOleChar_Ins(SpicydogDoc* pDoc,CFS_CHARS*,OLE_ACCEPT_PARAM&);
	BOOL AcceptOleChar_Ovr(SpicydogDoc* pDoc,CFS_CHARS*,OLE_ACCEPT_PARAM&);
};

