#include "stdafx.h"
#include "cAseManager.h"
#include "cAseLoader.h"
#include "cASEInstance.h"

cAseManager::cAseManager()
{
}


cAseManager::~cAseManager()
{
}

cASEInstance* cAseManager::GetAseObject(std::string sFolder, std::string sFile){
	if (m_mapGameObjectASE.find(sFolder + sFile) == m_mapGameObjectASE.end())
	{
		cAseLoader cAL;
		cASEInstance* add = cAL.Load(sFolder, sFile);
		add->SetASERefNumber(m_vecFolderSave.size());
		m_mapGameObjectASE[sFolder + sFile] = add;
		m_vecGameObjectASE4Save.push_back(add);
		m_vecFolderSave.push_back(sFolder);
		m_vecFileSave.push_back(sFile);
	}

	return m_mapGameObjectASE[sFolder + sFile];
}

void cAseManager::Destroy(){
	for (auto p : m_mapGameObjectASE){
		SAFE_RELEASE(p.second);
	}
	m_mapGameObjectASE.clear();
}

std::string cAseManager::GetAseMaterialListAsString(){
	std::stringstream ss;
	ss << "*MAP_OBJECT_MATERIAL_LIST {" << std::endl;
	ss << "*MAP_OBJECT_MATERIAL_COUNT " << m_vecGameObjectASE4Save.size() << std::endl;
	for (size_t i = 0; i < m_vecGameObjectASE4Save.size(); i++){
		ss << "*MAP_OBJECT_MATERIAL " << i << " {" << std::endl;
		ss << "*MAP_OBJECT_MATERIAL_PATH \"" << m_vecFolderSave[i] << "\"" << std::endl;
		ss << "*MAP_OBJECT_MATERIAL_FILE \"" << m_vecFileSave[i] << "\"" << std::endl;
		ss << "}" << std::endl;
	}
	ss << "}" << std::endl;
	return ss.str();
}