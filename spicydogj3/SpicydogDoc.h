#pragma once

#include "cmd/CmdHistory.h"
#include <memory>
#include "model/build/BuildOutput.h"
#include "utility/NamingRule.h"

class Appli;
class Player;
extern CLIPFORMAT CF_LAYERS;
extern CLIPFORMAT CF_CHARS;

class SpicydogDoc
:	public CDocument,
	public CmdHistory {
private:
	friend class SpicydogDocTempl;
	DECLARE_SERIAL(SpicydogDoc)
	typedef std::auto_ptr<Appli>  APPLIPTR;
	typedef std::auto_ptr<Player> PLAYERPTR;
	enum {
		UPDATED_SELECTION	= 1,
		UPDATED_PLAY		= 2,
		UPDATE_APPLICATION  = 0xFFFF
	};
	enum { EVENTNOTIFYCODE_BUILT = 0x10000 };
	BOOL m_bUpdatePubMet;
	BOOL m_bLoadFromOldVersion;

public:
	APPLIPTR    appli;
	PLAYERPTR   player;
	BuildOutput buildOutput;
	NamingRule  charNamingRule;
	NamingRule  layerNamingRule;
	NamingRule  stageNamingRule;
	SpicydogDoc();
	void BeginEdit();
	void UpdateAll(CView* pSender,LPARAM lHint = 0,CObject* pHint = NULL);

private:
	virtual ~SpicydogDoc();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void UpdateFrameCounts();
	virtual void DeleteContents();
	virtual int GetMaxUndoCmd() const;
	virtual HACCEL GetDefaultAccelerator();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void AssertValid() const;
	BOOL Build(Builder& builder);
	afx_msg void OnEmuPlay();
	afx_msg void OnAppConf();
	afx_msg void OnBuild();
	afx_msg void OnAppViewADF();
	afx_msg void OnPublishLocal();
	afx_msg void OnPublishUpload();
	afx_msg void OnEditUndoAll();
	afx_msg void OnEditRedoAll();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnClearHistory();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearHistory(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

