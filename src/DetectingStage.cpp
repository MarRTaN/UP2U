#include <DetectingStage.h>

void DetectingStage::setup(){
	pool_.setup();
	//stageVid_ = config_.stageVid;
	//stageVid_.setup();

}

void DetectingStage::update(){
	/*if (timeCount_ == 0){
		stageVid_.play();
		timeCount_ = 1;
	}*/
	eraseOldPairs();
	checkNewPersons();
}

void DetectingStage::draw(){

	for (int i = 0; i < pairs_.size(); i++){
		Texture texture;
		//if person look up, then pause chatboxVid and play bubbleVid
		if (pairs_[i].person.isLookUp){
			if (!pairs_[i].chatboxVid.isDone()) {
				pairs_[i].chatboxVid.pause();
			}
			if (!pairs_[i].bubbleVid.isDone()) {
				pairs_[i].bubbleVid.play();
				texture = pairs_[i].bubbleVid.getTexture();
				//console() << " i = " << i << " :: draw bubble" << endl;
			}
		}
		//else, pause bubbleVid and play chatboxVid.
		else{
			if (!pairs_[i].bubbleVid.isDone()) {
				pairs_[i].bubbleVid.pause();
			}
			if (!pairs_[i].chatboxVid.isDone()) {
				pairs_[i].chatboxVid.play();
				texture = pairs_[i].chatboxVid.getTexture();
				//console() << " i = " << i << " :: draw chatbox" << endl;
			}
		}

		//bound in frame
		Rectf textureRect;
		Vec2f shift(param_shiftX_, param_shiftY_);
		Vec2f radius(param_scale_, param_scale_);
		if (pairs_[i].person.center.x - radius.x + shift.x  < 0){
			if (pairs_[i].person.center.y - radius.y + shift.y < 0){
				textureRect = Rectf(0, 0, radius.x*2, radius.y*2);
			}
			else if (pairs_[i].person.center.y + radius.y + shift.y > getWindowHeight()){
				textureRect = Rectf(0, getWindowHeight()-radius.y*2, radius.x*2, getWindowHeight());
			}
			else{
				textureRect = Rectf(0, pairs_[i].person.center.y - radius.y + shift.y, radius.x * 2, pairs_[i].person.center.y + radius.y + shift.y);
			}
		}
		else if (pairs_[i].person.center.x + radius.x + shift.x  > getWindowWidth()){
			if (pairs_[i].person.center.y - radius.y + shift.y < 0){
				textureRect = Rectf(getWindowWidth()-radius.x*2, 0, getWindowWidth(), radius.y*2);
			}
			else if (pairs_[i].person.center.y + radius.y + shift.y > getWindowHeight()){
				textureRect = Rectf(getWindowWidth()-radius.x*2, getWindowHeight() - radius.y*2, getWindowWidth(), getWindowHeight());
			}
			else{
				textureRect = Rectf(getWindowWidth() - radius.x * 2, pairs_[i].person.center.y - radius.y + shift.y, getWindowWidth(), pairs_[i].person.center.y + radius.y + shift.y);
			}
		}
		else{
			if (pairs_[i].person.center.y - radius.y + shift.y < 0){
				textureRect = Rectf(pairs_[i].person.center.x - radius.x, 0, pairs_[i].person.center.x + radius.x, radius.y * 2);
			}
			else if (pairs_[i].person.center.y + radius.y + shift.y > getWindowHeight()){
				textureRect = Rectf(pairs_[i].person.center.x - radius.x, getWindowHeight() - radius.y * 2, pairs_[i].person.center.x + radius.x, getWindowHeight());
			}
			else{
				textureRect = Rectf(pairs_[i].person.center - radius + shift, pairs_[i].person.center + radius + shift);
			}
		}

		//get texture from chatbox/bubble vid and draw at center of that person.
		if (texture){
			ci::gl::enableAlphaBlending();
			displayArea_.draw(texture, textureRect);
			ci::gl::disableAlphaBlending();
		}
		console() << "i = " << i << ", id = " << pairs_[i].person.id << ", center" << pairs_[i].person.center << endl;
	}
}

void DetectingStage::setPersons(vector<Person> persons){
	persons_.resize(persons.size());
	sort(persons.begin(), persons.end());
	persons_ = persons;
}

Bit::Video DetectingStage::getChatboxVid(int gender1, int gender2){

	Bit::Video result; 
	result = pool_.getChatboxVid(gender1, gender2);
	return result;

}

Bit::Video DetectingStage::getBubbleVid(int gender1, int gender2){

	Bit::Video result;
	result = pool_.getBubbleVid(gender1, gender2);
	return result;

}

void DetectingStage::checkNewPersons(){
		
	vector<Pair> tempPairs;

	//match current pair w/ new vector<Persons>
	for (int i = 0; i < persons_.size(); i++){
		//assume this is new person
		bool isNewPerson = true;
		for (int j = 0; j < pairs_.size(); j++){
			//if it's current person, push existed pair in new vector<Pair>
			if (persons_[i].id == pairs_[j].person.id){
				pairs_[j].lastCenter = pairs_[j].person.center;
				pairs_[j].person.center = persons_[i].center;
				pairs_[j].person.isLookUp = persons_[i].isLookUp;
				tempPairs.push_back(pairs_[j]);
				isNewPerson = false;
				break;
			}
		}
		//if it's new person, pair up with vid depends on situation
		if (isNewPerson) {
			//console() << "PUSH NEW PAIR" << endl;
			tempPairs.push_back(pairUp(i));
		}
	}


	pairs_ = tempPairs;
	console() << "PAIRS SIZE = " << pairs_.size() << endl;

}

void DetectingStage::eraseOldPairs(){
	
	//erase in case of person in pair doesn't existed anymore.
	int pairCount = 0;
	while (pairCount < pairs_.size()){
		bool isExist = false;
		for (int i = 0; i < persons_.size(); i++){
			if (pairs_[pairCount].person.id == persons_[i].id){
				isExist = true;
				break;
			}
		}
		if (!isExist) pairs_.erase(pairs_.begin() + pairCount);
		else pairCount++;
	}
}

Pair DetectingStage::pairUp(int i){
	console() << "PAIR UP NEW PERSON" << endl;
	Pair tempPair;
	tempPair.person = persons_[i];
	
	//if this is the first one from the left, consider the right one next to him.
	if (i == 0 && persons_.size() > 1){
		console() << "CASE 1" << endl;
		tempPair.chatboxVid = getChatboxVid(persons_[i].gender, persons_[i + 1].gender);
		tempPair.bubbleVid = getBubbleVid(persons_[i].gender, persons_[i + 1].gender);
	}
	//if this is the second one or the third one from the left, consider the left one next to him.
	else if (i == 1 || i == 2){
		console() << "CASE 2" << endl;
		tempPair.chatboxVid = getChatboxVid(persons_[i].gender, persons_[i - 1].gender);
		tempPair.bubbleVid = getBubbleVid(persons_[i].gender, persons_[i - 1].gender);
	}
	//else he must be alone
	else{
		console() << "CASE 3" << endl;
		tempPair.chatboxVid = getChatboxVid(persons_[i].gender, UNDEFINED);
		tempPair.bubbleVid = getBubbleVid(persons_[i].gender, UNDEFINED);
	}
	return tempPair;
}

void DetectingStage::drawDebugMode(){

}

void DetectingStage::reset(){
	timeCount_ = 0;
	stageVid_.stop();
}


void DetectingStage::readConfig(Bit::JsonTree* tree){
	//config_.stageVid.readConfig(tree->getChildPtr("stageVid"));
	pool_.readConfig(tree->getChildPtr("pool"));
}

void DetectingStage::readParams(Bit::JsonTree* tree, Bit::ParamsRef params){
	params->addParam<float>(tree->getChildPtr("scale"), param_scale_);
	params->addParam<float>(tree->getChildPtr("shiftX"), param_shiftX_);
	params->addParam<float>(tree->getChildPtr("shiftY"), param_shiftY_);
}