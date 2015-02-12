#pragma once
#include "cLoader.h"

class cScene;
class cGameMapHeight;
class cGameASEObject;

class cMapLoader : public cLoader
{
private:
	std::vector<std::string> m_vecsObjFolders;
	std::vector<std::string> m_vecsObjFiles;

	void				ParseGameMapInfo(OUT cScene* pScene);
	cGameMapHeight*		ParseHeightMap();
	void				ParseMapObjectMaterialList();
	void				ParseMapObjectMaterial();
	cGameASEObject*		ParseMapObjectAse();

public:
	cMapLoader();
	virtual ~cMapLoader();

	void LoadGameMapFromFile(OUT cScene* pScene, IN std::string sFolder, IN std::string sFile);
};

