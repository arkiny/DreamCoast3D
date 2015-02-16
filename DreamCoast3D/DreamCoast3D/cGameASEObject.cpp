#include "stdafx.h"
#include "cGameASEObject.h"
#include "cASEInstance.h"
#include "cTransform.h"

cGameASEObject::cGameASEObject() 
	: m_pASEInstance(NULL)
	, m_pDebugBoxMesh(NULL)
{
}


cGameASEObject::~cGameASEObject()
{
	SAFE_RELEASE(m_pDebugBoxMesh);
}

void cGameASEObject::Setup(std::string sFolder, std::string sFile){
	m_pASEInstance = g_pAseManager->GetAseObject(sFolder, sFile);
	
	D3DXCreateBox(g_pD3DDevice, 
		m_pASEInstance->GetBoundingBox().vMax.x,
		m_pASEInstance->GetBoundingBox().vMax.y,
		m_pASEInstance->GetBoundingBox().vMax.z,
		&m_pDebugBoxMesh, NULL);
}

void cGameASEObject::Update(float fDelta){

}

void cGameASEObject::Render(){
	m_pASEInstance->Render(GetTransformMatrix());
	if (m_pDebugBoxMesh){
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		D3DXMATRIXA16 matWorld, matS, matT;
		D3DXMatrixScaling(&matS, m_pTransform->GetScaling().x, m_pTransform->GetScaling().y, m_pTransform->GetScaling().z);
		D3DXMatrixTranslation(&matT, GetPosition().x, GetPosition().y, GetPosition().z);
		matWorld = matS * matT;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pDebugBoxMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

ST_BOUNDING_BOX& cGameASEObject::GetBoundingBox(){
	if (m_pASEInstance){
		ST_BOUNDING_BOX ret = m_pASEInstance->GetBoundingBox();
		D3DXVec3TransformCoord(&ret.vMin, &ret.vMin, GetTransformMatrix());
		D3DXVec3TransformCoord(&ret.vMax, &ret.vMax, GetTransformMatrix());
		return ret;
	}
	return ST_BOUNDING_BOX();
}