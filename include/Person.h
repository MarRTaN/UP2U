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
		Vec2f			kinectCenter = Vec2f(0, 0); //The real center from kinect
		Vec2i			hairCentroid = Vec2f(0, 0);
		Vec2i			faceCentroid = Vec2f(0, 0);
		vector<Vec2f>   kinectCenterMean;
		float			angle = 0;
		vector<float>   angleMean;
		int				lostCount = 0;
		int				idleCount = 0;

		Rectf			position = Rectf(0, 0, 0, 0);
		Rectf			faceRectColorDebug = Rectf(0, 0, 0, 0);
		Surface			faceSurface;
		bool			isLookUp = false;
		bool			isActive = false;
		int				bufferCount = 0;
		int				bufferDelay = 40;
		int				gender = MALE;

		bool			isPersonLost();
		void			calAngleMean();
		void			calCenterMean();
};