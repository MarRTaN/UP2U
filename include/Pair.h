#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "BitVideo.h"
#include "cinder/gl/gl.h"
#include "Person.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class Pair{
	public:
		Person		person;
		Bit::Video	chatboxVid;
		Bit::Video	bubbleVid;
		bool		isNewPair = true;
		Vec2f		lastCenter = Vec2f(1, 1);
};