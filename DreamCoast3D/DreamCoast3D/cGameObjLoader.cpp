#include "stdafx.h"
#include "cGameObjLoader.h"
#include "cGameObjManager.h"
#include "cGameObject.h"
#include "cSkinnedMesh.h"
#include "cGameActionSkinnedMeshObj.h"
#include "cGameSMeshBodyObject.h"
#include "cGamePlayableObject.h"

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

void cGameObjLoader::LoadGameObjectsFromFile(OUT cGameObjManager* pGameManager, IN std::string sPath){
	fopen_s(&m_fp, sPath.c_str(), "r");

	while (char* szToken = GetToken()){
		if (isEqual(szToken, "*SKINNEDMESH_LIST")){
			ParseAndLoadSkinnedMeshList();
		}

		else if (isEqual(szToken, "*GAMEACTIONSKINNEDMESHOBJ")){
			cGameObject* p = ParseGameActionSkinnedMeshObj();
			pGameManager->AddGameObj(p);
			SAFE_RELEASE(p);
		}
		else if (isEqual(szToken, "*GAMEACTIONSKINNEDBODYMESHOBJ")){
			cGameObject* p = ParseAndLoadSkinnedBodyMesh();
			pGameManager->AddGameObj(p);
			SAFE_RELEASE(p);
		}
		else if (isEqual(szToken, "*PLAYABLEOBJ")){
			cGameObject* p = ParsePlayerbleObj();
			pGameManager->AddGameObj(p);
			pGameManager->SetPlayableGameObject(p);
			SAFE_RELEASE(p);
		}
		else if (isEqual(szToken, "#")){
			SkipBlock();
		}
	}
	fclose(m_fp);
}


void cGameObjLoader::LoadGameObjectsFromFile(cGameObjManager* pGameManager, std::string sFolder, std::string sFile){
	LoadGameObjectsFromFile(pGameManager, sFolder + sFile); 
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
		else if (isEqual(szToken, "*SCALE")){
			D3DXVECTOR3 scale;
			scale.x = GetFloat();
			scale.y = GetFloat();
			scale.z = GetFloat();
			ret->SetScale(scale);
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

cGameObject* cGameObjLoader::ParseAndLoadSkinnedBodyMesh(){
	cGameSMeshBodyObject* ret = NULL;
	int nBodyIndex = -1, nHeadIndex = -1, nHairIndex = -1;
	D3DXVECTOR3 pos(0,0,0), scale(1.0f, 1.0f, 1.0f);
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
			nBodyIndex = GetInteger();
		}
		else if (isEqual(szToken, "*SKINNEDMESHHEAD_REF")){
			nHeadIndex = GetInteger();
		}
		else if (isEqual(szToken, "*SKINNEDMESHAIR_REF")){
			nHairIndex = GetInteger();
		}
		else if (isEqual(szToken, "*POISTION")){
			pos.x = GetFloat();
			pos.y = GetFloat();
			pos.z = GetFloat();
		}
		else if (isEqual(szToken, "*SCALE")){
			scale.x = GetFloat();
			scale.y = GetFloat();
			scale.z = GetFloat();
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
			/*ret->GetSkinnedMesh()->SetAnimationLoop(0, false);
			ret->GetSkinnedMesh()->SetAnimationLoop(1, false);
			ret->GetSkinnedMesh()->SetAnimationLoop(2, false);
			ret->GetSkinnedMesh()->SetAnimationLoop(3, true);
			ret->GetSkinnedMesh()->SetAnimationLoop(4, true);
			ret->GetSkinnedMesh()->SetAnimationIndex(nIndex);*/
		}
	} while (nLevel > 0);

	ret = new cGameSMeshBodyObject;
	ret->Setup(m_vecsFolders[nBodyIndex], m_vecsFiles[nBodyIndex],
		m_vecsFolders[nHeadIndex], m_vecsFiles[nHeadIndex],
		m_vecsFolders[nHairIndex], m_vecsFiles[nHairIndex]);
	ret->SetPosition(pos);
	ret->SetScale(scale);

	return ret;
}

cGameObject* cGameObjLoader::ParsePlayerbleObj(){
	cGamePlayableObject* ret = NULL;
	int nBodyIndex = -1, nHeadIndex = -1, nHairIndex = -1;
	D3DXVECTOR3 pos(0, 0, 0), scale(1.0f, 1.0f, 1.0f);
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
			nBodyIndex = GetInteger();
		}
		else if (isEqual(szToken, "*SKINNEDMESHHEAD_REF")){
			nHeadIndex = GetInteger();
		}
		else if (isEqual(szToken, "*SKINNEDMESHAIR_REF")){
			nHairIndex = GetInteger();
		}
		else if (isEqual(szToken, "*POISTION")){
			pos.x = GetFloat();
			pos.y = GetFloat();
			pos.z = GetFloat();
		}
		else if (isEqual(szToken, "*SCALE")){
			scale.x = GetFloat();
			scale.y = GetFloat();
			scale.z = GetFloat();
		}
		else if (isEqual(szToken, "*SETINITANIMATION")){
			int nIndex = GetInteger();	
			/*ret->GetSkinnedMesh()->SetAnimationIndex(nIndex);*/
		}
	} while (nLevel > 0);

	ret = new cGamePlayableObject;
	ret->Setup(m_vecsFolders[nBodyIndex], m_vecsFiles[nBodyIndex],
		m_vecsFolders[nHeadIndex], m_vecsFiles[nHeadIndex],
		m_vecsFolders[nHairIndex], m_vecsFiles[nHairIndex]);
	ret->SetPosition(pos);
	ret->SetScale(scale);

	ret->GetSkinnedMesh()->SetAnimationLoop(ret->EPLAYABLESTATE_IDLE, true);
	ret->GetSkinnedMesh()->SetAnimationLoop(ret->EPLAYABLESTATE_MOVE, true);
	ret->GetSkinnedMesh()->SetAnimationLoop(ret->EPLAYABLESTATE_ATTACK, true);
	ret->GetSkinnedMesh()->SetAnimationLoop(4, true);

	ret->GetSkinnedMesh()->SetAnimationIndex(0);
	
	return ret;
}
//*PLAYABLEOBJ