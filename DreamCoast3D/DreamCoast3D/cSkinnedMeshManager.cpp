#include "StdAfx.h"
#include "cSkinnedMeshManager.h"
#include "cSkinnedMesh.h"

cSkinnedMeshManager::cSkinnedMeshManager(void)
{
}


cSkinnedMeshManager::~cSkinnedMeshManager(void)
{
}

void cSkinnedMeshManager::Destroy()
{
	for each(auto it in m_mapSkinnedMesh)
	{
		it.second->Destroy();
		//SAFE_RELEASE(it.second);
	}
	m_vecSkinnedMesh4Save.clear();
	m_mapSkinnedMesh.clear();
}

cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh(char* szFolder, char* szFilename)
{
	return GetSkinnedMesh(std::string(szFolder), std::string(szFilename));
}

cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh(std::string sFolder, std::string sFilename){
	std::string sFullPath = sFolder + sFilename;

	if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
		pSkinnedMesh->Load(sFolder, sFilename);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
		pSkinnedMesh->SetMeshRefNumber(m_vecSkinnedMesh4Save.size());
		m_vecSkinnedMesh4Save.push_back(pSkinnedMesh);
	}
	return m_mapSkinnedMesh[sFullPath];
}

std::string cSkinnedMeshManager::GetSkinnedMeshListAsString(){
	std::stringstream ss;
	ss << "*SKINNEDMESH_LIST {" << std::endl;
	ss << "*SKINNEDMESH_LIST_COUNT " << m_vecSkinnedMesh4Save.size() << std::endl;
	for (size_t i = 0; i < m_vecSkinnedMesh4Save.size(); i++){
		ss << "*SKINNEDMESH " << i << " {" << std::endl;
		ss << "*SKINNEDMESH_PATH " << "\"" << m_vecSkinnedMesh4Save[i]->GetSkinnedFolderPath() << "\"" << std::endl;
		ss << "*SKINNEDMESH_FILE " << "\"" << m_vecSkinnedMesh4Save[i]->GetSkinnedFilePath() << "\"" << std::endl;

		std::vector<std::string>	vecSave = m_vecSkinnedMesh4Save[i]->GetCollisionSphereFrameNames();
		std::vector<float>			vecRad = m_vecSkinnedMesh4Save[i]->GetCollisionSphereRadius();
		if (!vecSave.empty()){
			ss << "*SKINNEDMESH_CS_LIST {" << std::endl;

			for (size_t j = 0; j < vecSave.size(); j++){
				ss << "*SKINNEDMESH_CS " << "\"" << vecSave[j] << "\"" << "\t" << vecRad[j] << std::endl;
			}

			ss << "}" << std::endl;
		}

		vecSave = m_vecSkinnedMesh4Save[i]->GetAttackSphereFrameNames();
		vecRad = m_vecSkinnedMesh4Save[i]->GetAttackSphereRadius();

		if (!vecSave.empty()){
			ss << "*SKINNEDMESH_AS_LIST {" << std::endl;

			for (size_t j = 0; j < vecSave.size(); j++){
				ss << "*SKINNEDMESH_AS " << "\"" << vecSave[j] << "\"" << "\t" << vecRad[j] << std::endl;
			}
			ss << "}" << std::endl;
		}
		ss << "}" << std::endl;

	}
	ss << "}" << std::endl;
	return ss.str();
}