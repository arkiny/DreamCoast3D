#include "stdafx.h"
#include "cSphere.h"

cSphere::cSphere()
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}

cSphere::~cSphere()
{
}

void cSphere::Setup(){
	D3DXCreateSphere(g_pD3DDevice, 1.0f, 10, 10, &m_pMesh, NULL);
	m_stMtl.Ambient = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
}

void cSphere::Update(float fDelta){

}

void cSphere::Render(){
	cGameObject::Render(); // transform move
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, NULL);
	m_pMesh->DrawSubset(0);
}