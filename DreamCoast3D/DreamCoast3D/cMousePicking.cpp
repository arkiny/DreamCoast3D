#include "stdafx.h"
#include "cMousePicking.h"
#include "cEffectParticle.h"

#define MAP_SIZE 257

cMousePicking::cMousePicking()
	: m_isRightButton(false)
{
	ZeroMemory(&m_vMouse, sizeof(D3DXVECTOR2));
	ZeroMemory(&m_vMouseMove, sizeof(D3DXVECTOR2));
	ZeroMemory(&m_vClickedPosition, sizeof(D3DXVECTOR3));
}

cMousePicking::~cMousePicking()
{
}

void cMousePicking::SetVertex(std::vector<ST_PNT2_VERTEX> Vertex)
{
	m_vecVertex.resize(Vertex.size());
	m_vecVertex = Vertex;
}

void cMousePicking::Update()
{
	IntersetionTriUpdate();
	//GetPickingPoint();
}

void cMousePicking::MouseUpdate()
{
	if (g_pControlManager->GetInputInfo(VK_RBUTTON))
	{
		m_isRightButton = true;
		m_vMouse.x = g_pControlManager->GetRClickedCursorPosition().x;
		m_vMouse.y = g_pControlManager->GetRClickedCursorPosition().y;
	}
	else
	{
		m_isRightButton = false;
		ZeroMemory(&m_vMouse, sizeof(D3DXVECTOR2));
	}

	m_vMouseMove.x = g_pControlManager->GetCurrentCursorPosition().x;
	m_vMouseMove.y = g_pControlManager->GetCurrentCursorPosition().y;
}

ST_RAY cMousePicking::CalPickingRay(D3DXVECTOR2 vMouse)
{
	float fX = 0.0f;
	float fY = 0.0f;

	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIX proj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

	fX = (((2.0f * vMouse.x) / vp.Width) - 1.0f) / proj(0, 0);
	fY = (((-2.0f * vMouse.y) / vp.Height) + 1.0f) / proj(1, 1);

	ST_RAY ray;
	ray.vecOrigin = { 0.f, 0.f, 0.f };
	ray.vecDirection = { fX, fY, 1.0f };

	return ray;
}

void cMousePicking::TransformRay(ST_RAY* pRay, D3DXMATRIXA16* mat)
{
	D3DXVec3TransformCoord(
		&pRay->vecOrigin,
		&pRay->vecOrigin,
		mat);

	// 광선의 방향을 변환. w = 0
	D3DXVec3TransformNormal(
		&pRay->vecDirection,
		&pRay->vecDirection,
		mat);

	D3DXVec3Normalize(&pRay->vecDirection, &pRay->vecDirection);
}

D3DXVECTOR3 cMousePicking::GetPickingPoint()
{
	MouseUpdate();
	m_vClickedPosition = { 0, 0, 0 };
	if (m_vecVertex.size() > 0)
	{
		float fU = 0.f;
		float fV = 0.f;
		float fDist = 0.f;

		ST_RAY ray;
		ray = CalPickingRay(m_vMouseMove);

		D3DXMATRIXA16 view;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);

		D3DXMATRIXA16 viewInverse;
		D3DXMatrixInverse(&viewInverse, 0, &view);
		TransformRay(&ray, &viewInverse);

		for (int i = 0; i < m_vecVertex.size() - (MAP_SIZE + 2); i++)
		{
			if (i % MAP_SIZE != MAP_SIZE - 1)
			{
				bool isColliedTri = false;
				
				isColliedTri = D3DXIntersectTri(
					&m_vecVertex[i + 1].p,
					&m_vecVertex[i].p,
					&m_vecVertex[i + (MAP_SIZE)].p,
					&ray.vecOrigin,
					&ray.vecDirection,
					&fU, &fV, &fDist);
				if (isColliedTri == true)
				{
					m_vClickedPosition = ray.vecOrigin + (fDist*ray.vecDirection);
					//cEffectParticle* pcheck = new cEffectParticle;
					//pcheck->Setup();
					//pcheck->SetPosition(m_vClickedPosition);
					//if (m_pEffectDelegate){
					//	m_pEffectDelegate->AddEffect(pcheck);
					//	pcheck->Release();
					//}
					return m_vClickedPosition;
				}
				isColliedTri = D3DXIntersectTri(
					&m_vecVertex[i + (MAP_SIZE)].p,
					&m_vecVertex[i + (MAP_SIZE + 1)].p,
					&m_vecVertex[i + 1].p,
					&ray.vecOrigin,
					&ray.vecDirection,
					&fU, &fV, &fDist);
				if (isColliedTri == true)
				{
					m_vClickedPosition = ray.vecOrigin + (fDist*ray.vecDirection);
					//cEffectParticle* pcheck = new cEffectParticle;
					//pcheck->Setup();
					//pcheck->SetPosition(m_vClickedPosition);
					//if (m_pEffectDelegate){
					//	m_pEffectDelegate->AddEffect(pcheck);
					//	pcheck->Release();
					//}
					return m_vClickedPosition;
				}
			}
		}
	}
	return m_vClickedPosition;
}

void cMousePicking::IntersetionTriUpdate()
{
	MouseUpdate();

	float fU = 0.f;
	float fV = 0.f;
	float fDist = 0.f;

	if (m_isRightButton)
	{
		ST_RAY ray;
		ray = CalPickingRay(m_vMouse);

		D3DXMATRIXA16 view;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &view);

		D3DXMATRIXA16 viewInverse;
		D3DXMatrixInverse(&viewInverse, 0, &view);
		TransformRay(&ray, &viewInverse);

		for (int i = 0; i < m_vecVertex.size() - (MAP_SIZE + 2); i++)
		{
			if (i % MAP_SIZE != MAP_SIZE - 1)
			{
				bool isColliedTri = false;
				isColliedTri = D3DXIntersectTri(
					&m_vecVertex[i + 1].p,
					&m_vecVertex[i].p,
					&m_vecVertex[i + (MAP_SIZE)].p,
					&ray.vecOrigin,
					&ray.vecDirection,
					&fU, &fV, &fDist);
				if (isColliedTri == true)
				{
					m_vClickedPosition = ray.vecOrigin + (fDist*ray.vecDirection);
					//cEffectParticle* pcheck = new cEffectParticle;
					//pcheck->Setup();
					//pcheck->SetPosition(m_vClickedPosition);
					//if (m_pEffectDelegate){
					//	m_pEffectDelegate->AddEffect(pcheck);
					//	pcheck->Release();
					//}
					return;
				}
				isColliedTri = D3DXIntersectTri(
					&m_vecVertex[i + (MAP_SIZE)].p,
					&m_vecVertex[i + (MAP_SIZE + 1)].p,
					&m_vecVertex[i + 1].p,
					&ray.vecOrigin,
					&ray.vecDirection,
					&fU, &fV, &fDist);
				if (isColliedTri == true)
				{
					m_vClickedPosition = ray.vecOrigin + (fDist*ray.vecDirection);
					//cEffectParticle* pcheck = new cEffectParticle;
					//pcheck->Setup();
					//pcheck->SetPosition(m_vClickedPosition);
					//if (m_pEffectDelegate){
					//	m_pEffectDelegate->AddEffect(pcheck);
					//	pcheck->Release();
					//}
					return;
				}
			}
		}
	}

}