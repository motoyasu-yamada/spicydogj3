#pragma once

#include <memory>
#include <map>

class Cnt;
/**
	コントローラー管理クラス
	切り替え・コマンドルーティング処理をサポートする
*/
class CntMgr {
private:
	typedef	std::auto_ptr<Cnt>	CONTROLERPTR;
	typedef std::map<UINT,CONTROLERPTR>			CONTROLERS;
private:
	CONTROLERS				m_Controlers;
	UINT 					m_nActiveID;
	UINT					m_nDefaultID;
	Cnt*	m_pActive;
public:
	virtual void AppendControler(UINT nID,Cnt*);
	virtual void RemoveControler(UINT nID);
	virtual Cnt* GetActiveControler() const;
	virtual UINT GetActiveControlerID() const;
	virtual void SetDefaultControlerID(UINT nID);
	virtual void ActivateDefaultControler();
	virtual void ActivateControler(UINT nID);
	virtual HACCEL GetDefaultAccelerator();
protected:
	CntMgr();
	virtual ~CntMgr();
	BOOL DoCmdMsg(UINT nID,int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI);
	BOOL DoPreTranslateMessage(MSG* msg);
	void DoDraw(CDC*);
	BOOL DoSetCursor(CWnd*,UINT,UINT);	
	BOOL DoCommand(CWnd*,WPARAM,LPARAM);
private:
	BOOL DoCmd(UINT nID,void* pExtra, AFX_CMDHANDLERINFO* pHI);
	BOOL DoUpdateCmdUI(UINT nID,void* pExtra, AFX_CMDHANDLERINFO* pHI);
	BOOL IsValidControlerID(UINT nID) const;
}; // class CntMgr
/**
	コントローラー管理クラスの機能をウインドウ継承クラスにプラグインするクラス
*/
template <typename _BASET> 
class TWndControlerMgr : public _BASET,public CntMgr {
protected:
	virtual void OnDraw(CDC* pDC) {
		CntMgr::DoDraw(pDC);
	}
	virtual BOOL OnSetCursor(CWnd* pWnd,UINT hit,UINT msg) {
		if (CntMgr::DoSetCursor(pWnd,hit,msg))
			return TRUE;
		return _BASET::OnSetCursor(pWnd,hit,msg);
	}
	virtual BOOL OnCmdMsg(UINT nID,int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI) {
		if (CntMgr::DoCmdMsg(nID,nCode,pExtra,pHI))
			return TRUE;
		return _BASET::OnCmdMsg(nID,nCode,pExtra,pHI);
	}
	virtual BOOL PreTranslateMessage(MSG* msg) {
		if (CntMgr::DoPreTranslateMessage(msg))
			return TRUE;
		return _BASET::PreTranslateMessage(msg);
	}
	virtual BOOL OnCommand(WPARAM w,LPARAM l) {
		if (CntMgr::DoCommand(this,w,l))
			return TRUE;
		return _BASET::OnCommand(w,l);
	}
}; // class TWndControlerMgr
