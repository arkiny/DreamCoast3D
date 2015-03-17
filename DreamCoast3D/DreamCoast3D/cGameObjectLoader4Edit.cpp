#include "stdafx.h"
#include "cGameObjectLoader4Edit.h"
#include "cSceneEditMode.h"
#include "cGameObject.h"
#include "cSkinnedMesh.h"
#include "cGameActionSkinnedMeshObj.h"
#include "cGameSMeshBodyObject.h"
#include "cGamePlayableObject.h"
#include "cGameAIObject.h"
#include "cGameMapSkyObject.h"

cGameObjectLoader4Edit::cGameObjectLoader4Edit()
{
}


cGameObjectLoader4Edit::~cGameObjectLoader4Edit()
{
}

void cGameObjectLoader4Edit::LoadGameObjectsFromFile(OUT cSceneEditMode* pSceneEditMode, IN std::string sPath){
	fopen_s(&m_fp, sPath.c_str(), "r");

	while (char* szToken = GetToken()){
		if (isEqual(szToken, "*SKINNEDMESH_LIST")){
			ParseAndLoadSkinnedMeshList();
		}
	
		// 에디트 모드에서는 AI오브젝트와 플레이어블밖에 
		// 없을득
		// TODO
		// 추후에 AI오브젝트 외에 NPC오브젝트 관리도
		// 생각해봐야 할 문제
		else if (isEqual(szToken, "*PLAYABLEOBJ")){
			cGamePlayableObject* p = ParsePlayerbleObj();
			pSceneEditMode->AddGamePlayableObjectToPreset(p);
			SAFE_RELEASE(p);
		}
		else if (isEqual(szToken, "*GAMEAIOBJ")){
			cGameAIObject* p = ParseGameAIObj();
			pSceneEditMode->AddGameAIObjectToPreset(p);
			SAFE_RELEASE(p);
		}
		// TODO 하늘은 좀더 수정이 필요함
		/*else if (isEqual(szToken, "*GAMESKYOBJ")){
			cGameObject* p = new cGameMapSkyObject;
			p->Setup();
			pGameManager->AddGameObj(p);
			SAFE_RELEASE(p);
		}*/
		else if (isEqual(szToken, "#")){
			SkipBlock();
		}
	}
	fclose(m_fp);
}

void cGameObjectLoader4Edit::LoadGameObjectsFromFile(OUT cSceneEditMode* pSceneEditMode, IN std::string sFolder, IN std::string sFile){
	LoadGameObjectsFromFile(pSceneEditMode, sFolder + sFile);
}

void cGameObjectLoader4Edit::ParseAndLoadSkinnedMeshList(){
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

void cGameObjectLoader4Edit::ParseAndLoadSkinnedMeshtoManager(int nIndex){
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

void cGameObjectLoader4Edit::ParseSkinnedCSList(
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

void cGameObjectLoader4Edit::ParseSkinnedASList(
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

cGamePlayableObject* cGameObjectLoader4Edit::ParsePlayerbleObj(){
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
	//ret->GetSkinnedMesh()->SetAnimationLoop(4, true);

	ret->GetSkinnedMesh()->SetAnimationIndex(0);

	return ret;
}

cGameAIObject* cGameObjectLoader4Edit::ParseGameAIObj(){
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
