#ifndef BullH
#define BullH

#include "Application.h"

__declspec(align(16)) class Bull
{
public:
	Bull();
	Bull(const XMFLOAT4& pos, const XMFLOAT4& rot, const XMVECTOR& fVec);
	~Bull(void);
	void Update(const float& deltaTime);
	void Draw(void);
	bool GetActive(){ return p_active; }
private:
	void UpdateMatrices(void);
	CommonMesh* p_Bullet;

	static bool p_active;

	float timer;

	XMFLOAT4 m_v4Rot;
	XMFLOAT4 m_v4Pos;

	XMVECTOR m_vForwardVector;
	float m_fSpeed;

	XMMATRIX m_mWorldTrans;							// World translation matrix
	XMMATRIX m_mWorldMatrix;						// World transformation matrix
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