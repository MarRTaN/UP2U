#include <AdvertisingStage.h>

void AdvertisingStage::setup(){
	advertisingVid_ = config_.advertisingVid;
	advertisingVid_.setup();
}

void AdvertisingStage::update(){
	if (timeCount_ == 0) {
		advertisingVid_.play();
	}
	timeCount_ = 1;
}

void AdvertisingStage::draw(){
	Texture advertisingVidTexture = advertisingVid_.getTexture();
	if (advertisingVidTexture){
		enableAlphaBlending();
		//displayArea_.draw(advertisingVidTexture, getWindowBounds());
		disableAlphaBlending();
	}
}



void AdvertisingStage::reset(){
	timeCount_ = 0;
	advertisingVid_.pause();
}


void AdvertisingStage::drawDebugMode(){


}

void AdvertisingStage::readConfig(Bit::JsonTree* tree){
	config_.advertisingVid.readConfig(tree->getChildPtr("advertisingVid"));
}