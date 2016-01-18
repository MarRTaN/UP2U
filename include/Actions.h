#pragma once

#include "BitKinect.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "GameManager.h"

using namespace Bit;
using namespace ci::gl;

class Actions{
public:

	typedef struct Condition{
		int				conditionId;
		Vec3f			rotation;
		Vec3f			translation;
		float			count;
	};

	typedef struct Action{
		int					actId_;
		vector<Condition>	conditionsList_;
		Config				config_;

	};

};