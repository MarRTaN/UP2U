#include <DetectingStage.h>

void DetectingStage::setup(){
	stageVid_ = config_.stageVid;
	stageVid_.setup();
	pool_.setup();

}

void DetectingStage::update(){
	if (timeCount_ == 0) {
		stageVid_.play();
	}
	timeCount_ = 1;


}

void DetectingStage::draw(){
	Texture bubbleTexture = stageVid_.getTexture();
	if (bubbleTexture){
		ci::gl::enableAlphaBlending();
		/*for (int i = 0; i < persons_.size(); i++){
			displayArea_.draw(bubbleTexture, Vec2f(persons_[i].getHeadPos().x,persons_[i].getHeadPos().y),getWindowSize());
		}*/
		displayArea_.draw(bubbleTexture, Rectf(0,0,300,300));
		displayArea_.draw(bubbleTexture, Rectf(300, 50,600,350));
		ci::gl::disableAlphaBlending();
	}
}

void DetectingStage::setPersons(vector<Person> persons){
	persons_ = persons;
}


Bit::Video DetectingStage::getVid(int gender1, int gender2, int type){
	Bit::Video result;
	return result;

}

void DetectingStage::drawDebugMode(){

}

void DetectingStage::reset(){
	timeCount_ = 0;
	stageVid_.stop();
}


void DetectingStage::readConfig(Bit::JsonTree* tree){
	config_.stageVid.readConfig(tree->getChildPtr("stageVid"));
	pool_.readConfig(tree->getChildPtr("pool"));
}