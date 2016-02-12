#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "BitVideo.h"
#include "BitDisplayArea.h"
#include "Person.h"
#include "Pool.h"
#include "Pair.H"
#include "Up2UTypeDef.h"
#include "Config.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class DetectingStage{
public:
	typedef struct Config{
		Bit::Video			stageVid;
	};

	void				readConfig(Bit::JsonTree* tree);
	void				readParams(Bit::JsonTree* tree, Bit::ParamsRef params);
	void				setup();
	void				update();
	void				draw();
	void				checkNewPersons();
	void				eraseOldPairs();
	Pair				pairUp(int orderNo);
	void				drawDebugMode();
	void				reset();
	void				setPersons(vector<Person> persons);
	Bit::Video			getChatboxVid(int gender1, int gender2);
	Bit::Video			getBubbleVid(int gender1, int gender2);
	//Bit::Video			getRandomVid(bool isLookUp);


private:

	Pool					pool_;
	vector<Pair>			pairs_;
	Bit::DisplayArea		displayArea_;
	Bit::Video				stageVid_;
	vector<Person>			persons_;
	float					timeCount_ = 0;
	float					param_scale_;
	float					param_shiftX_;
	float					param_shiftY_;
	Config					config_;
};