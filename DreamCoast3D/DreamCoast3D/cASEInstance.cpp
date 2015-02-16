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

void cASEInstance::BuidlMesh( std::vector<ST_PNT_VERTEX>& vecVertex )
{
	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &m_matWorldTM);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}

	D3DXComputeBoundingBox(&vecVertex[0].p, vecVertex.size(), sizeof(ST_PNT_VERTEX), &m_stBoundingBox.vMin, &m_stBoundingBox.vMax);

	HRESULT hr = D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
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

void cASEInstance::Render(D3DXMATRIXA16* pMatrix)
{
	if(m_pMtlTex)
	{
		g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, pMatrix);
		g_pD3DDevice->SetTexture(0, m_pMtlTex->pTex);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->stMtl);
		m_pMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
	}

	/*if (m_pSphereMesh){
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		D3DXMATRIXA16 matTrans;
		D3DXVECTOR3 pos = cGameObject::GetPosition();
		D3DXMatrixTranslation(&matTrans,
			0,
			GetBoundingSphere()->m_vCenter.y,
			0
			);

		matTrans = matTrans * *GetTransformMatrix();

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matTrans);
		g_pD3DDevice->SetTexture(0, NULL);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->stMtl);
		m_pSphereMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}*/
	
	for each(auto pChild in m_vecChildren)
	{
		pChild->Render(pMatrix);
	}
}

void cASEInstance::Release(){
	cObject::Release();
}