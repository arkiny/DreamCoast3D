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

cMesh* cStaticMeshManager::GetStaticMesh(char* szPath){
	return GetStaticMesh(std::string(szPath));
}

cMesh* cStaticMeshManager::GetStaticMesh(std::string sPath){
	cMesh* ret = NULL;
	if (m_mapStaticMesh.find(sPath) == m_mapStaticMesh.end())
	{
		if (m_mapStaticMesh[sPath] != NULL)
			SAFE_RELEASE(m_mapStaticMesh[sPath]);
		cMesh* ret = new cMesh;

		ret->LoadFromFile(sPath);	
		
		m_mapStaticMesh[sPath] = ret;
	}
	return m_mapStaticMesh[sPath];
}


void cStaticMeshManager::Destroy(){
	for (auto p : m_mapStaticMesh){
		p.second->Release();
	}
}