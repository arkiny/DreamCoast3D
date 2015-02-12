#pragma once
#include "cCamera.h"

class cCameraEditing : public cCamera
{
protected:
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vDir;
	float		m_fAngleDegree;

	LPD3DXMESH		m_pMesh;
	D3DMATERIAL9	m_stMaterial;

public:
	cCameraEditing();
	virtual	~cCameraEditing();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void SetPosition(D3DXVECTOR3& vecPos) { m_vPosition = vecPos; }
};

