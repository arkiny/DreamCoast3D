#include "stdafx.h"
#include "cEffectMesh.h"
#include "cMesh.h"
#include "cTransform.h"

cEffectMesh::cEffectMesh()
	:m_fLifeSpan(1.0f),
	m_pEffectMesh(NULL)
{
}


cEffectMesh::~cEffectMesh()
{
	SAFE_RELEASE(m_pEffectMesh);
}

void cEffectMesh::Setup(){
	m_pEffectMesh = g_pStaticMeshManager->GetStaticMesh(std::string("../Resources/Effect/BloodRing/A_BloodRing005_SM.X"));
	m_pEffectMesh->AddRef();
	m_pTransform->SetScaling(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
}

void cEffectMesh::Start(){

}

void cEffectMesh::Update(float fDelta){

}

void cEffectMesh::Render(){
	D3DXMATRIXA16 matWorld;
	D3DXVECTOR3 v = m_pTransform->GetPosition();
	//D3DXMatrixIdentity(&matWorld);
	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	matView._41 = 0;
	matView._42 = 0;
	matView._43 = 0;
	D3DXMatrixInverse(&matInvView, 0, &matView);
	D3DXMATRIXA16 matscl;
	D3DXMatrixScaling(&matscl,
		m_pTransform->GetScaling().x,
		m_pTransform->GetScaling().y,
		m_pTransform->GetScaling().z);
	matInvView = matscl * matInvView;
	matInvView._41 = v.x;
	matInvView._42 = v.y;
	matInvView._43 = v.z;


	
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matInvView);

	m_pEffectMesh->Render(&matInvView);
}

void cEffectMesh::Exit(){

}