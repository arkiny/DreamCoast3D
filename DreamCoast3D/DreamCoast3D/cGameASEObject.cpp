#include "stdafx.h"
#include "cGameASEObject.h"
#include "cASEInstance.h"

cGameASEObject::cGameASEObject() 
	: m_pASEInstance(NULL)
	, m_pDebugBoxMesh(NULL)
{
	m_eGameObjectType = E_STATIC;
}


cGameASEObject::~cGameASEObject()
{
	SAFE_RELEASE(m_pDebugBoxMesh);
}

void cGameASEObject::Setup(std::string sFolder, std::string sFile){
	m_pASEInstance = g_pAseManager->GetAseObject(sFolder, sFile);
	
	m_sASEFolder = sFolder;
	m_sASEFile = sFile;

	D3DXCreateBox(g_pD3DDevice, 
		m_pASEInstance->GetBoundingBox().vMax.x - m_pASEInstance->GetBoundingBox().vMin.x,
		m_pASEInstance->GetBoundingBox().vMax.y - m_pASEInstance->GetBoundingBox().vMin.y,
		m_pASEInstance->GetBoundingBox().vMax.z - m_pASEInstance->GetBoundingBox().vMin.z,
		&m_pDebugBoxMesh, NULL);

	m_pBoundingBox = &m_pASEInstance->GetBoundingBox();
	float x = -1.0f;
	float y = -1.0f;
	float z = -1.0f;
	if (m_pBoundingBox){
		x = m_pBoundingBox->vMax.x - m_pBoundingBox->vMin.x;
		y = m_pBoundingBox->vMax.y - m_pBoundingBox->vMin.y;
		z = m_pBoundingBox->vMax.z - m_pBoundingBox->vMin.z;
	}

	float max = fmax(fmax(x, y),z);

	m_stBoundingSphere.m_fRadius = max / 2.0f;
}

void cGameASEObject::Update(float fDelta){

}

void cGameASEObject::Render(){
	m_pASEInstance->Render(GetTransformMatrix());

	if (GetAsyncKeyState(VK_TAB)){
		if (m_pDebugBoxMesh){
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			D3DXMATRIXA16 matT;
			D3DXMatrixTranslation(&matT,
				0,
				(m_pASEInstance->GetBoundingBox().vMax.y - m_pASEInstance->GetBoundingBox().vMin.y) / 2.0f,
				0);
			matT = matT * *GetTransformMatrix();
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
			m_pDebugBoxMesh->DrawSubset(0);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
}

ST_BOUNDING_BOX* cGameASEObject::GetBoundingBox(){
	if (m_pASEInstance){
		ST_BOUNDING_BOX ret = m_pASEInstance->GetBoundingBox();
		D3DXVec3TransformCoord(&ret.vMin, &ret.vMin, GetTransformMatrix());
		D3DXVec3TransformCoord(&ret.vMax, &ret.vMax, GetTransformMatrix());
		return &ret;
	}
	return &ST_BOUNDING_BOX();
}

void cGameASEObject::Clone(cGameObject** pTarget){
	cGameASEObject* p = new cGameASEObject;
	p->Setup(m_sASEFolder, m_sASEFile);
	D3DXVECTOR3 pCopyPos = this->GetPosition();
	p->SetPosition(pCopyPos);
	D3DXVECTOR3 vCopyScale = this->GetScale();
	p->SetScale(vCopyScale);
	p->Start();
	*pTarget = p;
}