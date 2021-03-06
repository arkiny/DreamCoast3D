#include "stdafx.h"
#include "cGameObjLoader.h"
#include "cGameObjManager.h"
#include "cGameObject.h"
#include "cSkinnedMesh.h"
#include "cGameActionSkinnedMeshObj.h"
#include "cGameSMeshBodyObject.h"
#include "cGamePlayableObject.h"
#include "cGameAIObject.h"
#include "cGameMapSkyObject.h"

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
		else if (isEqual(szToken, "*STATICMESH_LIST")){
			ParseAndLoadStaticMeshList();
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
		else if (isEqual(szToken, "*GAMEAIOBJ")){
			cGameObject* p = ParseGameAIObj();
			pGameManager->AddGameObj(p);
			SAFE_RELEASE(p);
		}
		// TODO 하늘은 좀더 수정이 필요함
		else if (isEqual(szToken, "*GAMESKYOBJ")){
			cGameObject* p = ParseSkyObj();
			pGameManager->AddGameObj(p);
			SAFE_RELEASE(p);
		}
		else if (isEqual(szToken, "#")){
			SkipBlock();
		}
	}
	fclose(m_fp);
}

cGameObject* cGameObjLoader::ParseSkyObj(){
	cGameMapSkyObject* p = new cGameMapSkyObject;
	D3DXCOLOR skycolor;
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*SKY_COLOR")){
			float r = GetFloat();
			float g = GetFloat();
			float b = GetFloat();
			skycolor = {r, g, b, 1.0f};
		}
	} while (nLevel > 0);
	p->Setup();
	p->SetSkyColor(skycolor);
	return p;

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
			ret->GetSkinnedMesh()->SetAnimationLoop(0, true);
			ret->GetSkinnedMesh()->SetAnimationLoop(1, true);
			ret->GetSkinnedMesh()->SetAnimationLoop(2, true);
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

void cGameObjLoader::ParseAndLoadStaticMeshList(){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*STATICMESH_LIST_COUNT")){
			int size = GetInteger();
			m_vecStaticFolders.resize(size);
			m_vecStaticFiles.resize(size);
		}
		else if (isEqual(szToken, "*STATICMESH")){
			int nIndex = GetInteger();
			ParseAndLoadStaticMeshtoManager(nIndex);
		}
	} while (nLevel > 0);
}

void cGameObjLoader::ParseAndLoadSkinnedMeshtoManager(int nIndex){
	std::string sFolder;
	std::string sFile;
	std::vector<std::string>	vecCSNodeList;
	std::vector<float>			vecCSRadiusList;
	std::vector<std::string>	vecASNodeList;
	std::vector<float>			vecASRadiusList;
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
		else if (isEqual(szToken, "*SKINNEDMESH_CS_LIST")){
			ParseSkinnedCSList(vecCSNodeList, vecCSRadiusList);
		}
		else if (isEqual(szToken, "*SKINNEDMESH_AS_LIST")){
			ParseSkinnedASList(vecASNodeList, vecASRadiusList);
		}
	} while (nLevel > 0);

	m_vecsFolders[nIndex] = sFolder;
	m_vecsFiles[nIndex] = sFile;
	
	g_pSkinnedMeshManager->GetSkinnedMesh(sFolder, sFile);
	g_pSkinnedMeshManager->GetSkinnedMesh(sFolder, sFile)
		->SetCollisionVectors(vecCSNodeList, vecCSRadiusList);
	g_pSkinnedMeshManager->GetSkinnedMesh(sFolder, sFile)
		->SetAttackVectors(vecASNodeList, vecASRadiusList);
}

void cGameObjLoader::ParseAndLoadStaticMeshtoManager(int nIndex){
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
		else if (isEqual(szToken, "*STATICMESH_PATH")){
			sFolder = GetToken();
		}
		else if (isEqual(szToken, "*STATICMESH_FILE")){
			sFile = GetToken();
		}
	} while (nLevel > 0);

	m_vecStaticFolders[nIndex] = sFolder;
	m_vecStaticFiles[nIndex] = sFile;

	g_pStaticMeshManager->GetStaticMesh(sFolder + sFile);
}

void cGameObjLoader::ParseSkinnedCSList(
	OUT std::vector<std::string>& vecNodeList,
	OUT std::vector<float>& vecRadiusList){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*SKINNEDMESH_CS")){
			// donothing
			std::string sToAdd;
			sToAdd = GetToken();
			float fToAdd = GetFloat();
			vecNodeList.push_back(sToAdd);
			vecRadiusList.push_back(fToAdd);
		}
	} while (nLevel > 0);
}

void cGameObjLoader::ParseSkinnedASList(
	OUT std::vector<std::string>& vecNodeList,
	OUT std::vector<float>& vecRadiusList){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*SKINNEDMESH_AS")){
			// donothing
			std::string sToAdd;
			sToAdd = GetToken();
			float fToAdd = GetFloat();
			vecNodeList.push_back(sToAdd);
			vecRadiusList.push_back(fToAdd);
		}
	} while (nLevel > 0);
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
	std::string	sMeshTexture = "";
	int nBodyIndex = -1, nHeadIndex = -1, nHairIndex = -1, nWeaponIndex = -1;
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
		else if (isEqual(szToken, "*STATICMESHWEAPON_REF")){
			nWeaponIndex = GetInteger();
			sMeshTexture = GetToken();
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
	if (nWeaponIndex >= 0){
		ret->Setup(m_vecsFolders[nBodyIndex], m_vecsFiles[nBodyIndex],
			m_vecsFolders[nHeadIndex], m_vecsFiles[nHeadIndex],
			m_vecsFolders[nHairIndex], m_vecsFiles[nHairIndex],
			m_vecStaticFolders[nWeaponIndex], m_vecStaticFiles[nWeaponIndex], sMeshTexture);
	}
	else {
		ret->Setup(m_vecsFolders[nBodyIndex], m_vecsFiles[nBodyIndex],
			m_vecsFolders[nHeadIndex], m_vecsFiles[nHeadIndex],
			m_vecsFolders[nHairIndex], m_vecsFiles[nHairIndex]);
	}

	ret->SetPosition(pos);
	ret->SetScale(scale);

	ret->GetSkinnedMesh()->SetAnimationLoop(ret->EPLAYABLESTATE_IDLE, true);
	//ret->GetSkinnedMesh()->SetAnimationLoop(ret->EPLAYABLESTATE_MOVE, true);
	//ret->GetSkinnedMesh()->SetAnimationLoop(ret->EPLAYABLESTATE_ATTACK, true);
	//ret->GetSkinnedMesh()->SetAnimationLoop(4, true);

	ret->GetSkinnedMesh()->SetAnimationIndex(0);

	return ret;
}
//*PLAYABLEOBJ

cGameObject* cGameObjLoader::ParseGameAIObj(){
	cGameAIObject* ret = NULL;
	int nBodyIndex = -1, nAniIndex = -1, nPattern = -1;
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
		else if (isEqual(szToken, "*GAMEAIOBJ_AI_PATTERN")){
			nPattern = GetInteger();
		}
		else if (isEqual(szToken, "*SETINITANIMATION")){
			nAniIndex = GetInteger();
			/*ret->GetSkinnedMesh()->SetAnimationIndex(nIndex);*/
		}
	} while (nLevel > 0);

	ret = new cGameAIObject;
	ret->Setup(m_vecsFolders[nBodyIndex], m_vecsFiles[nBodyIndex]);
	ret->SetPosition(pos);
	ret->SetScale(scale);

	ret->GetSkinnedMesh()->SetAnimationLoop(0, true);
	ret->GetSkinnedMesh()->SetAnimationLoop(1, true);
	ret->GetSkinnedMesh()->SetAnimationLoop(2, true);
	ret->GetSkinnedMesh()->SetAnimationLoop(3, true);

	if (nAniIndex > 0){
		ret->GetSkinnedMesh()->SetAnimationIndex(nAniIndex);
	}
	else {
		ret->GetSkinnedMesh()->SetAnimationIndex(0);
	}

	if (nPattern > 0){
		ret->SetAItype((cGameAIObject::E_AI_TYPE)nPattern);
	}
	else {
		ret->SetAItype(ret->E_AI_PASSIVE);
	}
	ret->Start();
	return ret;
}