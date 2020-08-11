#include "AnimationController.h"

int AnimationController::trackNum;

AnimationController::AnimationController(const XMFLOAT3& iPos, const XMFLOAT3& iRot, const double& scalar)
	:initP(iPos), initR(iRot), sF(scalar)
{
	strideNum = 0;
	timer = 0;
	activeTracks = 0;
	//to tell if blend in progress
	isBlending = false;
}


AnimationController::~AnimationController()
{
	for (std::map<std::string, Track*>::iterator it = allTracks.begin(); it != allTracks.end(); it++)
	{
		delete it->second;
	}
}

void AnimationController::LoadAnimation(MeshComponent* root, const std::string& fName, const std::string& aName, const double& sF, const bool& sLoop)
{
	Animation clip(fName, aName, sLoop, sF);
	if (!root->GetHasAnim())
	{
		root->SetHasAnim();
		//load animation into a track for use
		//set first i.e. idle
		SetInitialTrack(&clip);
	}
	else
	{
		//set additional tracks for use
		SetAdditionalTracks(&clip);
	}
}

Track*  AnimationController::GetClipByName(const std::string& name)
{
	return allTracks.find(name)->second;
}

void AnimationController::ChangeTrack(const std::string& name)
{
	//if blending dont allow change
	if (isBlending != true)
	{
		Track* change = GetClipByName(name);
		change->active = true;
		next = change->trackId;
		isBlending = true;
	}
}

bool AnimationController::GetIsActive(const std::string& name)
{
	Track* check = GetClipByName(name);
	return check->active;
}

void AnimationController::AdvanceTime(const double& time, std::map<std::string, MeshComponent*>& bHierarchy)
{
	for (unsigned int i = 0; i < animTracks.size(); ++i)
	{
		//if active track
		if (animTracks[i]->active)
		{
			//interpolate frames into framebuffer
			Interpolate(animTracks[i],time);
			//increment num of tracks
			++activeTracks;
		}
	}

	if (activeTracks == 2)
	{
		//if more than one active interpolate frame buffer data
		InterpolateTracks(animTracks, time);
		//after blend set to 0
		activeTracks = 0;
	}
	else
	{
		//if one stored current tracks data into current frame
		StoreCurrentFrame(animTracks[curr]->FData);
		//after assignment set to 0
		activeTracks = 0;
	}

	//update hierarchy
	UpdateHiearachy(bHierarchy);
}

void AnimationController::SetInitialTrack(Animation* initial)
{
	Track* init = new Track();
	init->trackId = trackNum++;
	init->active = true;
	init->AData = initial->GetFrameTrans();
	init->Keys = initial->GetKeyframes();
	init->shouldLoop = initial->GetShouldLoop();
	init->Timer = 0;
	SetUpCFrameVec(init->AData, init->FData);
	SetUpCFrameVec(init->FData, currFrame);
	allTracks.insert(std::pair<std::string, Track*>(initial->GetName(), init));
	animTracks.push_back(init);
	curr = init->trackId;
}

void AnimationController::SetAdditionalTracks(Animation* other)
{
	Track *track = new Track();
	track->trackId = trackNum++;
	track->active = false;
	track->AData = other->GetFrameTrans();
	track->Keys = other->GetKeyframes();
	track->shouldLoop = other->GetShouldLoop();
	track->Timer = 0;
	SetUpCFrameVec(currFrame,track->FData);
	allTracks.insert(std::pair<std::string, Track*>(other->GetName(), track));
	animTracks.push_back(track);
}


void AnimationController::SetUpCFrameVec(const std::vector<anim>& aData, std::vector<CFrame>& FData)
{
	unsigned int size = aData.size() / 4;
	for (unsigned int i = 0; i < size; ++i)
	{
		CFrame Bone;
		Bone.bName = aData[i * 4].name;
		FData.resize(i + 1, Bone);
	}
}

void AnimationController::SetUpCFrameVec(const std::vector<CFrame>& base, std::vector<CFrame>& FData)
{
	for (unsigned int i = 0; i < base.size(); ++i)
	{
		CFrame Bone;
		Bone.bName = base[i].bName;
		FData.resize(i + 1, Bone);
	}
}

void AnimationController::Interpolate(Track* cT, const double& t)
{
	cT->Timer += t;
	XMFLOAT4 trans, rot;

	//apologies ran out of time for comments
	//amount to jump for frame storage
	int jump = cT->AData.size() / cT->FData.size();

	for (unsigned int i = 0; i < cT->AData.size(); i += 4)
	{
		if (cT->Timer <= cT->Keys.front())
		{
			//check if root take into account intial position
			if (cT->AData[i].name == "root")
			{

				trans = XMFLOAT4(*cT->AData[i].val.begin() + initP.x,
					*(cT->AData[i].val.begin() + 1) + initP.y,
					*(cT->AData[i].val.begin() + 2) + initP.z, 0);
				cT->FData[i / jump].pos = trans;
			}
			else
			{
				trans = XMFLOAT4(*cT->AData[i].val.begin(),
					*(cT->AData[i].val.begin() + 1),
					*(cT->AData[i].val.begin() + 2), 0);
				cT->FData[i / jump].pos = trans;
			}
			//check if root take into account intial rotation
			if (cT->AData[i].name == "root")
			{
				rot = XMFLOAT4(cT->AData[i + 1].val.front() + initR.x,
					cT->AData[i + 2].val.front() + initR.y,
					cT->AData[i + 3].val.front() + +initR.z, 0);
				cT->FData[i / jump].rot = rot;
			}
			else
			{
				rot = XMFLOAT4(cT->AData[i + 1].val.front(),
					cT->AData[i + 2].val.front(),
					cT->AData[i + 3].val.front(), 0);
				cT->FData[i / jump].rot = rot;
			}
		}
		else if (cT->Timer >= cT->Keys.back())
		{
			//check if root take into account intial position
			if (cT->AData[i].name == "root")
			{
				trans = XMFLOAT4(*(cT->AData[i].val.end() - 3) + initP.x,
					*(cT->AData[i].val.end() - 2) + initP.y,
					*(cT->AData[i].val.end() - 1) + initP.z, 0);
				cT->FData[i / jump].pos = trans;
			}
			else
			{
				trans = XMFLOAT4(*(cT->AData[i].val.end() - 3),
					*(cT->AData[i].val.end() - 2),
					*(cT->AData[i].val.end() - 1), 0);
				cT->FData[i / jump].pos = trans;
			}

			if (cT->AData[i].name == "root")
			{
				rot = XMFLOAT4(cT->AData[i + 1].val.back() + initR.x,
					cT->AData[i + 2].val.back() + initR.y,
					cT->AData[i + 3].val.back() + initR.z, 0);
				cT->FData[i / jump].rot = rot;
			}
			else
			{
				rot = XMFLOAT4(cT->AData[i + 1].val.back(),
					cT->AData[i + 2].val.back(),
					cT->AData[i + 3].val.back(), 0);
				cT->FData[i / jump].rot = rot;
			}

			if (cT->shouldLoop)cT->Timer = 0;
		}
		else
		{
			for (unsigned int j = 0; j < cT->Keys.size() - 1; ++j)
			{
				if (cT->Timer >= cT->Keys[j] && cT->Timer <= cT->Keys[j + 1])
				{
					float lerpPercent = static_cast<float>((cT->Timer - cT->Keys[j]) / (cT->Keys[j + 1] - cT->Keys[j]));

					XMVECTOR t1, t2, r1, r2, ft, fr;

					strideNum = cT->AData[i].val.size() / cT->AData[i].stride;

					if (strideNum > 1)
					{
						int f = (j * cT->AData[i].stride);
						int s = ((j + 1) * cT->AData[i].stride);

						if (cT->AData[i].name == "root")
						{
							trans = XMFLOAT4(cT->AData[i].val[f] + initP.x,
								cT->AData[i].val[f + 1] + initP.y,
								cT->AData[i].val[f + 2] + initP.z, 0);
						}
						else
						{
							trans = XMFLOAT4(cT->AData[i].val[f],
								cT->AData[i].val[f + 1],
								cT->AData[i].val[f + 2], 0);
						}
						t1 = XMLoadFloat4(&trans);

						if (cT->AData[i].name == "root")
						{
							trans = XMFLOAT4(cT->AData[i].val[s] + initP.x,
								cT->AData[i].val[s + 1] + initP.y,
								cT->AData[i].val[s + 2] + initP.z, 0);
						}
						else
						{
							trans = XMFLOAT4(cT->AData[i].val[s],
								cT->AData[i].val[s + 1],
								cT->AData[i].val[s + 2], 0);
						}
						t2 = XMLoadFloat4(&trans);

						ft = XMVectorLerp(t1, t2, lerpPercent);
						XMVector4Normalize(ft);
						XMStoreFloat4(&trans, ft);

						cT->FData[i / jump].pos = trans;
					}

					int s =    cT->AData[i + 1].stride;

					if (cT->AData[i].name == "root")
					{
						rot = XMFLOAT4(cT->AData[i + 1].val[j] + initR.x,
							cT->AData[i + 2].val[j] + initR.x,
							cT->AData[i + 3].val[j] + initR.x, 0);
						r1 = XMLoadFloat4(&rot);
					}
					else
					{
						rot = XMFLOAT4(cT->AData[i + 1].val[j],
							cT->AData[i + 2].val[j],
							cT->AData[i + 3].val[j], 0);
						r1 = XMLoadFloat4(&rot);
					}

					if (cT->AData[i].name == "root")
					{
						rot = XMFLOAT4(cT->AData[i + 1].val[j + s] + initR.x,
							cT->AData[i + 2].val[j + s] + initR.y,
							cT->AData[i + 3].val[j + s] + initR.z, 0);
						r2 = XMLoadFloat4(&rot);
					}
					else
					{
						rot = XMFLOAT4(cT->AData[i + 1].val[j + s],
							cT->AData[i + 2].val[j + s],
							cT->AData[i + 3].val[j + s], 0);
						r2 = XMLoadFloat4(&rot);
					}
					
					fr = XMVectorLerp(r1, r2, lerpPercent);
					XMStoreFloat4(&rot, fr);
					cT->FData[i / jump].rot = rot;

					break;
				}
			}
		}
	}
}

void AnimationController::InterpolateTracks(std::vector<Track*> aTracks, const double& time)
{
	timer += time;

	XMFLOAT4 trans, rot;

	for (unsigned int i = 0; i < currFrame.size(); ++i)
	{
		float blendFactor = static_cast<float>(timer / 0.5f);

		if (blendFactor <= 1.0f)
		{
			XMVECTOR t1, t2, r1, r2, ft, fr;
			t1 = XMLoadFloat4(&aTracks[curr]->FData[i].pos);
			t2 = XMLoadFloat4(&aTracks[next]->FData[i].pos);

			ft = XMVectorLerp(t1, t2, blendFactor);
			XMVector4Normalize(ft);

			XMStoreFloat4(&trans, ft);

			currFrame[i].pos = trans;


			r1 = XMLoadFloat4(&aTracks[curr]->FData[i].rot);
			r2 = XMLoadFloat4(&aTracks[next]->FData[i].rot);

			fr = XMVectorLerp(r1, r2, blendFactor);
			XMVector4Normalize(fr);

			XMStoreFloat4(&rot, fr);

			currFrame[i].rot = rot;
		}
		else
		{
			aTracks[curr]->active = false;
			aTracks[curr]->Timer = 0;
			curr = aTracks[next]->trackId;
			timer = 0;
			isBlending = false;
		}
	}
}

void AnimationController::StoreCurrentFrame(std::vector<CFrame> curr)
{
	for (unsigned int i = 0; i < currFrame.size(); ++i)
	{
		currFrame[i].pos = curr[i].pos;
		currFrame[i].rot = curr[i].rot;
	}
}

void AnimationController::UpdateHiearachy(std::map<std::string, MeshComponent*>& bHierarchy)
{
	for (unsigned int i = 0; i < currFrame.size(); ++i)
	{
		std::string bone = currFrame[i].bName;
		MeshComponent* CurrBone = bHierarchy.find(bone)->second;
		
		CurrBone->GetLocPos() = currFrame[i].pos;
		CurrBone->GetLocRot() = currFrame[i].rot;
	}
}


