#pragma once
#include "cLoader.h"

class cScene;
class cGameMapHeight;

class cMapLoader : public cLoader
{
private:
	void				ParseGameMapInfo(OUT cScene* pScene);
	cGameMapHeight*		ParseHeightMap();

public:
	cMapLoader();
	virtual ~cMapLoader();

	void LoadGameMapFromFile(OUT cScene* pScene, IN std::string sFolder, IN std::string sFile);
};

