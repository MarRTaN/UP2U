#include <Person.h>

Vec2f Person::getCenterMean(){
	Vec2f sum = Vec2f(0,0);
	for (int i = 0; i < centerBuffer.size(); i++){
		sum += centerBuffer[i];
	}
	Vec2f mean = Vec2f(0, 0);
	if (centerBuffer.size() > 0)
		mean = Vec2f(sum.x / centerBuffer.size(), sum.y / centerBuffer.size());

 	return mean;
}

bool Person::isPersonLost(){
	if (lastCenter == Vec2f(0, 0)) return false;
	else if (lastCenter != Vec2f(0, 0) && lastCenter == center){
		lostCount++;
	}
	else lostCount = 0;
	if (lostCount > 300){
		return true;
	}
	return false;
}
