#include "stdafx.h"
#include "cCameraEditing.h"


cCameraEditing::cCameraEditing()
	:cCamera()
	, m_vPosition(0, 0, 0)
	, m_vDir(0, 0, 1)
	, m_fAngleDegree(0.0f)
	, m_pMesh(NULL)
{
	m_stMaterial = {};
}

cCameraEditing::~cCameraEditing()
{
	SAFE_RELEASE(m_pMesh);
}

void cCameraEditing::Setup(){
	cCamera::Setup();
	cCamera::SetTarget(&m_vPosition);
	D3DXCreateSphere(g_pD3DDevice, 2.0f, 5, 5, &m_pMesh, NULL);
	m_stMaterial.Ambient = D3DXCOLOR(.8f, .2f, .2f, 1.0f);
	m_stMaterial.Diffuse = D3DXCOLOR(.8f, .2f, .2f, 1.0f);
	m_stMaterial.Specular = D3DXCOLOR(.8f, .2f, .2f, 1.0f);
}

void cCameraEditing::Update(float fDelta){
	if (g_pControlManager->GetInputInfo('W')){
		m_vPosition += m_vDir * fDelta * 30.0f;
	}
	if (g_pControlManager->GetInputInfo('S')){
		m_vPosition -= m_vDir * fDelta * 30.0f;
	}
	if (g_pControlManager->GetInputInfo('A')){
		D3DXMATRIXA16 matR;
		m_fAngleDegree -= fDelta*4.0f;
		D3DXMatrixRotationY(&matR, m_fAngleDegree);
		m_vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);

	}
	if (g_pControlManager->GetInputInfo('D')){
		D3DXMATRIXA16 matR;
		m_fAngleDegree += fDelta*4.0f;
		D3DXMatrixRotationY(&matR, m_fAngleDegree);
		m_vDir = D3DXVECTOR3(0, 0, -1.f);
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);
	}
	cCamera::Update(fDelta);
}

void cCameraEditing::Render(){
	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &trans);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetMaterial(&m_stMaterial);
	m_pMesh->DrawSubset(0);
}