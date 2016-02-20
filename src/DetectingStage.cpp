#include <DetectingStage.h>

void DetectingStage::setup(){
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
		float h;
		if (i == 1) h = 0.01f;
		else if (i == 2) h = 0.12f;
		else h = 0.26f;
		gl::color(Color(CM_HSV, h, 0.77f, 0.96f));
		//console() << hsv_[0] << endl;
		//choose correct texture for each person
		Texture stickerTexture;

		//if (persons_[i].look == LOOKUP) {
		if (persons_[i].isLookUp) {
			if (persons_[i].segtion == LEFT) stickerTexture = smileLeftTexture_;
			else if (persons_[i].segtion == CENTER) stickerTexture = smileTexture_;
			else if (persons_[i].segtion == RIGHT) stickerTexture = smileRightTexture_;
		}
		//else if (persons_[i].look == LOOKDOWN) {
		else if (!persons_[i].isLookUp){
			if (persons_[i].segtion == LEFT) stickerTexture = seriousLeftTexture_;
			else if (persons_[i].segtion == CENTER) stickerTexture = seriousTexture_;
			else if (persons_[i].segtion == RIGHT) stickerTexture = seriousRightTexture_;
		}
		else if (persons_[i].look == TURNLEFT) {
			stickerTexture = turnLeftTexture_;
		}
		else if (persons_[i].look == TURNRIGHT) {
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