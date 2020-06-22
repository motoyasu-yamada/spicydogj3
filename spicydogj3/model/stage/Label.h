#pragma once
#include "sdom/olm.h"

class Stage;
class Label;
class Builder;

/** ���x���̕\������ */
typedef std::pair<const Label*,CString> LABEL_NAME;
typedef std::list<LABEL_NAME>     LABEL_NAME_LIST;

/** �W�����v��ƂȂ邱�Ƃ��ł��郉�x���C���^�t�F�[�X */
class Label : public COLMObject {
private:
	mutable int m_absolutePos;
public:
	Label();
	virtual ~Label() {}
	/** ���x���̃O���[�o���ȕ����t���[���ʒu */
	int GetLabelAbsolutePos() const;
	/** ���x�������擾 */
	virtual CString GetLabelName() const = 0;
	/** ���x�����R���{�{�b�N�X�ɑI�� */
	static void SetupLabelComboBox(CComboBox& combo,const LABEL_NAME_LIST& labels,const Label* select);
protected:
	/** ���x���̈ʒu�����߂� */
	virtual void BuildLabel(Builder*,int relative) const;
};

