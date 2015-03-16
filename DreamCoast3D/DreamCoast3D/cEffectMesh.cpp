#include "stdafx.h"
#include "cEffectMesh.h"


cEffectMesh::cEffectMesh()
	:m_fLifeSpan(1.0f),
	m_pEffectMesh(NULL)
{
}


cEffectMesh::~cEffectMesh()
{
	SAFE_RELEASE(m_pEffectMesh);
	SAFE_RELEASE(m_pTexture);
}

void cEffectMesh::Setup(){
	m_pEffectMesh = g_pStaticMeshManager->GetStaticMesh(std::string("../Resources/Effect/BloodRing/A_BloodRing005_SM.X"));
	m_pEffectMesh->AddRef();
	m_pTexture = g_pTextureManager->GetTexture(std::string("../Resources/Effect/BloodRing/A_blood_ring05_emis.png"));
	m_pTexture->AddRef();
}

void cEffectMesh::Start(){

}

void cEffectMesh::Update(float fDelta){

}

void cEffectMesh::Render(){
	//// Enable alpha blending.
	//lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
	//	TRUE);

	//// Set the source blend state.
	//lpD3DDevice->SetRenderState(D3DRS_SRCBLEND,
	//	D3DBLEND_SRCCOLOR);

	//// Set the destination blend state.
	//lpD3DDevice->SetRenderState(D3DRS_DESTBLEND,
	//	D3DBLEND_INVSRCCOLOR);
	//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//D3DMATERIAL9 mtrl;
	//mtrl.Diffuse = mtrl.Ambient = mtrl.Specular = mtrl.Emissive = D3DXCOLOR(0.0, 0.0, 0.0, 0.5f);
	//
	//g_pD3DDevice->SetMaterial(&mtrl);

	// create material
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Emissive.r = 0.75f;
	mtrl.Emissive.g = 0.0f;
	mtrl.Emissive.b = 0.0f;
	mtrl.Emissive.a = 1.0f;
	
	g_pD3DDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
	g_pD3DDevice->SetTexture(0, m_pTexture);
	//D3DMATERIAL9 mtrl;
	//ZeroMemory(&mtrl, sizeof(mtrl));
	//mtrl.Ambient = D3DXCOLOR(.8, .8, .8, 1.0f);
	//mtrl.Diffuse = D3DXCOLOR(.8, .8, .8, 1.0f);
	//mtrl.Specular = D3DXCOLOR(.8, .8, .8, 1.0f);
	g_pD3DDevice->SetMaterial(&mtrl);
	m_pEffectMesh->DrawSubset(0);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void cEffectMesh::Exit(){

}