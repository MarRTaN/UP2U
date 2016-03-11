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
	//WinExec("C:/faceApi.exe", SW_HIDE);
}

void PlayerManager::updateUsers(){
	sentImageFaceApi();
	if (isDataReady()) {
		if (motorStatus_ == false) {
			kinectDevice_->initMotor();
			kinectDevice_->moveMotor(motorAngle_);
			motorStatus_ = true;
		}

		//update framebound
		frameBound = Rectf(frameRatio_.x1 * getWindowWidth(), frameRatio_.y1 * getWindowHeight(), frameRatio_.x2 * getWindowWidth(), frameRatio_.y2 * getWindowHeight());


		int personCount = 0;
		//console() << "==================" << endl;
		while (personCount < persons_.size()){
			//console() << "ID : " << persons_[personCount].id << " , active = " << persons_[personCount].isActive << " , Lost = " << persons_[personCount].lostCount << " / " << getElapsedSeconds() << endl;
			//console() << "ID : " << persons_[personCount].id << " , pos = " << persons_[personCount].center << endl;
			if (persons_[personCount].isActive){
				if (persons_[personCount].center.x > frameBound.x1 &&
					persons_[personCount].center.x < frameBound.x2 &&
					persons_[personCount].center.y > frameBound.y1 &&
					persons_[personCount].center.y < frameBound.y2 &&
					!persons_[personCount].isPersonLost()) {
					personCount++;
					moveMotorDown();
				}
				else{
					persons_.erase(persons_.begin() + personCount);
					isMoving_ = false;
					moveMotorUp();
				}
			}
			else if(persons_[personCount].lostCount < getElapsedSeconds()){
				persons_.erase(persons_.begin() + personCount);
				isMoving_ = false;
				moveMotorUp();
			}
			else personCount++;
		}
	}
}

void PlayerManager::draw(){

	if (isDataReady()) {
		//dataMutex_.lock();
		Surface colorSurface = getColorSurface();
		Surface depthSurface = getDepthSurface();
		Texture video = gl::Texture(colorSurface);
		cv::Mat colorMat = toOcv(colorSurface);
		cv::Mat outputMat;

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

		if (isKinectDebugMode && backgroundTexture_){
			Surface bg = background_;
			Texture videoBg = Texture(bg);

			float videoW2 = videoBg.getWidth();
			float videoH2 = videoBg.getHeight();
			float windowW2 = getWindowWidth();
			float windowH2 = getWindowHeight();
			float imageRatio2 = windowW2 / videoW2;

			Area srcArea2(0, 0, videoBg.getWidth(), videoBg.getHeight());	//TODO change to videoH
			Rectf destRect2(windowW2 * 7 / 8, 0, windowW2, windowH2 / 8);					//TODO change to videoW

			//capture from kinect
			//Texture videoDepth = gl::Texture(depthSurface);
			//Area srcAreaDepth(0, 0, video.getWidth(), video.getHeight());
			//gl::color(10, 100, 200, 0.2);
			//gl::draw(videoDepth, srcAreaDepth, destRect1);

			gl::color(255,255,255,1);
			gl::draw(videoBg, srcArea2, destRect2);

			//draw boundary
			gl::lineWidth(2);
			gl::color(255, 0, 0);
			gl::drawStrokedRect(frameBound);
		}

		Vec2f circleCenter;
		int id = 0;

		//console() << "==========================" << endl;
		for (int i = 0; i < persons_.size(); i++){
			//console() << "id = " << persons_[i].id << " :: Pos : " << persons_[i].center.x << "," << persons_[i].center.y << endl;
			if (persons_[i].isActive){
				persons_[i].isActive = false;
				persons_[i].lostCount = getElapsedSeconds() + 2;
			}
		}

		if (kinectUsers_.size() < persons_.size()){
			if (countForDelay_ < 11) moveMotorUp();
			else if (countForDelay_ < 22) moveMotorDown();
			else countForDelay_ = 0;
			countForDelay_++;
		}

		headBound = Rectf(frameRatio_.x1 * videoW, frameRatio_.y1 * videoH, frameRatio_.x2 * videoW, frameRatio_.y2 * videoH);


		for (int k = 0; k < users_.size(); k++){
			circleCenter = Vec2f(users_[k].position.x, users_[k].position.y);
			if (circleCenter.y > headBound.y1 &&
				circleCenter.y < headBound.y2 &&
				circleCenter.x > headBound.x1 &&
				circleCenter.x < headBound.x2){
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
					data.angleMean.reserve(smoothMeanCount_ + 5);
					data.kinectCenterMean.reserve(smoothMeanCount_ + 5);
				}

				if (data.kinectCenterMean.size() > smoothMeanCount_){
					data.kinectCenterMean.erase(data.kinectCenterMean.begin());
				}

				data.kinectCenterMean.push_back(circleCenter);
				data.calCenterMean();

				data.angleLookLeft = angleLookLeft_;
				data.angleLookRight = angleLookRight_;
				
				//console() << "id = " << data.id << " :: centerMean : " << data.kinectCenter << " :: user id : " << users_[k].id << " , count lost : " << data.lostCount << "/" << getElapsedSeconds() << " :: isActive = " << data.isActive << endl;


				float facePoint = 0, hairPoint = 0;
				float faceFrameSizeX = faceFrameRatioX_ * users_[k].position.z;
				float faceFrameSizeY = faceFrameRatioY_ * users_[k].position.z;

				//float faceFrameSizeX = 60.f;
				//float faceFrameSizeY = 60.f;

				float center = spanCenter_ * video.getWidth();
				float factor = (data.kinectCenter.x - center) * spanX_;

				float colorX1 = data.kinectCenter.x + colorX_ + factor;
				float colorY1 = data.kinectCenter.y + colorY_;

				float colorX2 = colorX1 + faceFrameSizeX;
				float colorY2 = colorY1 + faceFrameSizeY;

				//Color
				Rectf faceRectColor(colorX1, colorY1, colorX2, colorY2);
				cv::Rect faceRect(colorX1, colorY1, colorX2 - colorX1, colorY2 - colorY1);
				data.faceRectColorDebug = faceRectColor;
				data.faceSurface = colorSurface;


				//Centroid
				Vec2i hairCentroid = Vec2i(0, 0);
				Vec2i faceCentroid = Vec2i(0, 0);

				cv::Mat cvtMat;
				cv::cvtColor(colorMat(faceRect), cvtMat, CV_BGR2HSV);
				//cv::inRange(cvtMat, cv::Scalar(110, 10, 60), cv::Scalar(170, 150, 255), outputMat);
				cv::inRange(cvtMat, cv::Scalar(0, 40, 60), cv::Scalar(27, 150, 255), outputMat);
				double sumfacePoint = cv::sum(outputMat)[0];

				/*boost::filesystem::path path("C:\\Users\\MarRTaN\\Downloads\\faceApiFile\\faces\\");
				path /= "test.jpg";
				writeImage(path, fromOcv(outputMat));

				console() << sumfacePoint << endl;*/

				Surface calSurface = fromOcv(outputMat);

				Texture faceVideoC = Texture(calSurface);
				Area faceAreaColor(0, 0, calSurface.getWidth(), calSurface.getHeight());

				Rectf faceDestC((users_[k].id - 1) * 60, 60, users_[k].id * 60, 120);
				gl::color(Color(255, 255, 255));
				gl::draw(faceVideoC, faceAreaColor, faceDestC);

				//----------------------------


				Surface calSurface2 = fromOcv(colorMat(faceRect));

				Texture faceVideoC2 = Texture(calSurface2);
				Area faceAreaColor2(0, 0, calSurface2.getWidth(), calSurface2.getHeight());

				Rectf faceDestC2((users_[k].id - 1) * 60, 120, users_[k].id * 60, 180);
				gl::color(Color(255, 255, 255));
				gl::draw(faceVideoC2, faceAreaColor2, faceDestC2);


				/*for (int y = faceRectColor.y1; y <= faceRectColor.y2; y += facePixelMultiply){
					for (int x = faceRectColor.x1; x <= faceRectColor.x2; x += facePixelMultiply){
						ColorT<uint8_t> skinColorRGB = colorSurface.getPixel(Vec2i(x, y));
						Vec3f skinColorHSV = skinColorRGB.get(CM_HSV);
						if (!isBackground(Vec2i(x, y), skinColorHSV)){
							if (skinColorHSV.x >= 0 && skinColorHSV.x <= 28.f / 255.f &&
								skinColorHSV.y >= 50.f / 255.f && skinColorHSV.y <= 255.f / 255.f &&
								skinColorHSV.z >= 60.f / 255.f && skinColorHSV.z <= 255.f / 255.f){
								data.faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(255, 255, 255, 1));
								facePoint++;
								faceCentroid += Vec2i(x, y);
							}
							else if (skinColorHSV.y >= 0.0f && skinColorHSV.y <= 0.5f &&
								skinColorHSV.z >= 0.0f && skinColorHSV.z <= 0.3f){
								data.faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(255, 0, 0, 1));
								hairPoint++;
								hairCentroid += Vec2i(x, y);
							}
							else{
								data.faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(255, 255, 0, 1));
							}
						}
						else {
							data.faceSurface.setPixel(Vec2i(x, y), ColorAT<uint8_t>(0, 255, 0, 1));
						}
					}
				}*/

				if (k < users_.size() &&
					//facePoint > faceRectColor.calcArea() / facePixelMultiply * 0.02 && 
					//hairPoint > faceRectColor.calcArea() / facePixelMultiply * 0.02) {
					facePoint > 0 &&
					hairPoint > 0) {

					//rebuild position
					data.center = data.kinectCenter * imageRatio;
					hairCentroid = hairCentroid * imageRatio;
					faceCentroid = faceCentroid * imageRatio;

					center = spanCenter_ * video.getWidth();
					factor = (data.center.x - center) * spanX_;

					colorX1 = data.center.x + colorX_ + factor;
					colorY1 = data.center.y + colorY_;

					colorX2 = colorX1 + faceFrameSizeX;
					colorY2 = colorY1 + faceFrameSizeY;

					//Color
					Rectf rebuildFaceRectColor(colorX1, colorY1, colorX2, colorY2);

					data.id = users_[k].id;
					data.lastCenter = data.center;
					data.position = rebuildFaceRectColor;
					data.faceCentroid = faceCentroid / facePoint;
					data.hairCentroid = hairCentroid / hairPoint;

					//if (data.faceCentroid.y > data.hairCentroid.y){
					if (true) {
						float diff = ( (data.faceCentroid.x - data.hairCentroid.x) * 1.f) / ( (data.faceCentroid.y - data.hairCentroid.y) * 1.f );
						float angle = atan(diff) * 180 / M_PI;

						if (data.angleMean.size() > smoothMeanCount_){
							data.angleMean.erase( data.angleMean.begin());
						}

						data.angleMean.push_back(angle);
						data.calAngleMean();

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

									if (!data.isImageSaved){
										/*boost::filesystem::path path("C:\\Users\\MarRTaN\\Downloads\\faceApiFile\\faces\\");
										path /= to_string(data.id) + ".jpg";
										writeImage(path, copyWindowSurface());*/
										data.isImageSaved = true;
									}
								}
								data.bufferCount++;
							}
						}

						//check person gender
						if (data.isImageSaved){
							if (data.gender == UNDEFINED && data.delayCallFaceApi >= 600){
								/*std::ifstream myfile;
								myfile.open("C:\\Users\\MarRTaN\\Downloads\\faceApiFile\\output.txt");
								std::string str;
								std::string file_contents;
								std::getline(myfile, str);
								file_contents += str;
								cout << "file :: " << file_contents << endl;

								if (file_contents != "" && file_contents != "[]"){
									rapidjson::Document d;

									char json[1024];
									std::strcpy(json, file_contents.c_str());

									d.Parse(json);

									string gender = d[0]["faceAttributes"]["gender"].GetString();
									if (gender == "male") data.gender = MALE;
									else if (gender == "female") data.gender = FEMALE;
								}*/
								data.delayCallFaceApi = 0;
							}
							data.delayCallFaceApi++;
						}

						if (index == persons_.size()) persons_.push_back(Person());
						persons_[index] = data;
					}
				}
			}
		}

		for (int i = 0; i < persons_.size(); i++){


			if (isKinectDebugMode){

				if (isDrawface){
					Rectf faceRectColor = persons_[i].faceRectColorDebug;
					Texture faceVideoC = Texture(persons_[i].faceSurface);
					Area faceAreaColor(faceRectColor.x1, faceRectColor.y1, faceRectColor.x2, faceRectColor.y2);

					Rectf faceDestC(id * 60, 60, (id + 1) * 60, 120);
					gl::color(Color(255, 255, 255));
					gl::draw(faceVideoC, faceAreaColor, faceDestC);
				}

				Rectf rebuildFaceRectColor = persons_[i].position;

				Area rebuildFaceAreaColor(rebuildFaceRectColor.x1, rebuildFaceRectColor.y1, rebuildFaceRectColor.x2, rebuildFaceRectColor.y2);


				if (!persons_[i].isLookUp) gl::color(Color(0, 255, 0));
				else					   gl::color(Color(255, 0, 0));
				gl::lineWidth(5);
				gl::drawStrokedRect(rebuildFaceAreaColor);
				gl::drawString(toString(persons_[i].id), Vec2f(rebuildFaceRectColor.x1, rebuildFaceRectColor.y1), ColorA(1.f, 0.f, 0.1f, 1.0f), Font("Arial", 30));
				gl::drawString(toString(persons_[i].angle), Vec2f(rebuildFaceRectColor.x2, rebuildFaceRectColor.y2), ColorA(1.f, 0.f, 0.1f, 1.0f), Font("Arial", 30));

				gl::color(0, 255, 255);
				gl::drawStrokedCircle(persons_[i].hairCentroid, 2);

				gl::color(255, 255, 0);
				gl::drawStrokedCircle(persons_[i].faceCentroid, 2);

				id++;
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
	if (!isMoving_) {
		motorAngle_ = -25;
		isMoving_ = true;
		kinectDevice_->moveMotor(motorAngle_);
	}
	else if (motorUpCount_ > 10){
		motorUpCount_ = 0;
		isMoving_ = false;
	}
	else{
		motorUpCount_++;
	}
}

void PlayerManager::moveMotorDown(){
	if (!isMoving_) {
		motorAngle_ = -25;
		isMoving_ = true;
		kinectDevice_->moveMotor(motorAngle_);
	}
	else if (motorDownCount_ > 10){
		motorDownCount_ = 0;
		isMoving_ = false;
	}
	else{
		motorDownCount_++;
	}
}

bool PlayerManager::isBackground(Vec2i pixel, Vec3f color){
	if (backgroundTexture_){
		ColorT<uint8_t> bgColorRGB = background_.getPixel(pixel);
		Vec3f bgColorHSV = bgColorRGB.get(CM_HSV);

		return	abs(bgColorHSV.x - color.x) <= 10.f / 255.f &&
			abs(bgColorHSV.y - color.y) <= 10.f / 255.f &&
			abs(bgColorHSV.z - color.z) <= 10.f / 255.f;
	}
	return false;
}

void PlayerManager::setBackground(){
	Surface bg = getColorSurface();
	backgroundTexture_ = Texture(bg);
	background_ = Surface(backgroundTexture_);
}

void PlayerManager::sentImageFaceApi(){
	//sender sender_("localhost", "6677");
	/*rapidjson::Document d;
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	d.SetObject();
	d.AddMember("test", "marrtan", allocator);
	sender_.send(d);*/
}

void PlayerManager::readConfig(Bit::JsonTree* tree){
	Kinect::readConfig(tree);
	userDetectRangeMin_ = tree->getChildPtr("userDetectRangeMin")->getValue<float>();
	userDetectRangeMax_ = tree->getChildPtr("userDetectRangeMax")->getValue<float>();
}

void PlayerManager::readParams(Bit::JsonTree* tree, Bit::ParamsRef params)
{
	params->addParam<float>(tree->getChildPtr("colorX1"), colorX_);
	params->addParam<float>(tree->getChildPtr("colorY1"), colorY_);
	params->addParam<float>(tree->getChildPtr("depthX1"), depthX_);
	params->addParam<float>(tree->getChildPtr("depthY1"), depthY_);
	params->addParam<float>(tree->getChildPtr("faceFrameRatioX"), faceFrameRatioX_);
	params->addParam<float>(tree->getChildPtr("faceFrameRatioY"), faceFrameRatioY_);
	params->addParam<float>(tree->getChildPtr("spanX"), spanX_);
	params->addParam<float>(tree->getChildPtr("spanCenter"), spanCenter_);
	params->addParam<float>(tree->getChildPtr("angleLookLeft"), angleLookLeft_);
	params->addParam<float>(tree->getChildPtr("angleLookRight"), angleLookRight_);
	params->addParam<int>(tree->getChildPtr("smoothMeanCount"), smoothMeanCount_);
	params->addParam<float>(tree->getChildPtr("faceDownPercentage"), faceDownPercentage_);
	params->addParam<float>(tree->getChildPtr("frameRatio")->getChildPtr("x1"), frameRatio_.x1);
	params->addParam<float>(tree->getChildPtr("frameRatio")->getChildPtr("y1"), frameRatio_.y1);
	params->addParam<float>(tree->getChildPtr("frameRatio")->getChildPtr("x2"), frameRatio_.x2);
	params->addParam<float>(tree->getChildPtr("frameRatio")->getChildPtr("y2"), frameRatio_.y2);

}