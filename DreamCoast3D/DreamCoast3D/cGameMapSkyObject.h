#pragma once
class cGameMapSkyObject : public cGameObject
{
protected:
	LPD3DXMESH		m_pMesh;
	D3DMATERIAL9	m_stMaterial;

public:
	cGameMapSkyObject();
	virtual ~cGameMapSkyObject();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
};

