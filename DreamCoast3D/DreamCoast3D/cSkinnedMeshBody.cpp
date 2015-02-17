#include "stdafx.h"
#include "cSkinnedMeshBody.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cSkinnedMeshBody::cSkinnedMeshBody()
	:m_pHead(NULL),
	m_pHair(NULL)
{
}


cSkinnedMeshBody::~cSkinnedMeshBody()
{
	SAFE_RELEASE(m_pHead);
	SAFE_RELEASE(m_pHair);
}

void cSkinnedMeshBody::Setup(
	std::string sFolder, std::string sFile,
	std::string sFolderHead, std::string sFileHead,
	std::string sFolderHair, std::string sFileHair
	){

	m_pRootFrame = g_pSkinnedMeshManager->GetSkinnedMesh(sFolder, sFile, m_pAnimControl);

	UINT uiNumAnim = m_pAnimControl->GetNumAnimationSets();
	for (UINT i = 0; i < uiNumAnim; ++i)
	{
		cAnimationSet* pAnimSet = new cAnimationSet;
		LPD3DXANIMATIONSET pAS = NULL;
		m_pAnimControl->GetAnimationSet(i, &pAS);
		pAnimSet->SetAnimationSet(pAS);
		SAFE_RELEASE(pAS);
		pAnimSet->SetIndex(m_vecAnimationSet.size());
		m_vecAnimationSet.push_back(pAnimSet);
	}

	m_pHead = new cSkinnedMesh;
	m_pHead->Setup(sFolderHead, sFileHead);
		
	m_pHair = new cSkinnedMesh;
	m_pHair->Setup(sFolderHair, sFileHair);
}

void cSkinnedMeshBody::Render(D3DXMATRIXA16* pParentWorldTM){
	m_vecAnimationSet[m_nCurrentAnimation]->Update(g_pTimer->DeltaTime());

	if (m_isAnimationBlending)
	{
		m_fPassedBlendTime += g_pTimer->DeltaTime();
		if (m_fPassedBlendTime >= m_fAnimationBlendTime)
		{
			m_isAnimationBlending = false;
			m_pAnimControl->SetTrackWeight(0, 1.0f);
			m_pAnimControl->SetTrackWeight(1, 0.0f);
			m_pAnimControl->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fAnimationBlendTime;
			m_pAnimControl->SetTrackWeight(0, fWeight);
			m_pAnimControl->SetTrackWeight(1, 1.0f - fWeight);
		}
	}

	m_pAnimControl->AdvanceTime(g_pTimer->DeltaTime(), NULL);

	UpdateWorldMatrix(m_pRootFrame, NULL);
	UpdateSkinnedMesh(m_pRootFrame);

	Render(m_pRootFrame, pParentWorldTM);
}

void cSkinnedMeshBody::Render(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM){
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->matWorldTM = (pFrame->TransformationMatrix) * (*pParentWorldTM);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->matWorldTM);
	
	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Neck"))
	{
		if (m_pHead){
			m_pHead->Render(&pBone->matWorldTM);
		}
	}
	else if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Head"))
	{
		if (m_pHair){
			m_pHair->Render(&pBone->matWorldTM);
		}
	}
	
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		for (size_t i = 0; i < pBoneMesh->dwNumSubset; ++i)
		{
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecMtlTex[i]->pTex);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtlTex[i]->stMtl);
			pBoneMesh->MeshData.pMesh->DrawSubset(i);
		}
	}
	if (pBone->pFrameSibling)
	{
		Render(pBone->pFrameSibling, pParentWorldTM);
	}
	
	if (pBone->pFrameFirstChild)
	{
		Render(pBone->pFrameFirstChild, &pBone->matWorldTM);
	}
}
