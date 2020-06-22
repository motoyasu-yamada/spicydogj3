#pragma once

#include "cnt\Cnt.h"

class Stage;
class KeyFrame;
class SpicydogDoc;
class StageView;
class TrackerEx;

class StageCnt : public TViewControler<StageView,SpicydogDoc> {
protected:
	Stage* GetTargetStage();
	BOOL SetTracker(TrackerEx& tracker);
	KeyFrame* GetSelKeyFrame();
};