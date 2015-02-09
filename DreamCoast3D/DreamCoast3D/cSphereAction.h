#pragma once

class cSphereAction : public cGameActionObject
{
private:
	LPD3DXMESH		m_pMesh;
	D3DMATERIAL9	m_stMtl;

public:
	cSphereAction();
	virtual ~cSphereAction();
	
	virtual void Setup(); 
	virtual void Update(float fDelta);
	virtual void Render();
};

