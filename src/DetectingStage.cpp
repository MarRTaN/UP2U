#include <DetectingStage.h>

void DetectingStage::setup(){
	//setup guage
	//guageVid_.setup();
	
	//get gauge image
	ci::Surface gaugeSurface(loadImage(gaugePath_));
	gaugeTexture_ = Texture(gaugeSurface);

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

	//Male
	ci::Surface smileMaleSurface(loadImage(smileMalePath_));
	smileMaleTexture_ = Texture(smileMaleSurface);

	ci::Surface smileLeftMaleSurface(loadImage(smileLeftMalePath_));
	smileLeftMaleTexture_ = Texture(smileLeftMaleSurface);

	ci::Surface smileRightMaleSurface(loadImage(smileRightMalePath_));
	smileRightMaleTexture_ = Texture(smileRightMaleSurface);

	ci::Surface seriousMaleSurface(loadImage(seriousMalePath_));
	seriousMaleTexture_ = Texture(seriousMaleSurface);

	ci::Surface seriousLeftMaleSurface(loadImage(seriousLeftMalePath_));
	seriousLeftMaleTexture_ = Texture(seriousLeftMaleSurface);

	ci::Surface seriousRightMaleSurface(loadImage(seriousRightMalePath_));
	seriousRightMaleTexture_ = Texture(seriousRightMaleSurface);

	ci::Surface turnLeftMaleSurface(loadImage(turnLeftMalePath_));
	turnLeftMaleTexture_ = Texture(turnLeftMaleSurface);

	ci::Surface turnRightMaleSurface(loadImage(turnRightMalePath_));
	turnRightMaleTexture_ = Texture(turnRightMaleSurface);

	//Female
	ci::Surface smileFemaleSurface(loadImage(smileFemalePath_));
	smileFemaleTexture_ = Texture(smileFemaleSurface);

	ci::Surface smileLeftFemaleSurface(loadImage(smileLeftFemalePath_));
	smileLeftFemaleTexture_ = Texture(smileLeftFemaleSurface);

	ci::Surface smileRightFemaleSurface(loadImage(smileRightFemalePath_));
	smileRightFemaleTexture_ = Texture(smileRightFemaleSurface);

	ci::Surface seriousFemaleSurface(loadImage(seriousFemalePath_));
	seriousFemaleTexture_ = Texture(seriousFemaleSurface);

	ci::Surface seriousLeftFemaleSurface(loadImage(seriousLeftFemalePath_));
	seriousLeftFemaleTexture_ = Texture(seriousLeftFemaleSurface);

	ci::Surface seriousRightFemaleSurface(loadImage(seriousRightFemalePath_));
	seriousRightFemaleTexture_ = Texture(seriousRightFemaleSurface);

	ci::Surface turnLeftFemaleSurface(loadImage(turnLeftFemalePath_));
	turnLeftFemaleTexture_ = Texture(turnLeftFemaleSurface);

	ci::Surface turnRightFemaleSurface(loadImage(turnRightFemalePath_));
	turnRightFemaleTexture_ = Texture(turnRightFemaleSurface);
	
	facesColor_.resize(15,0.12f);
	
	startTimeTalk_.resize(15, 0.0f);
	isStartTalking_.resize(15, false);

	startTimePhub_.resize(15, 0.0f);
	isStartPhubbing_.resize(15, false);

	fontS_ = Font("ThaiSansNeue-Regular.otf", param_fontSize_);
	fontM_ = Font("ThaiSansNeue-Regular.otf", param_fontSize_*0.8f);
	fontH_ = Font("ThaiSansNeue-Regular.otf", param_fontSize_*0.6f);
}

void DetectingStage::update(){
	if (timeCount_ == 0){
		//guageVid_.play();
		timeCount_ = 1;
	}
	for (int i = 0; i < persons_.size(); i++){
		if (persons_[i].center.x < getWindowWidth() / 3) persons_[i].segtion = LEFT;
		else if (persons_[i].center.x > getWindowWidth() * 2 / 3) persons_[i].segtion = RIGHT;
		else persons_[i].segtion = CENTER;
	}
	checkAction();
	timeRatio_ = (timePhub_*1.f) / ((timePhub_+ timeTalk_) * 1.f);
	if (currentRatio_ + 0.001f < timeRatio_) currentRatio_ += 0.001f;
	else if (currentRatio_ - 0.001f > timeRatio_) currentRatio_ -= 0.001f;
	//guageVid_.seekToNormalizedTime(currentRatio_);
}

void DetectingStage::draw(){
	
	//get guage
	Rectf gaugeRect = Rectf(0.0f, 0.0f, getWindowWidth(), getWindowHeight()*0.4f);
	//Texture guageTexture = guageVid_.getTexture();


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

	//draw red green rect
	Rectf greenRect = Rectf(0.0f, 0.0f, getWindowWidth(), getWindowHeight()*0.15f);
	Rectf redRect = Rectf(0.0f, 0.0f, (getWindowWidth()*currentRatio_), getWindowHeight()*0.15f);
	gl::color(67.f/255.f, 193.f/255.f, 30.f/255.f);
	drawSolidRect(greenRect);
	gl::color(255.f/255.f, 54.f/255.f, 54.f/255.f);
	drawSolidRect(redRect);
	gl::color(1, 1, 1);

	//draw guage
	if (gaugeTexture_){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		displayArea_.draw(gaugeTexture_, gaugeRect);
		drawString(phubSS, Vec2f(param_phubTimeSX_, param_bothTimeSY_), Color(0, 0, 0), fontS_);
		drawString(talkSS, Vec2f(param_talkTimeSX_, param_bothTimeSY_), Color(0, 0, 0), fontS_);
		drawString(phubMS, Vec2f(param_phubTimeMX_, param_bothTimeMY_), Color(0, 0, 0), fontM_);
		drawString(talkMS, Vec2f(param_talkTimeMX_, param_bothTimeMY_), Color(0, 0, 0), fontM_);
		drawString(phubHS, Vec2f(param_phubTimeHX_, param_bothTimeHY_), Color(0, 0, 0), fontH_);
		drawString(talkHS, Vec2f(param_talkTimeHX_, param_bothTimeHY_), Color(0, 0, 0), fontH_);
		glDisable(GL_BLEND);
	}


	//draw sticker
	for (int i = 0; i < persons_.size(); i++){
		if(!isDebugMode) gl::color(Color(CM_HSV, facesColor_[persons_[i].id], 0.77f, 0.96f));
		else gl::color(1, 1, 1, 0.5);
		//choose correct texture for each person
		Texture stickerTexture;

		if (persons_[i].getLook() == LOOKUP) {
			if (persons_[i].segtion == LEFT) {
				if (persons_[i].gender == MALE) stickerTexture = smileLeftMaleTexture_;
				else if (persons_[i].gender == FEMALE) stickerTexture = smileLeftFemaleTexture_;
				else stickerTexture = smileLeftTexture_;
			}
			else if (persons_[i].segtion == CENTER) {
				if (persons_[i].gender == MALE) stickerTexture = smileMaleTexture_;
				else if (persons_[i].gender == FEMALE) stickerTexture = smileFemaleTexture_;
				else stickerTexture = smileTexture_;
			}
			else if (persons_[i].segtion == RIGHT) {
				if (persons_[i].gender == MALE) stickerTexture = smileRightMaleTexture_;
				else if (persons_[i].gender == FEMALE) stickerTexture = smileRightFemaleTexture_;
				else stickerTexture = smileRightTexture_;
			}
		}
		else if (persons_[i].getLook() == LOOKDOWN) {
			if (persons_[i].segtion == LEFT) {
				if (persons_[i].gender == MALE) stickerTexture = seriousLeftMaleTexture_;
				else if (persons_[i].gender == FEMALE) stickerTexture = seriousLeftFemaleTexture_;
				else stickerTexture = seriousLeftTexture_;
			}
			else if (persons_[i].segtion == CENTER) {
				if (persons_[i].gender == MALE) stickerTexture = seriousMaleTexture_;
				else if (persons_[i].gender == FEMALE) stickerTexture = seriousFemaleTexture_;
				else stickerTexture = seriousTexture_;
			}
			else if (persons_[i].segtion == RIGHT) {
				if (persons_[i].gender == MALE) stickerTexture = seriousRightMaleTexture_;
				else if (persons_[i].gender == FEMALE) stickerTexture = seriousRightFemaleTexture_;
				else stickerTexture = seriousRightTexture_;
			}
		}
		else if (persons_[i].getLook() == TURNLEFT) {
			if (persons_[i].gender == MALE) stickerTexture = turnLeftMaleTexture_;
			else if (persons_[i].gender == FEMALE) stickerTexture = turnLeftFemaleTexture_;
			else stickerTexture = turnLeftTexture_;
		}
		else if (persons_[i].getLook() == TURNRIGHT) {
			if (persons_[i].gender == MALE) stickerTexture = turnRightMaleTexture_;
			else if (persons_[i].gender == FEMALE) stickerTexture = turnRightFemaleTexture_;
			else stickerTexture = turnRightTexture_;
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
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			displayArea_.draw(stickerTexture, stickerRect);
			ci::gl::disableAlphaBlending();
			glDisable(GL_BLEND);
		}
	}
	gl::color(1, 1, 1);
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

	gaugePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("gauge")->getValue<string>();
	seriousPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("serious")->getValue<string>();
	seriousLeftPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousLeft")->getValue<string>();
	seriousRightPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousRight")->getValue<string>();
	smilePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smile")->getValue<string>();
	smileLeftPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileLeft")->getValue<string>();
	smileRightPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileRight")->getValue<string>();
	turnLeftPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnLeft")->getValue<string>();
	turnRightPath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnRight")->getValue<string>();

	seriousMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousMale")->getValue<string>();
	seriousLeftMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousLeftMale")->getValue<string>();
	seriousRightMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousRightMale")->getValue<string>();
	smileMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileMale")->getValue<string>();
	smileLeftMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileLeftMale")->getValue<string>();
	smileRightMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileRightMale")->getValue<string>();
	turnLeftMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnLeftMale")->getValue<string>();
	turnRightMalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnRightMale")->getValue<string>();

	seriousFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousFemale")->getValue<string>();
	seriousLeftFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousLeftFemale")->getValue<string>();
	seriousRightFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("seriousRightFemale")->getValue<string>();
	smileFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileFemale")->getValue<string>();
	smileLeftFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileLeftFemale")->getValue<string>();
	smileRightFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("smileRightFemale")->getValue<string>();
	turnLeftFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnLeftFemale")->getValue<string>();
	turnRightFemalePath_ = Bit::Config::getAssetPath() + tree->getChildPtr("turnRightFemale")->getValue<string>();
}

void DetectingStage::readParams(Bit::JsonTree* tree, Bit::ParamsRef params){
	params->addParam<float>(tree->getChildPtr("scale"), param_scale_);
	params->addParam<float>(tree->getChildPtr("shiftX"), param_shiftX_);
	params->addParam<float>(tree->getChildPtr("shiftY"), param_shiftY_);
	params->addParam<float>(tree->getChildPtr("calShift"), param_calShift_);
	params->addParam<float>(tree->getChildPtr("fontSize"), param_fontSize_);
	params->addParam<float>(tree->getChildPtr("phubTimeSX"), param_phubTimeSX_);
	params->addParam<float>(tree->getChildPtr("talkTimeSX"), param_talkTimeSX_);
	params->addParam<float>(tree->getChildPtr("phubTimeMX"), param_phubTimeMX_);
	params->addParam<float>(tree->getChildPtr("talkTimeMX"), param_talkTimeMX_);
	params->addParam<float>(tree->getChildPtr("phubTimeHX"), param_phubTimeHX_);
	params->addParam<float>(tree->getChildPtr("talkTimeHX"), param_talkTimeHX_);
	params->addParam<float>(tree->getChildPtr("bothTimeSY"), param_bothTimeSY_);
	params->addParam<float>(tree->getChildPtr("bothTimeMY"), param_bothTimeMY_);
	params->addParam<float>(tree->getChildPtr("bothTimeHY"), param_bothTimeHY_);
}