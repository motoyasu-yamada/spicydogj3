#pragma once

class Action;

class ActionFactory {
public:
	static Action* NewPlayAction();
	static Action* NewStopAction();
	static Action* NewQuitAction();
	static Action* NewSoundPlayAction();
	static Action* NewSoundStopAction();
	static Action* NewJumpAction();

	static Action* NewSetVibrationAction();
	static Action* NewSetBacklightAction();
	static Action* NewSetBackgroundColorAction();
	static Action* NewSetSoftkeyAction();
	static Action* NewDelayJumpAction();
	static Action* NewRandomJumpAction();
};
