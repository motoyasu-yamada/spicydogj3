#include "stdafx.h"
#include "ActionFactory.h"

#include "PlayAction.h"
#include "StopAction.h"
#include "QuitAction.h"
#include "SoundPlayAction.h"
#include "SoundStopAction.h"
#include "JumpAction.h"
#include "SetVibrationAction.h"
#include "SetBacklightAction.h"
#include "SetBackgroundColorAction.h"
#include "SetSoftkeyAction.h"
#include "DelayJumpAction.h"
#include "RandomJumpAction.h"

#ifdef _DEBUG
#undef THIS_FILE
char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Action* ActionFactory::NewPlayAction(){
	return new PlayAction;
}
Action* ActionFactory::NewStopAction(){
	return new StopAction;
}
Action* ActionFactory::NewQuitAction(){
	return new QuitAction;
}
Action* ActionFactory::NewSoundPlayAction(){
	return new SoundPlayAction;
}
Action* ActionFactory::NewSoundStopAction(){
	return new SoundStopAction;
}
Action* ActionFactory::NewJumpAction(){
	return new JumpAction;
}
Action* ActionFactory::NewSetVibrationAction(){
	return new SetVibrationAction;
}
Action* ActionFactory::NewSetBacklightAction(){
	return new SetBacklightAction;
}
Action* ActionFactory::NewSetBackgroundColorAction(){
	return new SetBackgroundColorAction;
}
Action* ActionFactory::NewSetSoftkeyAction(){
	return new SetSoftkeyAction;
}
Action* ActionFactory::NewDelayJumpAction(){
	return new DelayJumpAction;
}
Action* ActionFactory::NewRandomJumpAction(){
	return new RandomJumpAction;
}