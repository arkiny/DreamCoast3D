#include "stdafx.h"
#include "cSphereAction.h"


cSphereAction::cSphereAction()
	:m_pMesh(NULL)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}


cSphereAction::~cSphereAction()
{
	SAFE_RELEASE(m_pMesh);
}

void cSphereAction::Setup(){
	// Protected 이므로 수정 가능
	m_stBoundingSphere.m_fRadius = 1.0f;
	m_stBoundingSphere.m_vCenter = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	SetPosition(m_stBoundingSphere.m_vCenter);

	D3DXCreateSphere(g_pD3DDevice, m_stBoundingSphere.m_fRadius, 10, 10, &m_pMesh, NULL);
	m_stMtl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.8f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.8f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.2f, 0.2f, 0.8f, 1.0f);
}

void cSphereAction::Update(float fDelta){
	cGameActionObject::Update(fDelta);
}

void cSphereAction::Render(){
	cGameObject::Render(); // 트랜스폼 매트릭스 므부는 게임오브젝트에서 실시
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, NULL);
	m_pMesh->DrawSubset(0);
}
