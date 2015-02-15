#pragma once

class cMtlTex;

class cASEInstance : public cObject
{
	friend class cAseLoader;
private:
	std::string						m_sNodeName;
	std::string						m_sParentName;
	std::vector<cASEInstance*>	m_vecChildren;
	D3DXMATRIXA16					m_matLocalTM;
	D3DXMATRIXA16					m_matWorldTM;
	LPD3DXMESH						m_pMesh;
	//LPD3DXMESH						m_pSphereMesh;
	cMtlTex*						m_pMtlTex;

	virtual void Update(D3DXMATRIXA16* pmatParent);

public:
	cASEInstance(void);
	virtual ~cASEInstance(void);

	virtual void AddChild(cASEInstance* pChild);
	virtual void CalcLocalTM(D3DXMATRIXA16* pParent);
	virtual void BuidlMesh(std::vector<ST_PNT_VERTEX>& vecVertex);
	
	virtual void SetMtlTex(cMtlTex* pMtlTex);
	virtual void Render(D3DXMATRIXA16* pParent);
	virtual void Release();
};

