#ifndef AnimationParserH
#define AnimationParserH

#include "Application.h"
#include <vector>

struct anim
{
	std::string name;
	std::string transName;
	std::vector<float> kframe;
	std::vector<float> val;
	int stride;
};

//a animation parser for all animations
__declspec(align(16)) class AnimationParser
{
public:
	AnimationParser();
	~AnimationParser();
	bool LoadFromFile(const std::string& fName, const std::string& aName);
	std::vector<anim> PrepAnimationData(const double& scaler = 1.0f);
private:
	std::vector<std::string> animData;
	std::vector<anim> animation;
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

