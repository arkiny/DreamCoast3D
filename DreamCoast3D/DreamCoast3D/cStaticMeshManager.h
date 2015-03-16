#pragma once

#define g_pStaticMeshManager cStaticMeshManager::GetInstance()

class cStaticMeshManager
{
public:
	SINGLETONE(cStaticMeshManager);

private:
	std::map<std::string, LPD3DXMESH>	m_mapStaticMesh;

public:
	LPD3DXMESH GetStaticMesh(char* szPath);
	LPD3DXMESH GetStaticMesh(std::string sPath);
	void Destroy();
};

