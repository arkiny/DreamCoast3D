#include "stdafx.h"
#include "cMapLoader4Edit.h"
#include "cSceneEditMode.h"

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