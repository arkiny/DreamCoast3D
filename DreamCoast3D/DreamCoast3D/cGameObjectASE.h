#pragma once

class cMtlTex;

class cGameObjectASE : public cGameObject
{
	friend class cAseLoader;
private:
	std::string						m_sNodeName;
	std::string						m_sParentName;
	std::vector<cGameObjectASE*>	m_vecChildren;
	D3DXMATRIXA16					m_matLocalTM;
	D3DXMATRIXA16					m_matWorldTM;
	LPD3DXMESH						m_pMesh;
	cMtlTex*						m_pMtlTex;

	virtual void Update(D3DXMATRIXA16* pmatParent);

public:
	cGameObjectASE(void);
	virtual ~cGameObjectASE(void);

	virtual void AddChild(cGameObjectASE* pChild);
	virtual void CalcLocalTM(D3DXMATRIXA16* pParent);
	virtual void BuidlMesh(std::vector<ST_PNT_VERTEX>& vecVertex);
	virtual void SetMtlTex(cMtlTex* pMtlTex);

	virtual void Render();
	virtual void Release();
};

