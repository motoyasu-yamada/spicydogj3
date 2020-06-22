#pragma once

#include <toolkit/secall.h>
#include "sdom/olm.h"
#include "wnd/ViewSwitch.H"
#include "wnd/ViewSwitchWnd.H"

class Char;
class SpicydogDoc;

class AppliFrameWnd : 
	public SECMDIChildWnd,
	public ViewSwitchListener,
	public COLMRefererListener
{
	friend class CHARVIEWCMP;
	struct CHARVIEW {
		TOLMReferer<Char> CharPtr;
		CView* ViewPtr;
	};
	DECLARE_DYNCREATE(AppliFrameWnd)
	typedef std::auto_ptr<CHARVIEW> CHARVIEWPTR;
	typedef std::list<CHARVIEWPTR> CHARVIEWLIST;
private:
	ViewSwitch      m_ViewSwitch;
	ViewSwitchWnd   m_ViewSwitchWnd;
	CHARVIEWLIST m_CharViewList;
	BOOL m_bEventHandling;
public:
	BOOL OpenCharGUI(Char*);
	void UpdateViewSwitchWnd();
protected:
	AppliFrameWnd();
private:
	virtual ~AppliFrameWnd();
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual void OnRemoveView(int n,CView*);
	virtual void OnActiveViewChanged(int nNew,int nOld);
	virtual void OnRemovedOLMObject(COLMObject*);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL DestroyWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void OnChildActivate();
	virtual void OnClose();
	void OpenNewCharViewGUI(SpicydogDoc*,Char*);
	void ReopenCharViewGUI(Char*);
	BOOL OpenCharModalGUI(SpicydogDoc*,Char*);
	BOOL CreateMovieView();
	BOOL CreateViewBarWnd();
	afx_msg void OnUpdateCloseCharView(CCmdUI*);
	afx_msg void OnUpdateCloseAllCharView(CCmdUI*);
	afx_msg void OnCloseCharView();
	afx_msg void OnCloseAllCharView();
	DECLARE_MESSAGE_MAP()
};

