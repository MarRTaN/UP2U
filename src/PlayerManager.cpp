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
	
	int cameraCount = 0;

	Capture::DeviceRef cameraDevice;

	// print the devices
	for (auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device) {
		if (cameraCount == selectedCamera_) cameraDevice = *device;
		cameraCount++;
	}

	try {
		mCapture_ = Capture::create(1920, 1080, cameraDevice);
		mCapture_->start();
	}
	catch (...) {
		console() << "Failed to initialize capture" << std::endl;
	}
}

void PlayerManager::updateUsers(){

	if (isDataReady()) {

		if (mCapture_ && mCapture_->checkNewFrame()) {
			mSurface_ = mCapture_->getSurface();
			mTexture_ = ci::gl::Texture::create(mSurface_);
		}

		/*if (motorStatus_ == false) {
			motorAngle_ = motorAngleDown_;
			kinectDevice_->initMotor();
			kinectDevice_->moveMotor(motorAngle_);
			motorStatus_ = true;
		}*/

		//update framebound
		frameBound = Rectf(frameRatio_.x1 * getWindowWidth(), frameRatio_.y1 * getWindowHeight(), frameRatio_.x2 * getWindowWidth(), frameRatio_.y2 * getWindowHeight());


		int personCount = 0;
		while (personCount < persons_.size()){
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
					ofstream savefile;
					savefile.open(saveImagePath_+"output.txt");
					savefile << "";
					savefile.close();

					persons_.erase(persons_.begin() + personCount);
					isMoving_ = false;
					moveMotorUp();
				}
			}
			else if(persons_[personCount].lostCount < getElapsedSeconds()){

				ofstream savefile;
				savefile.open(saveImagePath_+"output.txt");
				savefile << "";
				savefile.close();

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
		cv::Mat colorMat = toOcvRef(colorSurface);

		videoW = video.getWidth();
		videoH = video.getHeight();
		float windowW = getWindowWidth();
		float windowH = getWindowHeight();
		float imageRatio = windowW / videoW;
		float windowHeightShift = windowH;// (windowW * tx.getHeight() / tx.getWidth());

		Area srcArea(0, 0, video.getWidth(), video.getHeight());
		Rectf destRect(0, windowH - windowHeightShift, windowW, windowH);

		//capture from kinect 
		if (!isKinectDebugMode){

			if (mSurface_){
				Texture tx = Texture(mSurface_);
				Area srcAreaCamera(0.05 * tx.getWidth(), 0, 0.95 * tx.getWidth(), tx.getHeight());
				Rectf destCamera(windowW, 0.1 * windowH, 0, windowH);
				glPushMatrix();
				gl::draw(mTexture_, srcAreaCamera, destCamera);
				glPopMatrix();
			}

			if (captureDelay_ > 180){

				time_t rawtime;
				struct tm * timeinfo;
				char timeBuffer[80];
				char dateBuffer[80];

				time(&rawtime);
				timeinfo = localtime(&rawtime);

				strftime(timeBuffer, 80, "%I-%M-%S", timeinfo);
				strftime(dateBuffer, 80, "%Y-%m-%d", timeinfo);

				std::string strTime(timeBuffer);
				std::string strDate(dateBuffer);
				std::string detail = "Detail :: ";

				for (int i = 0; i < persons_.size(); i++){

					string direction = "";
					if (persons_[i].angle > persons_[i].angleLookRight){
						direction = "RIGHT";
					}
					else if (persons_[i].angle < persons_[i].angleLookLeft){
						direction = "LEFT";
					}
					else {
						if (persons_[i].isLookUp)
							direction = "UP";
						else
							direction = "DOWN";
					}

					detail += "id = " + to_string(i) +
						   "\nloc=" + to_string(persons_[i].center.x) + "," + to_string(persons_[i].center.y) +
						   "\ngen=" + to_string(persons_[i].gender) +
						   "\nlookup=" + direction + "\n";

					console() << detail << endl;
				}

				boost::filesystem::path path(saveCapturePath_ + strDate + "\\");
				path /= strTime + ".jpg";
				writeImage(path, mSurface_);

				string detailPath = saveCapturePath_ + strDate + "\\";

				ofstream sf;
				string savePath = detailPath + strTime + ".txt";
				console() << savePath << endl;
				sf.open(savePath);
				sf << detail;
				sf.close();

				captureDelay_ = 0;
			}
			
			captureDelay_++;

		}
		else{

			gl::color(Color(255, 255, 255));
			gl::draw(video, srcArea, destRect);

			if (backgroundTexture_){
				Surface bg = background_;
				Texture videoBg = Texture(bg);

				float videoW2 = videoBg.getWidth();
				float videoH2 = videoBg.getHeight();
				float windowW2 = getWindowWidth();
				float windowH2 = getWindowHeight();
				float imageRatio2 = windowW2 / videoW2;

				Area srcArea2(0, 0, videoBg.getWidth(), videoBg.getHeight());
				Rectf destRect2(windowW2 * 7 / 8, 0, windowW2, windowH2 / 8);

				gl::color(255, 255, 255, 1);
				gl::draw(videoBg, srcArea2, destRect2);
			}
			//draw boundary
			gl::lineWidth(2);
			gl::color(255, 0, 0);
			gl::drawStrokedRect(frameBound);
		}

		Vec2f circleCenter;
		int id = 0;

		for (int i = 0; i < persons_.size(); i++){
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
		
		int idNow = 1;
		for (int k = 0; k < users_.size(); k++){
			circleCenter = Vec2f(users_[k].position.x, users_[k].position.y);
			if (circleCenter.y > headBound.y1 &&
				circleCenter.y < headBound.y2 &&
				circleCenter.x > headBound.x1 &&
				circleCenter.x < headBound.x2 &&
				users_[k].position.z > 1500){

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

				data.depth = users_[k].position.z;
				
				float facePoint = 0, hairPoint = 0;
				float faceFrameSizeX = 120.f / ( users_[k].position.z / 1000.f);
				float faceFrameSizeY = 120.f / ( users_[k].position.z / 1000.f);

				//float faceFrameSizeX = 60.f;
				//float faceFrameSizeY = 60.f;

				console() << users_[k].position.z << endl;

				float center = spanCenter_ * video.getWidth();
				float factor = (data.kinectCenter.x - center) * spanX_;

				float colorX1 = data.kinectCenter.x + colorX_ + factor;
				float colorY1 = data.kinectCenter.y + colorY_;

				float colorX2 = colorX1 + faceFrameSizeX;
				float colorY2 = colorY1 + faceFrameSizeY;

				//Color
				Rectf faceRectColor(colorX1, colorY1, colorX2, colorY2);
				float faceRectWidth = colorX2 - colorX1;
				float faceRectHeight = colorY2 - colorY1;
				cv::Rect faceRect(colorX1, colorY1, faceRectWidth, faceRectHeight);

				data.faceRectColorDebug = faceRectColor;
				data.faceSurface = colorSurface;


				//Centroid
				faceData faceD, hairD;
				Vec2i hairCentroid = Vec2i(0, 0);
				Vec2i faceCentroid = Vec2i(0, 0);

				//Cal hair
				hairD = getCentroid(0, idNow, colorMat, faceRect, cv::Scalar(0, 0, 0), cv::Scalar(180, 255, 80), cv::Size(7, 7), Vec2f(150, 255));
				faceD = getCentroid(1, idNow, colorMat, faceRect, cv::Scalar(0, 40, 60), cv::Scalar(27, 150, 255), cv::Size(15, 15), Vec2f(120, 255));
				idNow++;

				hairCentroid = hairD.pos;
				faceCentroid = faceD.pos;
				
				hairPoint = hairD.count;
				facePoint = faceD.count;

				//console() << (facePoint + hairPoint) / (faceRect.area() * 1.f) << endl;

				if (k < users_.size() &&
					//facePoint > faceRectColor.calcArea() / facePixelMultiply * 0.02 && 
					//hairPoint > faceRectColor.calcArea() / facePixelMultiply * 0.02) {
					hairPoint != -1 &&
					facePoint != -1 &&
					(facePoint + hairPoint) / ( faceRect.area() * 1.f ) > 0.15) {

					//rebuild position
					data.center = data.kinectCenter * imageRatio;
					data.center.x = data.center.x + shift_.x;
					data.center.y = data.center.y + shift_.y;
					hairCentroid = hairCentroid * imageRatio;
					faceCentroid = faceCentroid * imageRatio;

					center = spanCenter_ * video.getWidth();
					factor = 0;// (data.center.x - center) * spanX_;

					colorX1 = data.center.x + colorX_ + factor;
					colorY1 = data.center.y + colorY_;

					colorX2 = colorX1 + faceFrameSizeX;
					colorY2 = colorY1 + faceFrameSizeY;

					//Color
					Rectf rebuildFaceRectColor(colorX1, colorY1, colorX2, colorY2);

					data.id = users_[k].id;
					data.lastCenter = data.center;
					data.position = rebuildFaceRectColor;
					data.faceCentroid = faceCentroid;
					data.hairCentroid = hairCentroid;

					if (true) {
						float diff = ((data.faceCentroid.x - data.hairCentroid.x) * 1.f) / ((data.faceCentroid.y - data.hairCentroid.y) * 1.f);
						float angle = atan(diff) * 180 / M_PI;

						if (data.angleMean.size() > smoothMeanCount_){
							data.angleMean.erase(data.angleMean.begin());
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
										boost::filesystem::path path(saveImagePath_ + "faces\\");
										path /= to_string(data.id) + ".jpg";
										cv::Mat cvtColorMat;
										cv::cvtColor(colorMat(faceRect), cvtColorMat, CV_BGR2RGB);
										cv::resize(cvtColorMat, cvtColorMat, cv::Size(faceRectWidth * 4, faceRectHeight * 4));
										writeImage(path, fromOcv(cvtColorMat));
										data.isImageSaved = true;
									}
								}
								data.bufferCount++;
							}
						}

						//check person gender
						if (data.isImageSaved){

							std::ifstream statusfile;
							statusfile.open(saveImagePath_ + "status.txt");
							std::string statusStr;
							std::string status_contents;
							std::getline(statusfile, statusStr);
							status_contents += statusStr;

							if (status_contents == "ready" &&
								data.gender == UNDEFINED && 
								data.delayCallFaceApi >= data.callFaceApi){
								
								ofstream savefile;
								savefile.open(saveImagePath_+"input.txt");
								savefile << saveImagePath_+"faces\\" << to_string(data.id) << ".jpg";
								savefile.close();

								//WinExec("C:/faceApi.exe", SW_HIDE);

								std::ifstream myfile;
								myfile.open(saveImagePath_+"output.txt");
								std::string str;
								std::string file_contents;
								std::getline(myfile, str);
								file_contents += str;

								console() << "id = " << data.id << ", content = " << file_contents << endl;

								if (file_contents != "" && file_contents != "[]"){
									rapidjson::Document d;

									char json[1024];
									std::strcpy(json, file_contents.c_str());

									d.Parse(json);

									string gender = d[0]["faceAttributes"]["gender"].GetString();
									if (gender == "male") data.gender = MALE;
									else if (gender == "female") data.gender = FEMALE;

									console() << "id = " << data.id << ", gender = " << gender << endl;
								}
								data.delayCallFaceApi = 0;
								data.callFaceApi *= 2;
							}
							data.delayCallFaceApi++;
						}

						if (index == persons_.size()) persons_.push_back(Person());
						persons_[index] = data;
					}
				}
			}
		}

		if (isKinectDebugMode){
			for (int i = 0; i < persons_.size(); i++){
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
				gl::color(Color(255, 0, 0));
				gl::lineWidth(5);
				gl::drawStrokedRect(rebuildFaceAreaColor);
				gl::drawString(toString(persons_[i].id), Vec2f(rebuildFaceRectColor.x1, rebuildFaceRectColor.y1), ColorA(1.f, 0.f, 0.1f, 1.0f), Font("Arial", 30));
				gl::drawString(toString(persons_[i].angle), Vec2f(rebuildFaceRectColor.x2, rebuildFaceRectColor.y2), ColorA(1.f, 0.f, 0.1f, 1.0f), Font("Arial", 30));

				gl::color(255, 0, 0);
				gl::drawStrokedCircle(persons_[i].hairCentroid + persons_[i].center, 6);

				gl::color(255, 255, 0);
				gl::drawStrokedCircle(persons_[i].faceCentroid + persons_[i].center, 6);

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
		if ((*it)->getHeadProjectivePosition().X > 20 && (*it)->getHeadProjectivePosition().X < 620){
			Skeleton user;
			user.id = (*it)->getId();
			user.position = Vec3f((*it)->getHeadProjectivePosition().X, (*it)->getHeadProjectivePosition().Y, (*it)->getHeadProjectivePosition().Z);
			users.push_back(user);
		}
	}
	users_ = users;
}

void PlayerManager::moveMotorUp(){
	/*if (!isMoving_) {
		motorAngle_ = motorAngleUp_;
		isMoving_ = true;
		kinectDevice_->moveMotor(motorAngle_);
	}
	else if (motorUpCount_ > 10){
		motorUpCount_ = 0;
		isMoving_ = false;
	}
	else{
		motorUpCount_++;
	}*/
}

void PlayerManager::moveMotorDown(){
	/*if (!isMoving_) {
		motorAngle_ = motorAngleDown_;
		isMoving_ = true;
		kinectDevice_->moveMotor(motorAngle_);
	}
	else if (motorDownCount_ > 10){
		motorDownCount_ = 0;
		isMoving_ = false;
	}
	else{
		motorDownCount_++;
	}*/
}

PlayerManager::faceData PlayerManager::getCentroid(int type, int idNow, cv::Mat colorMat, cv::Rect faceRect, cv::Scalar minColor, cv::Scalar maxColor, cv::Size blurSize, Vec2f threadhold){
	
	cv::Mat cvtHairMat;
	cv::Mat outputHairMat;

	cv::cvtColor(colorMat(faceRect), cvtHairMat, CV_RGB2HSV);
	cv::inRange(cvtHairMat, minColor, maxColor, outputHairMat);

	if (isKinectDebugMode){
		Surface calSurface2 = fromOcv(outputHairMat);
		Texture faceVideoC2 = Texture(calSurface2);
		Area faceAreaColor2(0, 0, calSurface2.getWidth(), calSurface2.getHeight());

		//Rectf faceDestC2((idNow - 1) * 60, (type * 60) + 60 + 500, idNow * 60, (type * 60) + 120 + 500);
		Rectf faceDestC2((idNow - 1) * 60, (type * 60) + 60 + 60, idNow * 60, (type * 60) + 120 + 60);
		gl::color(Color(255, 255, 255));
		gl::draw(faceVideoC2, faceAreaColor2, faceDestC2);
	}

	cv::Mat element_0 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::morphologyEx(outputHairMat, outputHairMat, cv::MORPH_CLOSE, element_0);

	if (isKinectDebugMode){
		Surface calSurface2 = fromOcv(outputHairMat);
		Texture faceVideoC2 = Texture(calSurface2);
		Area faceAreaColor2(0, 0, calSurface2.getWidth(), calSurface2.getHeight());

		//Rectf faceDestC2((idNow - 1) * 60, (type * 60) + 60 + 500, idNow * 60, (type * 60) + 120 + 500);
		Rectf faceDestC2((idNow - 1) * 60, (type * 60) + 60 + 60 + 120, idNow * 60, (type * 60) + 120 + 60 + 120);
		gl::color(Color(255, 255, 255));
		gl::draw(faceVideoC2, faceAreaColor2, faceDestC2);
	}

	/*cv::GaussianBlur(outputHairMat, outputHairMat, blurSize, 1, 1);
	cv::threshold(outputHairMat, outputHairMat, threadhold.x, threadhold.y, cv::THRESH_BINARY);*/
	cv::Mat element_1 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::morphologyEx(outputHairMat, outputHairMat, cv::MORPH_ERODE, element_1);

	if (isKinectDebugMode){
		Surface calSurface2 = fromOcv(outputHairMat);
		Texture faceVideoC2 = Texture(calSurface2);
		Area faceAreaColor2(0, 0, calSurface2.getWidth(), calSurface2.getHeight());

		//Rectf faceDestC2((idNow - 1) * 60, (type * 60) + 60 + 500, idNow * 60, (type * 60) + 120 + 500);
		Rectf faceDestC2((idNow - 1) * 60, (type * 60) + 60 + 60 + 240, idNow * 60, (type * 60) + 120 + 60 + 240);
		gl::color(Color(255, 255, 255));
		gl::draw(faceVideoC2, faceAreaColor2, faceDestC2);
	}

	double sumHairPoint = cv::sum(outputHairMat)[0];

	//contour

	vector<vector<cv::Point> > faceContours;
	cv::findContours(outputHairMat, faceContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	int bigestFaceContour = -1;
	int bigestFaceSize = 0;
	int cX = faceRect.x + faceRect.width / 2;
	int cY = faceRect.y + faceRect.height / 2;

	for (int ci = 0; ci < faceContours.size(); ci++){
		cv::Moments faceM = cv::moments(faceContours[ci]);
		double cYY = int(faceM.m01 / faceM.m00);
		if (type == 0 && cYY < faceRect.height / 2.f){
			if (bigestFaceContour == -1) {
				bigestFaceSize = faceContours[ci].size();
				bigestFaceContour = ci;
			}
			else if (faceContours[ci].size() > bigestFaceSize){
				bigestFaceContour = ci;
			}
		}
		else if (type == 1){
			if (ci == 0) {
				bigestFaceSize = faceContours[ci].size();
				bigestFaceContour = ci;
			}
			else if (faceContours[ci].size() > bigestFaceSize){
				bigestFaceContour = ci;
			}
		}
	}
	
	if (bigestFaceContour != -1){
		cv::Moments faceM = cv::moments(faceContours[bigestFaceContour]);
		cX = int(faceM.m10 / faceM.m00);
		cY = int(faceM.m01 / faceM.m00);

		if (isKinectDebugMode){
			if (type == 0) gl::color(Color(255, 0, 0));
			else		  gl::color(Color(255, 255, 0));
			float cxx = (cX + faceRect.x + faceRect.width) * getWindowWidth() / (1.2*640.f);
			float cyy = (cY + faceRect.y + faceRect.height) * getWindowHeight() / (1.2*480.f);
			//gl::drawSolidCircle(Vec2f(cX, cY), 5);
		}
	}

	faceData fd;
	fd.pos = Vec2i(cX, cY);
	fd.count = -1;
	if (bigestFaceContour != -1) fd.count = cv::contourArea(faceContours[bigestFaceContour]);

	return fd;
}

void PlayerManager::readConfig(Bit::JsonTree* tree){
	Kinect::readConfig(tree);
	userDetectRangeMin_ = tree->getChildPtr("userDetectRangeMin")->getValue<float>();
	userDetectRangeMax_ = tree->getChildPtr("userDetectRangeMax")->getValue<float>();
	selectedCamera_ = tree->getChildPtr("cameraId")->getValue<int>();
	motorAngleUp_ = tree->getChildPtr("motorAngleUp")->getValue<int>();
	motorAngleDown_ = tree->getChildPtr("motorAngleDown")->getValue<int>();
	saveImagePath_ = tree->getChildPtr("saveImagePath")->getValue<string>();
	saveCapturePath_ = tree->getChildPtr("saveCapturePath")->getValue<string>();
}

void PlayerManager::readParams(Bit::JsonTree* tree, Bit::ParamsRef params)
{
	params->addParam<float>(tree->getChildPtr("colorX1"), colorX_);
	params->addParam<float>(tree->getChildPtr("colorY1"), colorY_);
	params->addParam<float>(tree->getChildPtr("shift")->getChildPtr("x"), shift_.x);
	params->addParam<float>(tree->getChildPtr("shift")->getChildPtr("y"), shift_.y);
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