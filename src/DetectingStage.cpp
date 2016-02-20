#include <DetectingStage.h>

void DetectingStage::setup(){
	//setup guage
	guageVid_.setup();
	
	//get section image
	ci::Surface smileSurface(loadImage(smilePath_));
	smileTexture_ = Texture(smileSurface);

	ci::Surface smileLeftSurface(loadImage(smileLeftPath_));
	smileLeftTexture_ = Texture(smileLeftSurface);
	
	ci::Surface smileRightSurface(loadImage(smileRightPath_));
	smileRightTexture_ = Texture(smileRightSurface);

	ci::Surface seriousSurface(loadImage(seriousPath_));
	seriousTexture_ = Texture(seriousSurface);

	ci::Surface seriousLeftSurface(loadImage(seriousLeftPath_));
	seriousLeftTexture_ = Texture(seriousLeftSurface);

	ci::Surface seriousRightSurface(loadImage(seriousRightPath_));
	seriousRightTexture_ = Texture(seriousRightSurface);

	ci::Surface turnLeftSurface(loadImage(turnLeftPath_));
	turnLeftTexture_ = Texture(turnLeftSurface);

	ci::Surface turnRightSurface(loadImage(turnRightPath_));
	turnRightTexture_ = Texture(turnRightSurface);
	
	facesColor_.resize(4,0.12f);
	
	startTimeTalk_.resize(4,0.0f);
	isStartTalking_.resize(4, false);

	startTimePhub_.resize(4, 0.0f);
	isStartPhubbing_.resize(4, false);
}

void DetectingStage::update(){
	if (timeCount_ == 0){
		guageVid_.play();
		timeCount_ = 1;
	}
	for (int i = 0; i < persons_.size(); i++){
		if (persons_[i].center.x < getWindowWidth() / 3) persons_[i].segtion = LEFT;
		else if (persons_[i].center.x > getWindowWidth() * 2 / 3) persons_[i].segtion = RIGHT;
		else persons_[i].segtion = CENTER;
	}
	checkAction();
	timeRatio_ = (timePhub_*1.f) / ((timeTalk_+timePhub_) * 1.f);
	guageVid_.seekToNormalizedTime(timeRatio_);
	
}

void DetectingStage::draw(){
	
	//draw gauge
	Rectf guageRect = Rectf(getWindowWidth()*0.1, getWindowHeight()*0.1, getWindowWidth()*0.9, getWindowHeight()*0.28);
	Texture guageTexture = guageVid_.getTexture();
	if (guageTexture){
		ci::gl::enableAlphaBlending();
		displayArea_.draw(guageTexture, guageRect);
		ci::gl::disableAlphaBlending();
	}

	//draw time
	///calculate phub time
	int phubH = timePhub_ / 3600;
	int phubM = (timePhub_ / 60) % 60;
	int phubS = timePhub_ % 60;

	
	string phubHS,phubMS, phubSS,phubString;
	if (phubH < 10) phubHS = "0" + toString(phubH);
	else phubHS = toString(phubH);

	if (phubM < 10) phubMS = "0" + toString(phubM);
	else phubMS = toString(phubM);

	if (phubS < 10) phubSS = "0" + toString(phubS);
	else phubSS = toString(phubS);

	phubString = phubHS + ":" + phubMS + ":" + phubSS;

	///calculate talk string
	int talkH = timeTalk_ / 3600;
	int talkM = (timeTalk_ / 60) % 60;
	int talkS = timeTalk_ % 60;

	string talkHS, talkMS, talkSS, talkString;
	if (talkH < 10) talkHS = "0" + toString(talkH);
	else talkHS = toString(talkH);

	if (talkM < 10) talkMS = "0" + toString(talkM);
	else talkMS = toString(talkM);

	if (talkS < 10) talkSS = "0" + toString(talkS);
	else talkSS = toString(talkS);

	talkString = talkHS + ":" + talkMS + ":" + talkSS;

	enableAlphaBlending();
	drawString(phubString, Vec2f(param_phubTimeX_, param_bothTimeY_), Color(1, 1, 1), Font(loadAsset("ThaiSansNeue-Regular.otf"), param_fontSize_));
	drawString(talkString, Vec2f(param_talkTimeX_, param_bothTimeY_), Color(1, 1, 1), Font(loadAsset("ThaiSansNeue-Regular.otf"), param_fontSize_));
	disableAlphaBlending();

	//draw sticker
	for (int i = 0; i < persons_.size(); i++){
		gl::color(Color(CM_HSV, facesColor_[persons_[i].id], 0.77f, 0.96f));
		//choose correct texture for each person
		Texture stickerTexture;

		if (persons_[i].getLook() == LOOKUP) {
			if (persons_[i].segtion == LEFT) stickerTexture = smileLeftTexture_;
			else if (persons_[i].segtion == CENTER) stickerTexture = smileTexture_;
			else if (persons_[i].segtion == RIGHT) stickerTexture = smileRightTexture_;
		}
		else if (persons_[i].getLook() == LOOKDOWN) {
			if (persons_[i].segtion == LEFT) stickerTexture = seriousLeftTexture_;
			else if (persons_[i].segtion == CENTER) stickerTexture = seriousTexture_;
			else if (persons_[i].segtion == RIGHT) stickerTexture = seriousRightTexture_;
		}
		else if (persons_[i].getLook() == TURNLEFT) {
			stickerTexture = turnLeftTexture_;
		}
		else if (persons_[i].getLook() == TURNRIGHT) {
			stickerTexture = turnRightTexture_;;
		}

		//adjust position and size
		Vec2f shift(param_shiftX_, param_shiftY_);
		Vec2f scale(param_scale_, param_scale_);

		//calibrate position for each segment
		Vec2f calShift(0, 0);
		if (persons_[i].segtion == LEFT) calShift.x = -param_calShift_;
		else if (persons_[i].segtion == RIGHT) calShift.x = param_calShift_;
		else calShift.x = 0;

		Rectf stickerRect = Rectf(persons_[i].center - scale + shift + calShift, persons_[i].center + scale + shift + calShift);
		if (stickerTexture){
			ci::gl::enableAlphaBlending();
			displayArea_.draw(stickerTexture, stickerRect);
			ci::gl::disableAlphaBlending();
		}
	}
	gl::color(255, 255, 255);
}

void DetectingStage::setPersons(vector<Person> persons){
	persons_.resize(persons.size());
	sort(persons.begin(), persons.end());
	persons_ = persons;
}

void DetectingStage::checkAction(){
	for (int i = 0; i < persons_.size(); i++){

		if (persons_[i].getLook() == LOOKDOWN){
			minimizeFaceColor(persons_[i].id);
			calPhubTime(persons_[i].id);
			resetTimeTalk(persons_[i].id);
		}

		else if (persons_[i].getLook() == LOOKUP){
			normalizeFaceColor(persons_[i].id);
			resetTimePhub(persons_[i].id);
			resetTimeTalk(persons_[i].id);
		}

		else if (persons_[i].getLook() == TURNRIGHT){
			resetTimePhub(persons_[i].id);
			bool isMatched = false;
			for (int j = i; j < persons_.size(); j++){
				if (persons_[j].getLook() == TURNLEFT) {
					calTalkTime(persons_[i].id);
					isMatched = true;
					break;
				}
			}
			if (isMatched) maximizeFaceColor(persons_[i].id);
			else normalizeFaceColor(persons_[i].id);
		}

		else if (persons_[i].getLook() == TURNLEFT){
			resetTimePhub(persons_[i].id);
			bool isMatched = false;
			for (int j = i; j >= 0; j--){
				if (persons_[j].getLook() == TURNRIGHT) {
					calTalkTime(persons_[i].id);
					isMatched = true;
					break;
				}
			}
			if (isMatched) maximizeFaceColor(persons_[i].id);
			else normalizeFaceColor(persons_[i].id);
		}
	}
}

void DetectingStage::addColor(Vec3f added){
	hsv_ += added;
}

void DetectingStage::addTimeTalk(float t){
	timeTalk_ += t;
}

void DetectingStage::addTimePhub(float t){
	timePhub_ += t;
}

void DetectingStage::drawDebugMode(){

}

void DetectingStage::calPhubTime(int personId){
	
	if (!isStartPhubbing_[personId]){
		startTimePhub_[personId] = getElapsedSeconds() + 1;
		isStartPhubbing_[personId] = true;
	}
	else if (startTimePhub_[personId] < getElapsedSeconds()){
		timePhub_++;
		isStartPhubbing_[personId] = false;
	}
	/*int index = idToIndex(personId);
	if (!persons_[index].isStartPhubbing){
	persons_[index].startTimePhubbing = getElapsedSeconds() + 1;
	persons_[index].isStartPhubbing = true;
	}
	else if (persons_[index].startTimePhubbing < getElapsedSeconds()){
	decreaseFaceColor(personId);
	timePhub_++;
	persons_[index].isStartPhubbing = false;
	}*/
}

void DetectingStage::calTalkTime(int personId){
	int index = idToIndex(personId);
	if (!isStartTalking_[personId]){
		startTimeTalk_[personId] = getElapsedSeconds() + 1;
		isStartTalking_[personId] = true;
	}
	else if (startTimeTalk_[personId] < getElapsedSeconds()){
		timeTalk_++;
		isStartTalking_[personId] = false;
	}
	
	/*int index = idToIndex(personId);
	if (!persons_[index].isStartTalking){
		persons_[index].startTimeTalking = getElapsedSeconds() + 1;
		persons_[index].isStartTalking = true;
	}
	else if (persons_[index].startTimeTalking < getElapsedSeconds()){
		increaseFaceColor(personId);
		timeTalk_++;
		persons_[index].isStartTalking = false;
	}*/
}

void DetectingStage::resetTimeTalk(int personId){
	isStartTalking_[personId] = false;
}

void DetectingStage::resetTimePhub(int personId){
	isStartPhubbing_[personId] = false;
}

void DetectingStage::minimizeFaceColor(int personId){
	float min = 0.01f;
	if (facesColor_[personId] > min) facesColor_[personId] -= 0.002f;
}

void DetectingStage::maximizeFaceColor(int personId){
	float max = 0.26f;
	if (facesColor_[personId] < max) facesColor_[personId] += 0.002f;
}

void DetectingStage::normalizeFaceColor(int personId){
	float mean = 0.12f;
	if (facesColor_[personId] > mean) facesColor_[personId] -= 0.002f;
	else if(facesColor_[personId] < mean) facesColor_[personId] += 0.002f;
}

int DetectingStage::idToIndex(int id){
	for (int i = 0; i < persons_.size(); i++){
		if (persons_[i].id == id) return i;
	}
}

void DetectingStage::reset(){
	timeCount_ = 0;
	guageVid_.stop();
}

void DetectingStage::readConfig(Bit::JsonTree* tree){
	guageVid_.readConfig(tree->getChildPtr("guageVid"));
	seriousPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("serious")->getValue<string>();
	seriousLeftPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousLeft")->getValue<string>();
	seriousRightPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousRight")->getValue<string>();
	smilePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smile")->getValue<string>();
	smileLeftPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileLeft")->getValue<string>();
	smileRightPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileRight")->getValue<string>();
	turnLeftPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnLeft")->getValue<string>();
	turnRightPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnRight")->getValue<string>();
}

void DetectingStage::readParams(Bit::JsonTree* tree, Bit::ParamsRef params){
	params->addParam<float>(tree->getChildPtr("scale"), param_scale_);
	params->addParam<float>(tree->getChildPtr("shiftX"), param_shiftX_);
	params->addParam<float>(tree->getChildPtr("shiftY"), param_shiftY_);
	params->addParam<float>(tree->getChildPtr("calShift"), param_calShift_);
	params->addParam<float>(tree->getChildPtr("fontSize"), param_fontSize_);
	params->addParam<float>(tree->getChildPtr("phubTimeX"), param_phubTimeX_);
	params->addParam<float>(tree->getChildPtr("talkTimeX"), param_talkTimeX_);
	params->addParam<float>(tree->getChildPtr("bothTimeY"), param_bothTimeY_);
}