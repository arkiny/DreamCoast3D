#include "StdAfx.h"
#include "cGameObjectASE.h"
#include "cMtlTex.h"


cGameObjectASE::cGameObjectASE(void)
	: cGameObject()
	, m_pMesh(NULL)
	, m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cGameObjectASE::~cGameObjectASE(void)
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMtlTex);
	for each(auto pChild in m_vecChildren)
	{
		pChild->Release();
	}
}

void cGameObjectASE::AddChild( cGameObjectASE* pChild )
{
	pChild->CalcLocalTM(&m_matWorldTM);
	m_vecChildren.push_back(pChild);
}

void cGameObjectASE::CalcLocalTM( D3DXMATRIXA16* pParent )
{
	D3DXMATRIXA16 matInvParent;
	D3DXMatrixInverse(&matInvParent, 0, pParent);
	m_matLocalTM = m_matWorldTM * matInvParent;
}

void cGameObjectASE::BuidlMesh( std::vector<ST_PNT_VERTEX>& vecVertex )
{
	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &m_matWorldTM);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}

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

void cGameObjectASE::SetMtlTex( cMtlTex* pMtlTex )
{
	if(!m_pMtlTex)
	{
		pMtlTex->AddRef();
		m_pMtlTex = pMtlTex;
	}
}

void cGameObjectASE::Update( D3DXMATRIXA16* pmatParent )
{
	m_matWorldTM = m_matLocalTM;
	if(pmatParent)
		m_matWorldTM *= (*pmatParent);
	for each(auto pChild in m_vecChildren)
	{
		pChild->Update(&m_matWorldTM);
	}
}

void cGameObjectASE::Render()
{
	if(m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
		g_pD3DDevice->SetTexture(0, m_pMtlTex->pTex);
		g_pD3DDevice->SetMaterial(&m_pMtlTex->stMtl);
		m_pMesh->DrawSubset(0);
	}
	
	for each(auto pChild in m_vecChildren)
	{
		pChild->Render();
	}
}

void cGameObjectASE::Release()
{
	cGameObject::Release();
}
