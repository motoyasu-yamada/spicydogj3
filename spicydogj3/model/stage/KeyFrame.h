#pragma once

#include "Placing.h"
#include "Coloring.h"
#include "Animating.h"
#include "Label.h"
#include "model/action/ActionList.h"

class Appli;
class Char;
class KeyFrameState;
class KeyFrameLabel;
class KeyFrame;


/** キーフレーム状態 */
class KeyFrameState {
public:
	Placing   placing;
	Coloring  coloring;
	Animating animating;

	KeyFrameState();
	KeyFrameState& operator =(const KeyFrameState&);
	void Interpolate(const KeyFrameState*,const KeyFrameState*,int n,int len);
};

/** キーフレームのラベル情報 */
class KeyFrameLabel {
public:
	BOOL    useLabel;
	CString labelName;
	CString labelComment;
	KeyFrameLabel();
	KeyFrameLabel& operator =(const KeyFrameLabel&);
	/** ラベルにユーザが入力した値を検証 */
	static void DDV_Label(CDataExchange*,const Appli*,KeyFrameLabel*,LPCTSTR oldName);
};

/** キーフレーム */
class KeyFrame
:	public Label {
private:
	typedef std::auto_ptr<ActionList> ACTIONSP;
	int  m_framePos;
	BOOL m_empty;
	KeyFrameState m_state;
	KeyFrameLabel m_label;
	ActionList    m_actions;

public:
	KeyFrame();
	KeyFrame(int frame,BOOL empty);
	KeyFrame* Clone() const;
	virtual ~KeyFrame();
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual CString GetLabelName() const;
	void BuildKeyFrameLabel(Builder*) const;
	int GetFramePos() const;
	BOOL IsEmpty() const;
	void SetState(const KeyFrameState*);
	void GetState(KeyFrameState*) const;
	void SetLabel(const KeyFrameLabel*);
	void GetLabel(KeyFrameLabel*) const;
	BOOL HasAction() const;
	BOOL HasLabel() const;
	ActionList* GetActionList();
	void MoveFramePos(int);
	void SetFramePos(int n);
};

