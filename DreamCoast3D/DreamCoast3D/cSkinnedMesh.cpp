#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"
#include "cAnimationSet.h"

cSkinnedMesh::cSkinnedMesh(void)
	: m_pRootFrame(NULL)
	, m_fAnimationBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimationBlending(false)
	, m_nCurrentAnimation(0)
	, m_pAnimControl(NULL)
	, m_pDebugSphereBody(NULL)
{
}


cSkinnedMesh::~cSkinnedMesh(void)
{
	SAFE_RELEASE(m_pAnimControl);
	for (auto p : m_vecAnimationSet){
		SAFE_RELEASE(p);
	}
	SAFE_RELEASE(m_pDebugSphereBody);
}

void cSkinnedMesh::Setup(std::string sFolder, std::string sFile)
{
	// 인스탄싱을 마친 D3DXFRAME을 루트로 넘겨줌
	m_pRootFrame = g_pSkinnedMeshManager->GetSkinnedMesh(sFolder, sFile, m_pAnimControl);

	// 이펙트가 일어나는 손
	D3DXFrameFind(m_pRootFrame, "FxHand01");

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

	// 몸 중앙
	D3DXFRAME* pDummyRoot;
	pDummyRoot = D3DXFrameFind(m_pRootFrame, "FxCenter");
	if (pDummyRoot){
		D3DXMATRIXA16 mat = pDummyRoot->TransformationMatrix;
		D3DXVECTOR3 localCenter(0, 0, 0);
		D3DXVec3TransformCoord(&localCenter, &localCenter, &mat);
		m_stBoundingSphere.m_vCenter = localCenter;
		m_stBoundingSphere.m_fRadius = 15.0f;
		m_stUpdateBoundingSphere.m_vCenter = m_stBoundingSphere.m_vCenter;
		m_stUpdateBoundingSphere.m_fRadius = m_stBoundingSphere.m_fRadius;
		D3DXCreateSphere(g_pD3DDevice, m_stBoundingSphere.m_fRadius, 10, 10, &m_pDebugSphereBody, NULL);
	}
}

void cSkinnedMesh::Update(float fDelta)
{
	// TODO : 스킨드 매쉬를 인스탄싱해서 쓰려면,
	//        특이 경우로 렌더직전에 업데이트 해줘야한다.
}

void cSkinnedMesh::Render(D3DXMATRIXA16* pParentWorldTM)
{
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


void cSkinnedMesh::UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->matWorldTM = pBone->TransformationMatrix;
	if (pmatParent)
	{
		pBone->matWorldTM = pBone->matWorldTM * (*pmatParent);
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

void cSkinnedMesh::Render(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->matWorldTM = (pFrame->TransformationMatrix) * (*pParentWorldTM);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->matWorldTM);

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

	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxCenter"))
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetTexture(0, NULL);
		m_pDebugSphereBody->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		D3DXVec3TransformCoord(
			&m_stUpdateBoundingSphere.m_vCenter,
			&m_stBoundingSphere.m_vCenter,
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

void cSkinnedMesh::SetAnimationIndex(DWORD dwIndex)
{
	if (dwIndex >= m_vecAnimationSet.size())
		return;

	cAnimationSet* pCurr = m_vecAnimationSet[m_nCurrentAnimation];
	cAnimationSet* pNext = m_vecAnimationSet[dwIndex];
	if (pNext->GetIsLoop())
	{
		SetAnimationIndexBlend(dwIndex);
	}
	else
	{
		pNext->SetDelegate(this);
		pNext->SetPrevAnimation(pCurr);
		pNext->Start();
		SetAnimationIndexBlend(dwIndex);
	}
	m_nCurrentAnimation = dwIndex;
}

void cSkinnedMesh::SetAnimationIndexBlend(DWORD dwIndex)
{
	LPD3DXANIMATIONSET pPrev = NULL;
	LPD3DXANIMATIONSET pNext = NULL;

	m_pAnimControl->GetAnimationSet(dwIndex, &pNext);
	if (!pNext)
	{
		return;
	}
	m_pAnimControl->GetTrackAnimationSet(0, &pPrev);

	D3DXTRACK_DESC desc;
	m_pAnimControl->GetTrackDesc(0, &desc);
	m_pAnimControl->SetTrackDesc(1, &desc);

	m_pAnimControl->SetTrackWeight(0, 0.0f);
	m_pAnimControl->SetTrackWeight(1, 1.0f);

	m_pAnimControl->SetTrackAnimationSet(0, pNext);
	m_pAnimControl->SetTrackAnimationSet(1, pPrev);

	SAFE_RELEASE(pPrev);
	SAFE_RELEASE(pNext);

	m_fPassedBlendTime = 0.0f;
	m_isAnimationBlending = true;
}

void cSkinnedMesh::SetAnimationLoop(DWORD dwIndex, bool isLoop)
{
	if (dwIndex < m_vecAnimationSet.size())
	{
		m_vecAnimationSet[dwIndex]->SetIsLoop(isLoop);
	}
}

void cSkinnedMesh::UpdateSkinnedMesh(D3DXFRAME* pFrame)
{
	if (pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				// pCurrentBoneMatrices 를 계산하시오
				// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs 

				pBoneMesh->pCurrentBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE* src = NULL;
			BYTE* dest = NULL;

			pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);

			//MeshData.pMesh을 업데이트 시켜준다.
			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

			pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	ST_BONE* pBone = (ST_BONE*)pFrame;
	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		UpdateSkinnedMesh(pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		UpdateSkinnedMesh(pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::OnFinishAnimation(cAnimationSet* pSender)
{
	SetAnimationIndex(pSender->GetPrevAnimation()->GetIndex());
}

//충돌용 바운딩 스피어의 크기와 위치를 계산한다. : 민우
void cSkinnedMesh::CalcCollisionSphere(ST_BONE_MESH* pBoneMesh)
{
	ST_BONE_MESH_SPHERE* p = (ST_BONE_MESH_SPHERE*)pBoneMesh;
	D3DXMATRIXA16 matS, matT;
	D3DXMatrixIdentity(&matS);
	D3DXMatrixIdentity(&matT);

	//첫 프레임의 반지름을 OriginRadius라고 하고 이것을 기준으로 삼아 차이만큼 스케일링 한다.
	float fOffset = p->fRadius / p->fOriginRadius;
	D3DXMatrixScaling(&matS, fOffset, fOffset, fOffset);

	//스피어의 중심점으로 변환
	D3DXMatrixTranslation(&matT,
		p->vCenter.x, p->vCenter.y, p->vCenter.z);

	p->matSphereW = matS*matT;
}

//충돌용 바운딩 스피어를 그린다. : 민우
void cSkinnedMesh::RenderCollisionSphere(ST_BONE_MESH* pBoneMesh)
{
	ST_BONE_MESH_SPHERE* p = (ST_BONE_MESH_SPHERE*)pBoneMesh;
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &p->matSphereW);
	p->pSphereMesh->DrawSubset(0);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
