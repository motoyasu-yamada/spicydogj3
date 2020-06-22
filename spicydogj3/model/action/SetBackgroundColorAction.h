#pragma once

#include "Action.h"

class SDOMArchive;
class Builder;

/** 背景色変更アクション */
class SetBackgroundColorAction : public Action {
public:
	SetBackgroundColorAction();
	void SetColor(COLORREF);
	COLORREF GetColor() const;
private:
	SetBackgroundColorAction(const SetBackgroundColorAction&);
	virtual ~SetBackgroundColorAction();
	virtual Action* Clone() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual UINT GetActionNameResID() const;
	virtual CRuntimeClass* GetDialogClass() const;

	COLORREF m_color;
};