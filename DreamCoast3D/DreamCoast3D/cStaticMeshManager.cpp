#include "stdafx.h"
#include "cStaticMeshManager.h"
#include "cMesh.h"

cStaticMeshManager::cStaticMeshManager()
{
}


cStaticMeshManager::~cStaticMeshManager()
{
}
//

LPD3DXMESH cStaticMeshManager::GetStaticMesh(char* szPath){
	return GetStaticMesh(std::string(szPath));
}

LPD3DXMESH cStaticMeshManager::GetStaticMesh(std::string sPath){
	LPD3DXMESH ret = NULL;
	DWORD matNum;
	if (m_mapStaticMesh.find(sPath) == m_mapStaticMesh.end())
	{
		LPD3DXBUFFER pMaterialBuffer;
		HRESULT hr = D3DXLoadMeshFromX(
			sPath.c_str(),
			D3DXMESH_MANAGED,
			g_pD3DDevice, NULL,
			&pMaterialBuffer, NULL,
			&matNum, &ret);

		assert(hr == S_OK);
		if (m_mapStaticMesh[sPath] != NULL)
			SAFE_RELEASE(m_mapStaticMesh[sPath]);

		//D3DMATERIAL9* pMeshMaterials = new D3DMATERIAL9[matNum];

		//std::vector<D3DMATERIAL9> vecMeshMaterials(matNum);
		//D3DXMATERIAL* matMaterials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();
		//
		//for (DWORD i = 0; i < matNum; i++)
		//{
		//	//Copy the material
		//	vecMeshMaterials[i] = matMaterials[i].MatD3D;
		//	//Set the ambient color for the material (D3DX does not do this)
		//	vecMeshMaterials[i].Ambient = vecMeshMaterials[i].Diffuse;
		//	//Create the texture
		//	g_pTextureManager->GetTexture(matMaterials[i].pTextureFilename);
		//}

		//m_mapMtls[sPath] = vecMeshMaterials;
		m_mapStaticMesh[sPath] = ret;
		m_mapStaticMeshMtlNum[sPath] = matNum;
		//
		//delete[] matMaterials;
		//pMaterialBuffer->Release();
	}
	return m_mapStaticMesh[sPath];
}

void cStaticMeshManager::Destroy(){
	for (auto p : m_mapStaticMesh){
		p.second->Release();
	}
}