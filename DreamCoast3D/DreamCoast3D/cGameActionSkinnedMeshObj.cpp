#include "stdafx.h"
#include "cGameActionSkinnedMeshObj.h"
#include "cSkinnedMesh.h"
#include "cTransform.h"


cGameActionSkinnedMeshObj::cGameActionSkinnedMeshObj()
	:m_pSkinnedMesh(NULL),
	cGameActionObject()
{
}


cGameActionSkinnedMeshObj::~cGameActionSkinnedMeshObj()
{
	SAFE_RELEASE(m_pSkinnedMesh)
}

void cGameActionSkinnedMeshObj::Setup(std::string sFolder, std::string sFile){
	m_pSkinnedMesh = new cSkinnedMesh(sFolder, sFile);
	m_sFolder = sFolder;
	m_sFile = sFile;
}

void cGameActionSkinnedMeshObj::Update(float fDelta){
	cGameActionObject::Update(fDelta);
	if (m_pGameObjDeligate){
		m_pGameObjDeligate->isGameObjectCollided(this);
	}	
}

void cGameActionSkinnedMeshObj::Render(){
	if (m_pSkinnedMesh){
		m_pSkinnedMesh->Render(GetTransformMatrix());
	}
}

void cGameActionSkinnedMeshObj::RenderShadow() {
	if (m_pSkinnedMesh){
		m_pSkinnedMesh->UpdateAndRenderShadow(GetTransformMatrix());
	}
}


ST_BOUNDING_SPHERE* cGameActionSkinnedMeshObj::GetCollisionSphere(){
	ST_BOUNDING_SPHERE* ret = NULL;
	if (m_pSkinnedMesh){
		ret = &m_pSkinnedMesh->GetUpdateBoundingSphere();
		//ret->m_fRadius *= GetScale().x;
		return ret;
	}
	return ret;
}

ST_BOUNDING_SPHERE* cGameActionSkinnedMeshObj::GetAttackSphere(){
	ST_BOUNDING_SPHERE* ret = NULL;
	if (m_pSkinnedMesh){
		ret = &m_pSkinnedMesh->GetUpdateAttackSphere();
		//ret->m_fRadius *= GetScale().x;
		return ret;
	}
	return ret;
}

std::map<std::string, ST_BOUNDING_SPHERE>* cGameActionSkinnedMeshObj::GetUpdatedDetailedSphere(){
	std::map<std::string, ST_BOUNDING_SPHERE>* ret = NULL;
	if (m_pSkinnedMesh){
		ret = &m_pSkinnedMesh->GetUpdatedDetailedSphere();
		return ret;
	}
	return ret;
}

void cGameActionSkinnedMeshObj::ForcedMoving(D3DXVECTOR3 vDir, float fSpeed) {
	m_pTransform->SetPosition(m_pTransform->GetPosition() + vDir*g_pTimer->DeltaTime()*fSpeed);
}

//UINT uiNumAnim = m_pAnimControl->GetNumAnimationSets();
//for (UINT i = 0; i < uiNumAnim; ++i)
//{
//	cAnimationSet* pAnimSet = new cAnimationSet;
//	LPD3DXANIMATIONSET pAS = NULL;
//	m_pAnimControl->GetAnimationSet(i, &pAS);
//	pAnimSet->SetAnimationSet(pAS);
//	SAFE_RELEASE(pAS);
//	pAnimSet->SetIndex(m_vecAnimationSet.size());
//	m_vecAnimationSet.push_back(pAnimSet);
//}
