#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "BitVideo.h"
#include "BitDisplayArea.h"
#include "Person.h"
#include "Pool.h"
#include "Up2UTypeDef.h"
#include "Config.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class DetectingStage{
public:
	typedef struct Config{
		Bit::Video			stageVid;
	};

	void				readConfig(Bit::JsonTree* tree);

	void				setup();
	void				update();
	void				draw();
	void				drawDebugMode();
	void				reset();
	void				setPersons(vector<Person> persons);
	Bit::Video			getVid(int gender1, int gender2, int type);


private:

	Pool					pool_;

	Bit::DisplayArea		displayArea_;
	Bit::Video				stageVid_;

	vector<Person>			persons_;
	float					timeCount_ = 0;
	Config					config_;
};