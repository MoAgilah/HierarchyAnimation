#include "BulletPool.h"

BulletPool::BulletPool()
{
	//load mesh once
	p_Bullet = CommonMesh::LoadFromXFile(Application::s_pApp, "Resources/Plane/bullet.x");

	//make entire pool when created
	for (unsigned int i = 0; i < MaxBullet; ++i)
	{
		bullets.push_back(new Bullet(p_Bullet));
	}
}

BulletPool::~BulletPool()
{
	//delete all bullets
	for (unsigned int i = 0; i < bullets.size(); ++i)
	{
		delete bullets[i];
	}
	delete p_Bullet;
}

void BulletPool::Create(const XMMATRIX& gunMatrix, const XMVECTOR& Direction)
{
	for (unsigned int i = 0; i < MaxBullet; i++)
	{
		//find if inactive
		if (!bullets[i]->GetActive())
		{
			//if so activate and break out of loop
			bullets[i]->Activate(gunMatrix, Direction);
			break;
		}
	}
}

void BulletPool::Update(const double& deltaTime)
{
	for (unsigned int i = 0; i < MaxBullet; ++i)
	{
		if (bullets[i]->GetActive())
		{
			//update only the active one if return false for lifeline deactivate
			if (bullets[i]->Update(deltaTime))
			{
              	bullets[i]->Deactivate();
			}
		}
	}
}

void BulletPool::Draw()
{
	for (unsigned int i = 0; i < MaxBullet; ++i)
	{
		if (bullets[i]->GetActive())
		{
			//draw only active ones
			bullets[i]->Draw();
		}
	}
}