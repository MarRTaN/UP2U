#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Up2UTypeDef.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class Person{
	public:
		inline bool operator<(const Person& p1)
		{
			return (center.x < p1.center.x);
		}

		int				id = -1;
		Vec2f			center = Vec2f(0, 0);
		Vec2f			lastCenter = Vec2f(0, 0);
		int				lostCount = 0;

		Rectf			position = Rectf(0, 0, 0, 0);
		Surface			faceSurface;
		bool			isLookUp = false;
		bool			isActive = false;
		int				bufferCount = 0;
		int				bufferDelay = 40;
		int				gender = MALE;
		deque<Vec2f>	centerBuffer;

		Vec2f			getCenterMean();
		bool			isPersonLost();

		int				segtion;
		int				look;
};