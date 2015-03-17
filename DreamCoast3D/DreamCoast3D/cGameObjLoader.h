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
	cGameObject* ParsePlayerbleObj();
	cGameObject* ParseGameAIObj();
	cGameObject* ParseSkyObj();

	void ParseSkinnedCSList(OUT std::vector<std::string>& vecNodeList,
		OUT std::vector<float>& vecRadiusList);

	void ParseSkinnedASList(OUT std::vector<std::string>& vecNodeList,
		OUT std::vector<float>& vecRadiusList);

public:
	cGameObjLoader();
	virtual ~cGameObjLoader();

	void LoadGameObjectsFromFile(OUT cGameObjManager* pGameManager, IN std::string sPath);
	void LoadGameObjectsFromFile(OUT cGameObjManager* pGameManager, IN std::string sFolder, IN std::string sFile);
};

