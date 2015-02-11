#include "stdafx.h"
#include "cMapLoader.h"
#include "cScene.h"
#include "cGameMapHeight.h"


cMapLoader::cMapLoader()
{
}


cMapLoader::~cMapLoader()
{
}

void cMapLoader::LoadGameMapFromFile(OUT cScene* pScene, IN std::string sFolder, IN std::string sFile){
	fopen_s(&m_fp, (sFolder + sFile).c_str(), "r");

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
			cGameMapHeight* add = ParseHeightMap();
			pScene->AddMap(add);
		}
	} while (nLevel > 0);
}

cGameMapHeight* cMapLoader::ParseHeightMap(){
	int nLevel = 0;

	cGameMapHeight* ret = new cGameMapHeight;
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