#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "BitVideo.h"
#include "BitDisplayArea.h"
#include "Person.h"
#include "Up2UTypeDef.h"
#include "Config.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class DetectingStage{

public:
	void				readConfig(Bit::JsonTree* tree);
	void				readParams(Bit::JsonTree* tree, Bit::ParamsRef params);
	void				setup();
	void				update();
	void				draw();
	void				drawDebugMode();
	void				reset();
	void				setPersons(vector<Person> persons);
	void				checkAction();
	void				addColor(Vec3f added);
	void				addTimePhub(float t);
	void				addTimeTalk(float t);
	void				calPhubTime(int personId);
	void				calTalkTime(int personId);
	void				resetTimeTalk(int personId);
	void				resetTimePhub(int personId);
	void				minimizeFaceColor(int personId);
	void				maximizeFaceColor(int personId);
	void				normalizeFaceColor(int personId);
	int					idToIndex(int id);
	bool				isDebugMode = false;

private:

	Font					font_;
	Bit::DisplayArea		displayArea_;
	Bit::Video				guageVid_;
	vector<Person>			persons_;
	Vec3f					hsv_ = Vec3f(0,0,0);
	Vec3f					rgb_;
	float					timeCount_ = 0;
	int						timePhub_ = 1;
	int						timeTalk_ = 1;
	float					timeRatio_;
	float					currentRatio_ = 0.5f;

	vector<float>			facesColor_;
	vector<float>			startTimeTalk_;
	vector<bool>			isStartTalking_;
	vector<float>			startTimePhub_;
	vector<bool>			isStartPhubbing_;

	float					param_scale_;
	float					param_shiftX_;
	float					param_shiftY_;
	float					param_calShift_;
	float					param_fontSize_;
	float					param_phubTimeX_;
	float					param_talkTimeX_;
	float					param_bothTimeY_;

	string					seriousPath_;
	string					seriousLeftPath_;
	string					seriousRightPath_;

	string					smilePath_;
	string					smileLeftPath_;
	string					smileRightPath_;

	string					turnLeftPath_;
	string					turnRightPath_;

	Texture					smileTexture_;
	Texture					smileLeftTexture_;
	Texture					smileRightTexture_;

	Texture					seriousTexture_;
	Texture					seriousLeftTexture_;
	Texture					seriousRightTexture_;

	Texture					turnLeftTexture_;
	Texture					turnRightTexture_;

};