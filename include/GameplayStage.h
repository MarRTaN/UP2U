#pragma once

#include "Stage.h"
#include "Actions.h"

class GameplayStage : virtual public Stage{
public:
	typedef struct Config{
		Bit::Video			stageVid;
		Bit::Video			gaugeVid;
	};

	void				readConfig(Bit::JsonTree* tree);

	void				setup();
	void				update();
	void				draw();
	void				drawDebugMode();
	void				reset();
	void				setAllActionsList(vector<Actions::Action> list);
	void				setRandomedList(vector<int> list);
	void				nextAction();
	void				debugAction(int actionId);
	void				checkAction();
	void				updateGauge();

private:
	Bit::Video				gaugeVideo_;
	Actions::Action			currentAction_;
	bool					isCorrectAct_;
	vector<Actions::Action>	allActionList_;
	vector<int>				randomedIdList_;
	Config					config_;
};