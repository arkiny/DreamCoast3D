#include "stdafx.h"
#include "cGameLoader.h"
#include "cSceneManager.h"
#include "cScene.h"
#include "cMapLoader.h"
#include "cGameObjLoader.h"
#include "cUILoader.h"
#include "cCamera.h"
#include "cCameraEditing.h"
#include "cLightSource.h"

cGameLoader::cGameLoader()
{
}


cGameLoader::~cGameLoader()
{
}

void cGameLoader::LoadFromGameFile(
	OUT cSceneManager* pSceneMng,
	std::string& sPath){

	_ASSERT(pSceneMng);
	fopen_s(&m_fp, (sPath).c_str(), "r");

	while (char* szToken = GetToken()){
		if (isEqual(szToken, "*SCENE_LIST")){
			ParseSceneList(pSceneMng);
		}
		else if (isEqual(szToken, "#")){
			szToken = GetToken();
		}
	}

	fclose(m_fp);
}

void cGameLoader::LoadFromGameFile(cSceneManager* pSceneMng, std::string& sFolder, std::string& sFile){
	return LoadFromGameFile(pSceneMng, sFolder + sFile);
}

void cGameLoader::ParseSceneList(OUT cSceneManager* pSceneMng){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*SCENE_COUNT")){
			int nScene = GetInteger();
		}
		else if (isEqual(szToken, "*SCENE")){
			int nSceneNum = GetInteger();
			std::string sPath = GetToken();
			pSceneMng->AddSceneFilePath(sPath);
		}
	} while (nLevel > 0);
}