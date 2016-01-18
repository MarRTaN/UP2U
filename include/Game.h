#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "BitDisplayArea.h"
#include "IdleStage.h"
#include "ReadyStage.h"
#include "GameplayStage.h"
#include "EndingStage.h"
#include "Config.h"
#include "Actions.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class Game{

public:
	void				setup();
	void				update();
	void				draw();
	void				drawDebugMode();
	void				reset();
	void				goNextStage();
	int					getCurrentStage();

protected:
	string					gameId_;
	vector<Actions::Action>	allActionsList_;
	vector<int>				randomedIdList_;
	int						currentStageId_;
	IdleStage				idleStage_;
	ReadyStage				readyStage_;
	GameplayStage			gameplayStage_;
	EndingStage				endingStage_;
	Log						log_;
	Vec3f					playerRotation_;
	Vec3f					playerTranslation_;
	bool					isDebugMode_;
};