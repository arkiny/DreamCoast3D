#pragma once

class cASEInstance;

#define g_pAseManager cAseManager::GetInstance()

class cAseManager
{
	SINGLETONE(cAseManager);

private:
	std::map<std::string, cASEInstance*>		m_mapGameObjectASE;

public:
	cASEInstance* GetAseObject(std::string sFolder, std::string sFile);

	void Destroy();
};

