#pragma once

#include "BitKinect.h"
#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/Capture.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"
#include "cinder/Camera.h"
#include "cinder/ImageIo.h"
#include "rapidjson\document.h"
#include "CinderOpenCV.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Person.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <direct.h>



#include "Config.h"

using namespace Bit;
using namespace ci::app;
using namespace ci::gl;

class PlayerManager : public Kinect{

	struct Skeleton{
		int		id;
		Vec3f	position = Vec3f(0, 0, 0);
	};

	struct faceData{
		Vec2i	pos;
		double	count = -1;
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
		bool				isDrawface = false;
		bool				isDetected = false;

		void				moveMotorUp();
		void				moveMotorDown();

		vector<Person>		getPersons();
		void				setUsers();

		faceData			getCentroid(int type, int idNow, cv::Mat colorMat, cv::Rect faceRect, cv::Scalar minColor, cv::Scalar maxColor, cv::Size blurSize, Vec2f threadhold);

	private:
		CaptureRef					mCapture_;
		TextureRef					mTexture_;
		Surface						mSurface_;
		Bit::Config					config_;
		vector<Person>				persons_;
		float						userDetectRangeMin_;
		float						userDetectRangeMax_;
		list<KinectUser*>			kinectUsers_;
		vector<Skeleton>			users_;
		bool						motorStatus_ = false;
		int							selectedCamera_;
		string						saveImagePath_;
		string						saveCapturePath_;

		int					motorAngle_ = -60;
		int					facePixelMultiply = 2;
		
		float				videoW = 0;
		float				videoH = 0;

		Texture				backgroundTexture_;
		Surface				background_;
		bool				isMoving_ = false;
		int					motorUpCount_ = 0;
		int					motorDownCount_ = 0;
		int					motorAngleUp_ = -50;
		int					motorAngleDown_ = -60;

		//UserHead Frame
		float				colorX_;
		float				colorY_;

		Vec2f				shift_;

		float				spanX_;
		float				spanCenter_;

		float				faceFrameRatioX_;
		float				faceFrameRatioY_;

		float				faceDownPercentage_;

		int					countForDelay_ = 0;

		//boundary
		Rectf				frameBound;
		Rectf				headBound;
		Rectf				frameRatio_;

		//param
		float				angleLookLeft_ = 0;
		float				angleLookRight_ = 0;
		int					smoothMeanCount_ = 10;

		int					captureDelay_ = 10000000;
};