#include "Animation.h"

Animation::Animation(const std::string& fName, const std::string& afName, const bool& loop, const double& scalar)
	:name(afName), shouldLoop(loop)
{
	//share one parser for all animations
	if (Application::aniP->LoadFromFile(fName, afName)){
		//extract data from parser
		ani = Application::aniP->PrepAnimationData(scalar);
		//assing keyframes from beginning to end of animation
		SetKeyFrames();
	}
}


Animation::~Animation()
{
}

void Animation::SetKeyFrames(void)
{
	keyframes = ani[0].kframe;
	//if only one keyframe find the largest
	for (int i = 1; i < ani.size(); ++i)
	{
		if (keyframes.size() < ani[i].kframe.size())
		{
			keyframes = ani[i].kframe;
		}
	}
}

std::vector<anim> Animation::GetFrameTrans(void) const
{
	return ani;
}

std::vector<float> Animation::GetKeyframes(void) const
{
	return keyframes;
}

bool Animation::GetShouldLoop(void) const
{
	return shouldLoop;
}

std::string Animation::GetName(void) const
{
	return name;
}

	