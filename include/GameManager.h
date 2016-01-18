#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Actions.h"
#include "Log.h"
#include "Config.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class GameManager{
public:
	void setup();
	void update();
	void draw();
	void loadAction(Bit::JsonTree* action);
	vector<int> createRandomedList();
	void createNewGame();
	void destroyGame();

private:
	vector<Actions::Action>		allActions_;
	vector<int>					selectedId_;
	Config						config_;
	bool						isDebugMode_;
	Log							log_;

};