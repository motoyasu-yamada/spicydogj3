#pragma once

#include "model/build/BuildOutput.h"
#include "utility/DocumentEvent.h"
#include "utility/clinit.h"
#include "toolkit/secall.h"

class InWndOwner;
class BuildOutput;

class BuildBar
:	public SECStdBtn,
	public DocumentListener {
	DECLARE_BUTTON(BuildBar);
	DECLARE_CLINIT(BuildBar);

private:
	static CFont      s_font;
	static CImageList s_images;
	BuildOutput::STATE m_State;
	int m_nDataSize;
	int m_nMaxDataSize;
public:
	BuildBar();
	virtual ~BuildBar();
protected:
	void OnBuilt(DWORD dwNotifyCode,CDocument* p);
	void InvalidateMeter();
	void PaintMet(BuildOutput& bo);
	void DrawIcon(CDC&,CRect&,int);
	void DrawMet(CDC&,CRect&,int,int,COLORREF cr);
	void DrawErrMet(CDC&,CRect&,UINT);
	void DrawMetBkgnd(CDC&,CRect&);

	virtual void OnDocumentActivated(CDocument* p);
	virtual void OnNotify(DWORD dwNotifyCode,CDocument* p,LPARAM,LPVOID);
	virtual void DrawButton(CDC& dc, SECBtnDrawData& data);
	virtual void AdjustSize();
};
#define PROGRESS_BUTTON(BTNID)	{BUTTON_CLASS(BuildBar),BTNID,TBBS_BUTTON, 0,0,0,0,0},


