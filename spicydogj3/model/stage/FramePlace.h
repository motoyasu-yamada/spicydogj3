#pragma once
//////////////////////////////////////////////////////////////////////////
//	これは[SpicyDog]のソースコードです。
//	著作権 スパイシーソフト株式会社
//	このソースコードに関するすべての権利はスパイシーソフトが保有します。
//
//	This is a part of SpicyDog  2001.
//	Copyright (C) 2000 Spicysoft Corporation.
//	All rights reserved.
//////////////////////////////////////////////////////////////////////////

#include "SDBASE.H"
#include "lib\VerSerializer.h"

/**
	配置
*/
class FRAMEPLACE : public TVerSerializer<FRAMEPLACE>
{
public:
	FRAMEPLACE();
	SI16	TranslateX;
	SI16	TranslateY;
	double ScaleX;
	double ScaleY;
		//	0 = 0%,1 = 100%
	double Rotate;			
		//	V2B	  ... 0～360度
		//	V2B2  ... ラジアン

private:
	friend CArchive& AFXAPI operator << (CArchive&,const FRAMEPLACE&);
	friend CArchive& AFXAPI operator >> (CArchive&,FRAMEPLACE&);
	void Serialize(CArchive&);
	void SerializeV2B(int n,CArchive& ar);
	void SerializeV2B2(int n,CArchive& ar);
}; // class FRAMEPLACE

double ConvertMinute2Radian(double dMinute);
double ConvertRadian2Minute(double dRadian);
double ConvertRotateCount2Radian(int n);
void Interpolate(FRAMEPLACE&,int n1,int n2,int,const FRAMEPLACE&,const FRAMEPLACE&);
void Translate(MATRIX&,const FRAMEPLACE&);

CArchive& AFXAPI operator << (CArchive&,const FRAMEPLACE&);
CArchive& AFXAPI operator >> (CArchive&,FRAMEPLACE&);

CDumpContext& AFXAPI operator << (CDumpContext&,const FRAMEPLACE&);
