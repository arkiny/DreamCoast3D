#include "stdafx.h"
#include "cStaticMeshManager.h"


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
	if (m_mapStaticMesh.find(sPath) == m_mapStaticMesh.end())
	{
		if (m_mapStaticMesh[sPath] != NULL)
			SAFE_RELEASE(m_mapStaticMesh[sPath]);
		LPD3DXMESH ret = NULL;
		HRESULT hr;

		//D3DXLoadMeshFromXA(
		//	LPCSTR pFilename,
		//	DWORD Options,
		//	LPDIRECT3DDEVICE9 pD3DDevice,
		//	LPD3DXBUFFER *ppAdjacency,
		//	LPD3DXBUFFER *ppMaterials,
		//	LPD3DXBUFFER *ppEffectInstances,
		//	DWORD *pNumMaterials,
		//	LPD3DXMESH *ppMesh);
		//LPD3DXBUFFER material;

		//D3DXMATERIAL mat;
		hr = D3DXLoadMeshFromX(sPath.c_str(), D3DXMESH_SYSTEMMEM, g_pD3DDevice, NULL, NULL, NULL, NULL, &ret);

		//DWORD matSize = material->GetBufferSize();
		////std::vector<D3DXMATERIAL> mtls;
		////D3DXMATERIAL* mtls;
		//LPD3DXMATERIAL mtl;
		//memcpy(&mtl, material->GetBufferPointer(), material->GetBufferSize());

		assert(hr == S_OK);
		m_mapStaticMesh[sPath] = ret;
	}
	return m_mapStaticMesh[sPath];
}


void cStaticMeshManager::Destroy(){
	for (auto p : m_mapStaticMesh){
		p.second->Release();
	}
}