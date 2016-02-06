#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "BitVideo.h"
#include "Up2UTypeDef.h"
#include "Config.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace Bit;

class Pool{
public :
	typedef struct Config {
		Bit::JsonTree*		chatboxMMTree;
		Bit::JsonTree*		chatboxMFTree;
		Bit::JsonTree*		chatboxFMTree;
		Bit::JsonTree*		chatboxFFTree;
		Bit::JsonTree*		chatboxMXTree;
		Bit::JsonTree*		chatboxFXTree;

		Bit::JsonTree*		bubbleMMTree;
		Bit::JsonTree*		bubbleMFTree;
		Bit::JsonTree*		bubbleFMTree;
		Bit::JsonTree*		bubbleFFTree;
		Bit::JsonTree*		bubbleMXTree;
		Bit::JsonTree*		bubbleFXTree;
	};

	void		setup();
	void		loadChatBox();
	void		loadBubble();
	Bit::Video	getChatboxVid(int gender1, int gender2);
	Bit::Video	getBubbleVid(int gender1, int gender2); 
	void		readConfig(Bit::JsonTree* tree);
	

private :

	vector<Bit::Video>		chatboxMMList_;
	vector<Bit::Video>		chatboxMFList_;
	vector<Bit::Video>		chatboxFMList_;
	vector<Bit::Video>		chatboxFFList_;
	vector<Bit::Video>		chatboxMXList_;
	vector<Bit::Video>		chatboxFXList_;

	vector<Bit::Video>		bubbleMMList_;
	vector<Bit::Video>		bubbleFMList_;
	vector<Bit::Video>		bubbleMFList_;
	vector<Bit::Video>		bubbleFFList_;
	vector<Bit::Video>		bubbleMXList_;
	vector<Bit::Video>		bubbleFXList_;


	Config					config_;

};