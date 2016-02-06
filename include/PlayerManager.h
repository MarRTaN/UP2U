#pragma once

#include "BitKinect.h"
#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"

#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
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
		void				readParams(Bit::JsonTree* tree, Bit::ParamsRef params);

		void				threadUpdate();
		void				shutdown();
		bool				isMirror();
		void				setupUsers();
		void				updateUsers();
		void				draw();				//draw video from kinect
		bool				isKinectReady();
		Surface				getColorSurface();
		Surface				getDepthSurface();
		bool				isKinectDebugMode = false;
		bool				isDetected = false;

		vector<Person>		getPersons();

	private:
		Bit::Config					config_;
		vector<Person>				persons_;
		float						userDetectRangeMin_;
		float						userDetectRangeMax_;
		list<KinectUser*>			kinectUsers_;

		//UserHead Frame
		float				colorX_;
		float				colorY_;

		float				depthX_;
		float				depthY_;

		float				spanX_;
		float				spanCenter_;

		float				faceFrameRatioX_;
		float				faceFrameRatioY_;

		float				faceDownPercentage_;
};