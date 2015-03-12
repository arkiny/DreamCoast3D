#pragma once
class cGameMapSkyObject : public cGameObject
{
protected:
	LPD3DXMESH		m_pMesh;
	D3DMATERIAL9	m_stMaterial;

	// shader test¿ë
	/*D3DXVECTOR4						gLightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	LPD3DXEFFECT					m_pSpecularMapping = NULL;*/
public:
	cGameMapSkyObject();
	virtual ~cGameMapSkyObject();

	virtual void SetSkyColor(D3DXCOLOR color);

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
};

