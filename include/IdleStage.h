#pragma once

#include "Stage.h"

class IdleStage : virtual public Stage{
public:
	typedef struct Config{
		Bit::Video			stageVid;
		Bit::Video			countDownVid;
		float				nodThreshold;
		float				nodTimeOut;
	};

	void				readConfig(Bit::JsonTree* tree);

	void				setup();
	void				update();
	void				draw();
	void				drawDebugMode();
	void				reset();
	void				checkNod();

private:
	Bit::Video			countDownVid_;
	Rectf				countDownBound_;
	Config				config_;
};