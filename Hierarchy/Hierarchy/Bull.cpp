#include "Bull.h"

bool Bull::p_active = false;

Bull::Bull()
{}

Bull::Bull(const XMFLOAT4& pos, const XMFLOAT4& rot, const XMVECTOR& fVec)
{
	p_active = true;

	m_mWorldMatrix = XMMatrixIdentity();
	m_v4Rot = rot;
	m_v4Pos = pos;

	m_vForwardVector = fVec;

	p_Bullet = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/bullet.x");

	m_fSpeed = 1.5f;

	timer = 0;
}

Bull::~Bull()
{
	delete p_Bullet;
}

void Bull::Update(const float& dt)
{
	timer += 0.1f * dt;

	if (timer > 20)
	{
		p_active = false;
	}

	// Move Forward
	XMVECTOR vCurrPos = XMLoadFloat4(&m_v4Pos);
	vCurrPos += m_vForwardVector * (m_fSpeed * dt);
	XMStoreFloat4(&m_v4Pos, vCurrPos);
	UpdateMatrices();
}


void Bull::Draw(void)
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	p_Bullet->Draw();
}

void Bull::UpdateMatrices(void)
{
	XMVECTOR pVec;
	XMMATRIX mScale, mRotX, mRotY, mRotZ, mTrans;

	mScale = XMMatrixScaling(0.1, 0.1, 0.1);

	mRotX = XMMatrixRotationX(XMConvertToRadians(m_v4Rot.x));
	mRotY = XMMatrixRotationY(XMConvertToRadians(m_v4Rot.y));
	mRotZ = XMMatrixRotationZ(XMConvertToRadians(m_v4Rot.z));

	pVec = XMLoadFloat4(&m_v4Pos);
	mTrans = XMMatrixTranslationFromVector(pVec);

	m_mWorldMatrix = mScale * mRotX * mRotZ *mRotY * mTrans;
}