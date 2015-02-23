#include "stdafx.h"
#include "cMapLoader.h"
#include "cScene.h"
#include "cGameMapHeight.h"
#include "cGameASEObject.h"
#include "cGameMapHeightGridSystem.h"


cMapLoader::cMapLoader()
{
}


cMapLoader::~cMapLoader()
{
}

void cMapLoader::LoadGameMapFromFile(OUT cScene* pScene, IN std::string sPath){
	fopen_s(&m_fp, sPath.c_str(), "r");

	while (char* szToken = GetToken()){
		if (isEqual(szToken, "*GAME_MAP_INFO")){
			ParseGameMapInfo(pScene);
		}
		else if (isEqual(szToken, "*MAP_OBJECT_MATERIAL_LIST")){
			ParseMapObjectMaterialList();
		}
		else if (isEqual(szToken, "*MAP_OBJECT_ASE")){
			cGameASEObject* add = ParseMapObjectAse();
			if (add){
				pScene->AddStaticGameObj(add);
				SAFE_RELEASE(add);
			}
		}
		else if (isEqual(szToken, "#")){
			SkipBlock();
		}
	}
	fclose(m_fp);
}

void cMapLoader::LoadGameMapFromFile(OUT cScene* pScene, IN std::string sFolder, IN std::string sFile){
	LoadGameMapFromFile(pScene, (sFolder + sFile));
}

cGameASEObject* cMapLoader::ParseMapObjectAse(){
	cGameASEObject* ret = NULL;

	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*MAP_OBJECT_ASE_REF")){
			int ref = GetInteger();
			ret = new cGameASEObject;
			ret->Setup(m_vecsObjFolders[ref], m_vecsObjFiles[ref]);
		}
		else if (isEqual(szToken, "*MAP_OBJECT_ASE_POS")){
			ret->SetPosition(D3DXVECTOR3(GetFloat(), GetFloat(), GetFloat()));
		}
		else if (isEqual(szToken, "*MAP_OBJECT_ASE_SCALE")){
			ret->SetScale(D3DXVECTOR3(GetFloat(), GetFloat(), GetFloat()));
		}
	} while (nLevel > 0);

	return ret;
}

void cMapLoader::ParseMapObjectMaterialList(){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*MAP_OBJECT_MATERIAL_COUNT")){
			int numOfMaterial = GetInteger();
		}
		else if (isEqual(szToken, "*MAP_OBJECT_MATERIAL")){
			ParseMapObjectMaterial();
		}
	} while (nLevel > 0);
}

void cMapLoader::ParseMapObjectMaterial(){
	std::string sFolder;
	std::string sFile;
	int token = GetInteger();
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*MAP_OBJECT_MATERIAL_PATH")){
			sFolder = GetToken();
		}
		else if (isEqual(szToken, "*MAP_OBJECT_MATERIAL_FILE")){
			sFile = GetToken();
		}
	} while (nLevel > 0);
	
	m_vecsObjFiles.push_back(sFile);
	m_vecsObjFolders.push_back(sFolder);

	g_pAseManager->GetAseObject(sFolder, sFile);
}


void cMapLoader::ParseGameMapInfo(OUT cScene* pScene){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*GAME_MAP_COUNT")){
			int numOfGameMap = GetInteger();
		}
		else if (isEqual(szToken, "*HEIGHT_MAP")){
			cGameMapObject* add = ParseHeightMap();
			add->Setup();
			pScene->AddMap(add);
		}
	} while (nLevel > 0);
}

cGameMapHeight* cMapLoader::ParseHeightMap(){
	int nLevel = 0;
	cGameMapHeightGridSystem* ret = new cGameMapHeightGridSystem;
	std::string sTexture;
	std::string sRaw;
	int num = GetInteger();
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*GAME_MAP_HEIGHT_TEXTURE_PATH")){
			sTexture = GetToken();
		}
		else if (isEqual(szToken, "*GAME_MAP_HEIGHT_RAW_FILE")){
			sRaw = GetToken();
		}
	} while (nLevel > 0);

	ret->LoadFromFiles(sRaw, sTexture);
	return ret;
}