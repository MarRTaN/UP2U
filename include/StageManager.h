#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Log.h"
#include "Config.h"
#include "DetectingStage.h"
#include "AdvertisingStage.h"
#include "Person.h"
#include "Up2UTypeDef.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class StageManager : public Configurable{
public:
	void			setup();
	void			update();
	void			draw();
	void			setPlayerDetection(bool isDetecting);
	void			checkDetection();
	void			addLog(string logInfo);
	int				getCurrentStage();
	void			setPersons(vector<Person> persons);
	void			addColor(Vec3f added);
	void			addTimePhub(float t);
	void			addTimeTalk(float t);
	void			drawDebugMode();

	void			readConfig(Bit::JsonTree* tree);
	void			readParams(Bit::JsonTree* tree, Bit::ParamsRef params);

	bool			isDebugMode = false;

private:
	int							currentStageId_;
	int							lastStageId_;
	float						startTime_ = 0;
	float						timeOut_ = 1;
	float						switchStageDelay_;
	bool						isDetected_ = false;
	vector<Person>				persons_;
	AdvertisingStage			advertisingStage_;
	DetectingStage				detectingStage_;
	Log							log_;
	Config						config_;

	float						personLostSwitchTimeDelay_ = 0;

};