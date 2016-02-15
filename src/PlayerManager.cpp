#include <PlayerManager.h>

void PlayerManager::threadUpdate()
{
	Kinect::threadUpdate();
	dataMutex_.lock();
	if (isDataReady()) {
		//function here
		kinectUsers_ = kinectDevice_->getUsers();
		setUsers();
	}
	dataMutex_.unlock();
}


void PlayerManager::shutdown()
{
	Kinect::shutdown();
}


bool PlayerManager::isMirror()
{
	return mirror_;
}

bool PlayerManager::isKinectReady(){
	if (kinectStatus_ == ERROR_STATUS) return false;
	return true;
}

void PlayerManager::setupUsers(){
	persons_.reserve(15);
	users_.reserve(10);
}

void PlayerManager::updateUsers(){
	if (isDataReady()) {
		if (motorStatus_ == false) {
			kinectDevice_->initMotor();
			kinectDevice_->moveMotor(motorAngle_);
			motorStatus_ = true;
		}
		int personCount = 0;
		while (personCount < persons_.size()){
			if (persons_[personCount].isActive && !persons_[personCount].isPersonLost() && persons_[personCount].center.x > 0 && persons_[personCount].center.x < videoW) {
				//kinectDevice_->bootstrapingUser(false);
				//if (persons_[personCount].centerBuffer.size() >= 10) persons_[personCount].centerBuffer.pop_front();
				//if (persons_[personCount].center.x != 0 && persons_[personCount].center.y != 0) persons_[personCount].centerBuffer.push_back(persons_[personCount].center);
				personCount++;
			}
			else{
				//Vec2f centerMean = persons_[personCount].getCenterMean();
				//console() << persons_[personCount].center.x << "," << centerMean.x << endl;
				//if (abs(persons_[personCount].center.x - centerMean.x) > 100){
				//	//kinectDevice_->bootstrapingUser(false);
				//}
				//else {
				//	//kinectDevice_->bootstrapingUser(true);
					//kinectDevice_->resetKinectUsers(persons_[personCount].id);
					persons_.erase(persons_.begin() + personCount);
					//kinectDevice_->resetKinectUsers();
				//	console() << "It's a noiseeee !!!! :: " << personCount << endl;
				//}
				
			}
		}
	}
}

void PlayerManager::draw(){
	
	if (isDataReady()) {
		//dataMutex_.lock();
		Surface colorSurface = getColorSurface();
		Surface depthSurface = getDepthSurface();
		Texture video = gl::Texture(colorSurface);

		videoW = video.getWidth();
		videoH = video.getHeight();
		float windowW = getWindowWidth();
		float windowH = getWindowHeight();
		float imageRatio = windowW / videoW;

		Area srcArea1(0, 0, video.getWidth(), video.getHeight());	//TODO change to videoH
		Rectf destRect1(0, 0, windowW, windowH);					//TODO change to videoW

		//capture from kinect
		gl::color(Color(255, 255, 255));
		gl::draw(video, srcArea1, destRect1);

		Vec3f circleCenter;
		int id = 0;

		//console() << "==========================" << endl;
		for (int i = 0; i < persons_.size(); i++){
			//console() << "id = " << persons_[i].id << " :: Pos : " << persons_[i].center.x << "," << persons_[i].center.y << endl;
			persons_[i].isActive = false;
		}

		if (kinectUsers_.size() < persons_.size()){
			if (countForDelay_ < 11) moveMotorUp();
			else if (countForDelay_ < 22) moveMotorDown();
			else countForDelay_ = 0;
			countForDelay_++;
		}

		//console() << "-----------------------------------" << endl;
		for (int k = 0; k < users_.size(); k++){
			//console() << "ID = " << (*it)->getId() << " :: Pos : " << (*it)->getHeadProjectivePosition().X << "," << (*it)->getHeadProjectivePosition().Y << "," << (*it)->getHeadProjectivePosition().Z << endl;
			if (true){

				circleCenter = Vec3f(users_[k].position.x, users_[k].position.y, users_[k].position.z / 50.0f);

				//check is user Exist
				int index = -1;
				Person data;
				for (int i = 0; i < persons_.size(); i++){
					if (persons_[i].id == users_[k].id){
						index = i;
						data = persons_[i];
						data.isActive = true;
						break;
					}
				}

				if (index == -1) {
					index = persons_.size();
					data.isActive = true;
				}

				float facePoint = 0, hairPoint = 0;
				float faceFrameSizeX = faceFrameRatioX_ * users_[k].position.z;
				float faceFrameSizeY = faceFrameRatioY_ * users_[k].position.z;

				float center = spanCenter_ * video.getWidth();
				float factor = (circleCenter.x - center) * spanX_;

				float colorX1 = circleCenter.x + colorX_ + factor;
				float colorY1 = circleCenter.y + colorY_;

				float colorX2 = colorX1 + faceFrameSizeX;
				float colorY2 = colorY1 + faceFrameSizeY;

				//Color
				Rectf faceRectColor(colorX1, colorY1, colorX2, colorY2);
				data.faceSurface = colorSurface;

				for (int y = faceRectColor.y1; y <= faceRectColor.y2; y++){
					for (int x = faceRectColor.x1; x <= faceRectColor.x2; x++){
						ColorT<uint8_t> skinColorRGB = colorSurface.getPixel(Vec2i(x, y));
						Vec3f skinColorHSV = skinColorRGB.get(CM_HSV);
						if (skinColorHSV.x >= 0 && skinColorHSV.x <= 28.f / 255.f &&
							skinColorHSV.y >= 50.f / 255.f && skinColorHSV.y <= 255.f / 255.f &&
							skinColorHSV.z >= 60.f / 255.f && skinColorHSV.z <= 255.f / 255.f){
							data.faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(255, 255, 255, 1));
							facePoint++;
						}
						else if (skinColorHSV.y >= 0.0f && skinColorHSV.y <= 0.5f &&
								 skinColorHSV.z >= 0.0f && skinColorHSV.z <= 0.3f){
							data.faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(255, 0, 0, 1));
							hairPoint++;
						}
						else {
							data.faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(0, 0, 0, 1));
						}
					}
				}

				if (facePoint > faceRectColor.calcArea() * 0.1 && hairPoint > faceRectColor.calcArea() * 0.05 && k < users_.size()) {
					//if (true){
					//rebuild position

					circleCenter.x = circleCenter.x * imageRatio;
					circleCenter.y = circleCenter.y * imageRatio;

					center = spanCenter_ * video.getWidth();
					factor = (circleCenter.x - center) * spanX_;

					colorX1 = circleCenter.x + colorX_ + factor;
					colorY1 = circleCenter.y + colorY_;

					colorX2 = colorX1 + faceFrameSizeX;
					colorY2 = colorY1 + faceFrameSizeY;

					//Color
					Rectf rebuildFaceRectColor(colorX1, colorY1, colorX2, colorY2);

					data.id = users_[k].id;
					data.lastCenter = data.center;
					data.center = Vec2f(circleCenter.x, circleCenter.y);
					data.position = rebuildFaceRectColor;

					int bufferDelay = data.bufferDelay;

					if (facePoint / (facePoint + hairPoint) < faceDownPercentage_) {
						if (data.isLookUp == true){
							if (data.bufferCount > bufferDelay){
								data.isLookUp = false;
								data.bufferCount = 0;
							}
							data.bufferCount++;
						}
					}
					else {
						if (data.isLookUp == false){
							if (data.bufferCount > bufferDelay){
								data.isLookUp = true;
								data.bufferCount = 0;
							}
							data.bufferCount++;
						}
					}

					if (index == persons_.size()) persons_.push_back(Person());
					persons_[index] = data;

					if (isKinectDebugMode){

						Texture faceVideoC = Texture(data.faceSurface);
						Area faceAreaColor(faceRectColor.x1, faceRectColor.y1, faceRectColor.x2, faceRectColor.y2);

						Rectf faceDestC(id * 60, 60, (id + 1) * 60, 120);
						gl::color(Color(255, 255, 255));
						gl::draw(faceVideoC, faceAreaColor, faceDestC);

						Area rebuildFaceAreaColor(rebuildFaceRectColor.x1, rebuildFaceRectColor.y1, rebuildFaceRectColor.x2, rebuildFaceRectColor.y2);


						if (!data.isLookUp) gl::color(Color(0, 255, 0));
						else				gl::color(Color(255, 0, 0));
						gl::lineWidth(5);
						gl::drawStrokedRect(rebuildFaceAreaColor);

						gl::drawString(toString(data.id), Vec2f(rebuildFaceRectColor.x1, rebuildFaceRectColor.y1), ColorA(1.f, 0.f, 0.1f, 1.0f), Font("Arial", 30));

						id++;
					}
				}
			}
		}
		//dataMutex_.unlock();
	}
	gl::color(Color(255, 255, 255));
}

Surface PlayerManager::getColorSurface(){
	Surface surface;
	surface = *(kinectDevice_->getColorSurface());
	return surface;
}

Surface PlayerManager::getDepthSurface(){
	Surface surface;
	surface = *(kinectDevice_->getDepthSurfaceWithColor());
	return surface;
}

vector<Person> PlayerManager::getPersons(){
	return persons_;
}

void PlayerManager::setUsers(){
	vector<Skeleton> users;
	for (list<KinectUser*>::iterator it = kinectUsers_.begin(); it != kinectUsers_.end(); ++it){
		if ((*it)->getHeadProjectivePosition().X > 0 && (*it)->getHeadProjectivePosition().X < 640){
			Skeleton user;
			user.id = (*it)->getId();
			user.position = Vec3f((*it)->getHeadProjectivePosition().X, (*it)->getHeadProjectivePosition().Y, (*it)->getHeadProjectivePosition().Z);
			users.push_back(user);
		}
	}
	users_ = users;
}

void PlayerManager::moveMotorUp(){
	motorAngle_+=10;
	if (motorAngle_ > 50) motorAngle_ = 50;
	console() << motorAngle_ << endl;
	kinectDevice_->moveMotor(motorAngle_);
}

void PlayerManager::moveMotorDown(){
	motorAngle_-=10;
	if (motorAngle_ < -40) motorAngle_ = -40;
	console() << motorAngle_ << endl;
	kinectDevice_->moveMotor(motorAngle_);
}

void PlayerManager::readConfig(Bit::JsonTree* tree){
	Kinect::readConfig(tree);
	userDetectRangeMin_ = tree->getChildPtr("userDetectRangeMin")->getValue<float>();
	userDetectRangeMax_ = tree->getChildPtr("userDetectRangeMax")->getValue<float>();
	faceDownPercentage_ = tree->getChildPtr("faceDownPercentage")->getValue<float>();
}

void PlayerManager::readParams(Bit::JsonTree* tree, Bit::ParamsRef params)
{
	params->addParam<float>(tree->getChildPtr("colorX1"), colorX_);
	params->addParam<float>(tree->getChildPtr("colorY1"), colorY_);
	params->addParam<float>(tree->getChildPtr("depthX1"), depthX_);
	params->addParam<float>(tree->getChildPtr("depthY1"), depthY_);
	params->addParam<float>(tree->getChildPtr("faceFrameRationX"), faceFrameRatioX_);
	params->addParam<float>(tree->getChildPtr("faceFrameRationY"), faceFrameRatioY_);
	params->addParam<float>(tree->getChildPtr("spanX"), spanX_);
	params->addParam<float>(tree->getChildPtr("spanCenter"), spanCenter_);
}