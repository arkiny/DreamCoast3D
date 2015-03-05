#pragma once

#define g_pSkinnedMeshManager cSkinnedMeshManager::GetInstance()

class cSkinnedMesh;

class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*>	m_mapSkinnedMesh;
	std::vector<cSkinnedMesh*>				m_vecSkinnedMesh4Save;
	//int m_nRefNum = 0;
public:
	SINGLETONE(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(char* szFolder, char* szFilename);
	cSkinnedMesh* GetSkinnedMesh(std::string sFolder, std::string sFilename);

	std::string GetSkinnedMeshListAsString();
	std::string GetSkinnedMeshAsString();

	void Destroy();
};

