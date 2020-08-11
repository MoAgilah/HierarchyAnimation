#include "Bullet.h"

Bullet::Bullet(CommonMesh* Bullet)
{
	p_active = false;

	m_mWorldMatrix = XMMatrixIdentity();

	//they all share one bullet
	p_Bullet = Bullet;
	
	//the intention was to use gravity
	gravity = XMLoadFloat4(&XMFLOAT4(0, -0.981, 0, 0));
}

Bullet::~Bullet()
{
}

void Bullet::Activate(const XMMATRIX& gunMat, const XMVECTOR& Direction)
{
	p_active = true;
	

	m_vForwardVector = Direction;

	//do scaling once and parent to gun
	m_mWorldMatrix = XMMatrixScaling(0.1, 0.1, 0.1) * gunMat;

	//reintialise timer
	timer = 0.0f;
}

bool Bullet::GetActive() const
{
	return p_active;
}

void Bullet::Deactivate()
{
	p_active  = false;
}

bool Bullet::Update(const double& dt)
{
	//update lifetime
	timer += dt;

	UpdateMatrices();
	
	//if lifetime over deactivate
	return timer >= 2;
}

void Bullet::Draw(void)
{
	Application::s_pApp->SetWorldMatrix(m_mWorldMatrix);
	p_Bullet->Draw();
}

void Bullet::UpdateMatrices(void)
{
	//update worl with direction	
	m_mWorldMatrix *= XMMatrixTranslationFromVector(m_vForwardVector);
}