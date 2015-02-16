#include "stdafx.h"
#include "cSphere.h"

cSphere::cSphere()
	:m_pMesh(NULL)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}

cSphere::~cSphere()
{
	SAFE_RELEASE(m_pMesh);
}

void cSphere::Setup(){
	// Protected �̹Ƿ� ���� ����
	m_stBoundingSphere.m_fRadius = 1.0f;
	m_stBoundingSphere.m_vCenter = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	SetPosition(m_stBoundingSphere.m_vCenter);

	D3DXCreateSphere(g_pD3DDevice, m_stBoundingSphere.m_fRadius, 10, 10, &m_pMesh, NULL);
	m_stMtl.Ambient = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
}

void cSphere::Update(float fDelta){

}

void cSphere::Render(){
	cGameObject::Render(); // Ʈ������ ��Ʈ���� �Ǻδ� ���ӿ�����Ʈ���� �ǽ�
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, NULL);
	m_pMesh->DrawSubset(0);
}