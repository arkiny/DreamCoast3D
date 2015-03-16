#include "stdafx.h"
#include "cMesh.h"


cMesh::cMesh()
{
}


cMesh::~cMesh()
{
	SAFE_DELETE_ARRAY(m_pMeshMaterials);

	if (m_ppMeshTextures)
	{

		for (DWORD i = 0; i < m_dwMaterial; i++)
		{
			SAFE_RELEASE(m_ppMeshTextures[i]);
		}

	}
	SAFE_RELEASE(m_pTrTextures);
	SAFE_DELETE_ARRAY(m_ppMeshTextures);
	SAFE_RELEASE(m_pMesh);
}

void cMesh::LoadFromFile(std::string sPath){
	LPD3DXBUFFER pMaterialBuffer;
	HRESULT hr = D3DXLoadMeshFromX(
		sPath.c_str(), 
		D3DXMESH_MANAGED,
		g_pD3DDevice, NULL, 
		&pMaterialBuffer, NULL, 
		&m_dwMaterial, &m_pMesh);

	assert(hr == S_OK);

	// Store material and texture information 
	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();

	m_pMeshMaterials = new D3DMATERIAL9[m_dwMaterial];
	m_ppMeshTextures = new LPDIRECT3DTEXTURE9[m_dwMaterial];

	// Copy the materials and textures from the buffer to the member arrays 
	for (DWORD i = 0; i < m_dwMaterial; i++)
	{

		m_pMeshMaterials[i] = pMaterials[i].MatD3D;
		m_pMeshMaterials[i].Ambient = D3DXCOLOR(1.0, 1.0, 1.0, 0.0);
		//m_pMeshMaterials[i].Emissive = D3DXCOLOR(.7, .2, .2, 0.0);
		// Create the texture if it exists 
		m_ppMeshTextures[i] = NULL;
		if (pMaterials[i].pTextureFilename)
		{
			m_ppMeshTextures[i]
				= g_pTextureManager->GetTexture(pMaterials[i].pTextureFilename);
			m_ppMeshTextures[i]->AddRef();
		}
	}
	// Don¡¯t need this no more! 
	pMaterialBuffer->Release();
}

void cMesh::Render(D3DXMATRIXA16* pmat){
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
	
	if (g_pD3DDevice && m_pMesh)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, pmat);
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		DWORD numMaterials = m_dwMaterial;
		for (DWORD i = 0; i < numMaterials; i++)
		{
			g_pD3DDevice->SetMaterial(GetMeshMaterial(i));
			g_pD3DDevice->SetTexture(0, GetMeshTexture(i));
			m_pMesh->DrawSubset(i);
		}
	}
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
