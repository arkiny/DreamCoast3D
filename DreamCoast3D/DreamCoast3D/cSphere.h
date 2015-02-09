#pragma once

// 게임 오브젝트 사용을 위한 예시클래스

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

