#include "stdafx.h"
#include "Action.h"

#ifdef _DEBUG
#include <atlconv.h>
#endif
#include "sdom/sdomn.h"
#include "sdom/SDOMException.h"
#include "ActionFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace ActionImpl {
	const LPCSTR TNAME_PLAY_ACTION = "PlayAction";
	const LPCSTR TNAME_STOP_ACTION = "StopAction";
	const LPCSTR TNAME_QUIT_ACTION = "QuitAction";
	const LPCSTR TNAME_SOUND_PLAY_ACTION = "SoundPlayAction";
	const LPCSTR TNAME_SOUND_STOP_ACTION = "SoundStopAction";
	const LPCSTR TNAME_JUMP_ACTION = "JumpAction";
	const LPCSTR TNAME_SET_VIBRATION_ACTION = "SetVibrationAction";
	const LPCSTR TNAME_SET_BACKLIGHT_ACTION = "SetBacklightAction";
	const LPCSTR TNAME_SET_BACKGROUND_COLOR_ACTION = "SetBackgroundColorAction";
	const LPCSTR TNAME_SET_SOFTKEY_ACTION = "SetSoftkeyAction";
	const LPCSTR TNAME_DELAY_JUMP_ACTION  = "DelayJumpAction";
	const LPCSTR TNAME_RANDOM_JUMP_ACTION = "RandomJumpAction";
};

using namespace ActionImpl;

Action::Action() {
}

Action::~Action() {
}

CString Action::GetActionName() const {
	CString string;
	VERIFY(string.LoadString(GetActionNameResID()));
	return string;
}

SDOMSerializable* Action::factory(LPCWSTR name) {
	Action* (*actionFactory)();

	if (wcscmp(name,WSDOMN_PLAY_ACTION) == 0) {
		actionFactory = ActionFactory::NewPlayAction;

	} else if (wcscmp(name,WSDOMN_STOP_ACTION) == 0) {
		actionFactory = ActionFactory::NewStopAction;

	} else if (wcscmp(name,WSDOMN_QUIT_ACTION) == 0) {
		actionFactory = ActionFactory::NewQuitAction;
	
	} else if (wcscmp(name,WSDOMN_SOUND_PLAY_ACTION) == 0) {
		actionFactory = ActionFactory::NewSoundPlayAction;

	} else if (wcscmp(name,WSDOMN_SOUND_STOP_ACTION) == 0) {
		actionFactory = ActionFactory::NewSoundStopAction;

	} else if (wcscmp(name,WSDOMN_JUMP_ACTION) == 0) {
		actionFactory = ActionFactory::NewJumpAction;

	} else if (wcscmp(name,WSDOMN_SET_VIBRATION_ACTION) == 0) {
		actionFactory = ActionFactory::NewSetVibrationAction;

	} else if (wcscmp(name,WSDOMN_SET_BACKLIGHT_ACTION) == 0) {
		actionFactory = ActionFactory::NewSetBacklightAction;

	} else if (wcscmp(name,WSDOMN_SET_BACKGROUND_COLOR_ACTION) == 0) {
		actionFactory = ActionFactory::NewSetBackgroundColorAction;

	} else if (wcscmp(name,WSDOMN_SET_SOFTKEY_ACTION) == 0) {
		actionFactory = ActionFactory::NewSetSoftkeyAction;

	} else if (wcscmp(name,WSDOMN_DELAY_JUMP_ACTION) == 0) {
		actionFactory = ActionFactory::NewDelayJumpAction;

	} else if (wcscmp(name,WSDOMN_RANDOM_JUMP_ACTION) == 0) { 
		actionFactory = ActionFactory::NewRandomJumpAction;

	} else {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}

#if defined(_DEBUG)
	USES_CONVERSION;
	typedef std::auto_ptr<Action> AP;
	AP action = AP(actionFactory());
	LPCWSTR name2 = namer(action.get());
	ASSERT(wcscmp(name2,name)== 0);
#endif

	return actionFactory();
}

LPCWSTR Action::namer(SDOMSerializable* object) {
	ASSERT(AfxIsValidAddress(object,sizeof SDOMSerializable));
	CString name = object->getTypeName();

	if (name == TNAME_PLAY_ACTION) {
		return WSDOMN_PLAY_ACTION;

	} else if (name == TNAME_STOP_ACTION) {
		return WSDOMN_STOP_ACTION;

	} else if (name == TNAME_QUIT_ACTION) {
		return WSDOMN_QUIT_ACTION;

	} else if (name == TNAME_SOUND_PLAY_ACTION) {
		return WSDOMN_SOUND_PLAY_ACTION;

	} else if (name == TNAME_SOUND_STOP_ACTION) {
		return WSDOMN_SOUND_STOP_ACTION;

	} else if (name == TNAME_JUMP_ACTION) {
		return WSDOMN_JUMP_ACTION;

	} else if (name == TNAME_SET_VIBRATION_ACTION) {
		return WSDOMN_SET_VIBRATION_ACTION;

	} else if (name == TNAME_SET_BACKLIGHT_ACTION) {
		return WSDOMN_SET_BACKLIGHT_ACTION;

	} else if (name == TNAME_SET_BACKGROUND_COLOR_ACTION) {
		return WSDOMN_SET_BACKGROUND_COLOR_ACTION;

	} else if (name == TNAME_SET_SOFTKEY_ACTION) {
		return WSDOMN_SET_SOFTKEY_ACTION;

	} else if (name == TNAME_DELAY_JUMP_ACTION) {
		return WSDOMN_DELAY_JUMP_ACTION;

	} else if (name == TNAME_RANDOM_JUMP_ACTION) { 
		return WSDOMN_RANDOM_JUMP_ACTION;

	} else {
		SDOMException::throwException(SDOMException::CAUSE_CORRUPTED);
	}
}
