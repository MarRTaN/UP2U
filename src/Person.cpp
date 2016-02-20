#include <Person.h>

bool Person::isPersonLost(){
	if (lastCenter == Vec2f(0, 0)) return false;
	else if (abs(lastCenter.x - center.x) < 10 && 
			 abs(lastCenter.y - center.y) < 10){
		idleCount = getElapsedSeconds() + 3;
	}
	if (idleCount < getElapsedSeconds()){
		return true;
	}
	return false;
}

void Person::calAngleMean(){
	float sum = 0;
	for (int i = 0; i < angleMean.size(); i++){
		sum += angleMean[i];
	}
	angle = floor(sum / angleMean.size());
}

void Person::calCenterMean(){
	Vec2f sum = Vec2f(0,0);
	for (int i = 0; i < kinectCenterMean.size(); i++){
		sum += kinectCenterMean[i];
	}
	kinectCenter = Vec2f(floor(sum.x / kinectCenterMean.size()), floor(sum.y / kinectCenterMean.size()));
}

int Person::getLook(){
	if (angle > 50){
		return TURNRIGHT;
	}
	else if (angle < -50){
		return TURNLEFT;
	}
	else {
		if (isLookUp) return LOOKUP;
		else return LOOKDOWN;
	}
}
