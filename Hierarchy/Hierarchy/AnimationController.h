#ifndef AnimationControllerH
#define AnimationControllerH

#include "Animation.h"
#include "AnimationParser.h"
#include "MeshComponent.h"
#include <map>
#include <string>
#include <vector>

struct CFrame
{
	std::string bName;
	XMFLOAT4 pos;
	XMFLOAT4 rot;
};

struct Track
{
	int trackId;
	std::vector<anim> AData;
	std::vector<float> Keys;
	std::vector<CFrame> FData;
	double Timer;
	bool active;
	bool shouldLoop;
};

__declspec(align(16)) class AnimationController
{
public:
	AnimationController(const XMFLOAT3& iPos, const XMFLOAT3& iRot, const double& scalar);
	~AnimationController();
	void LoadAnimation(MeshComponent* root, const std::string& fName, const std::string& aName, const double& sF = 1.0, const bool& sLoop = true);
	void AdvanceTime(const double& time, std::map<std::string, MeshComponent*>& bHierarchy);
	Track* GetClipByName(const std::string& name);
	void ChangeTrack(const std::string& name);
	bool GetIsActive(const std::string& name);
private:
	void SetInitialTrack(Animation* initial);
	void SetAdditionalTracks(Animation* other);
	void SetUpCFrameVec(const std::vector<anim>& aData, std::vector<CFrame>& FData);
	void SetUpCFrameVec(const std::vector<CFrame>& base, std::vector<CFrame>& FData);
	void Interpolate(Track* cTrack, const double& time);
	void InterpolateTracks(std::vector<Track*> activeTracks, const double& time);
	void StoreCurrentFrame(std::vector<CFrame> curr);
	void UpdateHiearachy(std::map<std::string, MeshComponent*>& bHierarchy);
	XMFLOAT3 initP, initR;
	std::vector<Track*> animTracks;
	int activeTracks;
	static int trackNum;
	int curr;
	int next;
	double sF;
	double timer;
	bool isBlending;
	int strideNum;
	std::vector<CFrame> currFrame;
	std::map<std::string, Track*> allTracks;
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

#endif;
