#include <StageManager.h>

void StageManager::setup(){
	currentStageId_ = ADVERTISING_STAGE;
	lastStageId_ = ADVERTISING_STAGE;
	advertisingStage_.setup();
	detectingStage_.setup();
}

void StageManager::update(){
	checkDetection();
	/*if (lastStageId_ != currentStageId_){
		advertisingStage_.reset();
		detectingStage_.reset();
	}*/

	switch (currentStageId_){
	case ADVERTISING_STAGE: advertisingStage_.update(); break;
	case DETECTING_STAGE: detectingStage_.update(); detectingStage_.setPersons(persons_); break;
	}

	lastStageId_ = currentStageId_;


}
void StageManager::draw(){

	switch (currentStageId_){
		case ADVERTISING_STAGE: advertisingStage_.draw();  break;
		case DETECTING_STAGE: detectingStage_.draw(); break;
	}

	if (isDebugMode){
		switch (currentStageId_){
		case ADVERTISING_STAGE: advertisingStage_.drawDebugMode();  break;
		case DETECTING_STAGE: detectingStage_.drawDebugMode(); break;
		}
	}
}

void StageManager::setPlayerDetection(bool isDetecting){
	isDetected_ = isDetecting;
}

void StageManager::checkDetection(){
	if (isDetected_) {
		if (timeOut_ < getElapsedSeconds()){
			currentStageId_ = DETECTING_STAGE;
		}
		personLostSwitchTimeDelay_ = getElapsedSeconds() + switchStageDelay_;
	}
	else {
		if (personLostSwitchTimeDelay_ < getElapsedSeconds()){
			currentStageId_ = ADVERTISING_STAGE;
			startTime_ = getElapsedSeconds();
			timeOut_ = startTime_ + switchStageDelay_;
		}
	}
}

void StageManager::addLog(string logInfo){
	log_.addLog(logInfo, true);
}

int	 StageManager::getCurrentStage(){
	return currentStageId_;
}


void StageManager::setPersons(vector<Person> persons){
	persons_ = persons;
}

void StageManager::readConfig(Bit::JsonTree* tree){
	advertisingStage_.readConfig(tree->getChildPtr("advertisingStage"));
	detectingStage_.readConfig(tree->getChildPtr("detectingStage"));
	switchStageDelay_ = tree->getChildPtr("switchStageDelay")->getValue<float>();
}

void StageManager::readParams(Bit::JsonTree* tree, Bit::ParamsRef params){

	detectingStage_.readParams(tree->getChildPtr("detectionStage"), params);

}