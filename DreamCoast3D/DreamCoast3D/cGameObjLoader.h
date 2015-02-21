#pragma once
#include "cLoader.h"

class cGameObject;
class cGameObjManager;

class cGameObjLoader : public cLoader
{
protected:
	std::vector<std::string> m_vecsFolders;
	std::vector<std::string> m_vecsFiles;

	void ParseAndLoadSkinnedMeshList();
	void ParseAndLoadSkinnedMeshtoManager(int nIndex);
	
	cGameObject* ParseAndLoadSkinnedBodyMesh();
	cGameObject* ParseGameActionSkinnedMeshObj();

public:
	cGameObjLoader();
	virtual ~cGameObjLoader();

	void LoadGameObjectsFromFile(OUT cGameObjManager* pGameManager, IN std::string sPath);
	void LoadGameObjectsFromFile(OUT cGameObjManager* pGameManager, IN std::string sFolder, IN std::string sFile);
};

