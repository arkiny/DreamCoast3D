#include "stdafx.h"
#include "cGameObjLoader.h"
#include "cGameObjManager.h"
#include "cGameObject.h"
#include "cSkinnedMesh.h"
#include "cGameActionSkinnedMeshObj.h"

//Template
//int nLevel = 0;
//do{
//	char* szToken = GetToken();
//	if (isEqual(szToken, "{")){
//		++nLevel;
//	}
//	else if (isEqual(szToken, "}")){
//		--nLevel;
//	}
//	else if (isEqual(szToken, "")){
//	}
//	else if (isEqual(szToken, "")){
//	}
//	else if (isEqual(szToken, "")){
//	}
//	else if (isEqual(szToken, "")){
//	}
//} while (nLevel > 0);

cGameObjLoader::cGameObjLoader()
{
}


cGameObjLoader::~cGameObjLoader()
{
}

void cGameObjLoader::LoadGameObjectsFromFile(cGameObjManager* pGameManager, std::string sFolder, std::string sFile){
	fopen_s(&m_fp, (sFolder + sFile).c_str(), "r");
	
	while (char* szToken = GetToken()){
		if (isEqual(szToken, "*SKINNEDMESH_LIST")){
			ParseAndLoadSkinnedMeshList();
		}
			
		else if (isEqual(szToken, "*GAMEACTIONSKINNEDMESHOBJ")){
			cGameObject* p = ParseGameActionSkinnedMeshObj();
			pGameManager->AddGameObj(p);
			SAFE_RELEASE(p);
		}
		else if (isEqual(szToken, "#")){
			SkipBlock();
		}
	}

	fclose(m_fp);
}

cGameObject* cGameObjLoader::ParseGameActionSkinnedMeshObj(){
	cGameActionSkinnedMeshObj* ret = new cGameActionSkinnedMeshObj;

	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*SKINNEDMESH_REF")){
			int nIndex = GetInteger();
			ret->Setup(m_vecsFolders[nIndex], m_vecsFiles[nIndex]);
		}
		else if (isEqual(szToken, "*POISTION")){
			D3DXVECTOR3 pos;
			pos.x = GetFloat();
			pos.y = GetFloat();
			pos.z = GetFloat();
			ret->SetPosition(pos);
		}
		else if (isEqual(szToken, "*SETINITANIMATION")){
			int nIndex = GetInteger();
			//for (int i = 0; i < 5; i++){
			//	if (i != nIndex){
			//		ret->GetSkinnedMesh()->SetAnimationLoop(i, false);
			//	}
			//	else{
			//		ret->GetSkinnedMesh()->SetAnimationLoop(i, true);
			//	}
			//}
			ret->GetSkinnedMesh()->SetAnimationLoop(0, false);
			ret->GetSkinnedMesh()->SetAnimationLoop(1, false);
			ret->GetSkinnedMesh()->SetAnimationLoop(2, false);
			ret->GetSkinnedMesh()->SetAnimationLoop(3, true);
			ret->GetSkinnedMesh()->SetAnimationLoop(4, true);
			ret->GetSkinnedMesh()->SetAnimationIndex(nIndex);
		}
	} while (nLevel > 0);

	return ret;
}

void cGameObjLoader::ParseAndLoadSkinnedMeshList(){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*SKINNEDMESH_LIST_COUNT")){
			int size = GetInteger();
			m_vecsFolders.resize(size);
			m_vecsFiles.resize(size);
		}
		else if (isEqual(szToken, "*SKINNEDMESH")){
			int nIndex = GetInteger();
			ParseAndLoadSkinnedMeshtoManager(nIndex);
		}
	} while (nLevel > 0);
}

void cGameObjLoader::ParseAndLoadSkinnedMeshtoManager(int nIndex){
	std::string sFolder;
	std::string sFile;
	
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*SKINNEDMESH_PATH")){
			// donothing
			sFolder = GetToken();
		}
		else if (isEqual(szToken, "*SKINNEDMESH_FILE")){
			sFile = GetToken();
		}
	} while (nLevel > 0);

	m_vecsFolders[nIndex] = sFolder;
	m_vecsFiles[nIndex] = sFile;
	
	g_pSkinnedMeshManager->GetSkinnedMesh(sFolder, sFile);
}