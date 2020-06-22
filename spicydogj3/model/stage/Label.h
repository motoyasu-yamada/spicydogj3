#pragma once
#include "sdom/olm.h"

class Stage;
class Label;
class Builder;

/** ラベルの表示属性 */
typedef std::pair<const Label*,CString> LABEL_NAME;
typedef std::list<LABEL_NAME>     LABEL_NAME_LIST;

/** ジャンプ先となることができるラベルインタフェース */
class Label : public COLMObject {
private:
	mutable int m_absolutePos;
public:
	Label();
	virtual ~Label() {}
	/** ラベルのグローバルな物理フレーム位置 */
	int GetLabelAbsolutePos() const;
	/** ラベル名を取得 */
	virtual CString GetLabelName() const = 0;
	/** ラベルをコンボボックスに選択 */
	static void SetupLabelComboBox(CComboBox& combo,const LABEL_NAME_LIST& labels,const Label* select);
protected:
	/** ラベルの位置を求める */
	virtual void BuildLabel(Builder*,int relative) const;
};

