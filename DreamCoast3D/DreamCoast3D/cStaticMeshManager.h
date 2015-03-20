#pragma once

#define g_pStaticMeshManager cStaticMeshManager::GetInstance()

class cMesh;

class cStaticMeshManager
{
public:
	SINGLETONE(cStaticMeshManager);

private:
	std::map<std::string, LPD3DXMESH>					m_mapStaticMesh;
	std::map<std::string, DWORD>						m_mapStaticMeshMtlNum;
	std::map<std::string, std::vector<D3DMATERIAL9>>	m_mapMtls;
	std::map<std::string, std::string>					m_mapTextureName;

public:
	LPD3DXMESH GetStaticMesh(char* szPath);
	LPD3DXMESH GetStaticMesh(std::string sPath);
	std::string GetStaticMeshTexture(std::string sPath);
	std::string GetStaticMeshTexture(char* szPath);
	void Destroy();
};

