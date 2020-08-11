#ifndef APPLICATION_H
#define APPLICATION_H

#define WIN32_LEAN_AND_MEAN

#include <assert.h>

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>

#include "CommonApp.h"
#include "CommonMesh.h"

class Aeroplane;
class HeightMap;
class Robot;
class AnimationParser;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class Application : public CommonApp
{
public:
	static Application* s_pApp;
	static AnimationParser* aniP;
protected:
	bool HandleStart();
	void HandleStop();
	void HandleUpdate(const double& deltaTime);
	void HandleRender();

private:

	float m_rotationAngle;
	float m_cameraZ;
	bool m_bWireframe;



	int m_cameraState;

	Aeroplane* m_pAeroplane;
	HeightMap* m_pHeightMap;
	Robot* m_pRobot;
};

#endif

