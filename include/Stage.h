#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "BitVideo.h"
#include "BitDisplayArea.h"
#include "Config.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class Stage{

public:
	void				setup();
	void				update();
	void				draw();
	void				drawDebugMode();
	void				reset();
	bool				isStageChanged();
	void				readConfig(Bit::JsonTree* tree);

protected:
	Bit::Video			stageVid_;
	float				timeCount_ = 0;
	float				timeOut_;
	float				startTime_;
	bool				stageChanged_ = false;
	Bit::DisplayArea	displayArea_;
	bool				isDebugMode_ = false;
};