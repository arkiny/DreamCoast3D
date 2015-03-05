#pragma once
#include "cCamera.h"

class cMapToolCamera : public cCamera
{
public:
	cMapToolCamera();
	virtual ~cMapToolCamera();

	virtual void Setup();
	virtual void Update(float fDelta);

private:
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 m_vUp;
	float m_fAngleX;
	float m_fAngleY;

	D3DXVECTOR3* m_pvTarget;
	float m_fDist;
	float m_fMin;
	float m_fMax;

	// Focus
	D3DXVECTOR3 m_vFocus;
	float		m_fFocusSpeed;
	float		m_fFocusAngleX;
	float		m_fFocusAngleY;

	// Mouse
	POINT m_ptPrevMouse;
	bool m_isMButtonDown;
private:
	void KeyBoradUpdate();
	void MouseUpdate();
};

