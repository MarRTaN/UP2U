#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class Person{
	public:
		int			id = -1;
		Vec2f		center = Vec2f(0, 0);
		Rectf		position = Rectf(0, 0, 0, 0);
		Surface		faceSurface;
		bool		isLookUp = false;
		bool		isActive = false;
		int			bufferCount = 0;
		int			bufferDelay = 40;
};