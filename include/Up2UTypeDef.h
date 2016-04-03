#pragma once

typedef enum StageName{
	ADVERTISING_STAGE = 0,
	DETECTING_STAGE = 1,
};

typedef enum Gender{
	MALE = 0,
	FEMALE = 1,
	UNDEFINED = 2,
};

typedef enum Look{
	LOOKUP = 0,
	LOOKDOWN = 1,
	TURNLEFT = 2,
	TURNRIGHT = 3
};

typedef enum Segtion{
	LEFT = 0,
	CENTER = 1,
	RIGHT = 2
};

typedef enum Status{
	IDLE = 0,
	PHUB = 1,
	TALK = 2
};

typedef enum MiniStage{
	GAMEPLAY = 0,
	PHUBVID = 1,
	TALKVID = 2
};