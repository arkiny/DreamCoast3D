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

	
	// Attack�� �ӽ�ó��, HD
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


	//���߽��� ��ü�� �ٿ�����Ǿ ���س� ���� �� ���� cSkinnedMesh�� �ٿ�����Ǿ� ����鿡�� �������ش� : �ο�

	// HD : �����ӿ�ũ �������� �۾��� ������ �ִ� �Ϳ��� ����� �����ϴ� ���°� �ƴ϶�
	// ������ ���� ����ä �߰��� ������ �۾��� �켱���� �ϰ�, ���� �Ŀ� ���� �ڵ带 �����ϴ� �۾��� ���ؾ� �Ѵ�.
	// ���� ���� ������� ������ ���� ����ä ���� ��ӹ޾Ƽ� �۾��ϰ� ġȯ�� �ִ� �۾��� ��ġ���� �ؾ���.

	m_stUpdateBoundingSphere.m_vCenter = m_mapDebugOriginSphereBody[std::string("FxCenter")].m_vCenter;
	m_stUpdateBoundingSphere.m_fRadius = m_mapDebugOriginSphereBody[std::string("FxCenter")].m_fRadius / 2;

	SAFE_RELEASE(m_pDebugSphereBody);
	//�������� 1�� �صΰ� Ȯ���ؼ� ���� : �ο�
	//�����Ҷ� ���� ����� ������ �ȵǴµ�?
	D3DXCreateSphere(g_pD3DDevice, m_stUpdateBoundingSphere.m_fRadius, 10, 10, &m_pDebugSphereBody, NULL);


	
	//GetDetailCollisionBoundingSpheres(m_vecDetailBoundingSphere);	//TODO: ���⼭ ������ ���� ���� Update���� ��ġ�� ���������� �����ؾ� �Ѵ�. :�ο�
	//D3DXCreateSphere(g_pD3DDevice, 2.5f, 5, 5, &m_pDebugDetailSphereBody, NULL);//�������� �κ��� ������ ���� �ٿ�����Ǿ�
}

//ĳ���� ���� 1�� �浹(�ǰ�)üũ�� ��ü�� �ٿ�����Ǿ ���Ѵ� : �ο�
//void cSkinnedMeshBody::GetCollisionBoundingSphere(OUT D3DXVECTOR3& vCenter, OUT float& fRadius)
//{
//	D3DXFRAME* pFrameCenter;
//	pFrameCenter = D3DXFrameFind(m_pRootFrame, "FxCenter");
//	D3DXMATRIXA16 matCenterTM = pFrameCenter->TransformationMatrix;
//	D3DXVECTOR3	vLocalCenter(0, 0, 0); //�ٿ�� ���Ǿ��� �߽����� ���س� ����3
//	D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matCenterTM);
//	
//	//�ٿ�����Ǿ��� �߽����� �������� ����
//	vCenter = vLocalCenter;
//	fRadius = 20.f; //HACK: �� ��(������)�� ���ϴ� ��Ģ�� �־�� �Ѵ�. : �ο�
//}

//ĳ���� ��ü �� ������ �浹(�ǰ�)�� ������ ������ �ٿ�� ���Ǿ vector�����̳ʷ� ���Ѵ� : �ο�
//void cSkinnedMeshBody::GetDetailCollisionBoundingSpheres(OUT std::vector<ST_BOUNDING_SPHERE>& vecSphere)
//{
//	RecursivePushBoundingSphereAllBones(m_pRootFrame, vecSphere);
//}

//���ڷ� ���� ���������̳ʿ� ��������� ��� ���� ã�Ƽ� �ٿ�����Ǿ�� Push�Ѵ�.(vCenter�� ������ǥ��) : �ο�
//void cSkinnedMeshBody::RecursivePushBoundingSphereAllBones(D3DXFRAME* pFrame, std::vector<ST_BOUNDING_SPHERE>& vecSphere)
//{
//	if (pFrame)
//	{
//		D3DXMATRIX matTM = pFrame->TransformationMatrix;
//		D3DXVECTOR3	vLocalCenter(0, 0, 0); //�ٿ�� ���Ǿ��� �߽����� ���س� ����3
//		D3DXVec3TransformCoord(&vLocalCenter, &vLocalCenter, &matTM);
//		ST_BOUNDING_SPHERE stBs(vLocalCenter, 5.f);	//HACK: �������� ���ϴ� ��Ģ�� �־�� �Ѵ�. : �ο�
//		vecSphere.push_back(stBs); //���ڷ� ���� ���Ϳ� Push. ������ǥ�� �� �ִ� ����.
//
//		if (pFrame->pFrameSibling){ RecursivePushBoundingSphereAllBones(pFrame->pFrameFirstChild, vecSphere); }
//		if (pFrame->pFrameFirstChild){ RecursivePushBoundingSphereAllBones(pFrame->pFrameFirstChild, vecSphere); }
//	}
//}

//TODO: ��ü���� ���� �ٿ�����Ǿ �׸���. : �ο�
//void cSkinnedMeshBody::RenderBoundingSphere(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM)
//{
//	//���� ���� �׸��� ���� �̹� �����Ǵ� ���̱� ������ �ϴ� ����д�.
//}

//FIX: �������� ���� �ٿ�����Ǿ �׸���. �ܼ��� ǥ���� �غ��� ������, ����� ���� �̿����� �ʴ´�. Parent�� �ƴ϶� Combine�̶�� �������� �����ؾ��Ѵ�. : �ο�
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

	//���� Ÿ�� ���(�����ؾߵ�)
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
	//���� Ÿ�� ����ġ ����
	else if (m_isAnimationBlending && EANIMBLENDTYPE::EANIMBLENDTYPE_CONTINUE_WEIGHT)
	{
		m_fPassedBlendTime += g_pTimer->DeltaTime();
		//�����Ǿ�� ����ġ�� ��� �Ҹ�Ǹ� �з����� Ʈ���� ��Ȱ��ȭ �Ѵ�.
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
	//���� Ÿ�� ������ ����
	else if (m_isAnimationBlending && EANIMBLENDTYPE::EANIMBLENDTYPE_FREEZE_POSITION)
	{
		//���߿� ��ų�� ������ ����� �����Ǹ� �ҵ� ������ : �ο�
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

	// �Ӹ����� �����е��� ����Ʈ��������Ʈ������ �޾Ƽ� �Ӹ�ī�� ���� �ǽ�
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

	//��������� ��� �����ӿ� ���ؼ� ����.
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
	//FxTop, FxCenter, FxBottom�� �ٿ�����Ǿ �׸��� : �ο�

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

	// �Ӹ����� �����е��� ����Ʈ��������Ʈ������ �޾Ƽ� �Ӹ�ī�� ���� �ǽ�
	else if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Head"))
	{
		if (m_pHair){
			m_pHair->Render(&pBone->CombinedTransformationMatrix);
		}
	}

	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.
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
	//��������� ��� �����ӿ� ���ؼ� ����.
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
