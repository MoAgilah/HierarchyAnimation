#ifndef BulletH
#define BulletH

#include "Application.h"

__declspec(align(16)) class Bullet
{
public:
	Bullet(CommonMesh* p_Bullet);
	~Bullet();
	void Activate(const XMMATRIX& gunMatrix, const XMVECTOR& Direction);
	void Deactivate();
	bool Update(const double& deltaTime);
	void Draw(void);
	bool GetActive(void) const;
private:
	void UpdateMatrices(void);
	CommonMesh* p_Bullet;

	bool p_active;

	double timer;

	XMVECTOR m_vForwardVector;
	XMVECTOR gravity;

	XMMATRIX m_mWorldMatrix;	
	Bullet* next;
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