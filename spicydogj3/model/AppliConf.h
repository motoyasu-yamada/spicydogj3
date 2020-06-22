#pragma once

#include "sdom/SDOMSerializable.h"
#include "sdom/SDOMBuffer.h"
#include "model/build/Buildable.h"
#include <ltwrappr.h>

class Builder;

const LPCTSTR APPLI_TYPE_JPHONEF  = "jphone-f";
const LPCTSTR APPLI_TYPE_JPHONEFM = "jphone-fm";
const LPCTSTR APPLI_TYPE_JPHONEG  = "jphone-g";
const LPCTSTR APPLI_TYPE_JPHONEGM = "jphone-gm";

/** アプリ設定情報 */
class AppliConf
:	public CObject,
	public SDOMSerializable,
	public Buildable {
public:
	CString  title;
	CString  vendor;
	CString  comment;
	CString  appliType;
	CSize    stageSize;
	COLORREF bkgndColor;
	COLORREF borderColor;
	int      frameRate;
	BOOL     loopPlay;
	int      layerOverClock;
	LBuffer  icon;

	AppliConf();
	void CopyFrom(const AppliConf*);
	virtual void AssertValid() const;
	virtual void Serialize(SDOMArchive*);
	virtual void Build(Builder*) const;
private:
	static BOOL IsValidTitle(LPCTSTR);
	static BOOL IsValidVendor(LPCTSTR);
	static BOOL IsValidComment(LPCTSTR);
	static BOOL IsValidAppliType(LPCTSTR);
	static BOOL IsValidStageSize(CSize);
	static BOOL IsValidLayerOverClock(int);
};

/** 上書き時計表示方式 */
enum LAYER_OVER_CLOCK {
	CLOCK_NONE,
	DIGITAL_CLOCK_SMALL,
	DIGITAL_CLOCK_LARGE,
};

const int FPS_INTERVAL_MIN = 1;
const int FPS_INTERVAL_MAX = 10;