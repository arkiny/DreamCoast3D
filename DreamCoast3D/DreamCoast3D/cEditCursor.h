#pragma once
class cEditCursor : public cGameObject
{
protected:
	LPD3DXMESH			m_pMesh;
	ST_BOUNDING_BOX		m_stBox;
	D3DMATERIAL9		m_stMtl;

public:
	cEditCursor();
	virtual ~cEditCursor();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void SetBoxSize(ST_BOUNDING_BOX stBox);
	virtual void SetPosition(D3DXVECTOR3& newPos);
};

