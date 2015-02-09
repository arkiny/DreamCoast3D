#include "stdafx.h"
#include "cFrustum.h"


cFrustum::cFrustum()
{
}


cFrustum::~cFrustum()
{
}

void cFrustum::Setup(){
	m_vecProjVertex.resize(8);
	m_vecProjVertex[0] = D3DXVECTOR3(-1, -1, 0);
	m_vecProjVertex[1] = D3DXVECTOR3(-1, 1, 0);
	m_vecProjVertex[2] = D3DXVECTOR3(1, 1, 0);
	m_vecProjVertex[3] = D3DXVECTOR3(1, -1, 0);
				   
	m_vecProjVertex[4] = D3DXVECTOR3(-1, -1, 1);
	m_vecProjVertex[5] = D3DXVECTOR3(-1, 1, 1);
	m_vecProjVertex[6] = D3DXVECTOR3(1, 1, 1);
	m_vecProjVertex[7] = D3DXVECTOR3(1, -1, 1);

	m_vecPlane.resize(6);
}

void cFrustum::Update(float fDelta){
	D3DXMATRIXA16 matProj, matView;

	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	std::vector<D3DXVECTOR3> vecWorldVertex(8);
	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3Unproject(
			&vecWorldVertex[i],
			&m_vecProjVertex[i],
			NULL,
			&matProj,
			&matView,
			NULL);
	}

	D3DXPlaneFromPoints(&m_vecPlane[0],
		&vecWorldVertex[0],
		&vecWorldVertex[1],
		&vecWorldVertex[2]);

	D3DXPlaneFromPoints(&m_vecPlane[1],
		&vecWorldVertex[6],
		&vecWorldVertex[5],
		&vecWorldVertex[4]);

	D3DXPlaneFromPoints(&m_vecPlane[2],
		&vecWorldVertex[3],
		&vecWorldVertex[2],
		&vecWorldVertex[7]);

	D3DXPlaneFromPoints(&m_vecPlane[3],
		&vecWorldVertex[5],
		&vecWorldVertex[1],
		&vecWorldVertex[0]);

	D3DXPlaneFromPoints(&m_vecPlane[4],
		&vecWorldVertex[5],
		&vecWorldVertex[6],
		&vecWorldVertex[2]);

	D3DXPlaneFromPoints(&m_vecPlane[5],
		&vecWorldVertex[0],
		&vecWorldVertex[3],
		&vecWorldVertex[7]);
}

bool cFrustum::IsIn(ST_BOUNDING_SPHERE* pBoundingSphere){
	for each (D3DXPLANE p in m_vecPlane)
	{
		if (D3DXPlaneDotCoord(&p, &pBoundingSphere->m_vCenter) > pBoundingSphere->m_fRadius)
			return false;
	}
	return true;
}