#pragma once

#define g_pSkinnedMeshManager cSkinnedMeshManager::GetInstance()

class cSkinnedMesh;

class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*>	m_mapSkinnedMesh;
	std::vector<cSkinnedMesh*>				m_vecSkinnedMesh4Save;

public:
	SINGLETONE(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(char* szFolder, char* szFilename, int nRefIndex);
	cSkinnedMesh* GetSkinnedMesh(std::string sFolder, std::string sFilename, int nRefIndex);

	void Destroy();
};

