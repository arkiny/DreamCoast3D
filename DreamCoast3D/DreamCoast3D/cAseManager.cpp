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
		m_mapGameObjectASE[sFolder + sFile] = cAL.Load(sFolder, sFile);
	}

	return m_mapGameObjectASE[sFolder + sFile];
}

void cAseManager::Destroy(){
	for (auto p : m_mapGameObjectASE){
		SAFE_RELEASE(p.second);
	}
	m_mapGameObjectASE.clear();
}