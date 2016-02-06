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

class AdvertisingStage{
public:
	typedef struct Config{
		Bit::Video			advertisingVid;
	};

	void				readConfig(Bit::JsonTree* tree);

	void				setup();
	void				update();
	void				draw();
	void				reset();
	void				drawDebugMode();

private:
	Bit::Video			advertisingVid_;
	Bit::DisplayArea	displayArea_;
	float				timeCount_ = 0;
	bool				isDebugMode_ = false;
	Config				config_;
};