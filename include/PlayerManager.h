#pragma once

#include "BitKinect.h"
#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"
#include "cinder/Camera.h"
#include "Person.h"

#include "Config.h"

using namespace Bit;
using namespace ci::app;
using namespace ci::gl;

class PlayerManager : public Kinect{

public:

	void				readConfig(Bit::JsonTree* tree);

	void				threadUpdate();
	void				shutdown();
	bool				isMirror();
	void				draw();				//draw video from kinect
	bool				isKinectReady();
	Surface				getSurface();
	void				getPersonHead();
	bool				isKinectDebugMode = false;

private:
	vector<Person>		persons;
	float				userDetectRangeMin_;
	float				userDetectRangeMax_;
};