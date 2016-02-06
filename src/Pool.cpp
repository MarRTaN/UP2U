#include "Pool.h"

void Pool::setup(){
	loadChatBox();
	loadBubble();
}

void Pool::loadChatBox(){
	Bit::JsonTree* chatboxMMTree = config_.chatboxMMTree;
	Bit::JsonTree* chatboxMFTree = config_.chatboxMFTree;
	Bit::JsonTree* chatboxFMTree = config_.chatboxFMTree;
	Bit::JsonTree* chatboxFFTree = config_.chatboxFFTree;
	Bit::JsonTree* chatboxMXTree = config_.chatboxMXTree;
	Bit::JsonTree* chatboxFXTree = config_.chatboxFXTree;

	for (Bit::JsonTree::Iter it = chatboxMMTree->begin(); it != chatboxMMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		chatboxMMList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = chatboxMFTree->begin(); it != chatboxMFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		chatboxMFList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = chatboxFMTree->begin(); it != chatboxFMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		chatboxFMList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = chatboxFFTree->begin(); it != chatboxFFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		chatboxFFList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = chatboxMXTree->begin(); it != chatboxMXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		chatboxMXList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = chatboxFXTree->begin(); it != chatboxFXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		chatboxFXList_.push_back(tempVid);
	}

}

void Pool::loadBubble(){
	Bit::JsonTree* bubbleMMTree = config_.bubbleMMTree;
	Bit::JsonTree* bubbleMFTree = config_.bubbleMFTree;
	Bit::JsonTree* bubbleFMTree = config_.bubbleFMTree;
	Bit::JsonTree* bubbleFFTree = config_.bubbleFFTree;
	Bit::JsonTree* bubbleMXTree = config_.bubbleMXTree;
	Bit::JsonTree* bubbleFXTree = config_.bubbleFXTree;


	for (Bit::JsonTree::Iter it = bubbleMMTree->begin(); it != bubbleMMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		bubbleMMList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = bubbleMFTree->begin(); it != bubbleMFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		bubbleMFList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = bubbleFMTree->begin(); it != bubbleFMTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		bubbleFMList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = bubbleFFTree->begin(); it != bubbleFFTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		bubbleFFList_.push_back(tempVid);
	}

	for (Bit::JsonTree::Iter it = bubbleMXTree->begin(); it != bubbleMXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		bubbleMXList_.push_back(tempVid);
	}
	
	for (Bit::JsonTree::Iter it = bubbleFXTree->begin(); it != bubbleFXTree->end(); it++){
		Bit::JsonTree* tempTree = Bit::JsonTree::toBitJsonTreePtr(*it);
		Bit::Video tempVid;
		tempVid.readConfig(tempTree->getChildPtr("video"));
		bubbleFXList_.push_back(tempVid);
	}
}


Bit::Video Pool::getChatboxVid(int gender1, int gender2){
	
	Bit::Video result;
	if (gender1 == MALE && gender2 == MALE){
		srand(time(NULL));
		int randomNo = rand() % chatboxMMList_.size();
		result = chatboxMMList_[randomNo];
	}
	else if (gender1 == MALE && gender2 == FEMALE){
		srand(time(NULL));
		int randomNo = rand() % chatboxMFList_.size();
		result = chatboxMFList_[randomNo];
	}
	else if (gender1 == FEMALE && gender2 == MALE){
		srand(time(NULL));
		int randomNo = rand() % chatboxFMList_.size();
		result = chatboxFMList_[randomNo];
	}
	else if (gender1 == FEMALE && gender2 == FEMALE){
		srand(time(NULL));
		int randomNo = rand() % chatboxFFList_.size();
		result = chatboxFFList_[randomNo];
	}
	else if (gender1 == MALE && gender2 == UNDEFINED){
		srand(time(NULL));
		int randomNo = rand() % chatboxMXList_.size();
		result = chatboxMXList_[randomNo];
	}
	else if (gender1 == FEMALE && gender2 == UNDEFINED){
		srand(time(NULL));
		int randomNo = rand() % chatboxFXList_.size();
		result = chatboxFXList_[randomNo];
	}
	return result;
	
}

Bit::Video Pool::getBubbleVid(int gender1, int gender2){
	
	Bit::Video result;
	if (gender1 == MALE && gender2 == MALE){
		srand(time(NULL));
		int randomNo = rand() % bubbleMMList_.size();
		result = bubbleMMList_[randomNo];
	}
	else if (gender1 == MALE && gender2 == FEMALE){
		srand(time(NULL));
		int randomNo = rand() % bubbleMFList_.size();
		result = bubbleMFList_[randomNo];
	}
	else if (gender1 == FEMALE && gender2 == MALE){
		srand(time(NULL));
		int randomNo = rand() % bubbleFMList_.size();
		result = bubbleFMList_[randomNo];
	}
	else if (gender1 == FEMALE && gender2 == FEMALE){
		srand(time(NULL));
		int randomNo = rand() % bubbleFFList_.size();
		result = bubbleFFList_[randomNo];
	}
	else if (gender1 == MALE && gender2 == UNDEFINED){
		srand(time(NULL));
		int randomNo = rand() % bubbleMXList_.size();
		result = bubbleMXList_[randomNo];
	}
	else if (gender1 == FEMALE && gender2 == UNDEFINED){
		srand(time(NULL));
		int randomNo = rand() % bubbleFXList_.size();
		result = bubbleFXList_[randomNo];
	}
	return result;
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

}
