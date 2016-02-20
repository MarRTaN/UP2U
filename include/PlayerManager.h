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

	struct Skeleton{
		int		id;
		Vec3f	position = Vec3f(0, 0, 0);
	};

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

		void				moveMotorUp();
		void				moveMotorDown();

		vector<Person>		getPersons();
		void				setUsers();
		bool				isBackground(Vec2i pixel, Vec3f color);
		void				setBackground();

	private:
		Bit::Config					config_;
		vector<Person>				persons_;
		float						userDetectRangeMin_;
		float						userDetectRangeMax_;
		list<KinectUser*>			kinectUsers_;
		vector<Skeleton>			users_;
		bool						motorStatus_ = false;

		int					motorAngle_ = -20;
		int					facePixelMultiply = 2;

		float				videoW = 0;
		float				videoH = 0;

		Texture				backgroundTexture_;
		Surface				background_;

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

		int					countForDelay_ = 0;

		//boundary
		Rectf				frameBound;
		Rectf				headBound;
		float				frameX1 = 0.1, frameY1 = 0.1, frameX2 = 0.9, frameY2 = 0.6;
};