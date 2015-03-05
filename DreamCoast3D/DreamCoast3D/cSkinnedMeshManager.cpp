#include "StdAfx.h"
#include "cSkinnedMeshManager.h"
#include "cSkinnedMesh.h"

cSkinnedMeshManager::cSkinnedMeshManager(void)
{
}


cSkinnedMeshManager::~cSkinnedMeshManager(void)
{
}

void cSkinnedMeshManager::Destroy()
{
	for each(auto it in m_mapSkinnedMesh)
	{
		it.second->Destroy();
		//SAFE_RELEASE(it.second);
	}
	m_mapSkinnedMesh.clear();
}

cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh(char* szFolder, char* szFilename, int nRefIndex)
{
	return GetSkinnedMesh(std::string(szFolder), std::string(szFilename), nRefIndex);
}

cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh(std::string sFolder, std::string sFilename, int nRefIndex){
	std::string sFullPath = sFolder + sFilename;

	if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
		pSkinnedMesh->Load(sFolder, sFilename);
		pSkinnedMesh->SetMeshRefNumber(nRefIndex);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
		m_vecSkinnedMesh4Save.push_back(pSkinnedMesh);
	}
	return m_mapSkinnedMesh[sFullPath];
	
}