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
	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup(sFolder, sFile);
}

void cGameActionSkinnedMeshObj::Update(float fDelta){
	cGameActionObject::Update(fDelta);
	if (m_pSkinnedMesh){
		m_pSkinnedMesh->Update(fDelta);
	}
}

void cGameActionSkinnedMeshObj::Render(){

	if (m_pSkinnedMesh){
		m_pSkinnedMesh->Render(GetTransformMatrix());
	}

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
