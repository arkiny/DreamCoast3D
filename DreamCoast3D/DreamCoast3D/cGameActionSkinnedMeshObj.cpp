#include "stdafx.h"
#include "cGameActionSkinnedMeshObj.h"
#include "cSkinnedMesh.h"


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
}

void cGameActionSkinnedMeshObj::Update(float fDelta){
	cGameActionObject::Update(fDelta);
	
}

void cGameActionSkinnedMeshObj::Render(){
	if (m_pSkinnedMesh){
		m_pSkinnedMesh->UpdateAndRender(GetTransformMatrix());
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
