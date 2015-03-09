#pragma once

#define WIN_WIDTH		1280
#define WIN_HEIGHT		720
#define PI           3.14159265f
#define FOV          (PI/4.0f)                     // 시야각
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT) // 화면의 종횡비
#define NEAR_PLANE   1                             // 근접 평면
#define FAR_PLANE    10000                         // 원거리 평면

class cMtlTex;

class cASEInstance : public cObject
{
	friend class cAseLoader;
protected:
	SYNTHESIZE(ST_BOUNDING_BOX, m_stBoundingBox, BoundingBox);
private:
	std::string						m_sNodeName;
	std::string						m_sParentName;
	std::vector<cASEInstance*>	m_vecChildren;
	D3DXMATRIXA16					m_matLocalTM;
	D3DXMATRIXA16					m_matWorldTM;
	LPD3DXMESH						m_pMesh;
	//LPD3DXMESH						m_pSphereMesh;
	cMtlTex*						m_pMtlTex;

	// shader test용
	D3DXVECTOR4						gLightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	//LPD3DXEFFECT					m_pSpecularMapping = NULL;

	SYNTHESIZE(int, m_nAseRefNum, ASERefNumber);

	virtual void Update(D3DXMATRIXA16* pmatParent);

public:
	cASEInstance(void);
	virtual ~cASEInstance(void);

	virtual void AddChild(cASEInstance* pChild);
	virtual void CalcLocalTM(D3DXMATRIXA16* pParent);
	virtual void BuidlMesh(std::vector<ST_PNT2_VERTEX>& vecVertex);
	
	virtual void SetMtlTex(cMtlTex* pMtlTex);
	virtual cMtlTex* GetMtlTex() { return m_pMtlTex; }

	virtual void Render(D3DXMATRIXA16* pParent);
	virtual void RenderShadow(D3DXMATRIXA16* pParent);

	virtual void Release();
};

