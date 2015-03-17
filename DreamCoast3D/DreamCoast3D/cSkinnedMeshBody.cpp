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
	g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx");
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


	m_sMainCollisionSphere = "FxCenter";
	m_fMianColisionSphereRadius = 6.f;


	//몸중심의 전체적 바운딩스피어를 구해낸 다음 그 값을 cSkinnedMesh의 바운딩스피어 멤버들에게 전달해준다 : 민우

	// HD : 프레임워크 위에서의 작업은 기존에 있는 것에서 지우고 수정하는 형태가 아니라
	// 기존의 것을 놔둔채 추가해 나가는 작업을 우선으로 하고, 리뷰 후에 예전 코드를 삭제하는 작업을 행해야 한다.
	// 차라리 새로 만들려면 기존에 것을 놔둔채 새로 상속받아서 작업하고 치환해 주는 작업을 거치던가 해야함.

	m_stUpdateBoundingSphere.m_vCenter = m_mapDebugOriginSphereBody[std::string("FxCenter")].m_vCenter;
	m_stUpdateBoundingSphere.m_fRadius = m_mapDebugOriginSphereBody[std::string("FxCenter")].m_fRadius / 2;

	SAFE_RELEASE(m_pDebugSphereBody);
	//반지름을 1로 해두고 확장해서 렌더 : 민우
	//렌더할때 월드 사이즈가 적용이 안되는득?
	D3DXCreateSphere(g_pD3DDevice, m_stUpdateBoundingSphere.m_fRadius, 10, 10, &m_pDebugSphereBody, NULL);


	
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

void cSkinnedMeshBody::UpdateAndRenderShadow(D3DXMATRIXA16* pParentWorldTM/* = NULL*/){
	//g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
	//	->SetMatrix("gWorldMatrix", pParentWorldTM);
	g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
		->SetMatrix("gWorldMatrix", pParentWorldTM);

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
	m_pAnimController->AdvanceTime(g_pTimer->DeltaTime(), NULL);

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
		RenderShadow(m_pRootFrame);
	}
}

void cSkinnedMeshBody::RenderShadow(ST_BONE* pBone/* = NULL*/){
	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Neck"))
	{
		if (m_pHead){
			m_pHead->UpdateAndRenderShadow(&pBone->CombinedTransformationMatrix);
		}
	}

	// 머리부위 바이패드의 월드트랜스폼매트릭스를 받아서 머리카락 렌더 실시
	else if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Head"))
	{
		if (m_pHair){
			m_pHair->UpdateAndRenderShadow(&pBone->CombinedTransformationMatrix);
		}
	}

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

		//gWorldViewProjectionMatrix
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")
			->SetMatrix("gWorldViewProjectionMatrix", &matViewProj);
		//gWorldLightPosition
		D3DLIGHT9 stLight;
		g_pD3DDevice->GetLight(0, &stLight);
		D3DXVECTOR3 pos = 500 * stLight.Direction;
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")
			->SetVector("gWorldLightPosition", &D3DXVECTOR4(pos, 0.0f));
		//gWorldCameraPosition
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")
			->SetVector("gWorldCameraPosition", &D3DXVECTOR4(vEye, 1.0f));

		// set the current number of bones; this tells the effect which shader to use
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")
			->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

		g_pD3DDevice->GetLight(0, &stLight);
		D3DXVECTOR3 dir = stLight.Direction;
		D3DXVec3Normalize(&pos, &dir);
		pos = -500 * pos;

		D3DXMATRIXA16 matLightView;
		{
			D3DXVECTOR3 vLookatPt(128.0f, 0.0f, 128.0f);
			D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
			D3DXMatrixLookAtLH(&matLightView, &pos, &vLookatPt, &vUpVec);
		}

		D3DXMATRIXA16 matLightProjection; {
			//D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
			D3DXMatrixOrthoLH(&matLightProjection, 350, 350, 1, 3000);
		}

		//D3DXMATRIXA16 matView;
		D3DXMATRIXA16 matProjection;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
		D3DXMATRIXA16 matViewProject; {
			D3DXMatrixMultiply(&matViewProject, &matView, &matProjection);
		}

		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")
			->SetMatrix("gLightViewMatrix", &matLightView);
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")
			->SetMatrix("gLightProjectionMatrix", &matLightProjection);
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")
			->SetMatrix("gViewProjectionMatrix", &matViewProject);

		// set the technique we use to draw
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")->SetTechnique("CreateShadowShader");

		UINT uiPasses, uiPass;

		// run through each pass and draw
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")->Begin(&uiPasses, 0);
		for (uiPass = 0; uiPass < uiPasses; ++uiPass)
		{
			g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")->BeginPass(uiPass);

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
				g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")->SetMatrixArray("amPalette",
					m_pmWorkingPalette,
					pBoneMesh->dwNumPaletteEntries);
				g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")->CommitChanges();
				pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);

			}
			g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")->EndPass();
		}
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimationShadow.fx")->End();
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		RenderShadow((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		RenderShadow((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMeshBody::Render(D3DXMATRIXA16* pParentWorldTM /*= NULL*/){
	if (m_pRootFrame)
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity(&mat);
		if (pParentWorldTM){
			Update(m_pRootFrame, pParentWorldTM);
		}
		else{
			Update(m_pRootFrame, &mat);
		}
		Render(m_pRootFrame);
	}
}

void cSkinnedMeshBody::Render(ST_BONE* pBone /*= NULL*/)
{
	assert(pBone);
	//FxTop, FxCenter, FxBottom의 바운딩스피어를 그린다 : 민우

	if (GetAsyncKeyState(VK_TAB)){

		RenderDebugUpdateSphereBody(pBone, m_mapDebugUpdateSphereBody);
		
		if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("FxHand01"))
		{
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pMesh->DrawSubset(0);
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Neck"))
	{
		if (m_pHead){
			m_pHead->Render(&pBone->CombinedTransformationMatrix);
		}
	}

	// 머리부위 바이패드의 월드트랜스폼매트릭스를 받아서 머리카락 렌더 실시
	else if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Head"))
	{
		if (m_pHair){
			m_pHair->Render(&pBone->CombinedTransformationMatrix);
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





		//gWorldViewProjectionMatrix
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
			->SetMatrix("gWorldViewProjectionMatrix", &matViewProj);
		//gWorldLightPosition
		D3DLIGHT9 stLight;
		g_pD3DDevice->GetLight(0, &stLight);
		D3DXVECTOR3 pos = 500 * stLight.Direction;
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
			->SetVector("gWorldLightPosition", &D3DXVECTOR4(pos, 0.0f));
		//gWorldCameraPosition
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
			->SetVector("gWorldCameraPosition", &D3DXVECTOR4(vEye, 1.0f));

		// set the current number of bones; this tells the effect which shader to use
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

		g_pD3DDevice->GetLight(0, &stLight);
		D3DXVECTOR3 dir = stLight.Direction;
		D3DXVec3Normalize(&pos, &dir);
		pos = -500 * pos;

		D3DXMATRIXA16 matLightView;
		{
			D3DXVECTOR3 vLookatPt(128.0f, 0.0f, 128.0f);
			D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
			D3DXMatrixLookAtLH(&matLightView, &pos, &vLookatPt, &vUpVec);
		}

		D3DXMATRIXA16 matLightProjection; {
			//D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
			D3DXMatrixOrthoLH(&matLightProjection, 350, 350, 1, 3000);
		}

		//D3DXMATRIXA16 matView;
		D3DXMATRIXA16 matProjection;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
		D3DXMATRIXA16 matViewProject; {
			D3DXMatrixMultiply(&matViewProject, &matView, &matProjection);
		}

		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
			->SetMatrix("gLightViewMatrix", &matLightView);
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
			->SetMatrix("gLightProjectionMatrix", &matLightProjection);
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
			->SetMatrix("gViewProjectionMatrix", &matViewProject);
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
			->SetTexture("ShadowMap_Tex", g_pShaderManager->GetShadowRenderTarget());

		// set the technique we use to draw
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->SetTechnique("Skinning20");

		UINT uiPasses, uiPass;

		// run through each pass and draw
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->Begin(&uiPasses, 0);
		for (uiPass = 0; uiPass < uiPasses; ++uiPass)
		{
			g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->BeginPass(uiPass);

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
				g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->SetMatrixArray("amPalette",
					m_pmWorkingPalette,
					pBoneMesh->dwNumPaletteEntries);

				//DiffuseMap_Tex
				g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
					->SetTexture("DiffuseMap_Tex", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

				//SpecularMap_Tex
				g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
					->SetTexture("SpecularMap_Tex", pBoneMesh->vecSpecular[pBoneCombos[dwAttrib].AttribId]);

				//NormalMap_Tex
				g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")
					->SetTexture("NormalMap_Tex", pBoneMesh->vecNormal[pBoneCombos[dwAttrib].AttribId]);

				g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->CommitChanges();
				pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);

			}
			g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->EndPass();
		}
		g_pShaderManager->GetShader("../Resources/Shader/MultiAnimation.fx")->End();
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

int cSkinnedMeshBody::GetHeadRefNum(){
	return m_pHead->GetMeshRefNumber();
}
int cSkinnedMeshBody::GetHairRefNum(){
	return m_pHair->GetMeshRefNumber();
}
