#include "Aeroplane.h"

Hiearchy* Aeroplane::s_Hiearchie = nullptr;
BulletPool* Aeroplane::bPool = nullptr;
bool Aeroplane::s_bResourcesReady = false;

Aeroplane::Aeroplane(const XMFLOAT3& pos, const XMFLOAT3& rot)
{	
	//creates hiearchy one per plane
	s_Hiearchie = new Hiearchy("Plane", "pHierarchy.txt", pos, rot,1);

	CreateHierarchie();

	//sets up bullet pool one per plane
	bPool = new BulletPool();
	m_mCamWorldMatrix = XMMatrixIdentity();

	m_v4CamOff = XMFLOAT4(0.0f, 4.5f, -15.0f, 0.0f);
	m_v4CamRot = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	m_vCamWorldPos = XMVectorZero();
	m_vForwardVector = XMVectorZero();

	m_fSpeed = 0.0f; 

	m_bGunCam = false;
	justFired = false;
}

Aeroplane::~Aeroplane(void)
{
	delete s_Hiearchie;
	delete bPool;
}

void Aeroplane::CreateHierarchie(void)
{
	s_Hiearchie->LoadFromFile();
}

void Aeroplane::Update(const bool& bPlayerControl, const double& dt)
{
	
	UpdateMatrices();
	//update bullet pool
	bPool->Update(dt);

	if (bPlayerControl)
	{
		// You can also impose a take off seeepd of 0.5 if you like
		//Make the plane pitch upwards when you press "Q" 
		if (Application::s_pApp->IsKeyPressed('Q'))
		{
			if (s_Hiearchie->GetRoot()->GetLocRot().x >= -60)
			{
				s_Hiearchie->GetRoot()->GetLocRot().x -= 1;
			}
		}//return to level when released
		else  if (!Application::s_pApp->IsKeyPressed('Q') || !Application::s_pApp->IsKeyPressed('A'))
		{
			if (s_Hiearchie->GetRoot()->GetLocRot().x < 0)
			{
				s_Hiearchie->GetRoot()->GetLocRot().x += 1;
			}
		}

		//Make the plane pitch downwards when you press "A" 
		if (Application::s_pApp->IsKeyPressed('A'))
		{
			if (s_Hiearchie->GetRoot()->GetLocRot().x <= 60)
			{
				s_Hiearchie->GetRoot()->GetLocRot().x += 1;
			}
		}//return to level when released
		else  if (!Application::s_pApp->IsKeyPressed('Q') || !Application::s_pApp->IsKeyPressed('A'))
		{
			if (s_Hiearchie->GetRoot()->GetLocRot().x > 0)
			{
				s_Hiearchie->GetRoot()->GetLocRot().x -= 1;
			}
		}

		// Step 3: Make the plane yaw and roll left when you press "O" 
		if (Application::s_pApp->IsKeyPressed('P'))
		{

			if (s_Hiearchie->GetRoot()->GetLocRot().z >= -20)
			{
				s_Hiearchie->GetRoot()->GetLocRot().z -= 1;
			}
			s_Hiearchie->GetRoot()->GetLocRot().y += 1;
		}//return to level when released
		else  if (!Application::s_pApp->IsKeyPressed('O') || !Application::s_pApp->IsKeyPressed('P'))
		{
			if (s_Hiearchie->GetRoot()->GetLocRot().z < 0)
			{
				s_Hiearchie->GetRoot()->GetLocRot().z += 1;
			}
		}

		//Make the plane yaw and roll right when you press "P" 
		// Minimum roll = -20 degrees
		if (Application::s_pApp->IsKeyPressed('O'))
		{

			if (s_Hiearchie->GetRoot()->GetLocRot().z <= 20)
			{
				s_Hiearchie->GetRoot()->GetLocRot().z += 1;
			}
			s_Hiearchie->GetRoot()->GetLocRot().y -= 1;
		}// return to level when released
		else  if (!Application::s_pApp->IsKeyPressed('O') || !Application::s_pApp->IsKeyPressed('P'))
		{
			if (s_Hiearchie->GetRoot()->GetLocRot().z > 0)
			{
				s_Hiearchie->GetRoot()->GetLocRot().z -= 1;
			}
		}

		//loop de loop
		if (Application::s_pApp->IsKeyPressed('L'))
		{
			do
			{
				s_Hiearchie->GetRoot()->GetLocRot().x += sin(1.0f);
			} while (s_Hiearchie->GetRoot()->GetLocRot().x <= 360);
		}

		//barrell roll right
		if (Application::s_pApp->IsKeyPressed('R'))
		{
			do
			{
				s_Hiearchie->GetRoot()->GetLocRot().z += sin(1.0f);
			} while (s_Hiearchie->GetRoot()->GetLocRot().z <= 360);
		}

		
		
		//fire a bullet if you haven't just fired
		if (Application::s_pApp->IsKeyPressed(VK_SPACE) && justFired == false)
		{	//take in relative motion of the plane for the bullet
			XMVECTOR direction = XMVectorAdd(s_Hiearchie->GetRoot()->GetWorld().r[2] * m_fSpeed, s_Hiearchie->GetByName("gun")->GetWorld().r[2]);
			bPool->Create(s_Hiearchie->GetByName("gun")->GetWorld(), direction);
			justFired = true;	//delay firing until key up
		}//once key is released allow the option to fire
		else  if (!Application::s_pApp->IsKeyPressed(VK_SPACE) && justFired == true)
		{
			justFired = false;
		}

	}//end of if player control

	m_fSpeed += 0.001f;

	if (m_fSpeed > 1){ m_fSpeed = 1; }


	s_Hiearchie->GetByName("prop")->GetLocRot().z += 100.0f * m_fSpeed;
	s_Hiearchie->GetByName("turret")->GetLocRot().y += 0.1f;

	s_Hiearchie->GetByName("gun")->GetLocRot().x = (sin((float)XMConvertToRadians(s_Hiearchie->GetByName("turret")->GetLocRot().y*4.0f)) * 10.0f) - 10.0f;

	XMVECTOR vCurrPos = XMLoadFloat4(&s_Hiearchie->GetRoot()->GetLocPos());
	vCurrPos += m_vForwardVector * m_fSpeed;
	XMStoreFloat4(&s_Hiearchie->GetRoot()->GetLocPos(), vCurrPos);
}

void Aeroplane::Draw(void)
{
	s_Hiearchie->GetRoot()->Draw();
	bPool->Draw();
}

void Aeroplane::UpdateMatrices(void)
{
	XMVECTOR pVec;
	XMMATRIX mRotY,mTrans;
	XMMATRIX mPlaneCameraRot, mForwardMatrix;

	//update entire hiearchie
	s_Hiearchie->GetRoot()->updateMat();

	mRotY = XMMatrixRotationY(XMConvertToRadians(s_Hiearchie->GetRoot()->GetLocRot().y));
	pVec = XMLoadFloat4(&s_Hiearchie->GetRoot()->GetLocPos());

	mTrans = XMMatrixTranslationFromVector(pVec);

	//Also calculate mPlaneCameraRot which ignores rotations in Z and X for the camera to parent to
	pVec = XMLoadFloat4(&m_v4CamOff);
	m_mCamWorldMatrix = XMMatrixTranslationFromVector(pVec) * mRotY * mTrans;
	
	m_vForwardVector = s_Hiearchie->GetRoot()->GetWorld().r[2];
	
	pVec = XMLoadFloat4(&s_Hiearchie->GetByName("gun")->GetLocPos());
	mTrans = XMMatrixTranslationFromVector(pVec);

	XMMATRIX mGunTutTrans = mTrans * s_Hiearchie->GetByName("turret")->GetWorld();
	
	mRotY = XMMatrixRotationY(XMConvertToRadians(s_Hiearchie->GetByName("gun")->GetLocRot().y));

	// between parenting the camera to the plane(without X and Z rotations) and the gun based on m_bGunCam
	pVec = XMLoadFloat4(&m_v4CamOff);
	if (m_bGunCam)
		m_mCamWorldMatrix = XMMatrixTranslationFromVector(pVec) * mRotY * mGunTutTrans;

	// Get the camera's world position (m_vCamWorldPos) out of m_mCameraWorldMatrix
	m_vCamWorldPos = XMVectorZero();
	m_vCamWorldPos = XMVector3Transform(m_vCamWorldPos, m_mCamWorldMatrix);
}

MeshComponent* Aeroplane::GetRoot()
{
	//get root for basic collision checks with robot
	return s_Hiearchie->GetRoot();
}