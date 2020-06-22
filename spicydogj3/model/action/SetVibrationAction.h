#pragma once

#include "Action.h"

/** �o�C�u���[�V�����ݒ�A�N�V���� */
class SetVibrationAction : public Action {
public:
	SetVibrationAction();
	void SetVibration(BOOL enabled);
	BOOL GetVibration() const;

private:
	SetVibrationAction(const SetVibrationAction&);
	virtual ~SetVibrationAction();
	virtual Action* Clone() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual UINT GetActionNameResID() const;
	virtual CRuntimeClass* GetDialogClass() const;

	BOOL m_enabled;
};