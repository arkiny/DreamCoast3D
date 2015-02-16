#include "stdafx.h"
#include "cGameASEObject.h"
#include "cASEInstance.h"

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
		m_pASEInstance->GetBoundingBox().vMax.x - m_pASEInstance->GetBoundingBox().vMin.x,
		m_pASEInstance->GetBoundingBox().vMax.y - m_pASEInstance->GetBoundingBox().vMin.y,
		m_pASEInstance->GetBoundingBox().vMax.z - m_pASEInstance->GetBoundingBox().vMin.z,
		&m_pDebugBoxMesh, NULL);
}

void cGameASEObject::Update(float fDelta){

}

void cGameASEObject::Render(){
	m_pASEInstance->Render(GetTransformMatrix());
	if (m_pDebugBoxMesh){
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
		m_pDebugBoxMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

ST_BOUNDING_BOX cGameASEObject::GetBoundingBox(){
	if (m_pASEInstance){
		ST_BOUNDING_BOX ret = m_pASEInstance->GetBoundingBox();
		D3DXVec3TransformCoord(&ret.vMin, &ret.vMin, GetTransformMatrix());
		D3DXVec3TransformCoord(&ret.vMax, &ret.vMax, GetTransformMatrix());
		return ret;
	}
	return ST_BOUNDING_BOX();
}