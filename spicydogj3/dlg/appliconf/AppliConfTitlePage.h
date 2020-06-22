#pragma once

#include "utility/wnd/PreviewStatic.h"
#include <ltwrappr.h>

class AppliConf;
class AppliConfTitlePage : public CPropertyPageEx,PreviewStatic::PaintEventListener {
private:
	AppliConf*    m_appliConf;
	PreviewStatic m_iconPreview;
	LBitmap       m_icon;
	BOOL          m_isIconValid;
public:
	AppliConfTitlePage(AppliConf* appliConf);
	~AppliConfTitlePage();
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnSetActive();
	virtual void OnOK();
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	virtual void OnPSPaint(PreviewStatic&);
	virtual BOOL OnPSEraseBkgnd(PreviewStatic&,CDC&);
	afx_msg void OnChanged();
	afx_msg void OnLoadIcon();
	afx_msg void OnClearIcon();
	static void LoadIcon(LPCTSTR path,LBuffer& icon,LBitmap& bitmap);
	static void LoadIconBitmap(const LBuffer& buffer,LBitmap& bitmap);
	DECLARE_MESSAGE_MAP()
};
