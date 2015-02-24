#include "stdafx.h"
#include "cSkinnedMeshBody.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cSkinnedMeshBody::cSkinnedMeshBody()
	:m_pHead(NULL),
	m_pHair(NULL),
	m_pMesh(NULL)
{
}


cSkinnedMeshBody::~cSkinnedMeshBody()
{
	SAFE_RELEASE(m_pHead);
	SAFE_RELEASE(m_pHair);
	SAFE_RELEASE(m_pMesh);
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

	// 머리 셋업
	m_pHead = new cSkinnedMesh;
	m_pHead->Setup(sFolderHead, sFileHead);
		
	// 머리카락 셋업
	m_pHair = new cSkinnedMesh;
	m_pHair->Setup(sFolderHair, sFileHair);

	//D3DXCreateSphere(g_pD3DDevice, 1.0f, 10, 10, &m_pMesh, NULL);
	//D3DXFRAME* pDummyRoot = D3DXFrameFind(m_pRootFrame, "Dummy_root");	

	// 몸 중앙
	D3DXFRAME* pDummyRoot;
	pDummyRoot = D3DXFrameFind(m_pRootFrame, "FxCenter");
	D3DXMATRIXA16 mat = pDummyRoot->TransformationMatrix;
	D3DXVECTOR3 localCenter(0, 0, 0);
	D3DXVec3TransformCoord(&localCenter, &localCenter, &mat);
	m_stBoundingSphere.m_vCenter = localCenter;
	m_stBoundingSphere.m_fRadius = 10.0f;
	m_stUpdateBoundingSphere.m_vCenter = m_stBoundingSphere.m_vCenter;
	m_stUpdateBoundingSphere.m_fRadius = m_stBoundingSphere.m_fRadius;
	D3DXCreateSphere(g_pD3DDevice, m_stBoundingSphere.m_fRadius, 10, 10, &m_pDebugSphereBody, NULL);

	D3DXFRAME* pFxHand;
	pFxHand = D3DXFrameFind(m_pRootFrame, "FxHand01");
	mat = pFxHand->TransformationMatrix;
	D3DXVec3TransformCoord(&localCenter, &localCenter, &mat);
	m_stAttacSphere.m_vCenter = localCenter;
	m_stAttacSphere.m_fRadius = 5.0f;
	m_stUpdateAttacSphere.m_vCenter = m_stAttacSphere.m_vCenter;
	m_stUpdateAttacSphere.m_fRadius = m_stAttacSphere.m_fRadius;
	D3DXCreateSphere(g_pD3DDevice, m_stAttacSphere.m_fRadius, 10, 10, &m_pMesh, NULL);
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
	
	m_pAnimControl->AdvanceTime(g_pTimer->DeltaTime() * .5f, NULL);

	UpdateWorldMatrix(m_pRootFrame, NULL);
	UpdateSkinnedMesh(m_pRootFrame);

	Render(m_pRootFrame, pParentWorldTM);
}

void cSkinnedMeshBody::Render(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM){
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->matWorldTM = pBone->TransformationMatrix * (*pParentWorldTM);
	
	// 목부위 바이패드의 월드트랜스폼매트릭스를 받아서 머리 렌더 실시
	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Neck"))
	{
		if (m_pHead){
			m_pHead->Render(&pBone->matWorldTM);
		}
	}

	// 머리부위 바이패드의 월드트랜스폼매트릭스를 받아서 머리카락 렌더 실시
	else if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Head"))
	{
		if (m_pHair){
			m_pHair->Render(&pBone->matWorldTM);
		}
	}	
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH_SPHERE* pBoneMesh = (ST_BONE_MESH_SPHERE*)pBone->pMeshContainer;
		for (size_t i = 0; i < pBoneMesh->dwNumSubset; ++i)
		{
			
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &(pBone->matWorldTM));
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecMtlTex[i]->pTex);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtlTex[i]->stMtl);
			pBoneMesh->MeshData.pMesh->DrawSubset(i);
		}
	}

	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxCenter"))
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &(pBone->matWorldTM));
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetTexture(0, NULL);
		m_pDebugSphereBody->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		D3DXVec3TransformCoord(
			&m_stUpdateBoundingSphere.m_vCenter,
			&m_stBoundingSphere.m_vCenter,
			&pBone->matWorldTM);
	}

	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxHand01"))
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &(pBone->matWorldTM));
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetTexture(0, NULL);
		m_pMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		D3DXVec3TransformCoord(
			&m_stUpdateAttacSphere.m_vCenter,
			&m_stAttacSphere.m_vCenter,
			&pBone->matWorldTM);
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


void cSkinnedMeshBody::UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->matWorldTM = pBone->TransformationMatrix;

	if (pmatParent)
	{
		pBone->matWorldTM = pBone->TransformationMatrix * (*pmatParent);
	}

	if (pBone->pFrameSibling)
	{
		UpdateWorldMatrix(pBone->pFrameSibling, pmatParent);
	}
	if (pBone->pFrameFirstChild)
	{
		UpdateWorldMatrix(pBone->pFrameFirstChild, &pBone->matWorldTM);
	}
}

void cSkinnedMeshBody::SetAnimationIndex(DWORD dwIndex){
	cSkinnedMesh::SetAnimationIndex(dwIndex);
	if (m_pHair){
		m_pHair->SetAnimationIndex(dwIndex);
	}
	if (m_pHead){
		m_pHead->SetAnimationIndex(dwIndex);
	}
}

void cSkinnedMeshBody::SetAnimationLoop(DWORD dwIndex, bool isLoop)
{
	cSkinnedMesh::SetAnimationLoop(dwIndex, isLoop);
	if (m_pHair){
		m_pHair->SetAnimationLoop(dwIndex, isLoop);
	}
	if (m_pHead){
		m_pHead->SetAnimationLoop(dwIndex, isLoop);
	}
}