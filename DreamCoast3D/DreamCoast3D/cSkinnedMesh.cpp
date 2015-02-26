#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_fAnimationBlendTime(0.3f)
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
	, m_pEffect(NULL)
	, m_fAnimationBlendTime(0.3f)
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
	m_pEffect = pSkinnedMesh->m_pEffect;

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);

	
	//// 이펙트가 일어나는 손
	//D3DXFrameFind(m_pRootFrame, "FxHand01");

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

	//중복이라 잠깐 닫아둔다 : 민우
	//// 몸 중앙
	//D3DXFRAME* pDummyRoot;
	//pDummyRoot = D3DXFrameFind(m_pRootFrame, "FxCenter");
	//if (pDummyRoot){
	//	D3DXMATRIXA16 mat = pDummyRoot->TransformationMatrix;
	//	D3DXVECTOR3 localCenter(0, 0, 0);
	//	D3DXVec3TransformCoord(&localCenter, &localCenter, &mat);
	//	m_stBoundingSphere.m_vCenter = localCenter;
	//	m_stBoundingSphere.m_fRadius = 30.0f;
	//	m_stUpdateBoundingSphere.m_vCenter = m_stBoundingSphere.m_vCenter;
	//	m_stUpdateBoundingSphere.m_fRadius = m_stBoundingSphere.m_fRadius;
	//	SAFE_RELEASE(m_pDebugSphereBody);
	//	D3DXCreateSphere(g_pD3DDevice, m_stBoundingSphere.m_fRadius, 10, 10, &m_pDebugSphereBody, NULL);
	//}

	GetDebugOriginSphereBody(m_mapDebugOriginSphereBody, m_mapDebugUpdateSphereBody);

	if (m_mapDebugOriginSphereBody.find(std::string("FxCenter")) != m_mapDebugOriginSphereBody.end()){
		m_stUpdateBoundingSphere.m_vCenter = D3DXVECTOR3(0,0,0);
		m_stUpdateBoundingSphere.m_fRadius = m_mapDebugOriginSphereBody[std::string("FxCenter")].m_fRadius;

		SAFE_RELEASE(m_pDebugSphereBody);
		D3DXCreateSphere(g_pD3DDevice, m_stUpdateBoundingSphere.m_fRadius, 10, 10, &m_pDebugSphereBody, NULL);
	}
}

cSkinnedMesh::cSkinnedMesh()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_fAnimationBlendTime(0.3f)
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
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

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
	
	if (m_isAnimationBlending)
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
	
	m_pAnimController->AdvanceTime(g_pTimer->DeltaTime() * .5f, NULL);

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

	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxCenter"))
	{
		//if (m_pDebugSphereBody){
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			g_pD3DDevice->SetTexture(0, NULL);
			m_pDebugSphereBody->DrawSubset(0);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}
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
	m_isAnimationBlending = true;
}

void cSkinnedMesh::SetAnimationLoop(DWORD dwIndex, bool isLoop)
{
	if (dwIndex < m_vecAnimationSet.size())
	{
		m_vecAnimationSet[dwIndex]->SetIsLoop(isLoop);
	}
}

void cSkinnedMesh::OnFinishAnimation(cAnimationSet* pSender){
	SetAnimationIndex(pSender->GetPrevAnimation()->GetIndex());
}


void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
	this->Release();
}

void cSkinnedMesh::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

float cSkinnedMesh::GetCurrentAnimationPeriodTime(){
	LPD3DXANIMATIONSET pSet;
	m_pAnimController->GetTrackAnimationSet(0, &pSet);
	float fPeriodTime = pSet->GetPeriod();
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
		matTM = pFrame->TransformationMatrix;
		
		D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

		stBs.m_vCenter = vLocalCenter;
		stBs.m_fRadius = 15.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
		mapDebugOriginSphereBody["FxCenter"] = stBs;
		pFrame = NULL;
	}

	pFrame = D3DXFrameFind(m_pRootFrame, "FxTop");
	
	if (pFrame){
		matTM = pFrame->TransformationMatrix;
		vLocalCenter = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

		stBs.m_vCenter = vLocalCenter;
		stBs.m_fRadius = 10.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
		mapDebugOriginSphereBody["FxTop"] = stBs;
		pFrame = NULL;
	}

	pFrame = D3DXFrameFind(m_pRootFrame, "FxBottom");
	if (pFrame){
		matTM = pFrame->TransformationMatrix;
		vLocalCenter = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);

		stBs.m_vCenter = vLocalCenter;
		stBs.m_fRadius = 10.f; //HACK: 이 값(반지름)을 정하는 규칙이 있어야 한다. : 민우
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
		D3DXMatrixScaling(&matW, it.second.m_fRadius, it.second.m_fRadius, it.second.m_fRadius);
		matW *= pBone->CombinedTransformationMatrix;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
		m_pDebugSphereBody->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}