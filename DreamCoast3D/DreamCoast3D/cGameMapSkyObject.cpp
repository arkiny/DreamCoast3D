#include "stdafx.h"
#include "cGameMapSkyObject.h"


cGameMapSkyObject::cGameMapSkyObject()
	: m_pMesh(NULL)
{
	m_eGameObjectType = E_SKY;
	ZeroMemory(&m_stMaterial, sizeof(D3DMATERIAL9));
}

cGameMapSkyObject::~cGameMapSkyObject()
{
	SAFE_RELEASE(m_pMesh);
	//SAFE_RELEASE(m_pSpecularMapping);
}

void cGameMapSkyObject::Setup(){
	D3DXCreateSphere(g_pD3DDevice, 256.0f, 50, 50, &m_pMesh, NULL);
	m_stMaterial.Ambient = D3DXCOLOR(0.5f, 0.6f, 0.8f, 1.0f);
	m_stMaterial.Diffuse = D3DXCOLOR(0.5f, 0.6f, 0.8f, 1.0f);
	m_stMaterial.Specular = D3DXCOLOR(0.5f, 0.6f, 0.8f, 1.0f);
	SetPosition(D3DXVECTOR3(128, 0, 128));
	m_stBoundingSphere.m_fRadius = 256.0f;

	//HRESULT hr =
	//	D3DXCreateEffectFromFile(g_pD3DDevice, "../Resources/Shader/SpecularMapping.fx",
	//	NULL, NULL, NULL, NULL, &m_pSpecularMapping, NULL);
	//assert(hr == S_OK);
}

void cGameMapSkyObject::Update(float fDelta){

}

void cGameMapSkyObject::Render(){
	if (m_pMesh){
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		g_pD3DDevice->SetMaterial(&m_stMaterial);
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
		m_pMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}