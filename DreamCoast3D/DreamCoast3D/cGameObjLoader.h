#pragma once
#include "cLoader.h"

class cGameObject;
class cGameObjManager;

class cGameObjLoader : public cLoader
{
protected:
	std::vector<std::string> m_vecsFolders;
	std::vector<std::string> m_vecsFiles;

public:
	cGameObjLoader();
	~cGameObjLoader();

	void LoadGameObjectsFromFile(OUT cGameObjManager* pGameManager, IN std::string sFolder, IN std::string sFile);


	void ParseAndLoadSkinnedMeshList();
	void ParseAndLoadSkinnedMeshtoManager(int nIndex);
	cGameObject* ParseGameActionSkinnedMeshObj();
};

