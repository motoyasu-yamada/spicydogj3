#pragma once
//////////////////////////////////////////////////////////////////////////
//	�����[SpicyDog]�̃\�[�X�R�[�h�ł��B
//	���쌠 �X�p�C�V�[�\�t�g�������
//	���̃\�[�X�R�[�h�Ɋւ��邷�ׂĂ̌����̓X�p�C�V�[�\�t�g���ۗL���܂��B
//
//	This is a part of SpicyDog  2001.
//	Copyright (C) 2000 Spicysoft Corporation.
//	All rights reserved.
//////////////////////////////////////////////////////////////////////////

#include "SDBASE.H"
#include "lib\VerSerializer.h"

/**
	�z�u
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
		//	V2B	  ... 0�`360�x
		//	V2B2  ... ���W�A��

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
