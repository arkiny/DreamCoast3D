#include "stdafx.h"
#include "cGameObjectEraser.h"
#include "cSceneEditMode.h"


cGameObjectEraser::cGameObjectEraser() 
	:m_pMesh(NULL)
{
	m_eGameObjectType = E_ERASER;
	ZeroMemory(&m_stSphere, sizeof(ST_BOUNDING_SPHERE));
}


cGameObjectEraser::~cGameObjectEraser()
{
	SAFE_RELEASE(m_pMesh);
}

void cGameObjectEraser::Setup(){
	D3DXCreateSphere(g_pD3DDevice, m_fRadius, 10, 10, &m_pMesh, 0);
	m_stSphere.m_vCenter= D3DXVECTOR3(0, 0, 0);
	m_stSphere.m_fRadius = m_fRadius;
}

void cGameObjectEraser::Update(float fDelta){
	D3DXMATRIXA16 transform;
	D3DXVECTOR3 origin(0, 0, 0);
	D3DXVec3TransformCoord(&m_stSphere.m_vCenter, &origin, GetTransformMatrix());
}

void cGameObjectEraser::Render(){
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
	if (m_pMesh){
		m_pMesh->DrawSubset(0);
	}
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

ST_BOUNDING_SPHERE*	cGameObjectEraser::GetBoundingSphere(){
	return &m_stSphere;
}

void cGameObjectEraser::SetPosition(D3DXVECTOR3& newPos) {
	cGameObject::SetPosition(newPos);
	D3DXMATRIXA16 transform;
	D3DXVECTOR3 origin(0, 0, 0);
	D3DXVec3TransformCoord(&m_stSphere.m_vCenter, &origin, GetTransformMatrix());
}
