#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "BitVideo.h"
#include "BitDisplayArea.h"
#include "BitAudio.h"
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

	Font					fontS_;
	Font					fontM_;
	Font					fontH_;
	
	Bit::Audio				bgm_;

	Bit::DisplayArea		displayArea_;
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


	float					param_startGaugeX_;
	float					param_startGaugeY_;
	float					param_endGaugeX_;
	float					param_endGaugeY_;

	float					param_scale_;
	float					param_shiftX_;
	float					param_shiftY_;
	float					param_calShift_;
	float					param_fontSize_;

	float					param_phubTimeSX_;
	float					param_talkTimeSX_;
	float					param_phubTimeMX_;
	float					param_talkTimeMX_;
	float					param_phubTimeHX_;
	float					param_talkTimeHX_;

	float					param_bothTimeSY_;
	float					param_bothTimeMY_;
	float					param_bothTimeHY_;


	string					gaugePath_;

	string					seriousPath_;
	string					seriousLeftPath_;
	string					seriousRightPath_;

	string					seriousMalePath_;
	string					seriousLeftMalePath_;
	string					seriousRightMalePath_;

	string					seriousFemalePath_;
	string					seriousLeftFemalePath_;
	string					seriousRightFemalePath_;


	string					smilePath_;
	string					smileLeftPath_;
	string					smileRightPath_;

	string					smileMalePath_;
	string					smileLeftMalePath_;
	string					smileRightMalePath_;

	string					smileFemalePath_;
	string					smileLeftFemalePath_;
	string					smileRightFemalePath_;

	string					turnLeftPath_;
	string					turnRightPath_;

	string					turnLeftMalePath_;
	string					turnRightMalePath_;

	string					turnLeftFemalePath_;
	string					turnRightFemalePath_;

	Texture					gaugeTexture_;
	Texture					smileTexture_;
	Texture					smileLeftTexture_;
	Texture					smileRightTexture_;

	Texture					smileMaleTexture_;
	Texture					smileLeftMaleTexture_;
	Texture					smileRightMaleTexture_;

	Texture					smileFemaleTexture_;
	Texture					smileLeftFemaleTexture_;
	Texture					smileRightFemaleTexture_;

	Texture					seriousTexture_;
	Texture					seriousLeftTexture_;
	Texture					seriousRightTexture_;

	Texture					seriousMaleTexture_;
	Texture					seriousLeftMaleTexture_;
	Texture					seriousRightMaleTexture_;

	Texture					seriousFemaleTexture_;
	Texture					seriousLeftFemaleTexture_;
	Texture					seriousRightFemaleTexture_;

	Texture					turnLeftTexture_;
	Texture					turnRightTexture_;

	Texture					turnLeftMaleTexture_;
	Texture					turnRightMaleTexture_;

	Texture					turnLeftFemaleTexture_;
	Texture					turnRightFemaleTexture_;
};