#pragma once
#include "cLoader.h"

class cGameASEObject;
class cSceneEditMode;

class cMapLoader4Edit : public cLoader
{
private:
	std::vector<std::string> m_vecsObjFolders;
	std::vector<std::string> m_vecsObjFiles;

	void				ParseGameMapInfo(OUT cSceneEditMode* pScene);
	std::string			ParseHeightMap(OUT cSceneEditMode* pScene);
	
	void				ParseMapObjectMaterialList();
	void				ParseMapObjectMaterial();
	cGameASEObject*		ParseMapObjectAse();

public:
	cMapLoader4Edit();
	virtual ~cMapLoader4Edit();

	void LoadGameMapFromFile(OUT cSceneEditMode* pScene, IN std::string sPath);
	void LoadGameMapFromFile(OUT cSceneEditMode* pScene, IN std::string sFolder, IN std::string sFile);
};

