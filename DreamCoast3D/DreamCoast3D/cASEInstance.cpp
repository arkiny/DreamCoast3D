#include "StdAfx.h"
#include "cASEInstance.h"
#include "cMtlTex.h"


cASEInstance::cASEInstance(void)
	:  m_pMesh(NULL)
	, m_pMtlTex(NULL)
	//, m_pSphereMesh(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cASEInstance::~cASEInstance(void)
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMtlTex);
	//SAFE_RELEASE(m_pSphereMesh);
	for each(auto pChild in m_vecChildren)
	{
		pChild->Release();
	}
	//SAFE_RELEASE(m_pSpecularMapping);
}

void cASEInstance::AddChild( cASEInstance* pChild )
{
	pChild->CalcLocalTM(&m_matWorldTM);
	m_vecChildren.push_back(pChild);
}

void cASEInstance::CalcLocalTM( D3DXMATRIXA16* pParent )
{
	D3DXMATRIXA16 matInvParent;
	D3DXMatrixInverse(&matInvParent, 0, pParent);
	m_matLocalTM = m_matWorldTM * matInvParent;
}

void cASEInstance::BuidlMesh( std::vector<ST_PNT2_VERTEX>& vecVertex )
{
	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &m_matWorldTM);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}

	D3DXComputeBoundingBox(&vecVertex[0].p, 
		vecVertex.size(), 
		sizeof(ST_PNT2_VERTEX), 
		&m_stBoundingBox.vMin, 
		&m_stBoundingBox.vMax);

	HRESULT hr = D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT2_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT2_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT2_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		pI[i] = i;
	}
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (size_t i = 0; i < vecVertex.size() / 3; ++i)
	{
		pA[i] = 0;
	}
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuffer(vecVertex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT | 
		D3DXMESHOPT_COMPACT | 
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuffer[0], 0, 0, 0);
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx");
	g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx");
}

void cASEInstance::SetMtlTex( cMtlTex* pMtlTex )
{
	if(!m_pMtlTex)
	{
		pMtlTex->AddRef();
		m_pMtlTex = pMtlTex;
	}
}

void cASEInstance::Update( D3DXMATRIXA16* pmatParent )
{
	//m_matWorldTM = m_matLocalTM;
	//if(pmatParent)
	//	m_matWorldTM *= (*pmatParent);
	//for each(auto pChild in m_vecChildren)
	//{
	//	pChild->Update(&m_matWorldTM);
	//}
}

void cASEInstance::RenderShadow(D3DXMATRIXA16* pMatrix){

	D3DLIGHT9 stLight;
	g_pD3DDevice->GetLight(0, &stLight);
	D3DXVECTOR3 pos = -1000 * stLight.Direction;
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &pos, &vLookatPt, &vUpVec);
	}

	D3DXMATRIXA16 matLightProjection; {
		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
	}

	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	D3DXMATRIXA16 matViewProject; {
		D3DXMatrixMultiply(&matViewProject, &matView, &matProjection);
	}

	g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx")
		->SetMatrix("gWorldMatrix", pMatrix);
	g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx")
		->SetMatrix("gLightViewMatrix", &matLightView);
	g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx")
		->SetMatrix("gLightProjectionMatrix", &matLightProjection);


	if (m_pMtlTex)
	{
		// 쉐이더를 시작한다.
		UINT numPasses = 0;
		g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx")
			->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx")
					->BeginPass(i);
				{
					// 구체를 그린다.
					m_pMesh->DrawSubset(0);
				}
				g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx")
					->EndPass();
			}
		}
		g_pShaderManager->GetShader("../Resources/Shader/CreateShadow.fx")
			->End();
	}
}


void cASEInstance::Render(D3DXMATRIXA16* pMatrix)
{
	D3DLIGHT9 stLight;
	g_pD3DDevice->GetLight(0, &stLight);
	D3DXVECTOR3 pos = -1000 * stLight.Direction;
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &pos, &vLookatPt, &vUpVec);
	}

	D3DXMATRIXA16 matLightProjection; {
		D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000);
	}

	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjection);
	D3DXMATRIXA16 matViewProject; {
		D3DXMatrixMultiply(&matViewProject, &matView, &matProjection);
	}

	// 쉐이더 전역변수들을 설정
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetMatrix("gWorldMatrix", pMatrix);
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetMatrix("gViewMatrix", &matView);
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetMatrix("gProjectionMatrix", &matProjection);
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetVector("gLightColor", &gLightColor);
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetTexture("DiffuseMap_Tex", m_pMtlTex->pTex);
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetTexture("SpecularMap_Tex", m_pMtlTex->pTex);
	g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->SetVector("gWorldLightPosition", &D3DXVECTOR4(pos, 0.0f));

	if (m_pMtlTex)
	{
		// 쉐이더를 시작한다.
		UINT numPasses = 0;
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->BeginPass(i);
				{
					// 구체를 그린다.
					m_pMesh->DrawSubset(0);
				}
				g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->EndPass();
			}
		}
		g_pShaderManager->GetShader("../Resources/Shader/DetailBlending.fx")->End();
	}
	
	for each(auto pChild in m_vecChildren)
	{
		pChild->Render(pMatrix);
	}
}

void cASEInstance::Release(){
	cObject::Release();
}