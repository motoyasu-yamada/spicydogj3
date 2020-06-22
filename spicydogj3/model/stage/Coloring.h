#pragma once

#include "sdom/SDOMSerializable.h"

class Coloring : public CObject,public SDOMSerializable {
public:
	enum EDIT_TYPE {EDIT_TYPE_CUSTOMIZE,EDIT_TYPE_SELECT,EDIT_TYPE_MEIDO,EDIT_TYPE_NONE};
	int    editType;
	double mulRed;
	double mulGreen;
	double mulBlue;
	int	addRed;
	int	addGreen;
	int	addBlue;
	
	Coloring();
	Coloring(const Coloring&);
	Coloring& operator=(const Coloring&);
	void Interpolate(const Coloring*,const Coloring*,int n,int len,int easing);

private:
	void CopyFrom(const Coloring&);
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	static BOOL IsValidAdd(int);
	static BOOL IsValidMul(double);
};
COLORREF operator*(const Coloring& lhs,const COLORREF& rhs);
