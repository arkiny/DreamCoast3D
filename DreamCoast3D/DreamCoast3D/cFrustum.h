#pragma once

class cFrustum : public cObject
{
private:
	std::vector<D3DXVECTOR3>	m_vecProjVertex;
	std::vector<D3DXPLANE>		m_vecPlane;

public:
	cFrustum();
	~cFrustum();

	void Setup();
	void Update(float fDelta);
	bool IsIn(ST_BOUNDING_SPHERE* pBoundingSphere);
};

