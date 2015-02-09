#pragma once

// ���� ������Ʈ ����� ���� ����Ŭ����

class cSphere : public cGameObject
{
private:
	LPD3DXMESH		m_pMesh;
	D3DMATERIAL9	m_stMtl;

public:
	cSphere();
	virtual ~cSphere();

	void Setup();
	void Update(float fDelta);
	void Render();
};

