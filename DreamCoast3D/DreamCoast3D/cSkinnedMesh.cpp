#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	//, m_pEffect(NULL)
	, m_fAnimationBlendTime(ANIM_BLEND_TIME)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimationBlending(false)
	, m_nCurrentAnimation(0)
	, m_pDebugSphereBody(NULL)
	, m_pDebugDetailSphereBody(nullptr)
	//, m_vPosition(0, 0, 0)
{
	cSkinnedMesh(std::string(szFolder), std::string(szFilename));
}

cSkinnedMesh::cSkinnedMesh(std::string sFolder, std::string sFile)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	//, m_pEffect(NULL)
	, m_fAnimationBlendTime(ANIM_BLEND_TIME)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimationBlending(false)
	, m_nCurrentAnimation(0)
	, m_pDebugSphereBody(NULL)
	, m_pDebugDetailSphereBody(nullptr)
{
	cSkinnedMesh* pSkinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(sFolder, sFile);
	
	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	//m_pEffect = pSkinnedMesh->m_pEffect;
	m_nMeshRefNumber = pSkinnedMesh->GetMeshRefNumber();
	m_sSkinnedFolder = pSkinnedMesh->GetSkinnedFolderPath();
	m_sSkinnedFile = pSkinnedMesh->GetSkinnedFilePath();

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);

	UINT uiNumAnim = m_pAnimController->GetNumAnimationSets();
	for (UINT i = 0; i < uiNumAnim; ++i)
	{
		cAnimationSet* pAnimSet = new cAnimationSet;
		LPD3DXANIMATIONSET pAS = NULL;
		m_pAnimController->GetAnimationSet(i, &pAS);
		pAnimSet->SetAnimationSet(pAS);
		SAFE_RELEASE(pAS);
		pAnimSet->SetIndex(m_vecAnimationSet.size());
		m_vecAnimationSet.push_back(pAnimSet);
	}


	GetDebugOriginSphereBody(m_mapDebugOriginSphereBody, m_mapDebugUpdateSphereBody);

	if (m_mapDebugOriginSphereBody.find(std::string("FxCenter")) != m_mapDebugOriginSphereBody.end()){
		m_stUpdateBoundingSphere.m_vCenter = D3DXVECTOR3(0,0,0);
		m_stUpdateBoundingSphere.m_fRadius = m_mapDebugOriginSphereBody[std::string("FxCenter")].m_fRadius;

		SAFE_RELEASE(m_pDebugSphereBody);
		D3DXCreateSphere(g_pD3DDevice, m_stUpdateBoundingSphere.m_fRadius, 10, 10, &m_pDebugSphereBody, NULL);
	}

	// Attack용 임시처리, HD
	D3DXVECTOR3 localCenter(0, 0, 0);
	D3DXMATRIXA16 mat;
	D3DXFRAME* pFxCenter;
	pFxCenter = D3DXFrameFind(m_pRootFrame, "FxCenter");
	if (pFxCenter){
		mat = pFxCenter->TransformationMatrix;
		D3DXVec3TransformCoord(&localCenter, &localCenter, &mat);
		m_stAttacSphere.m_vCenter = localCenter;
		m_stAttacSphere.m_fRadius = m_mapDebugOriginSphereBody[std::string("FxCenter")].m_fRadius + 2.0f;
	}
	//D3DXCreateSphere(g_pD3DDevice, m_stAttacSphere.m_fRadius, 10, 10, &, NULL);
}

cSkinnedMesh::cSkinnedMesh()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	//, m_pEffect(NULL)
	, m_fAnimationBlendTime(ANIM_BLEND_TIME)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimationBlending(false)
	, m_nCurrentAnimation(0)
	, m_pDebugSphereBody(NULL)
	, m_pDebugDetailSphereBody(nullptr)
{
}

cSkinnedMesh::~cSkinnedMesh(void)
{
	SAFE_RELEASE(m_pAnimController);
	SAFE_RELEASE(m_pDebugSphereBody);
	SAFE_RELEASE(m_pDebugDetailSphereBody);
	for (auto p : m_vecAnimationSet){
		SAFE_RELEASE(p);
	}	
}

void cSkinnedMesh::Load(std::string sFolder, std::string sFile){
	m_sSkinnedFolder = sFolder;
	m_sSkinnedFile = sFile;
	
	g_pShaderManager->GetShader("MultiAnimation.fx");
	//m_pEffect = LoadEffect("MultiAnimation.fx");

	int nPaletteSize = 0;
	g_pShaderManager->GetShader("MultiAnimation.fx")->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	cAllocateHierarchy ah;
	ah.SetDirectory(sFolder);
	ah.SetDefaultPaletteSize(nPaletteSize);

	std::string sFullPath(sFolder);
	sFullPath += std::string(sFile);

	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRootFrame,
		&m_pAnimController);

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (m_pRootFrame)
		SetupBoneMatrixPtrs(m_pRootFrame);
}

void cSkinnedMesh::Load(char* szDirectory, char* szFilename)
{
	Load(std::string(szDirectory), std::string(szFilename));
}

void cSkinnedMesh::UpdateAndRender(D3DXMATRIXA16* pParentWorldTM)
{
	//if (m_pAnimController)
	//{
	//	m_pAnimController->AdvanceTime(g_pTimer->DeltaTime(), NULL);
	//}
	m_vecAnimationSet[m_nCurrentAnimation]->Update(g_pTimer->DeltaTime());
	
	//블렌딩 타입 노멀(수정해야됨)
	if (m_isAnimationBlending && EANIMBLENDTYPE::EANIMBLENDTYPE_NORMAL)
	{
		m_fPassedBlendTime += g_pTimer->DeltaTime();
		if (m_fPassedBlendTime >= m_fAnimationBlendTime)
		{
			m_isAnimationBlending = false;
			
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackWeight(1, 0.0f);
			m_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fAnimationBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}
	//블렌딩 타입 가중치 지속
	else if (m_isAnimationBlending && EANIMBLENDTYPE::EANIMBLENDTYPE_CONTINUE_WEIGHT)
	{
		m_fPassedBlendTime += g_pTimer->DeltaTime();
		//보존되어온 가중치가 모두 소모되면 밀려나는 트랙을 비활성화 한다.
		if (m_fAnimContinueWeight <= 0)
		{
			m_isAnimationBlending = false;
			
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackWeight(1, 0.0f);
			m_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			m_fAnimContinueWeight -= m_fPassedBlendTime / m_fAnimationBlendTime;
			m_pAnimController->SetTrackWeight(0, 1.0f - m_fAnimContinueWeight);
			m_pAnimController->SetTrackWeight(1, m_fAnimContinueWeight);
		}
	}
	//블렌딩 타입 포지션 정지
	else if (m_isAnimationBlending && EANIMBLENDTYPE::EANIMBLENDTYPE_FREEZE_POSITION)
	{
		//나중에 스킬과 공격이 제대로 구현되면 뚝딱 만들자 : 민우
	}
	
	//m_pAnimController->AdvanceTime(g_pTimer->DeltaTime() * .5f, NULL);
	m_pAnimController->AdvanceTime(g_pTimer->DeltaTime() , NULL);

	if (m_pRootFrame)
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity(&mat);
		//D3DXMatrixTranslation(&mat, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		if (pParentWorldTM){
			Update(m_pRootFrame, pParentWorldTM);
		}
		else{
			Update(m_pRootFrame, &mat);
		}
		Render(m_pRootFrame);
	}
}

void cSkinnedMesh::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	//gWorldMatrix
	g_pShaderManager->GetShader("MultiAnimation.fx")
		->SetMatrix("gWorldMatrix", &pCurrent->CombinedTransformationMatrix);

	
	if (pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);
	}

	//FxTop, FxCenter, FxBottom 세 군데의 위치를 갱신한다.
	GetDebugUpdateSphereBody(pCurrent, m_mapDebugOriginSphereBody, m_mapDebugUpdateSphereBody);

	if (pCurrent->Name != nullptr && std::string(pCurrent->Name) == std::string("FxCenter"))
	{
		D3DXVec3TransformCoord(
			&m_stUpdateBoundingSphere.m_vCenter,
			&D3DXVECTOR3(0,0,0),
			&pCurrent->CombinedTransformationMatrix);

		D3DXVec3TransformCoord(
			&m_stUpdateAttacSphere.m_vCenter,
			&D3DXVECTOR3(0,0,0),
			&pCurrent->CombinedTransformationMatrix
			);
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

void cSkinnedMesh::Render(ST_BONE* pBone /*= NULL*/)
{
	assert(pBone);
	
	if (GetAsyncKeyState(VK_TAB)){
		RenderDebugUpdateSphereBody(pBone, m_mapDebugUpdateSphereBody);

		if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxCenter"))
		{
			//if (m_pDebugSphereBody){
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			g_pD3DDevice->SetTexture(0, NULL);
			m_pDebugSphereBody->DrawSubset(0);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

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
			g_pShaderManager->GetShader("MultiAnimation.fx")->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);


			//gWorldViewProjectionMatrix
			g_pShaderManager->GetShader("MultiAnimation.fx")
				->SetMatrix("gWorldViewProjectionMatrix", &matViewProj);
			//gWorldLightPosition
			D3DLIGHT9 stLight;
			g_pD3DDevice->GetLight(0, &stLight);
			D3DXVECTOR3 pos = -1000 * stLight.Direction;
			g_pShaderManager->GetShader("MultiAnimation.fx")
				->SetVector("gWorldLightPosition", &D3DXVECTOR4(pos, 0.0f));
			//gWorldCameraPosition
			g_pShaderManager->GetShader("MultiAnimation.fx")
				->SetVector("gWorldCameraPosition", &D3DXVECTOR4(vEye, 1.0f));

			//DiffuseMap_Tex
			g_pShaderManager->GetShader("MultiAnimation.fx")
				->SetTexture("DiffuseMap_Tex", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

			//SpecularMap_Tex
			g_pShaderManager->GetShader("MultiAnimation.fx")
				->SetTexture("SpecularMap_Tex", pBoneMesh->vecSpecular[pBoneCombos[dwAttrib].AttribId]);

			//NormalMap_Tex
			g_pShaderManager->GetShader("MultiAnimation.fx")
				->SetTexture("NormalMap_Tex", pBoneMesh->vecNormal[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			g_pShaderManager->GetShader("MultiAnimation.fx")->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			g_pShaderManager->GetShader("MultiAnimation.fx")->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			g_pShaderManager->GetShader("MultiAnimation.fx")->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				g_pShaderManager->GetShader("MultiAnimation.fx")->BeginPass(uiPass);
				pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
				g_pShaderManager->GetShader("MultiAnimation.fx")->EndPass();
			}
			g_pShaderManager->GetShader("MultiAnimation.fx")->End();
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

void cSkinnedMesh::RenderShadow(ST_BONE* pBone /*= NULL*/){
	// TODO 멀티애니메이션 스킨드 메쉬에 섞어서 그림자 생성


	//assert(pBone);

	//if (GetAsyncKeyState(VK_TAB)){
	//	RenderDebugUpdateSphereBody(pBone, m_mapDebugUpdateSphereBody);

	//	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxCenter"))
	//	{
	//		//if (m_pDebugSphereBody){
	//		g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
	//		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//		g_pD3DDevice->SetTexture(0, NULL);
	//		m_pDebugSphereBody->DrawSubset(0);
	//		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//	}

	//}
	//// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	//// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	//if (pBone->pMeshContainer)
	//{
	//	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

	//	// get bone combinations
	//	LPD3DXBONECOMBINATION pBoneCombos =
	//		(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

	//	D3DXMATRIXA16 matViewProj, matView, matProj;
	//	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	//	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	//	matViewProj = matView * matProj;

	//	D3DXMATRIXA16 mView, mInvView;
	//	g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
	//	D3DXMatrixInverse(&mInvView, 0, &mView);
	//	D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
	//	D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

	//	// for each palette
	//	for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
	//	{
	//		// set each transform into the palette
	//		for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
	//		{
	//			DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
	//			if (dwMatrixIndex != UINT_MAX)
	//			{
	//				m_pmWorkingPalette[dwPalEntry] =
	//					pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
	//					(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
	//			}
	//		}

	//		
	//	}
	//}

	////재귀적으로 모든 프레임에 대해서 실행.
	//if (pBone->pFrameSibling)
	//{
	//	Render((ST_BONE*)pBone->pFrameSibling);
	//}

	//if (pBone->pFrameFirstChild)
	//{
	//	Render((ST_BONE*)pBone->pFrameFirstChild);
	//}
}

LPD3DXEFFECT cSkinnedMesh::LoadEffect(char* szFilename)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL, NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	g_pD3DDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice,
		szFilename,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}



void cSkinnedMesh::SetupBoneMatrixPtrs(ST_BONE* pBone)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
			// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
			// 찾아서 월드매트릭스를 걸어줘라.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;
				ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
	}
}

//void cSkinnedMesh::SetAnimationIndex(int nIndex)
//{
//	if (!m_pAnimController)
//		return;
//	LPD3DXANIMATIONSET pAnimSet = NULL;
//	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
//	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
//	SAFE_RELEASE(pAnimSet);
//}

void cSkinnedMesh::SetAnimationIndex(DWORD dwIndex)
{
	if (dwIndex >= m_vecAnimationSet.size())
		return;

	cAnimationSet* pCurr = m_vecAnimationSet[m_nCurrentAnimation];
	cAnimationSet* pNext = m_vecAnimationSet[dwIndex];
	
	//이 코드가 같은 애니메이션을 자꾸 중복실행하게 만든다. 상의해봐야 할 부분 : 민우
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
	/*LPD3DXANIMATIONSET pPrev = NULL;
	LPD3DXANIMATIONSET pNext = NULL;

	m_pAnimController->GetAnimationSet(dwIndex, &pNext);
	if (!pNext)
	{
		return;
	}
	m_pAnimController->GetTrackAnimationSet(0, &pPrev);

	D3DXTRACK_DESC desc;
	m_pAnimController->GetTrackDesc(0, &desc);
	m_pAnimController->SetTrackDesc(1, &desc);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	m_pAnimController->SetTrackAnimationSet(0, pNext);
	m_pAnimController->SetTrackAnimationSet(1, pPrev);

	SAFE_RELEASE(pPrev);
	SAFE_RELEASE(pNext);

	m_fPassedBlendTime = 0.0f;
	m_isAnimationBlending = true;*/
	SetAnimationIndexBlendEX(dwIndex, EANIMBLENDTYPE::EANIMBLENDTYPE_CONTINUE_WEIGHT);
}
void cSkinnedMesh::SetAnimationIndexBlendEX(DWORD dwIndex, EANIMBLENDTYPE eAnimBlendType)
{
	m_eAnimBlendType = eAnimBlendType;
	
	LPD3DXANIMATIONSET pPrev = NULL;
	LPD3DXANIMATIONSET pNext = NULL;

	m_pAnimController->GetAnimationSet(dwIndex, &pNext);
	if (!pNext)
	{
		return;
	}
	m_pAnimController->GetTrackAnimationSet(0, &pPrev);
	
	//이전 동작과 새 동작이 같은 동작이면 아무것도 하지 않는다. 이것은 블렌딩을 위한 임시방편이다. : 민우
	if (pNext == pPrev)
	{
		return;
	}
	D3DXTRACK_DESC desc;
	m_pAnimController->GetTrackDesc(0, &desc);
	m_pAnimController->SetTrackDesc(1, &desc);
	
	if (eAnimBlendType == EANIMBLENDTYPE::EANIMBLENDTYPE_NORMAL)
	{
		m_pAnimController->SetTrackWeight(0, 0.0f);
		m_pAnimController->SetTrackWeight(1, 1.0f);
		m_pAnimController->SetTrackEnable(1, true);
		
	}
	//동작이 전환될 때 밀려나는 트랙의 가중치를 보존해서 블렌딩 한다. 같은 패턴의 동작이 빠르게 전환되어도 전혀 어색함이 없다 : 민우
	else if (eAnimBlendType == EANIMBLENDTYPE::EANIMBLENDTYPE_CONTINUE_WEIGHT)
	{
		m_pAnimController->SetTrackWeight(0, 1 - desc.Weight);
		m_pAnimController->SetTrackWeight(1, desc.Weight);
		m_fAnimContinueWeight = desc.Weight;
	}
	//TODO: 미완성
	else if (eAnimBlendType == EANIMBLENDTYPE::EANIMBLENDTYPE_FREEZE_POSITION)
	{
		//애니메이션 Update할 때 계속 이 Position을 유지 시킨다.
		m_dAnimFreezePosition = desc.Position;
		m_pAnimController->SetTrackWeight(0, 0.0f);
		m_pAnimController->SetTrackWeight(1, 1.0f);
	}

	m_pAnimController->SetTrackAnimationSet(0, pNext);
	m_pAnimController->SetTrackAnimationSet(1, pPrev);
	m_pAnimController->SetTrackPosition(0, 0);
	
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
//Heedong.peter.lee
void cSkinnedMesh::OnFinishAnimation(cAnimationSet* pSender){
	SetAnimationIndex(pSender->GetPrevAnimation()->GetIndex());
}


void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	//SAFE_RELEASE(m_pEffect);
	this->Release();
}

//아직 쓰이지 않음
//void cSkinnedMesh::SetRandomTrackPosition()
//{
//	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
//}

double cSkinnedMesh::GetCurrentAnimationPeriodTime(){
	LPD3DXANIMATIONSET pSet;
	m_pAnimController->GetTrackAnimationSet(0, &pSet);
	double fPeriodTime = pSet->GetPeriod();
	return fPeriodTime;
}


void cSkinnedMesh::GetDebugOriginSphereBody(
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody)
{
	D3DXFRAME* pFrame;
	ST_BOUNDING_SPHERE stBs;
	
	pFrame = D3DXFrameFind(m_pRootFrame, "FxCenter");
	D3DXMATRIXA16 matTM;
	D3DXVECTOR3	vLocalCenter(0, 0, 0); //바운딩 스피어의 중심점을 구해낼 벡터3
	if (pFrame){
		//matTM = pFrame->TransformationMatrix;
		//D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

		stBs.m_vCenter = vLocalCenter;
		stBs.m_fRadius = 12.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
		mapDebugOriginSphereBody["FxCenter"] = stBs;
		pFrame = NULL;
	}

	pFrame = D3DXFrameFind(m_pRootFrame, "FxTop");
	
	if (pFrame){
		//matTM = pFrame->TransformationMatrix;
		vLocalCenter = D3DXVECTOR3(0, 0, 0);
		//D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

		stBs.m_vCenter = vLocalCenter;
		stBs.m_fRadius = 12.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
		mapDebugOriginSphereBody["FxTop"] = stBs;
		pFrame = NULL;
	}

	pFrame = D3DXFrameFind(m_pRootFrame, "FxBottom");
	if (pFrame){
		//matTM = pFrame->TransformationMatrix;
		vLocalCenter = D3DXVECTOR3(0, 0, 0);
		//D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

		stBs.m_vCenter = vLocalCenter;
		stBs.m_fRadius = 12.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
		mapDebugOriginSphereBody["FxBottom"] = stBs;
	}

	mapDebugUpdateSphereBody["FxCenter"] = mapDebugOriginSphereBody["FxCenter"];
	mapDebugUpdateSphereBody["FxBottom"] = mapDebugOriginSphereBody["FxBottom"];
	mapDebugUpdateSphereBody["FxTop"] = mapDebugOriginSphereBody["FxTop"];
}

void cSkinnedMesh::GetDebugUpdateSphereBody(
	IN ST_BONE* pBone,
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugOriginSphereBody,
	OUT std::map<std::string, ST_BOUNDING_SPHERE>& mapDebugUpdateSphereBody)
{
	for each(auto it in mapDebugUpdateSphereBody)
	{
		if (pBone->Name == nullptr || pBone->Name != it.first) { continue; }

 		D3DXVec3TransformCoord(
			&mapDebugUpdateSphereBody[it.first].m_vCenter,
			&D3DXVECTOR3(0, 0, 0),
			&pBone->CombinedTransformationMatrix);
	}
}

void cSkinnedMesh::RenderDebugUpdateSphereBody(
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
		//D3DXMatrixScaling(&matW, it.second.m_fRadius, it.second.m_fRadius, it.second.m_fRadius);
		//matW *= pBone->CombinedTransformationMatrix;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
		m_pDebugSphereBody->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}