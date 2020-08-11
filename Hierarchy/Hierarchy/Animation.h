#ifndef AnimationH
#define AnimationH

#include "Application.h"
#include "AnimationParser.h"
#include <string>
#include <vector>

//animation clip
__declspec(align(16)) class Animation
{
public:
	Animation(const std::string& fName, const std::string& afName, const bool& loop, const double& scalar = 1.0f);
	~Animation();
	void SetKeyFrames(void);
	std::vector<anim> GetFrameTrans(void) const;
	std::vector<float> GetKeyframes(void) const;
	bool GetShouldLoop(void) const;
	std::string GetName(void) const;
private:
	std::string name;
	std::vector<anim> ani;
	std::vector<float> keyframes;
	bool shouldLoop;
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

#endif

