#pragma once

#define g_pSkinnedMeshManager cSkinnedMeshManager::GetInstance()

//class cSkinnedMesh;

// TODO : After do cSkinnedMesh
class cSkinnedMeshManager
{
	SINGLETONE(cSkinnedMeshManager);

private:
	std::map<std::string, D3DXFRAME*>					m_mapSkinnedMeshes;
	std::map<std::string, LPD3DXANIMATIONCONTROLLER>	m_mapAnimationControl;
	
	void SetupBoneMatrixPtrs(D3DXFRAME* pFrame, D3DXFRAME* pFrameRoot);

public:
	D3DXFRAME* GetSkinnedMesh(std::string sFolder, std::string sFile);
	D3DXFRAME* GetSkinnedMesh(char* szFolder, char* szFile);

	D3DXFRAME* GetSkinnedMesh(std::string sFolder, std::string sFile, LPD3DXANIMATIONCONTROLLER *pAnimCon);
	D3DXFRAME* GetSkinnedMesh(char* szFolder, char* szFile, LPD3DXANIMATIONCONTROLLER *pAnimCon);

	void Destroy();
};

