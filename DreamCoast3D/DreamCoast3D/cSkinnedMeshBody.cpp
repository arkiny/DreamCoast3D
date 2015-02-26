#include "stdafx.h"
#include "cSkinnedMeshBody.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cSkinnedMeshBody::cSkinnedMeshBody()
	:m_pHead(NULL),
	m_pHair(NULL),
	m_pMesh(NULL),
	cSkinnedMesh()
{
}


cSkinnedMeshBody::~cSkinnedMeshBody()
{
	SAFE_RELEASE(m_pHead);
	SAFE_RELEASE(m_pHair);
	SAFE_RELEASE(m_pMesh);
}

cSkinnedMeshBody::cSkinnedMeshBody(std::string sFolder, std::string sFile,
	std::string sFolderHead, std::string sFileHead,
	std::string sFolderHair, std::string sFileHair) 
	:m_pHead(NULL),
	m_pHair(NULL),
	m_pMesh(NULL),
	cSkinnedMesh(sFolder, sFile)
{
	m_pHead = new cSkinnedMesh(sFolderHead, sFileHead);
	m_pHair = new cSkinnedMesh(sFolderHair, sFileHair);

	
	// Attack용 임시처리, HD
	D3DXVECTOR3 localCenter(0, 0, 0);
	D3DXMATRIXA16 mat;
	D3DXFRAME* pFxHand;
	pFxHand = D3DXFrameFind(m_pRootFrame, "FxHand01");
	mat = pFxHand->TransformationMatrix;
	D3DXVec3TransformCoord(&localCenter, &localCenter, &mat);
	m_stAttacSphere.m_vCenter = localCenter;
	m_stAttacSphere.m_fRadius = 5.0f;
	m_stUpdateAttacSphere.m_vCenter = m_stAttacSphere.m_vCenter;
	m_stUpdateAttacSphere.m_fRadius = m_stAttacSphere.m_fRadius;
	D3DXCreateSphere(g_pD3DDevice, m_stAttacSphere.m_fRadius, 10, 10, &m_pMesh, NULL);


	GetDebugOriginSphereBody(m_mapDebugOriginSphereBody, m_mapDebugUpdateSphereBody);
	//몸중심의 전체적 바운딩스피어를 구해낸 다음 그 값을 cSkinnedMesh의 바운딩스피어 멤버들에게 전달해준다 : 민우

	//GetCollisionBoundingSphere(m_stBoundingSphere.m_vCenter, m_stBoundingSphere.m_fRadius);
	//m_stUpdateBoundingSphere.m_vCenter = m_stBoundingSphere.m_vCenter;
	//m_stUpdateBoundingSphere.m_fRadius = m_stBoundingSphere.m_fRadius;
	D3DXCreateSphere(g_pD3DDevice, 1, 10, 10, &m_pDebugSphereBody, NULL);//반지름을 1로 해두고 확장해서 렌더 : 민우
	//SAFE_RELEASE(m_pDebugSphereBody);
	//GetDetailCollisionBoundingSpheres(m_vecDetailBoundingSphere);	//TODO: 여기서 구해진 것을 토대로 Update에서 위치를 지속적으로 갱신해야 한다. :민우
	//D3DXCreateSphere(g_pD3DDevice, 2.5f, 5, 5, &m_pDebugDetailSphereBody, NULL);//세부적인 부분을 보여줄 작은 바운딩스피어
}

//캐릭터 몸의 1차 충돌(피격)체크용 전체적 바운딩스피어를 구한다 : 민우
//void cSkinnedMeshBody::GetCollisionBoundingSphere(OUT D3DXVECTOR3& vCenter, OUT float& fRadius)
//{
//	D3DXFRAME* pFrameCenter;
//	pFrameCenter = D3DXFrameFind(m_pRootFrame, "FxCenter");
//	D3DXMATRIXA16 matCenterTM = pFrameCenter->TransformationMatrix;
//	D3DXVECTOR3	vLocalCenter(0, 0, 0); //바운딩 스피어의 중심점을 구해낼 벡터3
//	D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matCenterTM);
//	
//	//바운딩스피어의 중심점과 반지름을 대입
//	vCenter = vLocalCenter;
//	fRadius = 20.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
//}

//캐릭터 신체 각 부위의 충돌(피격)을 세부적 판정할 바운딩 스피어를 vector컨테이너로 구한다 : 민우
//void cSkinnedMeshBody::GetDetailCollisionBoundingSpheres(OUT std::vector<ST_BOUNDING_SPHERE>& vecSphere)
//{
//	RecursivePushBoundingSphereAllBones(m_pRootFrame, vecSphere);
//}

//인자로 받은 벡터컨테이너에 재귀적으로 모든 본을 찾아서 바운딩스피어값을 Push한다.(vCenter는 로컬좌표다) : 민우
//void cSkinnedMeshBody::RecursivePushBoundingSphereAllBones(D3DXFRAME* pFrame, std::vector<ST_BOUNDING_SPHERE>& vecSphere)
//{
//	if (pFrame)
//	{
//		D3DXMATRIX matTM = pFrame->TransformationMatrix;
//		D3DXVECTOR3	vLocalCenter(0, 0, 0); //바운딩 스피어의 중심점을 구해낼 벡터3
//		D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);
//		ST_BOUNDING_SPHERE stBs(vLocalCenter, 5.f);	//HACK: 반지름을 정하는 규칙이 있어야 한다. : 민우
//		vecSphere.push_back(stBs); //인자로 받은 벡터에 Push. 로컬좌표만 들어가 있는 상태.
//
//		if (pFrame->pFrameSibling){ RecursivePushBoundingSphereAllBones(pFrame->pFrameFirstChild, vecSphere); }
//		if (pFrame->pFrameFirstChild){ RecursivePushBoundingSphereAllBones(pFrame->pFrameFirstChild, vecSphere); }
//	}
//}

//TODO: 전체적인 본의 바운딩스피어를 그린다. : 민우
//void cSkinnedMeshBody::RenderBoundingSphere(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM)
//{
//	//세부 본을 그리다 보면 이미 구현되는 셈이기 때문에 일단 비워둔다.
//}

//FIX: 세부적인 본의 바운딩스피어를 그린다. 단순히 표현만 해보는 것으로, 연산된 값은 이용하지 않는다. Parent가 아니라 Combine이라는 개념으로 수정해야한다. : 민우
//void cSkinnedMeshBody::RenderDetailBoundingSphere(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM)
//{
//	ST_BONE* pBone = (ST_BONE*)pFrame;
//	D3DXMATRIXA16 matW;
//	D3DXMatrixIdentity(&matW);
//	matW = pBone->TransformationMatrix * (*pParentWorldTM);
//	
//	g_pD3DDevice->SetTexture(0, NULL);
//	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
//	m_pDebugDetailSphereBody->DrawSubset(0);
//	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//}
void cSkinnedMeshBody::GetDebugOriginSphereBody(
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody)
{
	D3DXFRAME* pFrame;
	ST_BOUNDING_SPHERE stBs;
	pFrame = D3DXFrameFind(m_pRootFrame, "FxCenter");
	D3DXMATRIXA16 matTM = pFrame->TransformationMatrix;
	D3DXVECTOR3	vLocalCenter(0, 0, 0); //바운딩 스피어의 중심점을 구해낼 벡터3
	D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

	stBs.m_vCenter = vLocalCenter;
	stBs.m_fRadius = 15.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
	mapDebugOriginSphereBody["FxCenter"] = stBs;
	
	pFrame = D3DXFrameFind(m_pRootFrame, "FxTop");
	matTM = pFrame->TransformationMatrix;
	vLocalCenter = D3DXVECTOR3(0, 0, 0);
	D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

	stBs.m_vCenter = vLocalCenter;
	stBs.m_fRadius = 10.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
	mapDebugOriginSphereBody["FxTop"] = stBs;

	pFrame = D3DXFrameFind(m_pRootFrame, "FxBottom");
	matTM = pFrame->TransformationMatrix;
	vLocalCenter = D3DXVECTOR3(0, 0, 0);
	D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

	stBs.m_vCenter = vLocalCenter;
	stBs.m_fRadius = 10.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
	mapDebugOriginSphereBody["FxBottom"] = stBs;

	mapDebugUpdateSphereBody["FxCenter"] = mapDebugOriginSphereBody["FxCenter"];
	mapDebugUpdateSphereBody["FxBottom"] = mapDebugOriginSphereBody["FxBottom"];
	mapDebugUpdateSphereBody["FxTop"] = mapDebugOriginSphereBody["FxTop"];
}
void cSkinnedMeshBody::GetDebugUpdateSphereBody(
	IN ST_BONE* pBone,
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody)
{
	for each(auto it in mapDebugUpdateSphereBody)
	{
		if (pBone->Name == nullptr || pBone->Name != it.first) { continue; }
		
		D3DXVec3TransformCoord(
			&it.second.m_vCenter,
			&mapDebugOriginSphereBody[it.first].m_vCenter,
			&pBone->CombinedTransformationMatrix);
	}
}
void cSkinnedMeshBody::RenderDebugUpdateSphereBody(
	IN ST_BONE* pBone,
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody)
{
	//원래 이 코드가 하는 일을 함수로 다르게 만들었다.
	/*if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxCenter"))
	{
		g_pD3DDevice->SetTexture(0, nullptr);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pD3DDevice->SetTexture(0, NULL);
		m_pDebugSphereBody->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	}*/
	for each(auto it in mapDebugUpdateSphereBody)
	{
		if (pBone->Name == nullptr || pBone->Name != it.first) { continue; }
		
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		
		D3DXMATRIXA16 matW; D3DXMatrixIdentity(&matW);
		//메쉬의 반지름이 1이므로 크기에 맞게 스케일링해서 그린다.
		D3DXMatrixScaling(&matW, it.second.m_fRadius, it.second.m_fRadius, it.second.m_fRadius);
		matW *= pBone->CombinedTransformationMatrix;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
		m_pDebugSphereBody->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}
void cSkinnedMeshBody::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	if (pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);

		// 목부위 바이패드의 월드트랜스폼매트릭스를 받아서 머리 렌더 실시
		if (pCurrent->Name != nullptr && std::string(pCurrent->Name) == std::string("Bip01-Neck"))
		{
			if (m_pHead){
				m_pHead->UpdateAndRender(&pCurrent->CombinedTransformationMatrix);
			}
		}

		// 머리부위 바이패드의 월드트랜스폼매트릭스를 받아서 머리카락 렌더 실시
		else if (pCurrent->Name != nullptr && std::string(pCurrent->Name) == std::string("Bip01-Head"))
		{
			if (m_pHair){
				m_pHair->UpdateAndRender(&pCurrent->CombinedTransformationMatrix);
			}
		}
	}

	//몸의 중심 바운딩스피어를 찾아서 1차 검사용으로 사용할 것이다.
	//if (pCurrent->Name != nullptr && std::string(pCurrent->Name) == std::string("FxCenter"))
	//{
	//	/*D3DXVec3TransformCoord(
	//		&m_mapDebugUpdateSphereBody["FxCenter"].m_vCenter,
	//		&m_mapDebugOriginSphereBody["FxCenter"].m_vCenter,
	//		&pCurrent->CombinedTransformationMatrix);*/
	//	D3DXVec3TransformCoord(
	//		&m_stUpdateBoundingSphere.m_vCenter,
	//		&m_stBoundingSphere.m_vCenter,
	//		&pCurrent->CombinedTransformationMatrix);
	//}

	//FxTop, FxCenter, FxBottom 세 군데의 위치를 갱신한다.
	GetDebugUpdateSphereBody(pCurrent, m_mapDebugOriginSphereBody, m_mapDebugUpdateSphereBody);

	// Hand
	if (pCurrent->Name != nullptr && std::string(pCurrent->Name) == std::string("FxHand01"))
	{
		D3DXVec3TransformCoord(
			&m_stUpdateAttacSphere.m_vCenter,
			&m_stAttacSphere.m_vCenter,
			&pCurrent->CombinedTransformationMatrix);
	}

	if (pCurrent->pFrameSibling)
	{
		Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
	}
}

void cSkinnedMeshBody::Render(ST_BONE* pBone /*= NULL*/)
{
	assert(pBone);
	//FxTop, FxCenter, FxBottom의 바운딩스피어를 그린다 : 민우
	RenderDebugUpdateSphereBody(pBone, m_mapDebugUpdateSphereBody);

	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxHand01"))
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);

			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			m_pEffect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			m_pEffect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				m_pEffect->BeginPass(uiPass);
				pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}
	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		Render((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		Render((ST_BONE*)pBone->pFrameFirstChild);
	}
}

//
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

