#pragma once

#include "Stage.h"
class EndingStage : virtual public Stage{
public:
	typedef struct Config{
		Bit::Video			stageVid;
	};

	void				readConfig(Bit::JsonTree* tree);

	void				setup();
	void				update();
	void				draw();
	void				drawDebugMode();
	void				reset();

private:
	Config				config_;
};