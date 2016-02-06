#include <PlayerManager.h>

void PlayerManager::threadUpdate()
{
	Kinect::threadUpdate();
	dataMutex_.lock();
	if (isDataReady()) {
		//function here
		kinectUsers_ = kinectDevice_->getUsers();
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
}

void PlayerManager::updateUsers(){
	if (isDataReady()) {
		int personCount = 0;
		while (personCount < persons_.size()){
			if (persons_[personCount].isActive) personCount++;
			else{
				persons_.erase(persons_.begin(), persons_.begin() + personCount);
			}
		}
	}
}

void PlayerManager::draw(){
	if (isDataReady()) {
		Surface colorSurface = getColorSurface();
		Surface depthSurface = getDepthSurface();
		Texture video = gl::Texture(colorSurface);

		float videoW = video.getWidth();
		float videoH = video.getHeight();
		float windowW = getWindowWidth();
		float windowH = getWindowHeight();

		float destX = ((windowH / videoH)*videoW);
		float delX = (destX - windowW) / 2.0f;

		Area srcArea1(0, 0, video.getWidth(), video.getHeight());	//TODO change to videoH
		Rectf destRect1(0, 0, windowW, windowH);					//TODO change to videoW

		//capture from kinect
		gl::color(Color(255, 255, 255));
		gl::draw(video, srcArea1, destRect1);

		Vec3f circleCenter;
		int id = 0;
		for (list<KinectUser*>::iterator it = kinectUsers_.begin(); it != kinectUsers_.end(); ++it){

			if ((*it)->getPosition().Z > 1000){

				circleCenter = Vec3f((*it)->getHeadProjectivePosition().X, (*it)->getHeadProjectivePosition().Y, (*it)->getHeadProjectivePosition().Z / 50.0f);

				//check is user Exist
				int index = -1;
				for (int i = 0; i < persons_.size(); i++){
					if (persons_[i].id == (*it)->getId()){
						index = i;
						persons_[i].isActive = true;
						break;
					}
				}

				if (index == -1) {
					index = persons_.size();
					persons_.push_back(Person());
					persons_[index].isActive = true;
				}

				persons_[index].id = (*it)->getId();
				persons_[index].center = Vec2f(circleCenter.x, circleCenter.y);

				float facePoint = 0, hairPoint = 0;
				float faceFrameSizeX = faceFrameRatioX_ * (*it)->getHeadProjectivePosition().Z;
				float faceFrameSizeY = faceFrameRatioY_ * (*it)->getHeadProjectivePosition().Z;

				float center = spanCenter_ * video.getWidth();
				float factor = (circleCenter.x - center) * spanX_;

				float colorX1 = circleCenter.x + colorX_ + factor;
				float colorY1 = circleCenter.y + colorY_;
				float depthX1 = circleCenter.x + depthX_;
				float depthY1 = circleCenter.y + depthY_;

				float colorX2 = colorX1 + faceFrameSizeX;
				float colorY2 = colorY1 + faceFrameSizeY;
				float depthX2 = depthX1 + faceFrameSizeX;
				float depthY2 = depthY1 + faceFrameSizeY;

				//Color
				Rectf faceRectColor(colorX1, colorY1, colorX2, colorY2);

				persons_[index].position = faceRectColor;
				persons_[index].faceSurface = colorSurface;

				for (int y = faceRectColor.y1; y <= faceRectColor.y2; y++){
					for (int x = faceRectColor.x1; x <= faceRectColor.x2; x++){
						ColorT<uint8_t> skinColorRGB = persons_[index].faceSurface.getPixel(Vec2i(x, y));
						Vec3f skinColorHSV = skinColorRGB.get(CM_HSV);
						if (skinColorHSV.x >= 0 && skinColorHSV.x <= 28.f / 255.f &&
							skinColorHSV.y >= 50.f / 255.f && skinColorHSV.y <= 255.f / 255.f &&
							skinColorHSV.z >= 60.f / 255.f && skinColorHSV.z <= 255.f / 255.f){
							persons_[index].faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(255, 255, 255, 1));
							facePoint++;
						}
						else if (skinColorHSV.z >= 0.0f && skinColorHSV.z <= 0.3f){
							persons_[index].faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(255, 0, 0, 1));
							hairPoint++;
						}
						else {
							persons_[index].faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(0, 0, 0, 1));
						}
					}
				}

				int bufferDelay = persons_[index].bufferDelay;

				if (facePoint / (facePoint + hairPoint) < faceDownPercentage_) {
					if (persons_[index].isLookUp == true){
						if (persons_[index].bufferCount > bufferDelay){
							persons_[index].isLookUp = false;
							persons_[index].bufferCount = 0;
						}
						persons_[index].bufferCount++;
					}
				}
				else {
					if (persons_[index].isLookUp == false){
						if (persons_[index].bufferCount > bufferDelay){
							persons_[index].isLookUp = true;
							persons_[index].bufferCount = 0;
						}
						persons_[index].bufferCount++;
					}
				}

				if (isKinectDebugMode){

					Texture faceVideoC = Texture(persons_[index].faceSurface);
					Area faceAreaColor(faceRectColor.x1, faceRectColor.y1, faceRectColor.x2, faceRectColor.y2);

					Rectf faceDestC(id * 60, 0, (id + 1) * 60, 60);
					gl::color(Color(255, 255, 255));
					gl::draw(faceVideoC, faceAreaColor, faceDestC);

					if (!persons_[index].isLookUp) gl::color(Color(0, 255, 0));
					else					   gl::color(Color(255, 0, 0));
					gl::lineWidth(5);
					gl::drawStrokedRect(faceRectColor);

					id++;
				}
			}

		}
	}
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