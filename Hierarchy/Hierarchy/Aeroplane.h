#ifndef AEROPLANE_H
#define AEROPLANE_H

#include "Application.h"
#include "Hierarchy.h"
#include "BulletPool.h"
#include <iostream>

__declspec(align(16)) class Aeroplane
{
public:
	Aeroplane(const XMFLOAT3& pos = XMFLOAT3(0, 0, 0), const XMFLOAT3& rot = XMFLOAT3(0, 0, 0));
	~Aeroplane(void);
	void Update(const bool& bPlayerContol, const double& deltaTime);
	void Draw(void);
	MeshComponent* GetRoot();
private:
	void UpdateMatrices(void);
	static void CreateHierarchie(void);

	static Hiearchy* s_Hiearchie;
	static bool s_bResourcesReady;
	bool justFired;
	bool stunt;

	XMVECTOR m_vForwardVector;
	float m_fSpeed;

	XMFLOAT4 m_v4CamRot;							// Local rotation angles
	XMFLOAT4 m_v4CamOff;							// Local offset

	XMVECTOR m_vCamWorldPos;						// World position
	XMMATRIX m_mCamWorldMatrix;						// Camera's world transformation matrix

	bool m_bGunCam;
	
	static BulletPool* bPool;
public:
	XMFLOAT4 GetFocusPosition(void){ return GetPosition(); }
	XMFLOAT4 GetCameraPosition(void) { XMFLOAT4 v4Pos; XMStoreFloat4(&v4Pos, m_vCamWorldPos); return v4Pos; }
	XMFLOAT4 GetPosition(void) { return s_Hiearchie->GetRoot()->GetLocPos(); }
	void SetGunCamera(bool value) { m_bGunCam = value; }

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