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
		string					playerId_;
		vector<depthVec>		depthPoints_;
		vector<float>			depthVals_;
		Vec3f					transVec_;
		Vec3f					rotateVec_;
};