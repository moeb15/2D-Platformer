#pragma once
#include <vector>

struct AnimationData {
	int frameCount;
	int duration;
};


std::vector<AnimationData> initializeAnimationData();