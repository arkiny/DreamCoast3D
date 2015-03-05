#pragma once

class cASEInstance;

#define g_pAseManager cAseManager::GetInstance()

class cAseManager
{
	SINGLETONE(cAseManager);

private:
	std::map<std::string, cASEInstance*>		m_mapGameObjectASE;
	std::vector<cASEInstance*>					m_vecGameObjectASE4Save;

	std::vector<std::string>					m_vecFolderSave;
	std::vector<std::string>					m_vecFileSave;

public:
	cASEInstance* GetAseObject(std::string sFolder, std::string sFile);
	std::string GetAseMaterialListAsString();
	void Destroy();
};

