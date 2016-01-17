#include <PlayerManager.h>

void PlayerManager::threadUpdate()
{
	Kinect::threadUpdate();
	dataMutex_.lock();
	if (isDataReady()) {
		//function here
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

void PlayerManager::draw(){
	if (isDataReady()) { // && isKinectDebugMode) {

		gl::color(1, 1, 1);

		Texture video = gl::Texture(getSurface());

		float videoW = video.getWidth();
		float videoH = video.getHeight();
		float windowW = getWindowWidth();
		float windowH = getWindowHeight();

		float destX = ((windowH / videoH)*videoW);
		float delX = (destX - windowW) / 2.0f;

		Area srcArea1(0, 0, video.getWidth(), video.getHeight());				//TODO change to videoH
		Rectf destRect1(0, 0, windowW, windowH);				//TODO change to videoW

		//capture from kinect
		gl::draw(video, srcArea1, destRect1);
	}
}

Surface PlayerManager::getSurface(){
	Surface surface;
	//dataMutex_.lock();
	surface = *(kinectDevice_->getColorSurface());
	//dataMutex_.unlock();
	return surface;
}

void PlayerManager::readConfig(Bit::JsonTree* tree){
	Kinect::readConfig(tree);
	userDetectRangeMin_ = tree->getChildPtr("userDetectRangeMin")->getValue<float>();
	userDetectRangeMax_ = tree->getChildPtr("userDetectRangeMax")->getValue<float>();
}