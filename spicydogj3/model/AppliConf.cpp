#include "stdafx.h"
#include "resource.h"
#include "AppliConf.h"

#include "sdom/sdomn.h"
#include "sdom/SDOMArchive.h"
#include "sdom/SDOMException.h"
#include "model/build/Builder.h"
#include "utility/utf8.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AppliConf::AppliConf() {
	title.LoadString(IDS_DEFAULT_APPLI_TITLE);
	vendor.LoadString(IDS_DEFAULT_APPLI_VENDOR);
	comment.LoadString(IDS_DEFAULT_APPLI_COMMENT);
	appliType      = APPLI_TYPE_JPHONEF;
	stageSize      = CSize(120,120);
	bkgndColor     = RGB(0xFF,0xFF,0xFF);
	borderColor    = RGB(0xFF,0xFF,0xFF);
	frameRate      = 10;
	loopPlay       = false;
	layerOverClock = DIGITAL_CLOCK_SMALL;
}

void AppliConf::CopyFrom(const AppliConf* conf){
	ASSERT_VALID(conf);

	title       = conf->title;
	vendor      = conf->vendor;
	comment     = conf->comment;
	icon.Copy(const_cast<LBuffer*>(&conf->icon));
	appliType   = conf->appliType;
	stageSize   = conf->stageSize;
	bkgndColor  = conf->bkgndColor;
	borderColor = conf->borderColor;
	frameRate   = conf->frameRate;
	loopPlay       = conf->loopPlay;
	layerOverClock = conf->layerOverClock;

	ASSERT_VALID(this);
}

void AppliConf::AssertValid() const {
	IsValidTitle(title);
	IsValidVendor(vendor);
	IsValidComment(comment);
	IsValidAppliType(appliType);
	IsValidStageSize(stageSize);
	IsValidLayerOverClock(layerOverClock);
}

void AppliConf::Serialize(SDOMArchive* p) {
	ASSERT_VALID(this);

	p->SerializeString(WSDOMN_TITLE,           title);
	p->SerializeString(WSDOMN_APPLI_TYPE,      appliType);
	p->SerializeSIZE(WSDOMN_STAGE_SIZE,      stageSize);
	p->SerializeCOLORREF(WSDOMN_BACKGROUND_COLOR,bkgndColor);
	p->SerializeCOLORREF(WSDOMN_BORDER_COLOR,    borderColor);
	p->SerializeINT(WSDOMN_FRAME_RATE,      frameRate);
	p->SerializeBOOL(WSDOMN_LOOP_PLAY,       loopPlay);
	
	CString value;
	if (p->IsStoring()) {
		switch(layerOverClock) {
		case CLOCK_NONE:
			value = ASDOMV_CLOCK_NONE;
			break;
		case DIGITAL_CLOCK_LARGE:
			value = ASDOMV_DIGITAL_CLOCK_LARGE;
			break;
		case DIGITAL_CLOCK_SMALL:
			value = ASDOMV_DIGITAL_CLOCK_SMALL;
			break;
		}
	}
	p->SerializeString(WSDOMN_SCREEN_CLOCK_TYPE,value);
	if (p->IsLoading()) {
		if (value == ASDOMV_CLOCK_NONE) {
			layerOverClock = CLOCK_NONE;
		} else if (value == ASDOMV_DIGITAL_CLOCK_LARGE){
			layerOverClock = DIGITAL_CLOCK_LARGE;
		} else if (value == ASDOMV_DIGITAL_CLOCK_SMALL) {
			layerOverClock = DIGITAL_CLOCK_SMALL;
		} else {
			SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
		}
	}

	p->SerializeString(WSDOMN_VENDOR,          vendor);
	p->SerializeBuffer(WSDOMN_ICON,            icon);
	p->SerializeString(WSDOMN_COMMENT,         comment);

	ASSERT_VALID(this);
}

void AppliConf::Build(Builder* builder) const {
	ASSERT(AfxIsValidAddress(builder,sizeof Builder));
	
	BuildContext context(builder,IDS_BUILD_CONTEXT_APPLICONF);

	switch(builder->GetBuildStep()) {
	case Builder::BUILDSTEP_FIRST:
		if (title.GetLength() == 0) {
			builder->NotifyErr(IDS_EN_NO_TITLE);
		}
		if (16 < ConvertToUTF8(title).GetLength()) {
			builder->NotifyErr(IDS_EN_TITLE_OVER_SIZE);
		}
		if (16 < ConvertToUTF8(vendor).GetLength()) {
			builder->NotifyErr(IDS_EN_VENDOR_OVER_SIZE);
		}
		break;
	case Builder::BUILDSTEP_LAST:
		builder->WriteXY(stageSize.cx,stageSize.cy);
		builder->WriteUI8(frameRate);
		builder->WriteColor(bkgndColor);
		builder->WriteColor(borderColor);
		builder->WriteUI8(loopPlay ? 0xFF : 0x00);
		builder->WriteUI8(layerOverClock);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

BOOL AppliConf::IsValidTitle(LPCTSTR sjis) {
	ASSERT(AfxIsValidString(sjis));
	CString utf = ConvertToUTF8(sjis);
	return (0 < utf.GetLength() && utf.GetLength() < 16);
}

BOOL AppliConf::IsValidVendor(LPCTSTR sjis) {
	ASSERT(AfxIsValidString(sjis));
	CString utf = ConvertToUTF8(sjis);
	return (0 < utf.GetLength() && utf.GetLength() < 16);
}
BOOL AppliConf::IsValidComment(LPCTSTR sjis) {
	ASSERT(AfxIsValidString(sjis));
	CString utf = ConvertToUTF8(sjis);
	return (0 < utf.GetLength() && utf.GetLength() < 16);
}

BOOL AppliConf::IsValidAppliType(LPCTSTR type) {
	return _tcscmp(type,ASDOMV_JPHONEF) == 0 &&
		   _tcscmp(type,ASDOMV_JPHONEG) == 0 &&
		   _tcscmp(type,ASDOMV_JPHONEFM) == 0 &&
		   _tcscmp(type,ASDOMV_JPHONEGM) == 0 
		   ; 
}

BOOL AppliConf::IsValidStageSize(CSize) { /** @todo */ return false; }

BOOL AppliConf::IsValidLayerOverClock(int clock) {
	return clock == DIGITAL_CLOCK_SMALL ||
		   clock == DIGITAL_CLOCK_LARGE ||
		   clock == CLOCK_NONE;
}
