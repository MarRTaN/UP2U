#include "Pool.h"

void Pool::setup(){
	loadChatBox();
	loadBubble();
	testVid.setup();
}

void Pool::loadChatBox(){
	int i = 1;
	for (Bit::JsonTree::Iter it = config_.chatboxMMTree->begin(); it != config_.chatboxMMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		chatboxMMList_.push_back(tempVid);
	}
	//isCMMPlaying_.resize(chatboxMMList_.size(),false);
	
	for (Bit::JsonTree::Iter it = config_.chatboxMFTree->begin(); it != config_.chatboxMFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		console() << *tempTree << endl;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		chatboxMFList_.push_back(tempVid);
	}
	//isCMFPlaying_.resize(chatboxMFList_.size(), false);

	
	for (Bit::JsonTree::Iter it = config_.chatboxFMTree->begin(); it != config_.chatboxFMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		chatboxFMList_.push_back(tempVid);
	}
	//isCFMPlaying_.resize(chatboxFMList_.size(), false);

	for (Bit::JsonTree::Iter it = config_.chatboxFFTree->begin(); it != config_.chatboxFFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		chatboxFFList_.push_back(tempVid);
	}
	//isCFFPlaying_.resize(chatboxFFList_.size(), false);

	for (Bit::JsonTree::Iter it = config_.chatboxMXTree->begin(); it != config_.chatboxMXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		chatboxMXList_.push_back(tempVid);
	}
	//isCMXPlaying_.resize(chatboxMXList_.size(), false);

	for (Bit::JsonTree::Iter it = config_.chatboxFXTree->begin(); it != config_.chatboxFXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		chatboxFXList_.push_back(tempVid);
	}
	//isCFXPlaying_.resize(chatboxFXList_.size(), false);

}

void Pool::loadBubble(){

	for (Bit::JsonTree::Iter it = config_.bubbleMMTree->begin(); it != config_.bubbleMMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		
		tempVid.setup();
		bubbleMMList_.push_back(tempVid);
	}
	//isBMMPlaying_.resize(bubbleMMList_.size(), false);

	for (Bit::JsonTree::Iter it = config_.bubbleMFTree->begin(); it != config_.bubbleMFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		bubbleMFList_.push_back(tempVid);
	}
	//isBMFPlaying_.resize(bubbleMFList_.size(), false);

	for (Bit::JsonTree::Iter it = config_.bubbleFMTree->begin(); it != config_.bubbleFMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		bubbleFMList_.push_back(tempVid);
	}
	//isBFMPlaying_.resize(bubbleFMList_.size(), false);

	for (Bit::JsonTree::Iter it = config_.bubbleFFTree->begin(); it != config_.bubbleFFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		bubbleFFList_.push_back(tempVid);
	}
	//isBFFPlaying_.resize(bubbleFFList_.size(), false);

	for (Bit::JsonTree::Iter it = config_.bubbleMXTree->begin(); it != config_.bubbleMXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		bubbleMXList_.push_back(tempVid);
	}
	//isBMXPlaying_.resize(bubbleMXList_.size(), false);
	
	
	for (Bit::JsonTree::Iter it = config_.bubbleFXTree->begin(); it != config_.bubbleFXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		tempVid.setup();
		bubbleFXList_.push_back(tempVid);
	}
	//isBFXPlaying_.resize(bubbleFXList_.size(), false);
}


Bit::Video Pool::getChatboxVid(int gender1, int gender2){
	Bit::Video result;
	if (gender1 == MALE && gender2 == MALE){
		/*srand(time(NULL));
		int randomNo = rand() % chatboxMMList_.size();
		result = chatboxMMList_[randomNo];*/
		result = chatboxMMList_[cmmIndex_];
		console() << "MM RANDOM CHATBOX = " << cmmIndex_ << " FROM " << chatboxMMList_.size() << endl;
		if (cmmIndex_ == chatboxMMList_.size() - 1) cmmIndex_ = 0;
		else cmmIndex_++;
	}
	else if (gender1 == MALE && gender2 == FEMALE){
		/*srand(time(NULL));
		int randomNo = rand() % chatboxMFList_.size();*/
		result = chatboxMFList_[cmfIndex_];
		console() << "MF RANDOM CHATBOX = " << cmfIndex_ << " FROM " << chatboxMFList_.size() << endl;
		if (cmfIndex_ == chatboxMFList_.size() - 1) cmfIndex_ = 0;
		else cmfIndex_++;
	}
	else if (gender1 == FEMALE && gender2 == MALE){
		/*srand(time(NULL));
		int randomNo = rand() % chatboxFMList_.size();*/
		result = chatboxFMList_[cfmIndex_];
		console() << "FM RANDOM CHATBOX = " << cfmIndex_ << " FROM " << chatboxFMList_.size() << endl;
		if (cfmIndex_ == chatboxFMList_.size() - 1) cfmIndex_ = 0;
		else cfmIndex_++;
	}
	else if (gender1 == FEMALE && gender2 == FEMALE){
		/*srand(time(NULL));
		int randomNo = rand() % chatboxFFList_.size();*/
		result = chatboxFFList_[cffIndex_];
		console() << "FF RANDOM CHATBOX = " << cffIndex_ << " FROM " << chatboxFFList_.size() << endl;
		if (cffIndex_ == chatboxFFList_.size() - 1) cffIndex_ = 0;
		else cffIndex_++;
	}
	else if (gender1 == MALE && gender2 == UNDEFINED){
		/*srand(time(NULL));
		console() << "SIZE = " << chatboxMXList_.size() << endl;
		int randomNo = rand() % chatboxMXList_.size();*/
		result = chatboxMXList_[cmxIndex_];
		console() << "MX RANDOM CHATBOX = " << cmxIndex_ << " FROM " << chatboxMXList_.size() << endl;
		if (cmxIndex_ == chatboxMXList_.size() - 1) cmxIndex_ = 0;
		else cmxIndex_++;
	}
	else if (gender1 == FEMALE && gender2 == UNDEFINED){
		/*srand(time(NULL));
		int randomNo = rand() % chatboxFXList_.size();*/
		result = chatboxFXList_[cfxIndex_];
		console() << "FX RANDOM CHATBOX = " << cfxIndex_ << " FROM " << chatboxFXList_.size() << endl;
		if (cfxIndex_ == chatboxFXList_.size() - 1) cfxIndex_ = 0;
		else cfxIndex_++;
	}
	return result;
	//return testVid;
}

Bit::Video Pool::getBubbleVid(int gender1, int gender2){

	Bit::Video result;
	if (gender1 == MALE && gender2 == MALE){
		if (bubbleMMList_.size() > 0){
			/*srand(time(NULL));
			int randomNo = rand() % bubbleMMList_.size();*/
			result = bubbleMMList_[bmmIndex_];
			console() << "MM RANDOM BUBBLE = " << bmmIndex_ << " FROM " << bubbleMMList_.size() << endl;
			if (bmmIndex_ == bubbleMMList_.size() - 1) bmmIndex_ = 0;
			else bmmIndex_++;
		}
	}
	else if (gender1 == MALE && gender2 == FEMALE){
		if (bubbleMFList_.size() > 0){
			/*srand(time(NULL));
			int randomNo = rand() % bubbleMFList_.size();*/
			result = bubbleMFList_[bmfIndex_];
			console() << "MF RANDOM BUBBLE = " << bmfIndex_ << " FROM " << bubbleMFList_.size() << endl;
			if (bmfIndex_ == bubbleMFList_.size() - 1) bmfIndex_ = 0;
			else bmfIndex_++;
		}
	}
	else if (gender1 == FEMALE && gender2 == MALE){
		if (bubbleFMList_.size() > 0){
			/*srand(time(NULL));
			int randomNo = rand() % bubbleFMList_.size();*/
			result = bubbleFMList_[bfmIndex_];
			console() << "FM RANDOM BUBBLE = " << bfmIndex_ << " FROM " << bubbleFMList_.size() << endl;
			if (bfmIndex_ == bubbleFMList_.size() - 1) bfmIndex_ = 0;
			else bfmIndex_++;
		}
	}
	else if (gender1 == FEMALE && gender2 == FEMALE){
		if (bubbleFFList_.size() > 0){
			/*srand(time(NULL));
			int randomNo = rand() % bubbleFFList_.size();*/
			result = bubbleFFList_[bffIndex_];
			console() << "FF RANDOM BUBBLE = " << bffIndex_ << " FROM " << bubbleFFList_.size() << endl;
			if (bffIndex_ == bubbleFFList_.size() - 1) bffIndex_ = 0;
			else bffIndex_++;
		}
	}
	else if (gender1 == MALE && gender2 == UNDEFINED){
		if (bubbleMXList_.size() > 0){
			/*srand(time(NULL));
			int randomNo = rand() % bubbleMXList_.size();*/
			result = bubbleMXList_[bmxIndex_];
			console() << "MX RANDOM BUBBLE = " << bmxIndex_ << " FROM " << bubbleMXList_.size() << endl;
			if (bmxIndex_ == bubbleMXList_.size() - 1) bmxIndex_ = 0;
			else bmxIndex_++;
		}
	}
	else if (gender1 == FEMALE && gender2 == UNDEFINED){
		if (bubbleFXList_.size() > 0){
			srand(time(NULL));
			int randomNo = rand() % bubbleFXList_.size();
			result = bubbleFXList_[bfxIndex_];
			console() << "FX RANDOM BUBBLE = " << bfxIndex_ << " FROM " << bubbleFXList_.size() << endl;
			if (bfxIndex_ == bubbleFXList_.size() - 1) bfxIndex_ = 0;
			else bfxIndex_++;
		}
	}
	return result;
	//return testVid;
}

void Pool::readConfig(Bit::JsonTree* tree){
	config_.chatboxMMTree = tree->getChildPtr("chatbox")->getChildPtr("mm");
	config_.chatboxMFTree = tree->getChildPtr("chatbox")->getChildPtr("mf");
	config_.chatboxFMTree = tree->getChildPtr("chatbox")->getChildPtr("fm");
	config_.chatboxFFTree = tree->getChildPtr("chatbox")->getChildPtr("ff");
	config_.chatboxMXTree = tree->getChildPtr("chatbox")->getChildPtr("mx");
	config_.chatboxFXTree = tree->getChildPtr("chatbox")->getChildPtr("fx");

	config_.bubbleMMTree = tree->getChildPtr("bubble")->getChildPtr("mm");
	config_.bubbleMFTree = tree->getChildPtr("bubble")->getChildPtr("mf");
	config_.bubbleFMTree = tree->getChildPtr("bubble")->getChildPtr("fm");
	config_.bubbleFFTree = tree->getChildPtr("bubble")->getChildPtr("ff");
	config_.bubbleMXTree = tree->getChildPtr("bubble")->getChildPtr("mx");
	config_.bubbleFXTree = tree->getChildPtr("bubble")->getChildPtr("fx");
	
	testVid.readConfig(tree->getChildPtr("testVid"));
}
