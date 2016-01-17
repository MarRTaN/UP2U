#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class Person{

	typedef struct depthVec{
		float p;
		float q;
		float c = -1;
		float r;
		float s;
		float t;
	};

	public:
		void					calDOF();
		Vec3f					getTransVec();
		Vec3f					getRotateVec();
		void					addDepthPoints(float p, float q, float c, float r, float s, float t);
		void					clearDepthPoints();

	private:
		string					playerId;
		vector<depthVec>		depthPoints;
		vector<float>			depthVals;
		Vec3f					transVec;
		Vec3f					rotateVec;
};