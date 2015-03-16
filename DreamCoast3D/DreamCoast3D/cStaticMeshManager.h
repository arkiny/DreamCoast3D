#pragma once

#define g_pStaticMeshManager cStaticMeshManager::GetInstance()

class cMesh;

class cStaticMeshManager
{
public:
	SINGLETONE(cStaticMeshManager);

private:
	std::map<std::string, cMesh*>	m_mapStaticMesh;

public:
	cMesh* GetStaticMesh(char* szPath);
	cMesh* GetStaticMesh(std::string sPath);
	void Destroy();
};

