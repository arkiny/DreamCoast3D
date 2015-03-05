#include "stdafx.h"
#include "cMapLoader4Edit.h"
#include "cSceneEditMode.h"
#include "cGameASEObject.h"

cMapLoader4Edit::cMapLoader4Edit()
{
}


cMapLoader4Edit::~cMapLoader4Edit()
{
}

void cMapLoader4Edit::LoadGameMapFromFile(OUT cSceneEditMode* pScene, IN std::string sPath){
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
				//pScene->AddStaticGameObj(add);
				pScene->AddStaticGameObjectToPreset(add);
				SAFE_RELEASE(add);
			}
		}
		else if (isEqual(szToken, "#")){
			SkipBlock();
		}
	}
	fclose(m_fp);
}

void cMapLoader4Edit::LoadGameMapFromFile(OUT cSceneEditMode* pScene, IN std::string sFolder, IN std::string sFile){
	LoadGameMapFromFile(pScene, (sFolder + sFile));
}

void cMapLoader4Edit::ParseGameMapInfo(OUT cSceneEditMode* pScene){
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
			ParseHeightMap(pScene);
		}
	} while (nLevel > 0);
}

std::string cMapLoader4Edit::ParseHeightMap(OUT cSceneEditMode* pScene){
	int nLevel = 0;
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

	pScene->AddMapPath(sRaw, sTexture);
	return (sTexture + sRaw);
}

cGameASEObject* cMapLoader4Edit::ParseMapObjectAse(){
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

void cMapLoader4Edit::ParseMapObjectMaterialList(){
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

void cMapLoader4Edit::ParseMapObjectMaterial(){
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