#ifndef BulletPoolH
#define BulletPoolH

#include "Bullet.h"
#include <vector>
#include <assert.h>

#define MaxBullet 100 
class BulletPool
{
public:
	BulletPool();
	~BulletPool();
	void Create(const XMMATRIX& gunMatrix, const XMVECTOR& Direction);
	void Update(const double& deltaTime);
	void Draw(void);
private:
	
	std::vector<Bullet*> bullets;
	CommonMesh* p_Bullet;
};

#endif