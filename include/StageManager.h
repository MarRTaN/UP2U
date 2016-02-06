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
	void			drawDebugMode();

	void			readConfig(Bit::JsonTree* tree);
	void			readParams(Bit::JsonTree* tree, Bit::ParamsRef params);

	bool			isDebugMode = false;

private:
	int							currentStageId_;
	int							lastStageId_;
	bool						isDetected_;
	vector<Person>				persons_;
	AdvertisingStage			advertisingStage_;
	DetectingStage				detectingStage_;
	Log							log_;
	Config						config_;

};